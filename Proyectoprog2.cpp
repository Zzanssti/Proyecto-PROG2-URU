#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;


// 1. ESTRUCTURAS PARA PERSISTENCIA (sin punteros)

struct Torneo {
    char nombre[100];
    char deporte[50];
    char formato[20];
    char fechaInicio[11];
    char fechaFin[11];
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Gol {
    int idJugador;   // 0 si es autogol o desconocido
    int minuto;
    int equipo;      // 0 = local, 1 = visitante
};

struct Equipo {
    int id;
    char nombre[100];
    char ciudad[100];
    char entrenador[100];
    int puntos;
    int victorias;
    int empates;
    int derrotas;
    int golesAFavor;
    int golesEnContra;
    int partidosIDs[50];
    int cantidadPartidos;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Jugador {
    int id;
    int idEquipo;
    char nombre[100];
    char cedula[20];
    char posicion[20];
    int edad;
    int numeroDorsal;
    int golesAnotados;
    int tarjetasAmarillas;
    int tarjetasRojas;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Partido {
    int id;
    int idEquipoLocal;
    int idEquipoVisitante;
    char fecha[11];
    char estado[12];      // "PROGRAMADO", "JUGADO", "CANCELADO"
    char descripcion[200];
    int golesLocal;
    int golesVisitante;
    Gol goles[22];        // máximo 22 goles por partido
    int numGoles;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

// Header de cada archivo (excepto torneo.bin)
struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
};


// 2. CONSTANTES Y DIRECTORIOS

const char* RUTA_TORNEO   = "datos/torneo.bin";
const char* RUTA_EQUIPOS  = "datos/equipos.bin";
const char* RUTA_JUGADORES= "datos/jugadores.bin";
const char* RUTA_PARTIDOS = "datos/partidos.bin";

const int MAX_RESULTADOS = 100;


// 3. FUNCIONES DE GESTIÓN DE ARCHIVOS Y HEADERS


// Crea el directorio "datos" si no existe
void crearDirectorioDatos() {
#ifdef _WIN32
    system("mkdir datos 2>nul");
#else
    system("mkdir -p datos");
#endif
}

// Inicializa un archivo: escribe un header con ceros y lo deja listo
bool inicializarArchivo(const char* nombreArchivo) {
    crearDirectorioDatos();
    ifstream prueba(nombreArchivo, ios::binary);
    if (prueba.good()) {
        prueba.close();
        return true;
    }
    prueba.close();

    ofstream out(nombreArchivo, ios::binary);
    if (!out) return false;
    ArchivoHeader header;
    memset(&header, 0, sizeof(header));
    header.version = 1;
    header.proximoID = 1;   //  Para que el primer ID sea 1
    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    out.close();
    return true;

}

// Lee el header de un archivo
ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header;
    memset(&header, 0, sizeof(header));
    ifstream in(nombreArchivo, ios::binary);
    if (!in) return header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    in.close();
    return header;
}

// Escribe el header en el archivo (sobrescribe solo los primeros bytes)
bool actualizarHeader(const char* nombreArchivo, const ArchivoHeader& header) {
    fstream file(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(0, ios::beg);
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.close();
    return true;
}

// Busca el índice físico (0-based) de un registro por su ID.
// Retorna -1 si no existe o está eliminado.
int buscarIndicePorID(const char* nombreArchivo, int id, int tamRegistro) {
    ifstream in(nombreArchivo, ios::binary);
    if (!in) return -1;

    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (header.cantidadRegistros == 0) { in.close(); return -1; }

    // Recorremos secuencialmente comparando el campo id
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        in.seekg(sizeof(header) + i * tamRegistro, ios::beg);
        int idActual;
        in.read(reinterpret_cast<char*>(&idActual), sizeof(int)); // asumimos que el primer campo es int id
        if (in.fail()) break;
        if (idActual == id) {
            // Verificar si no está eliminado (leer el flag eliminado)
            in.seekg(sizeof(header) + i * tamRegistro, ios::beg);
            char* buffer = new char[tamRegistro];
            in.read(buffer, tamRegistro);           
            bool eliminado = buffer[tamRegistro - 1]; // asumimos que es el último byte
            delete[] buffer;
            if (!eliminado) {
                in.close();
                return i;
            } else {
                break; // encontrado pero eliminado, no hay más con ese ID
            }
        }
    }
    in.close();
    return -1;
}

// 4. FUNCIONES CRUD PARA EQUIPOS

bool guardarEquipo(const Equipo& equipo) {
    ArchivoHeader header = leerHeader(RUTA_EQUIPOS);
    // Asignar ID si es 0 (nuevo)
    Equipo eq = equipo;
    if (eq.id == 0) {
        eq.id = header.proximoID++;
        header.cantidadRegistros++;
        header.registrosActivos++;
        header.version++;
    }
    // Escribir al final del archivo
    fstream file(RUTA_EQUIPOS, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(0, ios::end);
    file.write(reinterpret_cast<const char*>(&eq), sizeof(Equipo));
    if (file.fail()) { file.close(); return false; }
    file.close();
    // Actualizar header
    return actualizarHeader(RUTA_EQUIPOS, header);
}

bool leerEquipoPorID(int id, Equipo& resultado) {
    int idx = buscarIndicePorID(RUTA_EQUIPOS, id, sizeof(Equipo));
    if (idx == -1) return false;
    ifstream in(RUTA_EQUIPOS, ios::binary);
    if (!in) return false;
    in.seekg(sizeof(ArchivoHeader) + idx * sizeof(Equipo), ios::beg);
    in.read(reinterpret_cast<char*>(&resultado), sizeof(Equipo));
    in.close();
    return !resultado.eliminado;
}

bool actualizarEquipo(const Equipo& equipo) {
    int idx = buscarIndicePorID(RUTA_EQUIPOS, equipo.id, sizeof(Equipo));
    if (idx == -1) return false;
    fstream file(RUTA_EQUIPOS, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(sizeof(ArchivoHeader) + idx * sizeof(Equipo), ios::beg);
    file.write(reinterpret_cast<const char*>(&equipo), sizeof(Equipo));
    if (file.fail()) { file.close(); return false; }
    file.close();

    // Actualizar versión del header
    ArchivoHeader header = leerHeader(RUTA_EQUIPOS);
    header.version++;
    return actualizarHeader(RUTA_EQUIPOS, header);
}

bool eliminarEquipoLogico(int id) {
    int idx = buscarIndicePorID(RUTA_EQUIPOS, id, sizeof(Equipo));
    if (idx == -1) return false;
    // Leer el equipo
    Equipo eq;
    ifstream in(RUTA_EQUIPOS, ios::binary);
    if (!in) return false;
    in.seekg(sizeof(ArchivoHeader) + idx * sizeof(Equipo), ios::beg);
    in.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
    in.close();
    if (eq.eliminado) return false;

    eq.eliminado = true;
    // Actualizar en disco
    fstream file(RUTA_EQUIPOS, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(sizeof(ArchivoHeader) + idx * sizeof(Equipo), ios::beg);
    file.write(reinterpret_cast<const char*>(&eq), sizeof(Equipo));
    file.close();

    ArchivoHeader header = leerHeader(RUTA_EQUIPOS);
    header.registrosActivos--;
    header.version++;
    return actualizarHeader(RUTA_EQUIPOS, header);
}

int contarEquiposActivos() {
    ArchivoHeader header = leerHeader(RUTA_EQUIPOS);
    return header.registrosActivos;
}

// Búsqueda múltiple: llena un array de Equipo con los que coinciden con subcadena
int buscarEquiposPorNombre(const char* subcadena, Equipo resultados[], int maxResultados) {
    ifstream in(RUTA_EQUIPOS, ios::binary);
    if (!in) return 0;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    int encontrados = 0;
    char subMin[100];
    // convertir subcadena a minúsculas
    int i = 0;
    while (subcadena[i] != '\0') { subMin[i] = tolower(subcadena[i]); i++; }
    subMin[i] = '\0';

    for (int idx = 0; idx < header.cantidadRegistros && encontrados < maxResultados; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Equipo), ios::beg);
        Equipo eq;
        in.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
        if (in.fail()) break;
        if (!eq.eliminado) {
            char nomMin[100];
            int j = 0;
            while (eq.nombre[j] != '\0') { nomMin[j] = tolower(eq.nombre[j]); j++; }
            nomMin[j] = '\0';
            if (strstr(nomMin, subMin) != nullptr) {
                resultados[encontrados++] = eq;
            }
        }
    }
    in.close();
    return encontrados;
}

// 5. FUNCIONES CRUD PARA JUGADORES


bool guardarJugador(const Jugador& jugador) {
    ArchivoHeader header = leerHeader(RUTA_JUGADORES);
    Jugador j = jugador;
    if (j.id == 0) {
        j.id = header.proximoID++;
        header.cantidadRegistros++;
        header.registrosActivos++;
        header.version++;
    }
    fstream file(RUTA_JUGADORES, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(0, ios::end);
    file.write(reinterpret_cast<const char*>(&j), sizeof(Jugador));
    if (file.fail()) { file.close(); return false; }
    file.close();
    return actualizarHeader(RUTA_JUGADORES, header);
}

bool leerJugadorPorID(int id, Jugador& resultado) {
    int idx = buscarIndicePorID(RUTA_JUGADORES, id, sizeof(Jugador));
    if (idx == -1) return false;
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (!in) return false;
    in.seekg(sizeof(ArchivoHeader) + idx * sizeof(Jugador), ios::beg);
    in.read(reinterpret_cast<char*>(&resultado), sizeof(Jugador));
    in.close();
    return !resultado.eliminado;
}

bool actualizarJugador(const Jugador& jugador) {
    int idx = buscarIndicePorID(RUTA_JUGADORES, jugador.id, sizeof(Jugador));
    if (idx == -1) return false;
    fstream file(RUTA_JUGADORES, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(sizeof(ArchivoHeader) + idx * sizeof(Jugador), ios::beg);
    file.write(reinterpret_cast<const char*>(&jugador), sizeof(Jugador));
    if (file.fail()) { file.close(); return false; }
    file.close();

    ArchivoHeader header = leerHeader(RUTA_JUGADORES);
    header.version++;
    return actualizarHeader(RUTA_JUGADORES, header);
}

bool eliminarJugadorLogico(int id) {
    int idx = buscarIndicePorID(RUTA_JUGADORES, id, sizeof(Jugador));
    if (idx == -1) return false;
    Jugador j;
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (!in) return false;
    in.seekg(sizeof(ArchivoHeader) + idx * sizeof(Jugador), ios::beg);
    in.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
    in.close();
    if (j.eliminado) return false;

    j.eliminado = true;
    fstream file(RUTA_JUGADORES, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(sizeof(ArchivoHeader) + idx * sizeof(Jugador), ios::beg);
    file.write(reinterpret_cast<const char*>(&j), sizeof(Jugador));
    file.close();

    ArchivoHeader header = leerHeader(RUTA_JUGADORES);
    header.registrosActivos--;
    header.version++;
    return actualizarHeader(RUTA_JUGADORES, header);
}

int contarJugadoresActivos() {
    ArchivoHeader header = leerHeader(RUTA_JUGADORES);
    return header.registrosActivos;
}

// Buscar jugadores por nombre parcial
int buscarJugadoresPorNombre(const char* subcadena, Jugador resultados[], int maxResultados) {
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (!in) return 0;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    int encontrados = 0;
    char subMin[100];
    int i = 0;
    while (subcadena[i] != '\0') { subMin[i] = tolower(subcadena[i]); i++; }
    subMin[i] = '\0';

    for (int idx = 0; idx < header.cantidadRegistros && encontrados < maxResultados; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
        Jugador j;
        in.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
        if (in.fail()) break;
        if (!j.eliminado) {
            char nomMin[100];
            int jj = 0;
            while (j.nombre[jj] != '\0') { nomMin[jj] = tolower(j.nombre[jj]); jj++; }
            nomMin[jj] = '\0';
            if (strstr(nomMin, subMin) != nullptr) {
                resultados[encontrados++] = j;
            }
        }
    }
    in.close();
    return encontrados;
}

// Listar jugadores por equipo
int listarJugadoresPorEquipo(int idEquipo, Jugador resultados[], int maxResultados) {
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (!in) return 0;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    int encontrados = 0;
    for (int idx = 0; idx < header.cantidadRegistros && encontrados < maxResultados; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
        Jugador j;
        in.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
        if (in.fail()) break;
        if (!j.eliminado && j.idEquipo == idEquipo) {
            resultados[encontrados++] = j;
        }
    }
    in.close();
    return encontrados;
}

// 6. FUNCIONES CRUD PARA PARTIDOS

bool guardarPartido(const Partido& partido) {
    ArchivoHeader header = leerHeader(RUTA_PARTIDOS);
    Partido p = partido;
    if (p.id == 0) {
        p.id = header.proximoID++;
        header.cantidadRegistros++;
        header.registrosActivos++;
        header.version++;
    }
    fstream file(RUTA_PARTIDOS, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(0, ios::end);
    file.write(reinterpret_cast<const char*>(&p), sizeof(Partido));
    if (file.fail()) { file.close(); return false; }
    file.close();
    return actualizarHeader(RUTA_PARTIDOS, header);
}

bool leerPartidoPorID(int id, Partido& resultado) {
    int idx = buscarIndicePorID(RUTA_PARTIDOS, id, sizeof(Partido));
    if (idx == -1) return false;
    ifstream in(RUTA_PARTIDOS, ios::binary);
    if (!in) return false;
    in.seekg(sizeof(ArchivoHeader) + idx * sizeof(Partido), ios::beg);
    in.read(reinterpret_cast<char*>(&resultado), sizeof(Partido));
    in.close();
    return !resultado.eliminado;
}

bool actualizarPartido(const Partido& partido) {
    int idx = buscarIndicePorID(RUTA_PARTIDOS, partido.id, sizeof(Partido));
    if (idx == -1) return false;
    fstream file(RUTA_PARTIDOS, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(sizeof(ArchivoHeader) + idx * sizeof(Partido), ios::beg);
    file.write(reinterpret_cast<const char*>(&partido), sizeof(Partido));
    if (file.fail()) { file.close(); return false; }
    file.close();

    ArchivoHeader header = leerHeader(RUTA_PARTIDOS);
    header.version++;
    return actualizarHeader(RUTA_PARTIDOS, header);
}

bool eliminarPartidoLogico(int id) {
    int idx = buscarIndicePorID(RUTA_PARTIDOS, id, sizeof(Partido));
    if (idx == -1) return false;
    Partido p;
    ifstream in(RUTA_PARTIDOS, ios::binary);
    if (!in) return false;
    in.seekg(sizeof(ArchivoHeader) + idx * sizeof(Partido), ios::beg);
    in.read(reinterpret_cast<char*>(&p), sizeof(Partido));
    in.close();
    if (p.eliminado) return false;

    p.eliminado = true;
    fstream file(RUTA_PARTIDOS, ios::in | ios::out | ios::binary);
    if (!file) return false;
    file.seekp(sizeof(ArchivoHeader) + idx * sizeof(Partido), ios::beg);
    file.write(reinterpret_cast<const char*>(&p), sizeof(Partido));
    file.close();

    ArchivoHeader header = leerHeader(RUTA_PARTIDOS);
    header.registrosActivos--;
    header.version++;
    return actualizarHeader(RUTA_PARTIDOS, header);
}

int contarPartidosActivos() {
    ArchivoHeader header = leerHeader(RUTA_PARTIDOS);
    return header.registrosActivos;
}

// Listar partidos por estado
int listarPartidosPorEstado(const char* estado, Partido resultados[], int maxResultados) {
    ifstream in(RUTA_PARTIDOS, ios::binary);
    if (!in) return 0;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    int encontrados = 0;
    for (int idx = 0; idx < header.cantidadRegistros && encontrados < maxResultados; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Partido), ios::beg);
        Partido p;
        in.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (in.fail()) break;
        if (!p.eliminado && strcmp(p.estado, estado) == 0) {
            resultados[encontrados++] = p;
        }
    }
    in.close();
    return encontrados;
}

// Listar partidos por equipo (participacion)
int listarPartidosPorEquipo(int idEquipo, Partido resultados[], int maxResultados) {
    ifstream in(RUTA_PARTIDOS, ios::binary);
    if (!in) return 0;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    int encontrados = 0;
    for (int idx = 0; idx < header.cantidadRegistros && encontrados < maxResultados; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Partido), ios::beg);
        Partido p;
        in.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (in.fail()) break;
        if (!p.eliminado && (p.idEquipoLocal == idEquipo || p.idEquipoVisitante == idEquipo)) {
            resultados[encontrados++] = p;
        }
    }
    in.close();
    return encontrados;
}

// 7. FUNCIONES DE LÓGICA DE NEGOCIO (adaptada)


// Validación de cédula única recorre todos los jugadores activo
bool cedulaExiste(const char* cedula) {
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (!in) return false;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
        Jugador j;
        in.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
        if (in.fail()) break;
        if (!j.eliminado && strcmp(j.cedula, cedula) == 0) {
            in.close();
            return true;
        }
    }
    in.close();
    return false;
}

// Verificar si un equipo tiene partidos activos (no cancelados)
bool equipoTienePartidosActivos(int idEquipo) {
    ifstream in(RUTA_PARTIDOS, ios::binary);
    if (!in) return false;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Partido), ios::beg);
        Partido p;
        in.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (in.fail()) break;
        if (!p.eliminado && strcmp(p.estado, "CANCELADO") != 0 &&
            (p.idEquipoLocal == idEquipo || p.idEquipoVisitante == idEquipo)) {
            in.close();
            return true;
        }
    }
    in.close();
    return false;
}

