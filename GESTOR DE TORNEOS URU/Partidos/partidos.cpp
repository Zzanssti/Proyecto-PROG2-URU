#include "Partido.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>

// --- Constructores ---

Partido::Partido() {
    id = 0;
    idEquipoLocal = 0;
    idEquipoVisitante = 0;
    std::memset(fecha, 0, sizeof(fecha));
    std::strcpy(estado, "PROGRAMADO");
    std::memset(descripcion, 0, sizeof(descripcion));
    std::strcpy(descripcion, "Fase General");
    golesLocal = 0;
    golesVisitante = 0;
    numGoles = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
    // El arreglo de goles se inicializa automáticamente con el constructor por defecto de Gol
}

Partido::Partido(int idLocal, int idVisitante, const char* fecha,
                 const char* estado, const char* descripcion) {
    this->id = 0;
    setIdEquipoLocal(idLocal);
    setIdEquipoVisitante(idVisitante);
    setFecha(fecha);
    setEstado(estado);
    setDescripcion(descripcion);
    golesLocal = 0;
    golesVisitante = 0;
    numGoles = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Partido::Partido(const Partido& otro) {
    id = otro.id;
    idEquipoLocal = otro.idEquipoLocal;
    idEquipoVisitante = otro.idEquipoVisitante;
    std::strcpy(fecha, otro.fecha);
    std::strcpy(estado, otro.estado);
    std::strcpy(descripcion, otro.descripcion);
    golesLocal = otro.golesLocal;
    golesVisitante = otro.golesVisitante;
    numGoles = otro.numGoles;
    // Copiar el arreglo de goles
    for (int i = 0; i < numGoles; ++i) {
        goles[i] = otro.goles[i];
    }
    // Los goles restantes (si los hay) no se copian, pero no importa porque numGoles controla los válidos
    eliminado = otro.eliminado;
    fechaCreacion = otro.fechaCreacion;
    fechaUltimaModificacion = otro.fechaUltimaModificacion;
}

Partido::~Partido() {
    // No hay recursos dinamicos, el arreglo de goles se destruye automaticamente
}

// Getters

int Partido::getId() const { return id; }
int Partido::getIdEquipoLocal() const { return idEquipoLocal; }
int Partido::getIdEquipoVisitante() const { return idEquipoVisitante; }
const char* Partido::getFecha() const { return fecha; }
const char* Partido::getEstado() const { return estado; }
const char* Partido::getDescripcion() const { return descripcion; }
int Partido::getGolesLocal() const { return golesLocal; }
int Partido::getGolesVisitante() const { return golesVisitante; }
const Gol* Partido::getGoles() const { return goles; }
int Partido::getNumGoles() const { return numGoles; }
bool Partido::getEliminado() const { return eliminado; }
time_t Partido::getFechaCreacion() const { return fechaCreacion; }
time_t Partido::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// --- Setters con validaciones ---

void Partido::setId(int id) { this->id = id; }

bool Partido::setIdEquipoLocal(int id) {
    if (id <= 0) return false;
    idEquipoLocal = id;
    return true;
}

bool Partido::setIdEquipoVisitante(int id) {
    if (id <= 0) return false;
    idEquipoVisitante = id;
    return true;
}

void Partido::setFecha(const char* fecha) {
    // Validación básica: debe tener 10 caracteres con guiones en posiciones 4 y 7
    if (fecha && std::strlen(fecha) == 10 && fecha[4] == '-' && fecha[7] == '-') {
        std::strcpy(this->fecha, fecha);
    }
    // Si no es válido, no se modifica
}

bool Partido::setEstado(const char* estado) {
    if (estado && (std::strcmp(estado, "PROGRAMADO") == 0 ||
                   std::strcmp(estado, "JUGADO") == 0 ||
                   std::strcmp(estado, "CANCELADO") == 0)) {
        std::strcpy(this->estado, estado);
        return true;
    }
    return false;
}

void Partido::setDescripcion(const char* desc) {
    if (desc && std::strlen(desc) > 0 && std::strlen(desc) < 200) {
        std::strcpy(this->descripcion, desc);
    }
}

bool Partido::setGolesLocal(int goles) {
    if (goles < 0) return false;
    golesLocal = goles;
    return true;
}

bool Partido::setGolesVisitante(int goles) {
    if (goles < 0) return false;
    golesVisitante = goles;
    return true;
}

bool Partido::agregarGol(const Gol& gol) {
    if (numGoles >= 22) return false;
    if (!gol.esValido()) return false;
    goles[numGoles++] = gol;
    return true;
}

void Partido::setEliminado(bool estado) { eliminado = estado; }
void Partido::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }
void Partido::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

