#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

// 1. ESTRUCTURAS DE DATOS OBLIGATORIAS 

struct Torneo {
    char nombre[100];
    char deporte[50];
    char formato[20];     // "GRUPOS" o "ELIMINATORIA"
    char fechaInicio[11];  // YYYY-MM-DD
    char fechaFin[11];     // YYYY-MM-DD
};

struct Equipo {
    int id;
    char nombre[100];
    char city[100];        
    char entrenador[100];
    int puntos;
    int victorias;
    int empates;
    int derrotas;
    int puntosAFavor;
    int puntosEnContra;
    char fechaRegistro[11];
};

struct Jugador {
    int id;
    int idEquipo;          // Relación lógica (0 significa Agente Libre)
    char nombre[100];
    char cedula[20];
    char posicion[30];     
    int edad;
    int numeroDorsal;      
    char fechaRegistro[11];
};

struct Partido {
    int id;
    int idEquipoLocal;
    int idEquipoVisitante;
    int puntosLocal;
    int puntosVisitante;
    char fecha[11];
    char estado[15];       // "PROGRAMADO", "FINALIZADO" o "CANCELADO"
    char descripcion[200];
};

struct SistemaDeportivo {
    Torneo torneo;
    
    Equipo* equipos;
    int numEquipos;
    int capacidadEquipos;
    
    Jugador* jugadores;
    int numJugadores;
    int capacidadJugadores;
    
    Partido* partidos;
    int numPartidos;
    int capacidadPartidos;
    
    int siguienteIdEquipo;
    int siguienteIdJugador;
    int siguienteIdPartido;
};


// 2. PROTOTIPOS DE LAS FUNCIONES

void inicializarSistema(SistemaDeportivo* s, Torneo torneo);
void liberarSistema(SistemaDeportivo* s);

void redimensionarEquipos(SistemaDeportivo* s);
void redimensionarJugadores(SistemaDeportivo* s);
void redimensionarPartidos(SistemaDeportivo* s);

// Lógica de Negocio (Pura: Sin cin ni cout)
Equipo* agregarEquipo(SistemaDeportivo* s, const char* nombre, const char* ciudad, const char* entrenador);
Equipo* buscarEquipoPorID(SistemaDeportivo* s, int id);
Equipo* buscarEquipoPorNombre(SistemaDeportivo* s, const char* nombre);
Equipo** buscarEquiposPorNombreParcial(SistemaDeportivo* s, const char* subcadena, int* cantidad);
bool actualizarEquipoLogica(SistemaDeportivo* s, int id, const char* nombre, const char* ciudad, const char* entrenador);
Equipo** listarEquipos(SistemaDeportivo* s, int* cantidad); // Incluye ordenamiento oficial
bool eliminarEquipoLogica(SistemaDeportivo* s, int idEquipo);

Jugador* agregarJugador(SistemaDeportivo* s, int idEquipo, const char* nombre, const char* cedula, const char* posicion, int edad, int numeroDorsal);
bool cedulaExiste(SistemaDeportivo* s, const char* cedula);
Jugador* buscarJugadorPorID(SistemaDeportivo* s, int id);
Jugador** buscarJugadoresPorNombreParcial(SistemaDeportivo* s, const char* subcadena, int* cantidad);
bool actualizarJugadorLogica(SistemaDeportivo* s, int id, int nuevoIdEquipo, const char* nombre, const char* posicion, int edad, int dorsal);
Jugador** listarTodosLosJugadores(SistemaDeportivo* s, int* cantidad);
Jugador** listarJugadoresPorEquipo(SistemaDeportivo* s, int idequipo, int* cantidad);
bool eliminarJugadorLogica(SistemaDeportivo* s, int idJugador);

Partido* registrarPartido(SistemaDeportivo* s, int idLocal, int idVisitante, const char* fecha);
Partido* buscarPartidoPorID(SistemaDeportivo* s, int id);
bool partidoYaExiste(SistemaDeportivo* s, int idLocal, int idVisitante);
bool equipoTienePartidos(SistemaDeportivo* s, int idEquipo);
bool registrarResultadoPartido(SistemaDeportivo* s, int idPartido, int ptsLocal, int ptsVisitante);
bool cancelarPartidoLogica(SistemaDeportivo* s, int idPartido);
Partido** listarPartidos(SistemaDeportivo* s, int* cantidad);
Partido** buscarPartidosPorEquipo(SistemaDeportivo* s, int idEquipo, int* cantidad);
Partido** listarPartidosProgramados(SistemaDeportivo* s, int* cantidad);

// Capa de Presentación e Interfaz de Usuario
bool esTextoValido(const char* cadena);
bool esCedulaValida(const char* cadena);
bool esFechaFormatoValido(const char* cadena);
void eliminarEspaciosExtremos(char* cadena);
void convertirMinusculas(const char* origen, char* destino);
void configurarTorneo(Torneo* t);

// Menús e Interfaces Oficiales de Equipos
void menuRegistrarEquipo(SistemaDeportivo* s);
void menuBuscarEquipo(SistemaDeportivo* s);
void menuActualizarEquipo(SistemaDeportivo* s);
void menuListarEquipos(SistemaDeportivo* s);
void menuEliminarEquipo(SistemaDeportivo* s);
void mostrarEquipo(Equipo* e);

// Menús e Interfaces Oficiales de Jugadores
void menuRegistrarJugador(SistemaDeportivo* s);
void menuBuscarJugador(SistemaDeportivo* s);
void menuActualizarJugador(SistemaDeportivo* s);
void menuListarTodosLosJugadores(SistemaDeportivo* s);
void menuListarJugadoresPorEquipo(SistemaDeportivo* s);
void menuEliminarJugador(SistemaDeportivo* s);

// Menús e Interfaces Oficiales de Partidos
void menuProgramarPartido(SistemaDeportivo* s);
void menuRegistrarResultado(SistemaDeportivo* s);
void menuBuscarPartido(SistemaDeportivo* s);
void menuListarPartidos(SistemaDeportivo* s);
void menuCancelarPartido(SistemaDeportivo* s);


// 3. MAIN 