// Verificar si ya existe un partido entre dos equipos (activo, no cancelado)
bool partidoYaExiste(int idLocal, int idVisitante) {
    ifstream in(RUTA_PARTIDOS, ios::binary);
    if (!in) return false;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Partido), ios::beg);
        Partido p;
        in.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (in.fail()) break;
        if (!p.eliminado && strcmp(p.estado, "CANCELADO") != 0 &&
            ((p.idEquipoLocal == idLocal && p.idEquipoVisitante == idVisitante) ||
             (p.idEquipoLocal == idVisitante && p.idEquipoVisitante == idLocal))) {
            in.close();
            return true;
        }
    }
    in.close();
    return false;
}

//  AGREGAR EQUIPO 
bool agregarEquipo(const char* nombre, const char* ciudad, const char* entrenador, Equipo& nuevoEquipo) {
    // Verificar duplicado de nombre
    Equipo aux;
    // Para verificar nombre, podemos recorrer todos los equipos activos
    ifstream in(RUTA_EQUIPOS, ios::binary);
    if (in) {
        ArchivoHeader header;
        in.read(reinterpret_cast<char*>(&header), sizeof(header));
        for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
            in.seekg(sizeof(header) + idx * sizeof(Equipo), ios::beg);
            Equipo e;
            in.read(reinterpret_cast<char*>(&e), sizeof(Equipo));
            if (in.fail()) break;
            if (!e.eliminado && strcmp(e.nombre, nombre) == 0) {
                in.close();
                return false;
            }
        }
        in.close();
    }

    Equipo eq;
    memset(&eq, 0, sizeof(eq));
    strcpy(eq.nombre, nombre);
    strcpy(eq.ciudad, ciudad);
    strcpy(eq.entrenador, entrenador);
    eq.puntos = 0;
    eq.victorias = eq.empates = eq.derrotas = 0;
    eq.golesAFavor = eq.golesEnContra = 0;
    eq.cantidadPartidos = 0;
    eq.eliminado = false;
    eq.fechaCreacion = time(nullptr);
    eq.fechaUltimaModificacion = time(nullptr);

    if (!guardarEquipo(eq)) return false;
    // Recuperar el ID asignado
    ArchivoHeader header = leerHeader(RUTA_EQUIPOS);
    eq.id = header.proximoID - 1; // el ID recién asignado
    nuevoEquipo = eq;
    return true;
}

