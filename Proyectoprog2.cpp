#include <iostream>
#include <cstring>

using namespace std;

// 1. ESTRUCTURAS DE DATOS OBLIGATORIAS Según Especificación URU)

//strc torneo
struct Torneo {
    char nombre[100];
    char deporte[50];
    char formato[20];     // "GRUPOS" o "ELIMINATORIA"
    char fechaInicio[11];  // DD-MM-YYYY
    char fechaFin[11];     // DD-MM-YYYY
};
//strc equipos
struct Equipo {
    int id;
    char nombre[100];
    char city[100];        // Ciudad de origen
    char entrenador[100];
    int puntos;
    int victorias;
    int empates;
    int derrotas;
    int puntosAFavor;
    int puntosEnContra;
    char fechaRegistro[11];
};

// estructuras de Jugador y Partido listas para cuando las usemos
//strc jugador
struct Jugador {
    int id;
    int idEquipo;          // Relación lógica mediante ID entero
    char nombre[100];
    char cedula[20];
    char posicion[20];     
    int edad;
    int numeroDorsal;      
    char fechaRegistro[11];
};

//strc partido
struct Partido {
    int id;
    int idEquipoLocal;
    int idEquipoVisitante;
    int puntosLocal;
    int puntosVisitante;
    char fecha[11];
    char estado[12];       
    char descripcion[200];
};
   //strc sistema deportivo que contiene todo el torneo, equipos, jugadores y partidos en arrays dinámicos planos directos en el Heap
struct SistemaDeportivo {
    Torneo torneo;
    
    // Arrays dinámicos planos directos en el Heap
    Equipo* equipos;
    int numEquipos;
    int capacidadEquipos;
    
    Jugador* jugadores;
    int numJugadores;
    int capacidadJugadores;
    
    Partido* partidos;
    int numPartidos;
    int capacidadPartidos;
    
    // Contadores para los IDs autoincrementales
    int siguienteIdEquipo;
    int siguienteIdJugador;
    int siguienteIdPartido;
};

//PROTOTIPOS DE LAS FUNCIONES


// Inicialización y Liberación General
void inicializarSistema(SistemaDeportivo* s, Torneo torneo);
void liberarSistema(SistemaDeportivo* s);

// Redimensionamiento Automático (Capa de Lógica)
void redimensionarEquipos(SistemaDeportivo* s);

// LÓGICA DE EQUIPOS (Estrictamente prohibido usar cout o cin aquí)
Equipo* agregarEquipo(SistemaDeportivo* s, const char* nombre, const char* ciudad, const char* entrenador);
Equipo* buscarEquipoPorID(SistemaDeportivo* s, int id);
Equipo** listarEquipos(SistemaDeportivo* s, int* cantidad);

// PRESENTACIÓN DE EQUIPOS (Aquí es donde se manejan los cin, cout y blindajes)
bool esTextoValido(const char* cadena);
void menuRegistrarEquipo(SistemaDeportivo* s);
void menuListarEquipos(SistemaDeportivo* s);
void mostrarEquipo(Equipo* e);
void mostrarListaEquipos(Equipo** equipos, int cantidad);

// 3. MAIN 


int main() {
    SistemaDeportivo sistema;
    
    // Configuramos los datos fijos del torneo para la inicialización
    Torneo liga;
    strcpy(liga.nombre, "Copa");
    strcpy(liga.deporte, "Futbol Sala");
    strcpy(liga.formato, "GRUPOS");
    strcpy(liga.fechaInicio, "01-01-2026");
    strcpy(liga.fechaFin, "01-01-2026");
    
    // Inicializamos el sistema global en el Heap con capacidad base de 4
    inicializarSistema(&sistema, liga);
    
    int opcion = -1;
    do {
        cout << "\n==========================================" << endl;
        cout << "      SISTEMA DE GESTION DE TORNEOS    " << endl;
        cout << "      Torneo: " << sistema.torneo.nombre << endl;
        cout << "==========================================" << endl;
        cout << "1. Gestion de Equipos " << endl;
        cout << "0. Salir del Programa" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[ERROR]: Seleccion invalida." << endl;
            continue;
        }
        cin.ignore();
        
        if (opcion == 1) {
            int subOpcion = -1;
            do {
                cout << "\n╔════════════════════════════════════════╗" << endl;
                cout << "║           GESTION DE EQUIPOS           ║" << endl;
                cout << "╠════════════════════════════════════════╣" << endl;
                cout << "║ 1. Registrar Equipo                    ║" << endl;
                cout << "║ 2. Listar Equipos Registrados          ║" << endl;
                cout << "║ 0. Volver al Menu Principal            ║" << endl;
                cout << "╚════════════════════════════════════════╝" << endl;
                cout << "Seleccione una opcion: ";
                cin >> subOpcion;
                cin.ignore();
                
                if (subOpcion == 1) {
                    menuRegistrarEquipo(&sistema);
                } else if (subOpcion == 2) {
                    menuListarEquipos(&sistema);
                }
            } while (subOpcion != 0);
        }
        
    } while (opcion != 0);
    
    // Liberación total de memoria antes de salir
    liberarSistema(&sistema);
    cout << "\n[SISTEMA]: Memoria del Heap liberada. Proceso cerrado con exito." << endl;
    
    return 0;
}