int main() {
    SistemaDeportivo sistema;
    Torneo ligaURU;
    
    configurarTorneo(&ligaURU);
    inicializarSistema(&sistema, ligaURU);
    
    int opcion = -1;
    do {
        cout << "\n==========================================" << endl;
        cout << "      SISTEMA DE GESTIÓN DE TORNEOS URU   " << endl;
        cout << "      Torneo: " << sistema.torneo.nombre << endl;
        cout << "      Deporte: " << sistema.torneo.deporte << " (" << sistema.torneo.formato << ")" << endl;
        cout << "==========================================" << endl;
        cout << "1. Gestion de Equipos " << endl;
        cout << "2. Gestion de Jugadores " << endl;
        cout << "3. Gestion de Partidos " << endl;
        cout << "0. Salir del Programa y Liberar Memoria" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        if (cin.fail()) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "\n[ERROR 1]: Entrada invalida. Introduzca un numero entero." << endl;
            opcion = -1;
            continue;
        }
        cin.ignore(10000, '\n');
        
        if (opcion == 1) {
            int subOpcion = -1;
            do {
                cout << "\n╔═══════════════════════════════════════════╗" << endl;
                cout << "║          GESTIÓN DE EQUIPOS               ║" << endl;
                cout << "╠═══════════════════════════════════════════╣" << endl;
                cout << "║  1. Registrar equipo                      ║" << endl;
                cout << "║  2. Buscar equipo                         ║" << endl;
                cout << "║  3. Actualizar equipo                     ║" << endl;
                cout << "║  4. Listar equipos                        ║" << endl;
                cout << "║  5. Eliminar equipo                       ║" << endl;
                cout << "║  0. Volver al menú principal              ║" << endl;
                cout << "╚═══════════════════════════════════════════╝" << endl;
                cout << "Seleccione una opcion: ";
                cin >> subOpcion;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); subOpcion = -1; continue; }
                cin.ignore(10000, '\n');
                
                if (subOpcion == 1) menuRegistrarEquipo(&sistema);
                else if (subOpcion == 2) menuBuscarEquipo(&sistema);
                else if (subOpcion == 3) menuActualizarEquipo(&sistema);
                else if (subOpcion == 4) menuListarEquipos(&sistema);
                else if (subOpcion == 5) menuEliminarEquipo(&sistema);
            } while (subOpcion != 0);
        } 
        else if (opcion == 2) {
            int subOpcion = -1;
            do {
                cout << "\n╔═══════════════════════════════════════════╗" << endl;
                cout << "║        GESTIÓN DE JUGADORES               ║" << endl;
                cout << "╠═══════════════════════════════════════════╣" << endl;
                cout << "║  1. Registrar jugador                     ║" << endl;
                cout << "║  2. Buscar jugador                        ║" << endl;
                cout << "║  3. Actualizar jugador                    ║" << endl;
                cout << "║  4. Listar jugadores (todos)              ║" << endl;
                cout << "║  5. Listar jugadores por equipo           ║" << endl;
                cout << "║  6. Eliminar jugador                      ║" << endl;
                cout << "║  0. Volver al menú principal              ║" << endl;
                cout << "╚═══════════════════════════════════════════╝" << endl;
                cout << "Seleccione una opcion: ";
                cin >> subOpcion;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); subOpcion = -1; continue; }
                cin.ignore(10000, '\n');
                
                if (subOpcion == 1) menuRegistrarJugador(&sistema);
                else if (subOpcion == 2) menuBuscarJugador(&sistema);
                else if (subOpcion == 3) menuActualizarJugador(&sistema);
                else if (subOpcion == 4) menuListarTodosLosJugadores(&sistema);
                else if (subOpcion == 5) menuListarJugadoresPorEquipo(&sistema);
                else if (subOpcion == 6) menuEliminarJugador(&sistema);
            } while (subOpcion != 0);
        }
        else if (opcion == 3) {
            int subOpcion = -1;
            do {
                cout << "\n╔═══════════════════════════════════════════╗" << endl;
                cout << "║        GESTIÓN DE PARTIDOS                ║" << endl;
                cout << "╠═══════════════════════════════════════════╣" << endl;
                cout << "║  1. Programar partido                     ║" << endl;
                cout << "║  2. Registrar resultado                   ║" << endl;
                cout << "║  3. Buscar partido                        ║" << endl;
                cout << "║  4. Listar partidos                       ║" << endl;
                cout << "║  5. Cancelar partido                      ║" << endl;
                cout << "║  0. Volver al menú principal              ║" << endl;
                cout << "╚═══════════════════════════════════════════╝" << endl;
                cout << "Seleccione una opcion: ";
                cin >> subOpcion;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); subOpcion = -1; continue; }
                cin.ignore(10000, '\n');
                
                if (subOpcion == 1) menuProgramarPartido(&sistema);
                else if (subOpcion == 2) menuRegistrarResultado(&sistema);
                else if (subOpcion == 3) menuBuscarPartido(&sistema);
                else if (subOpcion == 4) menuListarPartidos(&sistema);
                else if (subOpcion == 5) menuCancelarPartido(&sistema);
            } while (subOpcion != 0);
        }
    } while (opcion != 0);
    
    liberarSistema(&sistema);
    cout << "\n[SISTEMA]: Memoria del Heap liberada. Proceso cerrado con exito." << endl;
    return 0;
}

// 
// 4. DESARROLLO DE LA CAPA DE LÓGICA Gestión del Heap

void inicializarSistema(SistemaDeportivo* s, Torneo torneo) {
    if (s == nullptr) return;
    s->torneo = torneo;
    s->capacidadEquipos = 4;
    s->equipos = new Equipo[s->capacidadEquipos];
    s->numEquipos = 0;
    
    s->capacidadJugadores = 4;
    s->jugadores = new Jugador[s->capacidadJugadores];
    s->numJugadores = 0;
    
    s->capacidadPartidos = 4;
    s->partidos = new Partido[s->capacidadPartidos];
    s->numPartidos = 0;
    
    s->siguienteIdEquipo = 1;
    s->siguienteIdJugador = 1;
    s->siguienteIdPartido = 1;
}

void redimensionarEquipos(SistemaDeportivo* s) {
    int nuevaCapacidad = s->capacidadEquipos * 2;
    Equipo* nuevoArray = new Equipo[nuevaCapacidad];
    for (int i = 0; i < s->numEquipos; i++) { nuevoArray[i] = s->equipos[i]; }
    delete[] s->equipos;
    s->equipos = nuevoArray;
    s->capacidadEquipos = nuevaCapacidad;
}