//  ACTUALIZAR EQUIPO (función auxiliar que actualiza todos los campos, usada internamente)
bool actualizarEquipoCompleto(int id, const char* nombre, const char* ciudad, const char* entrenador) {
    Equipo eq;
    if (!leerEquipoPorID(id, eq)) return false;
    // Verificar nombre duplicado (si cambia)
    if (strcmp(eq.nombre, nombre) != 0) {
        // Buscar si otro equipo tiene ese nombre
        ifstream in(RUTA_EQUIPOS, ios::binary);
        if (in) {
            ArchivoHeader header;
            in.read(reinterpret_cast<char*>(&header), sizeof(header));
            for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
                in.seekg(sizeof(header) + idx * sizeof(Equipo), ios::beg);
                Equipo e;
                in.read(reinterpret_cast<char*>(&e), sizeof(Equipo));
                if (in.fail()) break;
                if (!e.eliminado && e.id != id && strcmp(e.nombre, nombre) == 0) {
                    in.close();
                    return false;
                }
            }
            in.close();
        }
    }
    strcpy(eq.nombre, nombre);
    strcpy(eq.ciudad, ciudad);
    strcpy(eq.entrenador, entrenador);
    eq.fechaUltimaModificacion = time(nullptr);
    return actualizarEquipo(eq);
}

//  ELIMINAR EQUIPO (logica) 
bool eliminarEquipoLogica(int idEquipo) {
    // Verificar si tiene partidos activos
    if (equipoTienePartidosActivos(idEquipo)) {
        return false; // no se puede eliminar si tiene partidos
    }
    // Primero desvincular jugadores (poner idEquipo = 0)
    // Leer todos los jugadores y actualizar los que tengan este equipo
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (!in) return false;
    ArchivoHeader headerJ;
    in.read(reinterpret_cast<char*>(&headerJ), sizeof(headerJ));
    // Guardar los jugadores que hay que actualizar
    int jugadoresModificados[100];
    int countMod = 0;
    for (int idx = 0; idx < headerJ.cantidadRegistros; ++idx) {
        in.seekg(sizeof(headerJ) + idx * sizeof(Jugador), ios::beg);
        Jugador j;
        in.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
        if (in.fail()) break;
        if (!j.eliminado && j.idEquipo == idEquipo) {
            j.idEquipo = 0;
            j.fechaUltimaModificacion = time(nullptr);
            // Guardar en lista para actualizar después
            // actualizar directamente
            actualizarJugador(j);
        }
    }
    in.close();

    // Finalmente eliminar el equipo
    return eliminarEquipoLogico(idEquipo);
}

//  AGREGAR JUGADOR (ahora obliga a tener equipo)
bool agregarJugador(int idEquipo, const char* nombre, const char* cedula, const char* posicion,
                    int edad, int dorsal, Jugador& nuevoJugador) {
    // Validar que el equipo exista y no sea 0
    if (idEquipo == 0) return false;
    Equipo eq;
    if (!leerEquipoPorID(idEquipo, eq)) return false;

    // Cédula única
    if (cedulaExiste(cedula)) return false;
    // Dorsal único dentro del equipo
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (in) {
        ArchivoHeader header;
        in.read(reinterpret_cast<char*>(&header), sizeof(header));
        for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
            in.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
            Jugador j;
            in.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
            if (in.fail()) break;
            if (!j.eliminado && j.idEquipo == idEquipo && j.numeroDorsal == dorsal) {
                in.close();
                return false;
            }
        }
        in.close();
    }

    Jugador j;
    memset(&j, 0, sizeof(j));
    j.idEquipo = idEquipo;
    strcpy(j.nombre, nombre);
    strcpy(j.cedula, cedula);
    strcpy(j.posicion, posicion);
    j.edad = edad;
    j.numeroDorsal = dorsal;
    j.golesAnotados = 0;
    j.tarjetasAmarillas = 0;
    j.tarjetasRojas = 0;
    j.eliminado = false;
    j.fechaCreacion = time(nullptr);
    j.fechaUltimaModificacion = time(nullptr);

    if (!guardarJugador(j)) return false;
    ArchivoHeader header = leerHeader(RUTA_JUGADORES);
    j.id = header.proximoID - 1;
    nuevoJugador = j;
    return true;
}

//  ACTUALIZAR JUGADOR (función auxiliar que actualiza todos los campos)
bool actualizarJugadorCompleto(int id, int nuevoIdEquipo, const char* nombre, const char* posicion,
                               int edad, int dorsal) {
    if (nuevoIdEquipo == 0) return false;
    Jugador j;
    if (!leerJugadorPorID(id, j)) return false;
    Equipo eq;
    if (!leerEquipoPorID(nuevoIdEquipo, eq)) return false;

    // Verificar dorsal único en el nuevo equipo
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (in) {
        ArchivoHeader header;
        in.read(reinterpret_cast<char*>(&header), sizeof(header));
        for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
            in.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
            Jugador aux;
            in.read(reinterpret_cast<char*>(&aux), sizeof(Jugador));
            if (in.fail()) break;
            if (!aux.eliminado && aux.id != id && aux.idEquipo == nuevoIdEquipo && aux.numeroDorsal == dorsal) {
                in.close();
                return false;
            }
        }
        in.close();
    }

    j.idEquipo = nuevoIdEquipo;
    strcpy(j.nombre, nombre);
    strcpy(j.posicion, posicion);
    j.edad = edad;
    j.numeroDorsal = dorsal;
    j.fechaUltimaModificacion = time(nullptr);
    return actualizarJugador(j);
}

//  ELIMINAR JUGADOR 
bool eliminarJugadorLogica(int id) {
    return eliminarJugadorLogico(id);
}

//  PROGRAMAR PARTIDO 
bool programarPartido(int idLocal, int idVisitante, const char* fecha, Partido& nuevoPartido) {
    if (idLocal == idVisitante) return false;
    Equipo el, ev;
    if (!leerEquipoPorID(idLocal, el) || !leerEquipoPorID(idVisitante, ev)) return false;
    if (partidoYaExiste(idLocal, idVisitante)) return false;

    Partido p;
    memset(&p, 0, sizeof(p));
    p.idEquipoLocal = idLocal;
    p.idEquipoVisitante = idVisitante;
    strcpy(p.fecha, fecha);
    strcpy(p.estado, "PROGRAMADO");
    strcpy(p.descripcion, "Fase General");
    p.golesLocal = 0;
    p.golesVisitante = 0;
    p.numGoles = 0;
    p.eliminado = false;
    p.fechaCreacion = time(nullptr);
    p.fechaUltimaModificacion = time(nullptr);

    if (!guardarPartido(p)) return false;
    ArchivoHeader header = leerHeader(RUTA_PARTIDOS);
    p.id = header.proximoID - 1;
    nuevoPartido = p;
    return true;
}

// REGISTRAR RESULTADO
bool registrarResultadoPartido(int idPartido, int ptsLocal, int ptsVisitante,
                               const Gol goles[], int numGoles) {
    // Leer el partido
    Partido p;
    if (!leerPartidoPorID(idPartido, p)) return false;
    if (strcmp(p.estado, "PROGRAMADO") != 0) return false;
    if (numGoles > 22) return false; // límite

    // Leer equipos
    Equipo local, visitante;
    if (!leerEquipoPorID(p.idEquipoLocal, local)) return false;
    if (!leerEquipoPorID(p.idEquipoVisitante, visitante)) return false;

    // Verificar que los jugadores anotadores existen (si idJugador != 0)
    for (int i = 0; i < numGoles; ++i) {
        if (goles[i].idJugador != 0) {
            Jugador j;
            if (!leerJugadorPorID(goles[i].idJugador, j)) return false;
            // Verificar que el jugador pertenezca al equipo correspondiente
            if (goles[i].equipo == 0 && j.idEquipo != p.idEquipoLocal) return false;
            if (goles[i].equipo == 1 && j.idEquipo != p.idEquipoVisitante) return false;
        }
    }

    // Actualizar estadísticas de equipos
    if (ptsLocal > ptsVisitante) {
        local.puntos += 3; local.victorias++;
        visitante.derrotas++;
    } else if (ptsLocal < ptsVisitante) {
        visitante.puntos += 3; visitante.victorias++;
        local.derrotas++;
    } else {
        local.puntos += 1; local.empates++;
        visitante.puntos += 1; visitante.empates++;
    }
    local.golesAFavor += ptsLocal; local.golesEnContra += ptsVisitante;
    visitante.golesAFavor += ptsVisitante; visitante.golesEnContra += ptsLocal;

    // Agregar ID del partido a los arrays de partidosIDs de cada equipo
    if (local.cantidadPartidos < 50) {
        local.partidosIDs[local.cantidadPartidos++] = p.id;
    } else {
        cout << "[ADVERTENCIA] Límite de partidos para equipo local alcanzado.\n";
    }
    if (visitante.cantidadPartidos < 50) {
        visitante.partidosIDs[visitante.cantidadPartidos++] = p.id;
    } else {
        cout << "[ADVERTENCIA] Límite de partidos para equipo visitante alcanzado.\n";
    }

    // Actualizar estadísticas de jugadores anotadores
    for (int i = 0; i < numGoles; ++i) {
        if (goles[i].idJugador != 0) {
            Jugador j;
            if (leerJugadorPorID(goles[i].idJugador, j)) {
                j.golesAnotados++;
                j.fechaUltimaModificacion = time(nullptr);
                if (!actualizarJugador(j)) {
                    // Si falla, deberíamos deshacer todo, pero aquí asumimos que no falla
                }
            }
        }
    }

    // Actualizar partido
    p.golesLocal = ptsLocal;
    p.golesVisitante = ptsVisitante;
    strcpy(p.estado, "JUGADO");
    p.numGoles = numGoles;
    for (int i = 0; i < numGoles; ++i) {
        p.goles[i] = goles[i];
    }
    p.fechaUltimaModificacion = time(nullptr);

    // Escribir todos los cambios (en orden)
    if (!actualizarPartido(p)) return false;
    if (!actualizarEquipo(local)) return false;
    if (!actualizarEquipo(visitante)) return false;

    return true;
}