//DESARROLLO DE LA CAPA DE LÓGICA 

void inicializarSistema(SistemaDeportivo* s, Torneo torneo) {
    if (s == nullptr) return;
    
    s->torneo = torneo;
    
    // arranca obligatoriamente con capacidad inicial de 4
    s->capacidadEquipos = 4;
    s->equipos = new Equipo[s->capacidadEquipos];
    s->numEquipos = 0;
    
    s->capacidadJugadores = 4;
    s->jugadores = new Jugador[s->capacidadJugadores];
    s->numJugadores = 0;
    
    s->capacidadPartidos = 4;
    s->partidos = new Partido[s->capacidadPartidos];
    s->numPartidos = 0;
    
    // Los IDs comienzan siempre en 1
    s->siguienteIdEquipo = 1;
    s->siguienteIdJugador = 1;
    s->siguienteIdPartido = 1;
}

void redimensionarEquipos(SistemaDeportivo* s) {
    int nuevaCapacidad = s->capacidadEquipos * 2;
    Equipo* nuevoArray = new Equipo[nuevaCapacidad];
    
    // Copiamos los bloques de estructuras de un array al otro directamente
    for (int i = 0; i < s->numEquipos; i++) {
        nuevoArray[i] = s->equipos[i];
    }
    
    // Borramos el bloque viejo del Heap
    delete[] s->equipos;
    
    // Reenganchamos el puntero al nuevo bloque duplicado
    s->equipos = nuevoArray;
    s->capacidadEquipos = nuevaCapacidad;
}

Equipo* agregarEquipo(SistemaDeportivo* s, const char* nombre, const char* ciudad, const char* entrenador) {
    if (s == nullptr || nombre == nullptr || strlen(nombre) == 0) return nullptr;
    
    // Regla de Negocio: Evitar que registren dos equipos con el mismo nombre
    for (int i = 0; i < s->numEquipos; i++) {
        if (strcmp(s->equipos[i].nombre, nombre) == 0) {
            return nullptr; // Retorna nullptr si el nombre ya existe
        }
    }
    
    // Si se llena el arreglo plano, se redimensiona automáticamente
    if (s->numEquipos == s->capacidadEquipos) {
        redimensionarEquipos(s);
    }
    
    int indice = s->numEquipos;
    
    s->equipos[indice].id = s->siguienteIdEquipo++;
    strcpy(s->equipos[indice].nombre, nombre);
    strcpy(s->equipos[indice].city, ciudad);
    strcpy(s->equipos[indice].entrenador, entrenador);
    
    // Valores por defecto en 0
    s->equipos[indice].puntos = 0;
    s->equipos[indice].victorias = 0;
    s->equipos[indice].empates = 0;
    s->equipos[indice].derrotas = 0;
    s->equipos[indice].puntosAFavor = 0;
    s->equipos[indice].puntosEnContra = 0;
    strcpy(s->equipos[indice].fechaRegistro, "2026-06-04");
    
    s->numEquipos++;
    
    // Retornamos la dirección exacta de la estructura que acabamos de meter
    return &(s->equipos[indice]);
}

Equipo* buscarEquipoPorID(SistemaDeportivo* s, int id) {
    for (int i = 0; i < s->numEquipos; i++) {
        if (s->equipos[i].id == id) {
            return &(s->equipos[i]);
        }
    }
    return nullptr;
}

Equipo** listarEquipos(SistemaDeportivo* s, int* cantidad) {
    if (s == nullptr || s->numEquipos == 0) {
        *cantidad = 0;
        return nullptr;
    }
    
    *cantidad = s->numEquipos;
    // Generamos un array temporal de punteros que exige el enunciado
    Equipo** arrayPunteros = new Equipo*[*cantidad];
    
    for (int i = 0; i < s->numEquipos; i++) {
        arrayPunteros[i] = &(s->equipos[i]);
    }
    
    return arrayPunteros;
}