void redimensionarJugadores(SistemaDeportivo* s) {
    int nuevaCapacidad = s->capacidadJugadores * 2;
    Jugador* nuevoArray = new Jugador[nuevaCapacidad];
    for (int i = 0; i < s->numJugadores; i++) { nuevoArray[i] = s->jugadores[i]; }
    delete[] s->jugadores;
    s->jugadores = nuevoArray;
    s->capacidadJugadores = nuevaCapacidad;
}

void redimensionarPartidos(SistemaDeportivo* s) {
    int nuevaCapacidad = s->capacidadPartidos * 2;
    Partido* nuevoArray = new Partido[nuevaCapacidad];
    for (int i = 0; i < s->numPartidos; i++) { nuevoArray[i] = s->partidos[i]; }
    delete[] s->partidos;
    s->partidos = nuevoArray;
    s->capacidadPartidos = nuevaCapacidad;
}

Equipo* buscarEquipoPorNombre(SistemaDeportivo* s, const char* nombre) {
    for (int i = 0; i < s->numEquipos; i++) {
        if (strcmp(s->equipos[i].nombre, nombre) == 0) return &(s->equipos[i]);
    }
    return nullptr;
}

Equipo** buscarEquiposPorNombreParcial(SistemaDeportivo* s, const char* subcadena, int* cantidad) {
    if (s == nullptr || s->numEquipos == 0 || subcadena == nullptr || strlen(subcadena) == 0) {
        *cantidad = 0; return nullptr;
    }
    char subMin[100];
    convertirMinusculas(subcadena, subMin);
    
    int contador = 0;
    for (int i = 0; i < s->numEquipos; i++) {
        char nombreMin[100];
        convertirMinusculas(s->equipos[i].nombre, nombreMin);
        if (strstr(nombreMin, subMin) != nullptr) contador++;
    }
    
    *cantidad = contador;
    if (contador == 0) return nullptr;
    
    Equipo** resultado = new Equipo*[contador];
    int idx = 0;
    for (int i = 0; i < s->numEquipos; i++) {
        char nombreMin[100];
        convertirMinusculas(s->equipos[i].nombre, nombreMin);
        if (strstr(nombreMin, subMin) != nullptr) {
            resultado[idx++] = &(s->equipos[i]);
        }
    }
    return resultado;
}

Equipo* agregarEquipo(SistemaDeportivo* s, const char* nombre, const char* ciudad, const char* entrenador) {
    if (s == nullptr || nombre == nullptr || strlen(nombre) == 0) return nullptr;
    if (buscarEquipoPorNombre(s, nombre) != nullptr) return nullptr; // Validación de duplicados (CU 6)
    
    if (s->numEquipos == s->capacidadEquipos) redimensionarEquipos(s);
    
    int indice = s->numEquipos;
    s->equipos[indice].id = s->siguienteIdEquipo++;
    strcpy(s->equipos[indice].nombre, nombre);
    strcpy(s->equipos[indice].city, ciudad);
    strcpy(s->equipos[indice].entrenador, entrenador);
    s->equipos[indice].puntos = 0;
    s->equipos[indice].victorias = 0;
    s->equipos[indice].empates = 0;
    s->equipos[indice].derrotas = 0;
    s->equipos[indice].puntosAFavor = 0;
    s->equipos[indice].puntosEnContra = 0;
    strcpy(s->equipos[indice].fechaRegistro, "2026-06-08");
    
    s->numEquipos++;
    return &(s->equipos[indice]);
}

Equipo* buscarEquipoPorID(SistemaDeportivo* s, int id) {
    for (int i = 0; i < s->numEquipos; i++) {
        if (s->equipos[i].id == id) return &(s->equipos[i]);
    }
    return nullptr;
}

bool actualizarEquipoLogica(SistemaDeportivo* s, int id, const char* nombre, const char* ciudad, const char* entrenador) {
    Equipo* eq = buscarEquipoPorID(s, id);
    if (eq == nullptr) return false;
    
    if (strcmp(eq->nombre, nombre) != 0) {
        if (buscarEquipoPorNombre(s, nombre) != nullptr) return false;
    }
    
    strcpy(eq->nombre, nombre);
    strcpy(eq->city, ciudad);
    strcpy(eq->entrenador, entrenador);
    return true;
}

// Listar Equipos con Ordenamiento
Equipo** listarEquipos(SistemaDeportivo* s, int* cantidad) {
    if (s == nullptr || s->numEquipos == 0) { *cantidad = 0; return nullptr; }
    *cantidad = s->numEquipos;
    Equipo** arrayPunteros = new Equipo*[*cantidad];
    for (int i = 0; i < s->numEquipos; i++) { arrayPunteros[i] = &(s->equipos[i]); }
    
    // Algoritmo de Burbuja con criterios encadenados de desempate
    for (int i = 0; i < *cantidad - 1; i++) {
        for (int j = 0; j < *cantidad - i - 1; j++) {
            bool intercambiar = false;
            int puntosJ = arrayPunteros[j]->puntos;
            int puntosJ1 = arrayPunteros[j+1]->puntos;
            
            if (puntosJ < puntosJ1) {
                intercambiar = true;
            } else if (puntosJ == puntosJ1) {
                int difJ = arrayPunteros[j]->puntosAFavor - arrayPunteros[j]->puntosEnContra;
                int difJ1 = arrayPunteros[j+1]->puntosAFavor - arrayPunteros[j+1]->puntosEnContra;
                
                if (difJ < difJ1) {
                    intercambiar = true;
                } else if (difJ == difJ1) {
                    if (arrayPunteros[j]->puntosAFavor < arrayPunteros[j+1]->puntosAFavor) {
                        intercambiar = true;
                    }
                }
            }
            if (intercambiar) {
                Equipo* temp = arrayPunteros[j];
                arrayPunteros[j] = arrayPunteros[j+1];
                arrayPunteros[j+1] = temp;
            }
        }
    }
    return arrayPunteros;
}

bool eliminarEquipoLogica(SistemaDeportivo* s, int idEquipo) {
    int idx = -1;
    for (int i = 0; i < s->numEquipos; i++) {
        if (s->equipos[i].id == idEquipo) { idx = i; break; }
    }
    if (idx == -1) return false;

    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].idEquipo == idEquipo) s->jugadores[i].idEquipo = 0; 
    }

    for (int i = idx; i < s->numEquipos - 1; i++) {
        s->equipos[i] = s->equipos[i + 1];
    }
    s->numEquipos--;
    return true;
}