// CANCELAR PARTIDO (con reversion) 
bool cancelarPartidoLogica(int idPartido) {
    Partido p;
    if (!leerPartidoPorID(idPartido, p)) return false;
    if (strcmp(p.estado, "CANCELADO") == 0) return false;

    // Si está jugado, revertir estadísticas
    if (strcmp(p.estado, "JUGADO") == 0) {
        Equipo local, visitante;
        if (!leerEquipoPorID(p.idEquipoLocal, local)) return false;
        if (!leerEquipoPorID(p.idEquipoVisitante, visitante)) return false;

        // Revertir estadísticas de equipos
        if (p.golesLocal > p.golesVisitante) {
            local.puntos -= 3; local.victorias--;
            visitante.derrotas--;
        } else if (p.golesLocal < p.golesVisitante) {
            visitante.puntos -= 3; visitante.victorias--;
            local.derrotas--;
        } else {
            local.puntos -= 1; local.empates--;
            visitante.puntos -= 1; visitante.empates--;
        }
        local.golesAFavor -= p.golesLocal; local.golesEnContra -= p.golesVisitante;
        visitante.golesAFavor -= p.golesVisitante; visitante.golesEnContra -= p.golesLocal;

        // Revertir goles de jugadores
        for (int i = 0; i < p.numGoles; ++i) {
            if (p.goles[i].idJugador != 0) {
                Jugador j;
                if (leerJugadorPorID(p.goles[i].idJugador, j)) {
                    j.golesAnotados--;
                    j.fechaUltimaModificacion = time(nullptr);
                    actualizarJugador(j);
                }
            }
        }

        // Actualizar equipos
        local.fechaUltimaModificacion = time(nullptr);
        visitante.fechaUltimaModificacion = time(nullptr);
        if (!actualizarEquipo(local)) return false;
        if (!actualizarEquipo(visitante)) return false;
    }

    // Cambiar estado a CANCELADO y limpiar goles
    strcpy(p.estado, "CANCELADO");
    p.golesLocal = 0;
    p.golesVisitante = 0;
    p.numGoles = 0;
    p.fechaUltimaModificacion = time(nullptr);
    return actualizarPartido(p);
}

//  LISTAR EQUIPOS ORDENADOS (tabla de posiciones) 
// Retorna en un array estático los equipos ordenados, y la cantidad
int listarEquiposOrdenados(Equipo resultados[], int maxResultados) {
    ifstream in(RUTA_EQUIPOS, ios::binary);
    if (!in) return 0;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    int count = 0;
    Equipo* temp = new Equipo[header.registrosActivos];
    for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Equipo), ios::beg);
        Equipo e;
        in.read(reinterpret_cast<char*>(&e), sizeof(Equipo));
        if (in.fail()) break;
        if (!e.eliminado) {
            temp[count++] = e;
        }
    }
    in.close();

    // Ordenar por puntos DESC, diferencia de goles DESC, goles a favor DESC
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            bool intercambiar = false;
            if (temp[j].puntos < temp[j+1].puntos) {
                intercambiar = true;
            } else if (temp[j].puntos == temp[j+1].puntos) {
                int dif1 = temp[j].golesAFavor - temp[j].golesEnContra;
                int dif2 = temp[j+1].golesAFavor - temp[j+1].golesEnContra;
                if (dif1 < dif2) intercambiar = true;
                else if (dif1 == dif2 && temp[j].golesAFavor < temp[j+1].golesAFavor) {
                    intercambiar = true;
                }
            }
            if (intercambiar) {
                Equipo aux = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = aux;
            }
        }
    }

    int copiar = (count < maxResultados) ? count : maxResultados;
    for (int i = 0; i < copiar; ++i) resultados[i] = temp[i];
    delete[] temp;
    return copiar;
}

// LISTAR TODOS LOS JUGADORES (para reportes) 
int listarTodosLosJugadores(Jugador resultados[], int maxResultados) {
    ifstream in(RUTA_JUGADORES, ios::binary);
    if (!in) return 0;
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    int count = 0;
    for (int idx = 0; idx < header.cantidadRegistros && count < maxResultados; ++idx) {
        in.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
        Jugador j;
        in.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
        if (in.fail()) break;
        if (!j.eliminado) {
            resultados[count++] = j;
        }
    }
    in.close();
    return count;
}

// 8. FUNCIONES DE MANTENIMIENTO


void verificarIntegridadReferencial() {
    cout << "\n╔══════════════════════════════════════════════════════╗\n";
    cout << "║      REPORTE DE INTEGRIDAD REFERENCIAL              ║\n";
    cout << "╠══════════════════════════════════════════════════════╣\n";

    int equiposVerificados = 0, jugadoresVerificados = 0, partidosVerificados = 0;
    int errores = 0;

    // Verificar jugadores: idEquipo debe existir en equipos
    ifstream inJ(RUTA_JUGADORES, ios::binary);
    if (inJ) {
        ArchivoHeader header;
        inJ.read(reinterpret_cast<char*>(&header), sizeof(header));
        for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
            inJ.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
            Jugador j;
            inJ.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
            if (inJ.fail()) break;
            if (!j.eliminado) {
                jugadoresVerificados++;
                if (j.idEquipo != 0) {
                    Equipo eq;
                    if (!leerEquipoPorID(j.idEquipo, eq)) {
                        cout << "ROTO: Jugador ID " << j.id << " apunta a Equipo ID " << j.idEquipo << " (no existe)\n";
                        errores++;
                    }
                } else {
                    cout << "ADVERTENCIA: Jugador ID " << j.id << " no tiene equipo asignado (idEquipo=0).\n";
                }
            }
        }
        inJ.close();
    }

    // Verificar partidos: equipos local y visitante deben existir
    ifstream inP(RUTA_PARTIDOS, ios::binary);
    if (inP) {
        ArchivoHeader header;
        inP.read(reinterpret_cast<char*>(&header), sizeof(header));
        for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
            inP.seekg(sizeof(header) + idx * sizeof(Partido), ios::beg);
            Partido p;
            inP.read(reinterpret_cast<char*>(&p), sizeof(Partido));
            if (inP.fail()) break;
            if (!p.eliminado) {
                partidosVerificados++;
                Equipo el, ev;
                if (!leerEquipoPorID(p.idEquipoLocal, el)) {
                    cout << "ROTO: Partido ID " << p.id << " tiene Local ID " << p.idEquipoLocal << " (no existe)\n";
                    errores++;
                }
                if (!leerEquipoPorID(p.idEquipoVisitante, ev)) {
                    cout << "ROTO: Partido ID " << p.id << " tiene Visitante ID " << p.idEquipoVisitante << " (no existe)\n";
                    errores++;
                }
                // Verificar goles: cada idJugador debe existir
                for (int g = 0; g < p.numGoles; ++g) {
                    if (p.goles[g].idJugador != 0) {
                        Jugador j;
                        if (!leerJugadorPorID(p.goles[g].idJugador, j)) {
                            cout << "ROTO: Gol en Partido ID " << p.id << " tiene Jugador ID " << p.goles[g].idJugador << " (no existe)\n";
                            errores++;
                        }
                    }
                }
            }
        }
        inP.close();
    }

    // Contar equipos activos
    equiposVerificados = contarEquiposActivos();

    cout << "Equipos verificados   : " << equiposVerificados << "\n";
    cout << "Jugadores verificados : " << jugadoresVerificados << "\n";
    cout << "Partidos verificados  : " << partidosVerificados << "\n";
    cout << "Referencias rotas encontradas: " << errores << "\n";
    if (errores == 0) {
        cout << "Estado: ✓ SISTEMA ÍNTEGRO\n";
    } else {
        cout << "Estado: ✗ SE ENCONTRARON INCONSISTENCIAS\n";
    }
    cout << "╚══════════════════════════════════════════════════════╝\n";
}

bool crearBackup() {
    time_t ahora = time(nullptr);
    struct tm* tiempo = localtime(&ahora);
    char nombreCarpeta[100];
    strftime(nombreCarpeta, sizeof(nombreCarpeta), "backup_%Y-%m-%d_%H-%M", tiempo);
    // Crear carpeta
#ifdef _WIN32
    string comando = "mkdir " + string(nombreCarpeta);
#else
    string comando = "mkdir -p " + string(nombreCarpeta);
#endif
    system(comando.c_str());

    const char* archivos[] = {RUTA_TORNEO, RUTA_EQUIPOS, RUTA_JUGADORES, RUTA_PARTIDOS};
    const char* nombres[] = {"torneo.bin", "equipos.bin", "jugadores.bin", "partidos.bin"};
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        ifstream src(archivos[i], ios::binary);
        if (!src) {
            cout << "No se pudo abrir " << archivos[i] << " para backup.\n";
            ok = false;
            continue;
        }
        string destPath = string(nombreCarpeta) + "/" + nombres[i];
        ofstream dst(destPath, ios::binary);
        if (!dst) {
            cout << "No se pudo crear " << destPath << "\n";
            ok = false;
            src.close();
            continue;
        }
        dst << src.rdbuf();
        src.close();
        dst.close();
    }
    if (ok) {
        cout << "Backup creado exitosamente en carpeta: " << nombreCarpeta << "\n";
    } else {
        cout << "Hubo errores durante el backup.\n";
    }
    return ok;
}


// 9. FUNCIONES DE PRESENTACIÓN (menus y utilidades)