void liberarSistema(SistemaDeportivo* s) {
    if (s == nullptr) return;
    
    if (s->equipos != nullptr) {
        delete[] s->equipos;
        s->equipos = nullptr;
    }
    if (s->jugadores != nullptr) {
        delete[] s->jugadores;
        s->jugadores = nullptr;
    }
    if (s->partidos != nullptr) {
        delete[] s->partidos;
        s->partidos = nullptr;
    }
    s->numEquipos = 0;
    s->numJugadores = 0;
    s->numPartidos = 0;
}

// 5. DESARROLLO DE LA CAPA DE PRESENTACIÓN 

// Función auxiliar para validar que el texto solo contenga letras, espacios o puntos
bool esTextoValido(const char* cadena) {
    if (strlen(cadena) == 0) return false;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        char c = cadena[i];
        // Permite letras mayúsculas, minúsculas, espacios y puntos (como en F.C. Barcelona)
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '.')) {
            return false; // Retorna falso si encuentra números o símbolos raros
        }
    }
    return true;
}

void menuRegistrarEquipo(SistemaDeportivo* s) {
    char nombre[100];
    char ciudad[100];
    char entrenador[100];
    
    cout << "\n--- REGISTRAR NUEVO EQUIPO ---" << endl;
    
    // Bucle de validación para el Nombre del Equipo
    do {
        cout << "Ingrese el nombre del equipo (Solo letras): ";
        cin.getline(nombre, 100);
        if (strlen(nombre) == 0) {
            cout << "[ERROR]: El campo no puede estar vacio." << endl;
        } else if (!esTextoValido(nombre)) {
            cout << "[ERROR]: El nombre no puede contener numeros ni caracteres especiales." << endl;
        }
    } while (strlen(nombre) == 0 || !esTextoValido(nombre));

    // Bucle de validación para la Ciudad
    do {
        cout << "Ingrese la ciudad de origen (Solo letras): ";
        cin.getline(ciudad, 100);
        if (strlen(ciudad) == 0) {
            cout << "[ERROR]: El campo no puede estar vacio." << endl;
        } else if (!esTextoValido(ciudad)) {
            cout << "[ERROR]: La ciudad no puede contener numeros ni caracteres especiales." << endl;
        }
    } while (strlen(ciudad) == 0 || !esTextoValido(ciudad));

    // Bucle de validación para el Entrenador
    do {
        cout << "Ingrese el nombre del entrenador (Solo letras): ";
        cin.getline(entrenador, 100);
        if (strlen(entrenador) == 0) {
            cout << "[ERROR]: El campo no puede estar vacio." << endl;
        } else if (!esTextoValido(entrenador)) {
            cout << "[ERROR]: El nombre del entrenador no puede contener numeros ni caracteres especiales." << endl;
        }
    } while (strlen(entrenador) == 0 || !esTextoValido(entrenador));
    
    // Llamamos a la lógica pasándole los datos ya limpios y verificados
    Equipo* resultado = agregarEquipo(s, nombre, ciudad, entrenador);
    
    if (resultado == nullptr) {
        cout << "[ERROR]: No se pudo registrar. El nombre '" << nombre << "' ya existe." << endl;
    } else {
        cout << "\n[SISTEMA]: Equipo guardado con exito en la capa de logica!" << endl;
        mostrarEquipo(resultado);
    }
}

void menuListarEquipos(SistemaDeportivo* s) {
    int cantidad = 0;
    // La lógica nos da el arreglo temporal de punteros
    Equipo** lista = listarEquipos(s, &cantidad);
    
    if (lista == nullptr || cantidad == 0) {
        cout << "No hay equipos registrados en la base de datos." << endl;
    } else {
        mostrarListaEquipos(lista, cantidad);
        
        // la capa de presentación destruye el array temporal de punteros
        delete[] lista; 
    }
}

void mostrarEquipo(Equipo* e) {
    if (e == nullptr) return;
    cout << "ID: " << e->id << " | Nombre: " << e->nombre 
         << " | Ciudad: " << e->city << " | Entrenador: " << e->entrenador 
         << " | PTS: " << e->puntos << endl;
}

void mostrarListaEquipos(Equipo** equipos, int cantidad) {
    cout << "\n=======================================================" << endl;
    cout << "               LISTADO GENERAL DE EQUIPOS              " << endl;
    cout << "=======================================================" << endl;
    for (int i = 0; i < cantidad; i++) {
        cout << " # " << (i + 1) << " -> ";
        mostrarEquipo(equipos[i]);
    }
    cout << "=======================================================" << endl;
    cout << " Total de equipos en el sistema: " << cantidad << endl;
}