bool cedulaExiste(SistemaDeportivo* s, const char* cedula) {
    for (int i = 0; i < s->numJugadores; i++) {
        if (strcmp(s->jugadores[i].cedula, cedula) == 0) return true;
    }
    return false;
}

Jugador* agregarJugador(SistemaDeportivo* s, int idEquipo, const char* nombre, const char* cedula, const char* posicion, int edad, int numeroDorsal) {
    if (s == nullptr || nombre == nullptr || strlen(nombre) == 0) return nullptr;
    if (idEquipo != 0 && buscarEquipoPorID(s, idEquipo) == nullptr) return nullptr;
    if (cedulaExiste(s, cedula)) return nullptr; // Validación de Cédula única (CU 6)

    // Validación de Dorsal único por equipo (CU 6)
    if (idEquipo != 0) {
        for (int i = 0; i < s->numJugadores; i++) {
            if (s->jugadores[i].idEquipo == idEquipo && s->jugadores[i].numeroDorsal == numeroDorsal) return nullptr;
        }
    }
    
    if (s->numJugadores == s->capacidadJugadores) redimensionarJugadores(s);

    int idx = s->numJugadores;
    s->jugadores[idx].id = s->siguienteIdJugador++;
    s->jugadores[idx].idEquipo = idEquipo;
    strcpy(s->jugadores[idx].nombre, nombre);
    strcpy(s->jugadores[idx].cedula, cedula);
    strcpy(s->jugadores[idx].posicion, posicion);
    s->jugadores[idx].edad = edad;
    s->jugadores[idx].numeroDorsal = numeroDorsal;
    strcpy(s->jugadores[idx].fechaRegistro, "2026-06-08");
    s->numJugadores++;
    return &(s->jugadores[idx]);
}

Jugador* buscarJugadorPorID(SistemaDeportivo* s, int id) {
    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].id == id) return &(s->jugadores[i]);
    }
    return nullptr;
}

Jugador** buscarJugadoresPorNombreParcial(SistemaDeportivo* s, const char* subcadena, int* cantidad) {
    if (s == nullptr || s->numJugadores == 0 || subcadena == nullptr || strlen(subcadena) == 0) {
        *cantidad = 0; return nullptr;
    }
    char subMin[100];
    convertirMinusculas(subcadena, subMin);
    
    int contador = 0;
    for (int i = 0; i < s->numJugadores; i++) {
        char nombreMin[100];
        convertirMinusculas(s->jugadores[i].nombre, nombreMin);
        if (strstr(nombreMin, subMin) != nullptr) contador++;
    }
    
    *cantidad = contador;
    if (contador == 0) return nullptr;
    
    Jugador** resultado = new Jugador*[contador];
    int idx = 0;
    for (int i = 0; i < s->numJugadores; i++) {
        char nombreMin[100];
        convertirMinusculas(s->jugadores[i].nombre, nombreMin);
        if (strstr(nombreMin, subMin) != nullptr) {
            resultado[idx++] = &(s->jugadores[i]);
        }
    }
    return resultado;
}

bool actualizarJugadorLogica(SistemaDeportivo* s, int id, int nuevoIdEquipo, const char* nombre, const char* posicion, int edad, int dorsal) {
    Jugador* j = buscarJugadorPorID(s, id);
    if (j == nullptr) return false;
    if (nuevoIdEquipo != 0 && buscarEquipoPorID(s, nuevoIdEquipo) == nullptr) return false;

    for (int i = 0; i < s->numJugadores; i++) {
        if (nuevoIdEquipo != 0 && s->jugadores[i].idEquipo == nuevoIdEquipo && s->jugadores[i].numeroDorsal == dorsal && s->jugadores[i].id != id) {
            return false;
        }
    }

    j->idEquipo = nuevoIdEquipo;
    strcpy(j->nombre, nombre);
    strcpy(j->posicion, posicion);
    j->edad = edad;
    j->numeroDorsal = dorsal;
    return true;
}

Jugador** listarTodosLosJugadores(SistemaDeportivo* s, int* cantidad) {
    if (s == nullptr || s->numJugadores == 0) { *cantidad = 0; return nullptr; }
    *cantidad = s->numJugadores;
    Jugador** array = new Jugador*[*cantidad];
    for (int i = 0; i < s->numJugadores; i++) { array[i] = &(s->jugadores[i]); }
    return array;
}

Jugador** listarJugadoresPorEquipo(SistemaDeportivo* s, int idequipo, int* cantidad) {
    if (s == nullptr || s->numJugadores == 0) { *cantidad = 0; return nullptr; }
    int contador = 0;
    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].idEquipo == idequipo) contador++;
    }
    *cantidad = contador;
    if (contador == 0) return nullptr;
    
    Jugador** arrayPunteros = new Jugador*[*cantidad];
    int idxRef = 0;
    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].idEquipo == idequipo) arrayPunteros[idxRef++] = &(s->jugadores[i]);
    }
    return arrayPunteros;
}

bool eliminarJugadorLogica(SistemaDeportivo* s, int idJugador) {
    int idx = -1;
    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].id == idJugador) { idx = i; break; }
    }
    if (idx == -1) return false;

    for (int i = idx; i < s->numJugadores - 1; i++) {
        s->jugadores[i] = s->jugadores[i + 1];
    }
    s->numJugadores--;
    return true;
}

bool partidoYaExiste(SistemaDeportivo* s, int idLocal, int idVisitante) {
    for (int i = 0; i < s->numPartidos; i++) {
        if (strcmp(s->partidos[i].estado, "CANCELADO") == 0) continue;
        if ((s->partidos[i].idEquipoLocal == idLocal && s->partidos[i].idEquipoVisitante == idVisitante) ||
            (s->partidos[i].idEquipoLocal == idVisitante && s->partidos[i].idEquipoVisitante == idLocal)) {
            return true;
        }
    }
    return false;
}

bool equipoTienePartidos(SistemaDeportivo* s, int idEquipo) {
    for (int i = 0; i < s->numPartidos; i++) {
        if (strcmp(s->partidos[i].estado, "CANCELADO") == 0) continue;
        if (s->partidos[i].idEquipoLocal == idEquipo || s->partidos[i].idEquipoVisitante == idEquipo) return true;
    }
    return false;
}