//  Utilidades de validacion
bool esTextoValido(const char* cadena) {
    if (strlen(cadena) == 0) return false;
    for (int i = 0; cadena[i] != '\0'; i++) {
        char c = cadena[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '.')) return false;
    }
    return true;
}

bool esCedulaValida(const char* cadena) {
    int len = strlen(cadena);
    if (len < 7 || len > 10) return false;
    for (int i = 0; i < len; i++) { if (!isdigit((unsigned char)cadena[i])) return false; }
    return true;
}

bool esFechaFormatoValido(const char* cadena) {
    if (strlen(cadena) != 10) return false;
    if (cadena[4] != '-' || cadena[7] != '-') return false;
    for (int i = 0; i < 10; i++) { if (i != 4 && i != 7 && !isdigit((unsigned char)cadena[i])) return false; }
    return true;
}

void eliminarEspaciosExtremos(char* cadena) {
    int len = strlen(cadena);
    while (len > 0 && isspace((unsigned char)cadena[len - 1])) { cadena[len - 1] = '\0'; len--; }
    int inicio = 0;
    while (cadena[inicio] != '\0' && isspace((unsigned char)cadena[inicio])) inicio++;
    if (inicio > 0) memmove(cadena, cadena + inicio, strlen(cadena + inicio) + 1);
}

void convertirMinusculas(const char* origen, char* destino) {
    int i = 0;
    for (; origen[i] != '\0'; i++) destino[i] = tolower((unsigned char)origen[i]);
    destino[i] = '\0';
}

//  Mostrar equipo 
void mostrarEquipo(const Equipo& e) {
    cout << "ID: " << e.id << " | " << e.nombre << " (" << e.ciudad << ") | PTS: " << e.puntos
         << " | V: " << e.victorias << " | E: " << e.empates << " | D: " << e.derrotas
         << " | GF: " << e.golesAFavor << " | GC: " << e.golesEnContra << endl;
}

//  Mostrar jugador (ya no muestra "Agente Libre")
void mostrarJugador(const Jugador& j) {
    Equipo eq;
    char equipoNom[100] = "Sin equipo";
    if (j.idEquipo != 0 && leerEquipoPorID(j.idEquipo, eq)) {
        strcpy(equipoNom, eq.nombre);
    }
    cout << "ID: " << j.id << " | " << j.nombre << " | CI: " << j.cedula
         << " | Equipo: " << equipoNom << " | Dorsal: #" << j.numeroDorsal
         << " | Goles: " << j.golesAnotados << endl;
}

//  Mostrar partido 
void mostrarPartido(const Partido& p) {
    Equipo l, v;
    char nomL[100] = "???", nomV[100] = "???";
    if (leerEquipoPorID(p.idEquipoLocal, l)) strcpy(nomL, l.nombre);
    if (leerEquipoPorID(p.idEquipoVisitante, v)) strcpy(nomV, v.nombre);
    cout << "Partido ID: " << p.id << " [" << p.fecha << "] Estado: " << p.estado << "\n";
    cout << "   " << nomL << " " << p.golesLocal << " - " << p.golesVisitante << " " << nomV << endl;
    if (p.numGoles > 0) {
        cout << "   GOLES:\n";
        for (int i = 0; i < p.numGoles; ++i) {
            char jugadorNom[100] = "Jugador desconocido";
            if (p.goles[i].idJugador != 0) {
                Jugador j;
                if (leerJugadorPorID(p.goles[i].idJugador, j)) {
                    strcpy(jugadorNom, j.nombre);
                }
            }
            cout << "      [" << (p.goles[i].equipo == 0 ? "LOCAL" : "VISITANTE") << "] "
                 << "Min. " << p.goles[i].minuto << " - " << jugadorNom << endl;
        }
    }
}

//  Menus
void menuConfigurarTorneo() {
    Torneo t;
    cout << "==========================================\n";
    cout << "   CONFIGURACION DEL TORNEO URU           \n";
    cout << "==========================================\n";

    // Si ya existe un torneo, mostrar datos actuales y preguntar si modificar
    ifstream in(RUTA_TORNEO, ios::binary);
    if (in) {
        in.read(reinterpret_cast<char*>(&t), sizeof(Torneo));
        in.close();
        cout << "Datos actuales del torneo:\n";
        cout << "Nombre: " << t.nombre << "\nDeporte: " << t.deporte << "\nFormato: " << t.formato << "\n";
        cout << "Inicio: " << t.fechaInicio << " - Fin: " << t.fechaFin << "\n";
        char resp;
        cout << "¿Desea modificarlos? (s/n): ";
        cin >> resp;
        cin.ignore(10000, '\n');
        if (resp != 's' && resp != 'S') {
            cout << "Configuracion cancelada.\n";
            return;
        }
    }

    // Pedir nuevos datos
    do {
        cout << "Ingrese el nombre del torneo: ";
        cin.getline(t.nombre, 100);
        eliminarEspaciosExtremos(t.nombre);
    } while (strlen(t.nombre) == 0);

    do {
        cout << "Ingrese el deporte (ej. Futbol, Baloncesto, Voleibol): ";
        cin.getline(t.deporte, 50);
        eliminarEspaciosExtremos(t.deporte);
    } while (strlen(t.deporte) == 0);

    // Seleccionar formato
    int opcFormato;
    do {
        cout << "Seleccione el formato:\n";
        cout << "1. GRUPOS\n";
        cout << "2. ELIMINATORIA\n";
        cout << "Opcion: ";
        cin >> opcFormato;
        cin.ignore(10000, '\n');
    } while (opcFormato != 1 && opcFormato != 2);
    if (opcFormato == 1) strcpy(t.formato, "GRUPOS");
    else strcpy(t.formato, "ELIMINATORIA");

    do {
        cout << "Fecha de inicio (YYYY-MM-DD): ";
        cin.getline(t.fechaInicio, 11);
        eliminarEspaciosExtremos(t.fechaInicio);
    } while (!esFechaFormatoValido(t.fechaInicio));

    do {
        cout << "Fecha de fin (YYYY-MM-DD): ";
        cin.getline(t.fechaFin, 11);
        eliminarEspaciosExtremos(t.fechaFin);
    } while (!esFechaFormatoValido(t.fechaFin));

    t.fechaCreacion = time(nullptr);
    t.fechaUltimaModificacion = time(nullptr);

    ofstream out(RUTA_TORNEO, ios::binary);
    if (out) {
        out.write(reinterpret_cast<const char*>(&t), sizeof(Torneo));
        out.close();
        cout << "\nTorneo configurado exitosamente.\n";
    } else {
        cout << "Error al guardar el torneo.\n";
    }
}

void menuRegistrarEquipo() {
    char nombre[100], ciudad[100], entrenador[100];
    cout << "\n--- REGISTRAR NUEVO EQUIPO ---\n";
    do {
        cout << "Nombre del equipo: "; cin.getline(nombre, 100); eliminarEspaciosExtremos(nombre);
        Equipo aux;
        if (leerEquipoPorID(0, aux)) {} // no importa, solo para buscar duplicado, pero usamos agregarEquipo que lo verifica
    } while (!esTextoValido(nombre));
    do { cout << "Ciudad de origen: "; cin.getline(ciudad, 100); eliminarEspaciosExtremos(ciudad); } while (!esTextoValido(ciudad));
    do { cout << "Entrenador: "; cin.getline(entrenador, 100); eliminarEspaciosExtremos(entrenador); } while (!esTextoValido(entrenador));

    Equipo nuevo;
    if (agregarEquipo(nombre, ciudad, entrenador, nuevo)) {
        cout << "[SISTEMA]: Equipo guardado con exito! ID: " << nuevo.id << "\n";
    } else {
        cout << "[ERROR]: No se pudo guardar (nombre duplicado o error).\n";
    }
}

void menuBuscarEquipo() {
    int opc;
    cout << "\n--- BUSCAR EQUIPO ---\n1. Por ID\n2. Por Nombre Parcial\nSeleccione: ";
    cin >> opc; cin.ignore();
    if (opc == 1) {
        int id; cout << "ID: "; cin >> id;
        Equipo e;
        if (leerEquipoPorID(id, e)) mostrarEquipo(e);
        else cout << "[ERROR]: Equipo no encontrado.\n";
    } else if (opc == 2) {
        char criterio[100]; cout << "Texto a buscar: "; cin.getline(criterio, 100);
        Equipo resultados[MAX_RESULTADOS];
        int cant = buscarEquiposPorNombre(criterio, resultados, MAX_RESULTADOS);
        if (cant == 0) cout << "No se encontraron equipos.\n";
        else for (int i = 0; i < cant; ++i) mostrarEquipo(resultados[i]);
    }
}