//Metodos de negocio 

bool Partido::esValido() const {
    return (idEquipoLocal > 0 && idEquipoVisitante > 0 &&
            idEquipoLocal != idEquipoVisitante &&
            std::strlen(fecha) == 10 &&
            (std::strcmp(estado, "PROGRAMADO") == 0 ||
             std::strcmp(estado, "JUGADO") == 0 ||
             std::strcmp(estado, "CANCELADO") == 0));
}

bool Partido::estaJugado() const {
    return std::strcmp(estado, "JUGADO") == 0;
}

bool Partido::estaProgramado() const {
    return std::strcmp(estado, "PROGRAMADO") == 0;
}

bool Partido::estaCancelado() const {
    return std::strcmp(estado, "CANCELADO") == 0;
}

//Metodos de presentacion

void Partido::mostrarBasico() const {
    std::cout << "Partido ID: " << id << " | " << fecha
              << " | " << estado << " | " << idEquipoLocal
              << " vs " << idEquipoVisitante
              << " (" << golesLocal << "-" << golesVisitante << ")";
}

void Partido::mostrarCompleto() const {
    std::cout << "\n╔══════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              FICHA DEL PARTIDO                     ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║ ID              : " << std::setw(30) << id << " ║" << std::endl;
    std::cout << "║ Local ID        : " << std::setw(30) << idEquipoLocal << " ║" << std::endl;
    std::cout << "║ Visitante ID    : " << std::setw(30) << idEquipoVisitante << " ║" << std::endl;
    std::cout << "║ Fecha           : " << std::setw(30) << fecha << " ║" << std::endl;
    std::cout << "║ Estado          : " << std::setw(30) << estado << " ║" << std::endl;
    std::cout << "║ Descripción     : " << std::setw(30) << descripcion << " ║" << std::endl;
    std::cout << "║ Goles Local     : " << std::setw(30) << golesLocal << " ║" << std::endl;
    std::cout << "║ Goles Visitante : " << std::setw(30) << golesVisitante << " ║" << std::endl;
    std::cout << "║ Número de goles : " << std::setw(30) << numGoles << " ║" << std::endl;
    if (numGoles > 0) {
        std::cout << "║ Detalle de goles: " << std::setw(30) << " " << " ║" << std::endl;
        for (int i = 0; i < numGoles; ++i) {
            std::cout << "║   ";
            goles[i].mostrar();
            std::cout << std::setw(30 - (int)std::strlen("Jugador ID: X | Minuto: X | Equipo: X")) 
                      << " " << " ║" << std::endl;  // Ajuste de alineacion
        }
    }
    std::cout << "║ Eliminado       : " << std::setw(30) << (eliminado ? "Sí" : "No") << " ║" << std::endl;
    std::cout << "║ Creado          : " << std::setw(30) << ctime(&fechaCreacion) << " ║" << std::endl;
    std::cout << "║ Última modif.   : " << std::setw(30) << ctime(&fechaUltimaModificacion) << " ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════╝" << std::endl;
}

// Metodo estatico

size_t Partido::obtenerTamano() {
    return sizeof(Partido);
}