Partido* registrarPartido(SistemaDeportivo* s, int idLocal, int idVisitante, const char* fecha) {
    if (s == nullptr || idLocal == idVisitante) return nullptr; // Rechazar contra sí mismo (CU 6)
    if (buscarEquipoPorID(s, idLocal) == nullptr || buscarEquipoPorID(s, idVisitante) == nullptr) return nullptr;
    if (partidoYaExiste(s, idLocal, idVisitante)) return nullptr; // Rechazar cruce duplicado (CU 2 / CU 6)
    
    if (s->numPartidos == s->capacidadPartidos) redimensionarPartidos(s);
    
    int idx = s->numPartidos;
    s->partidos[idx].id = s->siguienteIdPartido++;
    s->partidos[idx].idEquipoLocal = idLocal;
    s->partidos[idx].idEquipoVisitante = idVisitante;
    s->partidos[idx].puntosLocal = 0;
    s->partidos[idx].puntosVisitante = 0;
    strcpy(s->partidos[idx].fecha, fecha);
    strcpy(s->partidos[idx].estado, "PROGRAMADO");
    strcpy(s->partidos[idx].descripcion, "Fase General");
    
    s->numPartidos++;
    return &(s->partidos[idx]);
}

Partido* buscarPartidoPorID(SistemaDeportivo* s, int id) {
    for (int i = 0; i < s->numPartidos; i++) {
        if (s->partidos[i].id == id) return &(s->partidos[i]);
    }
    return nullptr;
}

bool registrarResultadoPartido(SistemaDeportivo* s, int idPartido, int ptsLocal, int ptsVisitante) {
    if (s == nullptr || ptsLocal < 0 || ptsVisitante < 0) return false;
    
    Partido* p = buscarPartidoPorID(s, idPartido);
    if (p == nullptr || strcmp(p->estado, "FINALIZADO") == 0 || strcmp(p->estado, "CANCELADO") == 0) return false; // CU 6
    
    p->puntosLocal = ptsLocal;
    p->puntosVisitante = ptsVisitante;
    strcpy(p->estado, "FINALIZADO");
    
    Equipo* loc = buscarEquipoPorID(s, p->idEquipoLocal);
    Equipo* vis = buscarEquipoPorID(s, p->idEquipoVisitante);
    
    if (loc != nullptr && vis != nullptr) {
        loc->puntosAFavor += ptsLocal; loc->puntosEnContra += ptsVisitante;
        vis->puntosAFavor += ptsVisitante; vis->puntosEnContra += ptsLocal;
        if (ptsLocal > ptsVisitante) { loc->puntos += 3; loc->victorias++; vis->derrotas++; } 
        else if (ptsLocal < ptsVisitante) { vis->puntos += 3; vis->victorias++; loc->derrotas++; } 
        else { loc->puntos += 1; vis->puntos += 1; loc->empates++; vis->empates++; }
    }
    return true;
}

// Reversión Absoluta de Estadísticas 
bool cancelarPartidoLogica(SistemaDeportivo* s, int idPartido) {
    Partido* p = buscarPartidoPorID(s, idPartido);
    if (p == nullptr || strcmp(p->estado, "CANCELADO") == 0) return false;
    
    if (strcmp(p->estado, "FINALIZADO") == 0) {
        Equipo* loc = buscarEquipoPorID(s, p->idEquipoLocal);
        Equipo* vis = buscarEquipoPorID(s, p->idEquipoVisitante);
        
        if (loc != nullptr && vis != nullptr) {
            loc->puntosAFavor -= p->puntosLocal; loc->puntosEnContra -= p->puntosVisitante;
            vis->puntosAFavor -= p->puntosVisitante; vis->puntosEnContra -= p->puntosLocal;
            
            if (p->puntosLocal > p->puntosVisitante) { loc->puntos -= 3; loc->victorias--; vis->derrotas--; } 
            else if (p->puntosLocal < p->puntosVisitante) { vis->puntos -= 3; vis->victorias--; loc->derrotas--; } 
            else { loc->puntos -= 1; vis->puntos -= 1; loc->empates--; vis->empates--; }
        }
    }
    strcpy(p->estado, "CANCELADO");
    return true;
}

// Búsquedas y listados dinámicos en el Heap
Partido** listarPartidos(SistemaDeportivo* s, int* cantidad) {
    if (s == nullptr || s->numPartidos == 0) { *cantidad = 0; return nullptr; }
    *cantidad = s->numPartidos;
    Partido** arr = new Partido*[*cantidad];
    for (int i = 0; i < s->numPartidos; i++) { arr[i] = &(s->partidos[i]); }
    return arr;
}

Partido** buscarPartidosPorEquipo(SistemaDeportivo* s, int idEquipo, int* cantidad) {
    if (s == nullptr || s->numPartidos == 0) { *cantidad = 0; return nullptr; }
    int contador = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (s->partidos[i].idEquipoLocal == idEquipo || s->partidos[i].idEquipoVisitante == idEquipo) contador++;
    }
    *cantidad = contador;
    if (contador == 0) return nullptr;
    Partido** resultado = new Partido*[contador];
    int idx = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (s->partidos[i].idEquipoLocal == idEquipo || s->partidos[i].idEquipoVisitante == idEquipo) {
            resultado[idx++] = &(s->partidos[i]);
        }
    }
    return resultado;
}

Partido** listarPartidosProgramados(SistemaDeportivo* s, int* cantidad) {
    if (s == nullptr || s->numPartidos == 0) { *cantidad = 0; return nullptr; }
    int contador = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (strcmp(s->partidos[i].estado, "PROGRAMADO") == 0) contador++;
    }
    *cantidad = contador;
    if (contador == 0) return nullptr;
    Partido** resultado = new Partido*[contador];
    int idx = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (strcmp(s->partidos[i].estado, "PROGRAMADO") == 0) {
            resultado[idx++] = &(s->partidos[i]);
        }
    }
    return resultado;
}

void liberarSistema(SistemaDeportivo* s) {
    if (s == nullptr) return;
    if (s->equipos != nullptr) delete[] s->equipos;
    if (s->jugadores != nullptr) delete[] s->jugadores;
    if (s->partidos != nullptr) delete[] s->partidos;
}


// 5. DESARROLLO DE LA CAPA DE PRESENTACIÓN (Interfaz y Validaciones)


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