// --- NUEVO MENÚ ACTUALIZAR EQUIPO CON SELECCIÓN DE CAMPO ---
void menuActualizarEquipo() {
    int id;
    cout << "\n--- ACTUALIZAR EQUIPO ---\nID del equipo: ";
    cin >> id; cin.ignore();
    Equipo e;
    if (!leerEquipoPorID(id, e)) {
        cout << "[ERROR]: ID invalido.\n";
        return;
    }

    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗\n";
        cout << "║     EDITANDO EQUIPO: " << e.nombre << " (ID " << e.id << ")" << "   ║\n";
        cout << "╠═══════════════════════════════════════════╣\n";
        cout << "║ 1. Nombre actual : " << e.nombre << "\n";
        cout << "║ 2. Ciudad actual : " << e.ciudad << "\n";
        cout << "║ 3. Entrenador    : " << e.entrenador << "\n";
        cout << "║ 0. Guardar cambios y salir               ║\n";
        cout << "╚═══════════════════════════════════════════╝\n";
        cout << "Seleccione campo a modificar (0 para terminar): ";
        cin >> opcion; cin.ignore();

        char nuevoValor[100];
        bool cambioRealizado = false;

        switch (opcion) {
            case 1:
                do {
                    cout << "Nuevo nombre: ";
                    cin.getline(nuevoValor, 100);
                    eliminarEspaciosExtremos(nuevoValor);
                } while (!esTextoValido(nuevoValor));
                // Verificar duplicado (si cambia)
                if (strcmp(e.nombre, nuevoValor) != 0) {
                    // Buscar si otro equipo tiene ese nombre
                    ifstream in(RUTA_EQUIPOS, ios::binary);
                    bool duplicado = false;
                    if (in) {
                        ArchivoHeader header;
                        in.read(reinterpret_cast<char*>(&header), sizeof(header));
                        for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
                            in.seekg(sizeof(header) + idx * sizeof(Equipo), ios::beg);
                            Equipo aux;
                            in.read(reinterpret_cast<char*>(&aux), sizeof(Equipo));
                            if (in.fail()) break;
                            if (!aux.eliminado && aux.id != id && strcmp(aux.nombre, nuevoValor) == 0) {
                                duplicado = true;
                                break;
                            }
                        }
                        in.close();
                    }
                    if (duplicado) {
                        cout << "[ERROR]: Ya existe un equipo con ese nombre.\n";
                    } else {
                        strcpy(e.nombre, nuevoValor);
                        cambioRealizado = true;
                    }
                }
                break;
            case 2:
                do {
                    cout << "Nueva ciudad: ";
                    cin.getline(nuevoValor, 100);
                    eliminarEspaciosExtremos(nuevoValor);
                } while (!esTextoValido(nuevoValor));
                strcpy(e.ciudad, nuevoValor);
                cambioRealizado = true;
                break;
            case 3:
                do {
                    cout << "Nuevo entrenador: ";
                    cin.getline(nuevoValor, 100);
                    eliminarEspaciosExtremos(nuevoValor);
                } while (!esTextoValido(nuevoValor));
                strcpy(e.entrenador, nuevoValor);
                cambioRealizado = true;
                break;
            case 0:
                break;
            default:
                cout << "[ERROR]: Opción inválida.\n";
        }

        if (cambioRealizado) {
            e.fechaUltimaModificacion = time(nullptr);
            if (actualizarEquipo(e)) {
                cout << "[SISTEMA]: Campo actualizado correctamente.\n";
            } else {
                cout << "[ERROR]: Falló la actualización.\n";
                // Releer el equipo para evitar inconsistencias
                if (!leerEquipoPorID(id, e)) {
                    cout << "[ERROR]: No se pudo recuperar el equipo.\n";
                    return;
                }
            }
        }
    } while (opcion != 0);

    cout << "[SISTEMA]: Edición finalizada.\n";
}

void menuListarEquipos() {
    Equipo resultados[MAX_RESULTADOS];
    int cant = listarEquiposOrdenados(resultados, MAX_RESULTADOS);
    if (cant == 0) { cout << "No hay equipos registrados.\n"; return; }
    cout << "\n=========================================================================\n";
    cout << "                  TABLA DE POSICIONES OFICIAL (URU)                      \n";
    cout << "=========================================================================\n";
    for (int i = 0; i < cant; ++i) {
        cout << " # " << (i+1) << " -> ";
        mostrarEquipo(resultados[i]);
    }
}

void menuEliminarEquipo() {
    int id; cout << "\n--- ELIMINAR EQUIPO ---\nID: "; cin >> id; cin.ignore();
    Equipo e;
    if (!leerEquipoPorID(id, e)) { cout << "[ERROR]: ID inexistente.\n"; return; }
    if (equipoTienePartidosActivos(id)) {
        char confirm[10];
        cout << "[ADVERTENCIA]: El equipo tiene partidos activos. Escriba 'SI' para confirmar: ";
        cin.getline(confirm, 10);
        if (strcmp(confirm, "SI") != 0) { cout << "[ABORTADO]\n"; return; }
    }
    if (eliminarEquipoLogica(id)) cout << "[SISTEMA]: Equipo removido (borrado lógico).\n";
    else cout << "[ERROR]: No se pudo eliminar.\n";
}

void menuRegistrarJugador() {
    int idEquipo, edad, dorsal; char nombre[100], cedula[20], posicion[30];
    cout << "\n--- REGISTRAR NUEVO JUGADOR ---\n";
    cout << "ID del equipo (obligatorio): ";
    cin >> idEquipo; cin.ignore();
    if (idEquipo == 0) {
        cout << "[ERROR]: Debe especificar un equipo existente (ID > 0).\n";
        return;
    }
    Equipo eq;
    if (!leerEquipoPorID(idEquipo, eq)) {
        cout << "[ERROR]: Equipo no existe.\n";
        return;
    }
    cout << "Nombre: "; cin.getline(nombre, 100); eliminarEspaciosExtremos(nombre);
    if (!esTextoValido(nombre)) { cout << "[ERROR]: Nombre invalido.\n"; return; }
    cout << "Cedula: "; cin.getline(cedula, 20); eliminarEspaciosExtremos(cedula);
    if (!esCedulaValida(cedula)) { cout << "[ERROR]: Cedula invalida.\n"; return; }
    strcpy(posicion, "JUGADOR");
    cout << "Edad (14-60): "; cin >> edad;
    cout << "Dorsal (1-99): "; cin >> dorsal; cin.ignore();
    Jugador nuevo;
    if (agregarJugador(idEquipo, nombre, cedula, posicion, edad, dorsal, nuevo)) {
        cout << "[SISTEMA]: Jugador registrado con ID " << nuevo.id << "\n";
    } else {
        cout << "[ERROR]: Cedula duplicada o dorsal ya usado en el equipo.\n";
    }
}

void menuBuscarJugador() {
    int opc; cout << "\n--- BUSCAR JUGADOR ---\n1. Por ID\n2. Por Nombre Parcial\nSeleccione: ";
    cin >> opc; cin.ignore();
    if (opc == 1) {
        int id; cout << "ID: "; cin >> id;
        Jugador j;
        if (leerJugadorPorID(id, j)) mostrarJugador(j);
        else cout << "[ERROR]: Jugador no encontrado.\n";
    } else if (opc == 2) {
        char criterio[100]; cout << "Texto: "; cin.getline(criterio, 100);
        Jugador resultados[MAX_RESULTADOS];
        int cant = buscarJugadoresPorNombre(criterio, resultados, MAX_RESULTADOS);
        if (cant == 0) cout << "Sin coincidencias.\n";
        else for (int i = 0; i < cant; ++i) mostrarJugador(resultados[i]);
    }
}

// --- NUEVO MENÚ ACTUALIZAR JUGADOR CON SELECCIÓN DE CAMPO ---
void menuActualizarJugador() {
    int id;
    cout << "\n--- ACTUALIZAR JUGADOR ---\nID del jugador: ";
    cin >> id; cin.ignore();
    Jugador j;
    if (!leerJugadorPorID(id, j)) {
        cout << "[ERROR]: ID invalido.\n";
        return;
    }

    int opcion;
    do {
        // Obtener nombre del equipo actual para mostrar
        char nombreEquipo[100] = "Sin equipo";
        if (j.idEquipo != 0) {
            Equipo eq;
            if (leerEquipoPorID(j.idEquipo, eq)) strcpy(nombreEquipo, eq.nombre);
        }

        cout << "\n╔═══════════════════════════════════════════╗\n";
        cout << "║     EDITANDO JUGADOR: " << j.nombre << " (ID " << j.id << ")" << "   ║\n";
        cout << "╠═══════════════════════════════════════════╣\n";
        cout << "║ 1. Nombre   : " << j.nombre << "\n";
        cout << "║ 2. Equipo   : " << nombreEquipo << " (ID " << j.idEquipo << ")\n";
        cout << "║ 3. Edad     : " << j.edad << "\n";
        cout << "║ 4. Dorsal   : " << j.numeroDorsal << "\n";
        cout << "║ 0. Guardar cambios y salir               ║\n";
        cout << "╚═══════════════════════════════════════════╝\n";
        cout << "Seleccione campo a modificar (0 para terminar): ";
        cin >> opcion; cin.ignore();

        bool cambioRealizado = false;

        switch (opcion) {
            case 1: {
                char nuevoNombre[100];
                do {
                    cout << "Nuevo nombre: ";
                    cin.getline(nuevoNombre, 100);
                    eliminarEspaciosExtremos(nuevoNombre);
                } while (!esTextoValido(nuevoNombre));
                strcpy(j.nombre, nuevoNombre);
                cambioRealizado = true;
                break;
            }
            case 2: {
                int nuevoEquipo;
                cout << "Nuevo ID de equipo (obligatorio): ";
                cin >> nuevoEquipo; cin.ignore();
                if (nuevoEquipo == 0) {
                    cout << "[ERROR]: El equipo no puede ser 0.\n";
                    break;
                }
                Equipo eq;
                if (!leerEquipoPorID(nuevoEquipo, eq)) {
                    cout << "[ERROR]: Equipo no existe.\n";
                    break;
                }
                // Verificar dorsal único en el nuevo equipo (si el dorsal no ha cambiado, solo si cambia de equipo)
                if (j.idEquipo != nuevoEquipo) {
                    ifstream in(RUTA_JUGADORES, ios::binary);
                    bool duplicado = false;
                    if (in) {
                        ArchivoHeader header;
                        in.read(reinterpret_cast<char*>(&header), sizeof(header));
                        for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
                            in.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
                            Jugador aux;
                            in.read(reinterpret_cast<char*>(&aux), sizeof(Jugador));
                            if (in.fail()) break;
                            if (!aux.eliminado && aux.id != id && aux.idEquipo == nuevoEquipo && aux.numeroDorsal == j.numeroDorsal) {
                                duplicado = true;
                                break;
                            }
                        }
                        in.close();
                    }
                    if (duplicado) {
                        cout << "[ERROR]: El dorsal " << j.numeroDorsal << " ya está usado en el equipo " << eq.nombre << ".\n";
                        break;
                    }
                }
                j.idEquipo = nuevoEquipo;
                cambioRealizado = true;
                break;
            }
            case 3: {
                int nuevaEdad;
                cout << "Nueva edad (14-60): ";
                cin >> nuevaEdad; cin.ignore();
                if (nuevaEdad < 14 || nuevaEdad > 60) {
                    cout << "[ERROR]: Edad fuera de rango.\n";
                    break;
                }
                j.edad = nuevaEdad;
                cambioRealizado = true;
                break;
            }
            case 4: {
                int nuevoDorsal;
                cout << "Nuevo dorsal (1-99): ";
                cin >> nuevoDorsal; cin.ignore();
                if (nuevoDorsal < 1 || nuevoDorsal > 99) {
                    cout << "[ERROR]: Dorsal fuera de rango.\n";
                    break;
                }
                // Verificar dorsal único en el mismo equipo (si el equipo no cambia)
                if (j.idEquipo != 0) {
                    ifstream in(RUTA_JUGADORES, ios::binary);
                    bool duplicado = false;
                    if (in) {
                        ArchivoHeader header;
                        in.read(reinterpret_cast<char*>(&header), sizeof(header));
                        for (int idx = 0; idx < header.cantidadRegistros; ++idx) {
                            in.seekg(sizeof(header) + idx * sizeof(Jugador), ios::beg);
                            Jugador aux;
                            in.read(reinterpret_cast<char*>(&aux), sizeof(Jugador));
                            if (in.fail()) break;
                            if (!aux.eliminado && aux.id != id && aux.idEquipo == j.idEquipo && aux.numeroDorsal == nuevoDorsal) {
                                duplicado = true;
                                break;
                            }
                        }
                        in.close();
                    }
                    if (duplicado) {
                        cout << "[ERROR]: Dorsal " << nuevoDorsal << " ya está usado en el equipo actual.\n";
                        break;
                    }
                }
                j.numeroDorsal = nuevoDorsal;
                cambioRealizado = true;
                break;
            }
            case 0:
                break;
            default:
                cout << "[ERROR]: Opción inválida.\n";
        }

        if (cambioRealizado) {
            j.fechaUltimaModificacion = time(nullptr);
            if (actualizarJugador(j)) {
                cout << "[SISTEMA]: Campo actualizado correctamente.\n";
            } else {
                cout << "[ERROR]: Falló la actualización.\n";
                // Releer el jugador para evitar inconsistencias
                if (!leerJugadorPorID(id, j)) {
                    cout << "[ERROR]: No se pudo recuperar el jugador.\n";
                    return;
                }
            }
        }
    } while (opcion != 0);

    cout << "[SISTEMA]: Edición finalizada.\n";
}

void menuListarTodosLosJugadores() {
    Jugador resultados[MAX_RESULTADOS];
    int cant = listarTodosLosJugadores(resultados, MAX_RESULTADOS);
    if (cant == 0) { cout << "No hay jugadores.\n"; return; }
    cout << "\n--- LISTADO GENERAL DE JUGADORES ---\n";
    for (int i = 0; i < cant; ++i) mostrarJugador(resultados[i]);
}

void menuListarJugadoresPorEquipo() {
    int id; cout << "\nID del equipo: "; cin >> id; cin.ignore();
    Equipo eq; if (!leerEquipoPorID(id, eq)) { cout << "[ERROR]: Equipo no existe.\n"; return; }
    Jugador resultados[MAX_RESULTADOS];
    int cant = listarJugadoresPorEquipo(id, resultados, MAX_RESULTADOS);
    cout << "\n--- PLANTILLA DE JUGADORES: " << eq.nombre << " ---\n";
    if (cant == 0) { cout << "No tiene jugadores asignados.\n"; return; }
    for (int i = 0; i < cant; ++i) mostrarJugador(resultados[i]);
}

void menuEliminarJugador() {
    int id; cout << "\n--- ELIMINAR JUGADOR ---\nID: "; cin >> id;
    if (eliminarJugadorLogica(id)) cout << "[SISTEMA]: Jugador eliminado.\n";
    else cout << "[ERROR]: ID inexistente.\n";
}

void menuProgramarPartido() {
    int loc, vis; char fecha[12];
    cout << "\n--- PROGRAMAR NUEVO PARTIDO ---\n";
    cout << "ID Local: "; cin >> loc;
    cout << "ID Visitante: "; cin >> vis; cin.ignore();
    if (loc == vis) { cout << "[ERROR]: Mismo equipo.\n"; return; }
    cout << "Fecha (YYYY-MM-DD): "; cin.getline(fecha, 12);
    Partido p;
    if (programarPartido(loc, vis, fecha, p)) {
        cout << "[SISTEMA]: Partido agendado con ID " << p.id << "\n";
    } else {
        cout << "[ERROR]: IDs inválidos o duplicado.\n";
    }
}

void menuRegistrarResultado() {
    int idPart, ptsL, ptsV;
    cout << "\n--- REGISTRAR RESULTADO ---\n";
    cout << "ID Partido: "; cin >> idPart;
    Partido p;
    if (!leerPartidoPorID(idPart, p)) { cout << "[ERROR]: Partido no existe.\n"; return; }
    if (strcmp(p.estado, "PROGRAMADO") != 0) { cout << "[ERROR]: No está programado.\n"; return; }
    cout << "Goles Local: "; cin >> ptsL;
    cout << "Goles Visitante: "; cin >> ptsV;
    int numGoles = 0;
    Gol goles[22];
    char resp;
    do {
        if (numGoles >= 22) { cout << "Límite de 22 goles alcanzado.\n"; break; }
        cout << "¿Registrar gol? (s/n): "; cin >> resp;
        if (resp != 's' && resp != 'S') break;
        int equipo, minuto, idJug;
        cout << "Equipo (0=Local, 1=Visitante): "; cin >> equipo;
        cout << "Minuto: "; cin >> minuto;
        cout << "ID Jugador (0 si desconocido): "; cin >> idJug;
        if (idJug != 0) {
            Jugador j; if (!leerJugadorPorID(idJug, j)) { cout << "Jugador no existe, se omite.\n"; continue; }
        }
        goles[numGoles].idJugador = idJug;
        goles[numGoles].minuto = minuto;
        goles[numGoles].equipo = equipo;
        numGoles++;
    } while (true);

    if (registrarResultadoPartido(idPart, ptsL, ptsV, goles, numGoles)) {
        cout << "[SISTEMA]: Resultado registrado exitosamente.\n";
    } else {
        cout << "[ERROR]: Fallo al registrar.\n";
    }
}

void menuBuscarPartido() {
    int opc; cout << "\n--- BUSCAR PARTIDO ---\n1. Por ID\n2. Por Equipo\nSeleccione: ";
    cin >> opc; cin.ignore();
    if (opc == 1) {
        int id; cout << "ID: "; cin >> id;
        Partido p;
        if (leerPartidoPorID(id, p)) mostrarPartido(p);
        else cout << "[ERROR]: No existe.\n";
    } else if (opc == 2) {
        int idEq; cout << "ID Equipo: "; cin >> idEq;
        Partido resultados[MAX_RESULTADOS];
        int cant = listarPartidosPorEquipo(idEq, resultados, MAX_RESULTADOS);
        if (cant == 0) cout << "Sin partidos.\n";
        else for (int i = 0; i < cant; ++i) mostrarPartido(resultados[i]);
    }
}

void menuListarPartidos() {
    int opc; cout << "\n--- LISTAR PARTIDOS ---\n1. Todos\n2. Solo PROGRAMADOS\nSeleccione: ";
    cin >> opc; cin.ignore();
    Partido resultados[MAX_RESULTADOS];
    int cant;
    if (opc == 2) cant = listarPartidosPorEstado("PROGRAMADO", resultados, MAX_RESULTADOS);
    else cant = listarPartidosPorEstado("", resultados, MAX_RESULTADOS); // Todos (no filtro)
    if (cant == 0) { cout << "No hay partidos.\n"; return; }
    cout << "\n--- CALENDARIO DE PARTIDOS ---\n";
    for (int i = 0; i < cant; ++i) mostrarPartido(resultados[i]);
}

void menuCancelarPartido() {
    int id; cout << "\n--- CANCELAR PARTIDO ---\nID: "; cin >> id;
    if (cancelarPartidoLogica(id)) {
        cout << "[SISTEMA]: Partido cancelado y estadísticas revertidas.\n";
    } else {
        cout << "[ERROR]: No se pudo cancelar (no existe o ya cancelado).\n";
    }
}

//  Reportes 

void reporteTablaPosiciones() {
    Equipo resultados[MAX_RESULTADOS];
    int cant = listarEquiposOrdenados(resultados, MAX_RESULTADOS);
    if (cant == 0) { cout << "No hay equipos.\n"; return; }
    cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║                  TABLA DE POSICIONES                       ║\n";
    cout << "╠════╦══════════════════════╦══════╦═══╦═══╦═══╦══════╦══════╣\n";
    cout << "║ #  ║ Equipo               ║  PTS ║ V ║ E ║ D ║  GF  ║  GC  ║\n";
    cout << "╠════╬══════════════════════╬══════╬═══╬═══╬═══╬══════╬══════╣\n";
    for (int i = 0; i < cant; ++i) {
        cout << "║ " << setw(2) << (i+1) << " ║ " << setw(20) << resultados[i].nombre
             << " ║ " << setw(4) << resultados[i].puntos
             << " ║ " << setw(2) << resultados[i].victorias
             << " ║ " << setw(2) << resultados[i].empates
             << " ║ " << setw(2) << resultados[i].derrotas
             << " ║ " << setw(4) << resultados[i].golesAFavor
             << " ║ " << setw(4) << resultados[i].golesEnContra << " ║\n";
    }
    cout << "╚════╩══════════════════════╩══════╩═══╩═══╩═══╩══════╩══════╝\n";
}