void configurarTorneo(Torneo* t) {
    if (t == nullptr) return;
    cout << "==========================================" << endl;
    cout << "   CONFIGURACION INICIAL DEL TORNEO URU   " << endl;
    cout << "==========================================" << endl;
    do {
        cout << "Ingrese el nombre del torneo: "; cin.getline(t->nombre, 100); eliminarEspaciosExtremos(t->nombre);
    } while (strlen(t->nombre) == 0);
    strcpy(t->deporte, "Inter-Escuelas");
    strcpy(t->formato, "GRUPOS");
    strcpy(t->fechaInicio, "2026-06-08");
    strcpy(t->fechaFin, "2026-07-30");
}


//GESTIÓN DE EQUIPOS


void menuRegistrarEquipo(SistemaDeportivo* s) {
    char nombre[100], ciudad[100], entrenador[100];
    cout << "\n--- REGISTRAR NUEVO EQUIPO ---" << endl;
    do { 
        cout << "Nombre del equipo: "; cin.getline(nombre, 100); eliminarEspaciosExtremos(nombre);
        if (buscarEquipoPorNombre(s, nombre) != nullptr) { cout << "[ERROR]: El equipo ya existe (Nombre Duplicado)." << endl; nombre[0] = '\0'; }
    } while (!esTextoValido(nombre));
    do { cout << "Ciudad de origen: "; cin.getline(ciudad, 100); eliminarEspaciosExtremos(ciudad); } while (!esTextoValido(ciudad));
    do { cout << "Entrenador: "; cin.getline(entrenador, 100); eliminarEspaciosExtremos(entrenador); } while (!esTextoValido(entrenador));
    
    if (agregarEquipo(s, nombre, ciudad, entrenador) != nullptr) cout << "[SISTEMA]: Equipo guardado con exito!" << endl;
}

void menuBuscarEquipo(SistemaDeportivo* s) {
    int opc; cout << "\n--- BUSCAR EQUIPO ---\n1. Por ID\n2. Por Nombre Parcial (Coincidencia)\nSeleccione: "; cin >> opc;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return; }
    cin.ignore(10000, '\n');

    if (opc == 1) {
        int id; cout << "ID del equipo: "; cin >> id;
        Equipo* e = buscarEquipoPorID(s, id);
        if (e == nullptr) cout << "[ERROR]: Equipo no encontrado." << endl;
        else mostrarEquipo(e);
    } else if (opc == 2) {
        char criterio[100]; cout << "Texto a buscar (ej. 'mara'): "; cin.getline(criterio, 100);
        int cant = 0; Equipo** res = buscarEquiposPorNombreParcial(s, criterio, &cant);
        if (res == nullptr) cout << "No se encontraron equipos." << endl;
        else {
            for (int i = 0; i < cant; i++) mostrarEquipo(res[i]);
            delete[] res;
        }
    }
}

void menuActualizarEquipo(SistemaDeportivo* s) {
    int id; char nombre[100], ciudad[100], entrenador[100];
    cout << "\n--- ACTUALIZAR EQUIPO ---" << endl;
    cout << "ID del equipo a modificar: "; cin >> id;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return; }
    cin.ignore(10000, '\n');
    
    if (buscarEquipoPorID(s, id) == nullptr) { cout << "[ERROR]: ID invalido." << endl; return; }
    do { cout << "Nuevo Nombre: "; cin.getline(nombre, 100); eliminarEspaciosExtremos(nombre); } while (!esTextoValido(nombre));
    do { cout << "Nueva Ciudad: "; cin.getline(ciudad, 100); eliminarEspaciosExtremos(ciudad); } while (!esTextoValido(ciudad));
    do { cout << "Nuevo Entrenador: "; cin.getline(entrenador, 100); eliminarEspaciosExtremos(entrenador); } while (!esTextoValido(entrenador));
    
    if (actualizarEquipoLogica(s, id, nombre, ciudad, entrenador)) cout << "[SISTEMA]: Datos actualizados." << endl;
    else cout << "[ERROR]: Nombre de equipo ya ocupado." << endl;
}

void menuListarEquipos(SistemaDeportivo* s) {
    int cantidad = 0;
    Equipo** lista = listarEquipos(s, &cantidad);
    if (lista == nullptr || cantidad == 0) { cout << "No hay equipos registrados." << endl; } 
    else {
        cout << "\n=========================================================================" << endl;
        cout << "                  TABLA DE POSICIONES OFICIAL (URU)                      " << endl;
        cout << "=========================================================================" << endl;
        for (int i = 0; i < cantidad; i++) { cout << " # " << (i+1) << " -> "; mostrarEquipo(lista[i]); }
        delete[] lista;
    }
}

void menuEliminarEquipo(SistemaDeportivo* s) {
    int idEquipo; cout << "\n--- ELIMINAR EQUIPO ---" << endl;
    cout << "ID del equipo a eliminar: "; cin >> idEquipo;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return; }
    cin.ignore(10000, '\n');

    Equipo* eq = buscarEquipoPorID(s, idEquipo);
    if (eq == nullptr) { cout << "[ERROR]: ID inexistente." << endl; return; }

    if (equipoTienePartidos(s, idEquipo)) { // Advertencia de integridad referencial (CU 6)
        char confirmacion[10];
        cout << "[ADVERTENCIA]: El equipo tiene partidos agendados. Escriba 'SI' para confirmar: ";
        cin.getline(confirmacion, 10);
        if (strcmp(confirmacion, "SI") != 0) { cout << "[ABORTADO]: Operacion cancelada." << endl; return; }
    }
    if (eliminarEquipoLogica(s, idEquipo)) cout << "[SISTEMA]: Equipo removido del sistema." << endl;
}

void mostrarEquipo(Equipo* e) {
    if (e == nullptr) return;
    cout << "ID: " << e->id << " | " << e->nombre << " (" << e->city << ") | PTS: " << e->puntos 
         << " | V: " << e->victorias << " | E: " << e->empates << " | D: " << e->derrotas 
         << " | AF: " << e->puntosAFavor << " | EC: " << e->puntosEnContra << endl;
}

//GESTIÓN DE JUGADORES

void menuRegistrarJugador(SistemaDeportivo* s) {
    int idEquipo, edad, dorsal; char nombre[100], cedula[20], posicion[30];
    cout << "\n--- REGISTRAR NUEVO JUGADOR ---" << endl;
    
    cout << "ID del equipo a asociar (0 para Agente Libre / Escriba '-1' para CANCELAR): "; cin >> idEquipo;
    if (cin.fail() || idEquipo == -1) { cin.clear(); cin.ignore(10000, '\n'); cout << "[SISTEMA]: Registro Cancelado." << endl; return; } // CU 7
    if (idEquipo != 0 && buscarEquipoPorID(s, idEquipo) == nullptr) { cout << "[ERROR]: ID de equipo no existe." << endl; return; }
    cin.ignore(10000, '\n');

    cout << "Nombre completo (O digite 'CANCELAR'): "; cin.getline(nombre, 100); eliminarEspaciosExtremos(nombre);
    if (strcmp(nombre, "CANCELAR") == 0) { cout << "[SISTEMA]: Registro Cancelado." << endl; return; } // CU 7
    if (!esTextoValido(nombre)) { cout << "[ERROR]: Nombre invalido." << endl; return; }

    cout << "Cedula: "; cin.getline(cedula, 20); eliminarEspaciosExtremos(cedula);
    if (strcmp(cedula, "CANCELAR") == 0) { cout << "[SISTEMA]: Registro Cancelado." << endl; return; }
    if (!esCedulaValida(cedula)) { cout << "[ERROR]: Formato de cedula incorrecto." << endl; return; }
    if (cedulaExiste(s, cedula)) { cout << "[ERROR]: Esta cedula ya se encuentra registrada." << endl; return; } // CU 6
    
    strcpy(posicion, "JUGADOR");
    cout << "Edad (14-60): "; cin >> edad;
    cout << "Dorsal (1-99): "; cin >> dorsal;
    cin.ignore(10000, '\n');

    if (agregarJugador(s, idEquipo, nombre, cedula, posicion, edad, dorsal) != nullptr) {
        cout << "[SISTEMA]: Jugador registrado de forma exitosa!" << endl;
    } else {
        cout << "[ERROR]: El numero de dorsal ya esta en uso por este equipo." << endl; // CU 6
    }
}

void menuBuscarJugador(SistemaDeportivo* s) {
    int opc; cout << "\n--- BUSCAR JUGADOR ---\n1. Por ID\n2. Por Coincidencia de Nombre (Parcial)\nSeleccione: "; cin >> opc;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return; }
    cin.ignore(10000, '\n');

    if (opc == 1) {
        int id; cout << "Ingrese ID: "; cin >> id;
        Jugador* j = buscarJugadorPorID(s, id);
        if (j == nullptr) cout << "[ERROR]: Jugador no encontrado." << endl;
        else {
            Equipo* eq = buscarEquipoPorID(s, j->idEquipo);
            cout << "ID: " << j->id << " | Nombre: " << j->nombre << " | CI: " << j->cedula << " | Equipo: " << (eq ? eq->nombre : "Agente Libre") << endl;
        }
    } else if (opc == 2) {
        char criterio[100]; cout << "Texto a buscar (ej. 'car'): "; cin.getline(criterio, 100); // CU 5
        int cant = 0; Jugador** res = buscarJugadoresPorNombreParcial(s, criterio, &cant);
        if (res == nullptr) cout << "Sin coincidencias en el sistema." << endl;
        else {
            for (int i = 0; i < cant; i++) {
                Equipo* eq = buscarEquipoPorID(s, res[i]->idEquipo);
                cout << "ID: " << res[i]->id << " | " << res[i]->nombre << " | CI: " << res[i]->cedula << " | Equipo: " << (eq ? eq->nombre : "Agente Libre") << endl;
            }
            delete[] res;
        }
    }
}

void menuActualizarJugador(SistemaDeportivo* s) {
    int id, nuevoEq, edad, dorsal; char nombre[100], posicion[30];
    cout << "\n--- ACTUALIZAR JUGADOR ---" << endl;
    cout << "ID del jugador a modificar: "; cin >> id;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return; }
    cin.ignore(10000, '\n');

    if (buscarJugadorPorID(s, id) == nullptr) { cout << "[ERROR]: ID invalido." << endl; return; }
    do { cout << "Nuevo Nombre: "; cin.getline(nombre, 100); eliminarEspaciosExtremos(nombre); } while (!esTextoValido(nombre));
    strcpy(posicion, "JUGADOR");
    cout << "Nuevo ID Equipo asignado: "; cin >> nuevoEq;
    cout << "Nueva Edad: "; cin >> edad;
    cout << "Nuevo Dorsal: "; cin >> dorsal;
    cin.ignore(10000, '\n');

    if (actualizarJugadorLogica(s, id, nuevoEq, nombre, posicion, edad, dorsal)) cout << "[SISTEMA]: Datos actualizados." << endl;
    else cout << "[ERROR]: Modificacion denegada (Dorsal duplicado o equipo inexistente)." << endl;
}

void menuListarTodosLosJugadores(SistemaDeportivo* s) {
    int cant = 0; Jugador** lista = listarTodosLosJugadores(s, &cant);
    if (lista == nullptr) { cout << "No hay jugadores registrados." << endl; return; }
    cout << "\n--- LISTADO GENERAL DE JUGADORES ---" << endl;
    for (int i = 0; i < cant; i++) {
        Equipo* eq = buscarEquipoPorID(s, lista[i]->idEquipo);
        cout << "ID: " << lista[i]->id << " | " << lista[i]->nombre << " | CI: " << lista[i]->cedula << " | Equipo: " << (eq ? eq->nombre : "Agente Libre") << endl;
    }
    delete[] lista;
}

void menuListarJugadoresPorEquipo(SistemaDeportivo* s) {
    int idEquipo; cout << "\nID del equipo a consultar: "; cin >> idEquipo;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return; }
    
    Equipo* eq = buscarEquipoPorID(s, idEquipo);
    if (eq == nullptr) { cout << "[ERROR]: Equipo no encontrado." << endl; return; }
    
    int cantidad = 0; Jugador** lista = listarJugadoresPorEquipo(s, idEquipo, &cantidad);
    cout << "\n--- PLANTILLA DE JUGADORES: " << eq->nombre << " ---" << endl; // Muestra nombre de equipo (CU 1)
    if (lista == nullptr) { cout << "Este equipo no posee jugadores asignados." << endl; } 
    else {
        for (int i = 0; i < cantidad; i++) {
            cout << "ID: " << lista[i]->id << " | " << lista[i]->nombre << " | CI: " << lista[i]->cedula << " | Dorsal: #" << lista[i]->numeroDorsal << endl;
        }
        delete[] lista;
    }
}