void reporteGoleadores() {
    Jugador jugadores[MAX_RESULTADOS];
    int cant = listarTodosLosJugadores(jugadores, MAX_RESULTADOS);
    // Ordenar por goles descendente
    for (int i = 0; i < cant - 1; ++i) {
        for (int j = 0; j < cant - i - 1; ++j) {
            if (jugadores[j].golesAnotados < jugadores[j+1].golesAnotados) {
                Jugador aux = jugadores[j];
                jugadores[j] = jugadores[j+1];
                jugadores[j+1] = aux;
            }
        }
    }
    int top = (cant < 10) ? cant : 10;
    cout << "\n╔══════════════════════════════════════════════════════╗\n";
    cout << "║            TABLA DE GOLEADORES (TOP 10)            ║\n";
    cout << "╠════╦══════════════════╦══════════════╦═══════╗\n";
    cout << "║ #  ║ Jugador          ║ Equipo       ║ Goles ║\n";
    cout << "╠════╬══════════════════╬══════════════╬═══════╣\n";
    for (int i = 0; i < top; ++i) {
        char equipoNom[100] = "Sin equipo";
        if (jugadores[i].idEquipo != 0) {
            Equipo eq; if (leerEquipoPorID(jugadores[i].idEquipo, eq)) strcpy(equipoNom, eq.nombre);
        }
        cout << "║ " << setw(2) << (i+1) << " ║ " << setw(16) << jugadores[i].nombre
             << " ║ " << setw(12) << equipoNom
             << " ║ " << setw(5) << jugadores[i].golesAnotados << " ║\n";
    }
    cout << "╚════╩══════════════════╩══════════════╩═══════╝\n";
}

void reporteFichaTecnica() {
    int id; cout << "ID del partido: "; cin >> id; cin.ignore();
    Partido p;
    if (!leerPartidoPorID(id, p)) { cout << "[ERROR]: Partido no existe.\n"; return; }
    Equipo loc, vis;
    char nomL[100]="???", nomV[100]="???";
    if (leerEquipoPorID(p.idEquipoLocal, loc)) strcpy(nomL, loc.nombre);
    if (leerEquipoPorID(p.idEquipoVisitante, vis)) strcpy(nomV, vis.nombre);

    cout << "\nFICHA TÉCNICA DEL PARTIDO\n";
    cout << "Partido ID : " << p.id << " Fecha: " << p.fecha << "\n";
    cout << "Estado     : " << p.estado << "\n";
    cout << nomL << " " << p.golesLocal << " - " << p.golesVisitante << " " << nomV << "\n";
    if (p.numGoles > 0) {
        cout << "GOLES:\n";
        for (int i = 0; i < p.numGoles; ++i) {
            char jugNom[100] = "Jugador desconocido";
            if (p.goles[i].idJugador != 0) {
                Jugador j; if (leerJugadorPorID(p.goles[i].idJugador, j)) strcpy(jugNom, j.nombre);
            }
            cout << "   [" << (p.goles[i].equipo == 0 ? "LOCAL" : "VISITANTE") << "] "
                 << "Min. " << p.goles[i].minuto << " - " << jugNom << "\n";
        }
    } else {
        cout << "No se registraron goles.\n";
    }
}

// 10. MAIN

int main() {
    // Inicializar archivos
    if (!inicializarArchivo(RUTA_TORNEO)) {
        cout << "Error al inicializar torneo.bin\n";
        return 1;
    }
    if (!inicializarArchivo(RUTA_EQUIPOS)) {
        cout << "Error al inicializar equipos.bin\n";
        return 1;
    }
    if (!inicializarArchivo(RUTA_JUGADORES)) {
        cout << "Error al inicializar jugadores.bin\n";
        return 1;
    }
    if (!inicializarArchivo(RUTA_PARTIDOS)) {
        cout << "Error al inicializar partidos.bin\n";
        return 1;
    }

    // Configurar torneo si es primera ejecución (si el archivo está vacío)
    ifstream test(RUTA_TORNEO, ios::binary);
    Torneo t;
    test.read(reinterpret_cast<char*>(&t), sizeof(Torneo));
    test.close();
    if (t.nombre[0] == '\0') {
        menuConfigurarTorneo();
    }

    int opcion;
    do {
        cout << "\n==========================================" << endl;
        cout << "      SISTEMA DE GESTIÓN DE TORNEOS URU   " << endl;
        // Mostrar datos del torneo desde archivo
        ifstream in(RUTA_TORNEO, ios::binary);
        if (in) {
            Torneo aux;
            in.read(reinterpret_cast<char*>(&aux), sizeof(Torneo));
            in.close();
            cout << "      Torneo: " << aux.nombre << endl;
            cout << "      Deporte: " << aux.deporte << " (" << aux.formato << ")" << endl;
        }
        cout << "==========================================" << endl;
        cout << "1. Gestion de Equipos" << endl;
        cout << "2. Gestion de Jugadores" << endl;
        cout << "3. Gestion de Partidos" << endl;
        cout << "4. Tabla de Posiciones" << endl;
        cout << "5. Reportes" << endl;
        cout << "6. Mantenimiento" << endl;
        cout << "0. Salir" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); opcion = -1; continue; }
        cin.ignore(10000, '\n');

        if (opcion == 1) {
            int sub;
            do {
                cout << "\n╔═══════════════════════════════════════════╗\n";
                cout << "║          GESTIÓN DE EQUIPOS               ║\n";
                cout << "╠═══════════════════════════════════════════╣\n";
                cout << "║  1. Registrar equipo                      ║\n";
                cout << "║  2. Buscar equipo                         ║\n";
                cout << "║  3. Actualizar equipo                     ║\n";
                cout << "║  4. Listar equipos                        ║\n";
                cout << "║  5. Eliminar equipo                       ║\n";
                cout << "║  0. Volver                                ║\n";
                cout << "╚═══════════════════════════════════════════╝\n";
                cout << "Seleccione: "; cin >> sub; cin.ignore();
                if (sub == 1) menuRegistrarEquipo();
                else if (sub == 2) menuBuscarEquipo();
                else if (sub == 3) menuActualizarEquipo();
                else if (sub == 4) menuListarEquipos();
                else if (sub == 5) menuEliminarEquipo();
            } while (sub != 0);
        } else if (opcion == 2) {
            int sub;
            do {
                cout << "\n╔═══════════════════════════════════════════╗\n";
                cout << "║        GESTIÓN DE JUGADORES               ║\n";
                cout << "╠═══════════════════════════════════════════╣\n";
                cout << "║  1. Registrar jugador                     ║\n";
                cout << "║  2. Buscar jugador                        ║\n";
                cout << "║  3. Actualizar jugador                    ║\n";
                cout << "║  4. Listar todos los jugadores            ║\n";
                cout << "║  5. Listar jugadores por equipo           ║\n";
                cout << "║  6. Eliminar jugador                      ║\n";
                cout << "║  0. Volver                                ║\n";
                cout << "╚═══════════════════════════════════════════╝\n";
                cout << "Seleccione: "; cin >> sub; cin.ignore();
                if (sub == 1) menuRegistrarJugador();
                else if (sub == 2) menuBuscarJugador();
                else if (sub == 3) menuActualizarJugador();
                else if (sub == 4) menuListarTodosLosJugadores();
                else if (sub == 5) menuListarJugadoresPorEquipo();
                else if (sub == 6) menuEliminarJugador();
            } while (sub != 0);
        } else if (opcion == 3) {
            int sub;
            do {
                cout << "\n╔═══════════════════════════════════════════╗\n";
                cout << "║        GESTIÓN DE PARTIDOS                ║\n";
                cout << "╠═══════════════════════════════════════════╣\n";
                cout << "║  1. Programar partido                     ║\n";
                cout << "║  2. Registrar resultado                   ║\n";
                cout << "║  3. Buscar partido                        ║\n";
                cout << "║  4. Listar partidos                       ║\n";
                cout << "║  5. Cancelar partido                      ║\n";
                cout << "║  0. Volver                                ║\n";
                cout << "╚═══════════════════════════════════════════╝\n";
                cout << "Seleccione: "; cin >> sub; cin.ignore();
                if (sub == 1) menuProgramarPartido();
                else if (sub == 2) menuRegistrarResultado();
                else if (sub == 3) menuBuscarPartido();
                else if (sub == 4) menuListarPartidos();
                else if (sub == 5) menuCancelarPartido();
            } while (sub != 0);
        } else if (opcion == 4) {
            reporteTablaPosiciones();
        } else if (opcion == 5) {
            int sub;
            do {
                cout << "\n╔═══════════════════════════════════════════╗\n";
                cout << "║              REPORTES                     ║\n";
                cout << "╠═══════════════════════════════════════════╣\n";
                cout << "║  1. Tabla de posiciones                   ║\n";
                cout << "║  2. Tabla de goleadores (Top 10)          ║\n";
                cout << "║  3. Ficha técnica de partido              ║\n";
                cout << "║  0. Volver                                ║\n";
                cout << "╚═══════════════════════════════════════════╝\n";
                cout << "Seleccione: "; cin >> sub; cin.ignore();
                if (sub == 1) reporteTablaPosiciones();
                else if (sub == 2) reporteGoleadores();
                else if (sub == 3) reporteFichaTecnica();
            } while (sub != 0);
        } else if (opcion == 6) {
            int sub;
            do {
                cout << "\n╔═══════════════════════════════════════════╗\n";
                cout << "║           MANTENIMIENTO                   ║\n";
                cout << "╠═══════════════════════════════════════════╣\n";
                cout << "║  1. Verificar integridad referencial      ║\n";
                cout << "║  2. Crear backup de datos                 ║\n";
                cout << "║  0. Volver                                ║\n";
                cout << "╚═══════════════════════════════════════════╝\n";
                cout << "Seleccione: "; cin >> sub; cin.ignore();
                if (sub == 1) verificarIntegridadReferencial();
                else if (sub == 2) crearBackup();
            } while (sub != 0);
        }
    } while (opcion != 0);

    cout << "\n[SISTEMA]: Programa finalizado. Los datos están guardados en disco.\n";
    return 0;
}