void menuEliminarJugador(SistemaDeportivo* s) {
    int id; cout << "\n--- ELIMINAR JUGADOR ---" << endl;
    cout << "ID del jugador a remover: "; cin >> id;
    if (eliminarJugadorLogica(s, id)) cout << "[SISTEMA]: Jugador eliminado con éxito." << endl;
    else cout << "[ERROR]: ID inexistente." << endl;
}

// VISTAS: GESTIÓN DE PARTIDOS

void menuProgramarPartido(SistemaDeportivo* s) {
    int loc, vis; char fecha[12];
    cout << "\n--- PROGRAMAR NUEVO PARTIDO ---" << endl;
    cout << "ID Equipo Local: "; cin >> loc; 
    cout << "ID Equipo Visitante: "; cin >> vis; 
    cin.ignore(10000, '\n');
    
    if (loc == vis) { cout << "[ERROR]: Un equipo no puede jugar contra si mismo." << endl; return; } // CU 6
    if (partidoYaExiste(s, loc, vis)) { cout << "[ERROR]: Ya existe un partido agendado entre ambos equipos." << endl; return; } // CU 2
    
    cout << "Fecha del encuentro (YYYY-MM-DD): "; cin.getline(fecha, 12);
    if (registrarPartido(s, loc, vis, fecha) != nullptr) {
        cout << "[SISTEMA]: Partido agendado en estado 'PROGRAMADO'." << endl;
    } else {
        cout << "[ERROR]: Uno de los IDs ingresados no existe." << endl;
    }
}

void menuRegistrarResultado(SistemaDeportivo* s) {
    int idPart, pLoc, pVis;
    cout << "\n--- REGISTRAR RESULTADO DE PARTIDO ---" << endl;
    cout << "ID del Partido: "; cin >> idPart; 
    cout << "Puntos Local: "; cin >> pLoc; 
    cout << "Puntos Visitante: "; cin >> pVis;
    
    if (registrarResultadoPartido(s, idPart, pLoc, pVis)) {
        cout << "[SISTEMA]: Marcador almacenado y tabla de posiciones actualizada." << endl;
    } else {
        cout << "[ERROR]: Accion denegada. El partido no existe o ya ha sido JUGADO/CANCELADO." << endl; // CU 6
    }
}

void menuBuscarPartido(SistemaDeportivo* s) {
    int opc; cout << "\n--- BUSCAR PARTIDO ---\n1. Por ID de Partido\n2. Por ID de un Equipo (Local/Visitante)\nSeleccione: "; cin >> opc;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return; }
    cin.ignore(10000, '\n');

    if (opc == 1) {
        int id; cout << "ID del partido: "; cin >> id;
        Partido* p = buscarPartidoPorID(s, id);
        if (p == nullptr) { cout << "[ERROR]: Partido no encontrado." << endl; return; }
        Equipo* l = buscarEquipoPorID(s, p->idEquipoLocal);
        Equipo* v = buscarEquipoPorID(s, p->idEquipoVisitante);
        cout << "ID: " << p->id << " [" << p->fecha << "] (" << p->estado << ")\n   " << (l?l->nombre:"???") << " " << p->puntosLocal << " - " << p->puntosVisitante << " " << (v?v->nombre:"???") << endl;
    } else if (opc == 2) {
        int idEq; cout << "ID del equipo: "; cin >> idEq; // CU 5
        int cant = 0; Partido** lista = buscarPartidosPorEquipo(s, idEq, &cant);
        if (lista == nullptr) cout << "No se registran partidos para este equipo." << endl;
        else {
            for (int i = 0; i < cant; i++) {
                Equipo* l = buscarEquipoPorID(s, lista[i]->idEquipoLocal);
                Equipo* v = buscarEquipoPorID(s, lista[i]->idEquipoVisitante);
                cout << "ID: " << lista[i]->id << " [" << lista[i]->fecha << "] (" << lista[i]->estado << ") -> " << (l?l->nombre:"???") << " vs " << (v?v->nombre:"???") << endl;
            }
            delete[] lista;
        }
    }
}

void menuListarPartidos(SistemaDeportivo* s) {
    int opc; cout << "\n--- LISTAR PARTIDOS ---\n1. Ver Calendario General\n2. Ver Unicamente Partidos PROGRAMADOS\nSeleccione: "; cin >> opc;
    if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return; }
    cin.ignore(10000, '\n');

    int cant = 0; Partido** lista = nullptr;
    if (opc == 2) {
        lista = listarPartidosProgramados(s, &cant); // CU 5
        cout << "\n--- PARTIDOS PROGRAMADOS (PENDIENTES) ---" << endl;
    } else {
        lista = listarPartidos(s, &cant);
        cout << "\n--- CALENDARIO GENERAL DE ENCUENTROS ---" << endl;
    }

    if (lista == nullptr) { cout << "No hay partidos que listar bajo ese criterio." << endl; return; }
    for (int i = 0; i < cant; i++) {
        Equipo* l = buscarEquipoPorID(s, lista[i]->idEquipoLocal);
        Equipo* v = buscarEquipoPorID(s, lista[i]->idEquipoVisitante);
        cout << "Partido ID: " << lista[i]->id << " [" << lista[i]->fecha << "] Status: (" << lista[i]->estado << ")\n";
        cout << "   " << (l ? l->nombre : "???") << " (" << lista[i]->puntosLocal << ") vs (" << lista[i]->puntosVisitante << ") " << (v ? v->nombre : "???") << "\n\n";
    }
    delete[] lista;
}

void menuCancelarPartido(SistemaDeportivo* s) {
    int idPart; cout << "\n--- CANCELAR ENCUENTRO OFICIAL ---" << endl;
    cout << "ID del partido a cancelar: "; cin >> idPart;
    cin.ignore(10000, '\n');

    if (cancelarPartidoLogica(s, idPart)) { // Revierte estadísticas automáticamente
        cout << "[SISTEMA]: El partido fue CANCELADO con exito y las estadisticas fueron revertidas." << endl;
    } else {
        cout << "[ERROR]: No se pudo procesar. El partido no existe o ya habia sido cancelado." << endl;
    }
}