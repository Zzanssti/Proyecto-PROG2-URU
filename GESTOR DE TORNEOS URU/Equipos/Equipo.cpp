#include "Equipo.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>

static std::string timeToStringLocal(time_t t) {
    struct tm* tiempo = localtime(&t);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tiempo);
    return std::string(buffer);
}

// CONSTRUCTORES 

Equipo::Equipo() {
    id = 0;
    std::memset(nombre, 0, sizeof(nombre));
    std::memset(ciudad, 0, sizeof(ciudad));
    std::memset(entrenador, 0, sizeof(entrenador));
    puntos = victorias = empates = derrotas = 0;
    golesAFavor = golesEnContra = 0;
    std::memset(partidosIDs, 0, sizeof(partidosIDs));
    cantidadPartidos = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Equipo::Equipo(const char* nombre, const char* ciudad, const char* entrenador) {
    id = 0;
    setNombre(nombre);
    setCiudad(ciudad);
    setEntrenador(entrenador);
    puntos = victorias = empates = derrotas = 0;
    golesAFavor = golesEnContra = 0;
    std::memset(partidosIDs, 0, sizeof(partidosIDs));
    cantidadPartidos = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Equipo::Equipo(const Equipo& otro) {
    id = otro.id;
    std::strcpy(nombre, otro.nombre);
    std::strcpy(ciudad, otro.ciudad);
    std::strcpy(entrenador, otro.entrenador);
    puntos = otro.puntos;
    victorias = otro.victorias;
    empates = otro.empates;
    derrotas = otro.derrotas;
    golesAFavor = otro.golesAFavor;
    golesEnContra = otro.golesEnContra;
    std::memcpy(partidosIDs, otro.partidosIDs, sizeof(partidosIDs));
    cantidadPartidos = otro.cantidadPartidos;
    eliminado = otro.eliminado;
    fechaCreacion = otro.fechaCreacion;
    fechaUltimaModificacion = otro.fechaUltimaModificacion;
}

Equipo::~Equipo() {
    // No hay recursos dinámicos que liberar
    // Destructor definido por claridad y cumplir con el requerimiento del proyecto.
}

//GETTERS

int Equipo::getId() const { return id; }
const char* Equipo::getNombre() const { return nombre; }
const char* Equipo::getCiudad() const { return ciudad; }
const char* Equipo::getEntrenador() const { return entrenador; }
int Equipo::getPuntos() const { return puntos; }
int Equipo::getVictorias() const { return victorias; }
int Equipo::getEmpates() const { return empates; }
int Equipo::getDerrotas() const { return derrotas; }
int Equipo::getGolesAFavor() const { return golesAFavor; }
int Equipo::getGolesEnContra() const { return golesEnContra; }
const int* Equipo::getPartidosIDs() const { return partidosIDs; }
int Equipo::getCantidadPartidos() const { return cantidadPartidos; }
bool Equipo::getEliminado() const { return eliminado; }
time_t Equipo::getFechaCreacion() const { return fechaCreacion; }
time_t Equipo::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// SETTERS

void Equipo::setId(int id) { this->id = id; }

void Equipo::setNombre(const char* nombre) {
    if (nombre && std::strlen(nombre) > 0 && std::strlen(nombre) < 100) {
        std::strcpy(this->nombre, nombre);
    }
}

void Equipo::setCiudad(const char* ciudad) {
    if (ciudad && std::strlen(ciudad) > 0 && std::strlen(ciudad) < 100) {
        std::strcpy(this->ciudad, ciudad);
    }
}

void Equipo::setEntrenador(const char* entrenador) {
    if (entrenador && std::strlen(entrenador) > 0 && std::strlen(entrenador) < 100) {
        std::strcpy(this->entrenador, entrenador);
    }
}

bool Equipo::setPuntos(int pts) {
    if (pts < 0) return false;
    puntos = pts;
    return true;
}

bool Equipo::setVictorias(int v) {
    if (v < 0) return false;
    victorias = v;
    return true;
}

bool Equipo::setEmpates(int e) {
    if (e < 0) return false;
    empates = e;
    return true;
}

bool Equipo::setDerrotas(int d) {
    if (d < 0) return false;
    derrotas = d;
    return true;
}

bool Equipo::setGolesAFavor(int gf) {
    if (gf < 0) return false;
    golesAFavor = gf;
    return true;
}

bool Equipo::setGolesEnContra(int gc) {
    if (gc < 0) return false;
    golesEnContra = gc;
    return true;
}

void Equipo::agregarPartidoID(int idPartido) {
    if (cantidadPartidos < 50) {
        partidosIDs[cantidadPartidos++] = idPartido;
    } else {
        // Podríamos lanzar excepcion o ignorar, pero por ahora solo ignoramos
        // en el proyecto original se mostraba advertencia, pero aqui no hay cout
    }
}

void Equipo::setEliminado(bool estado) { eliminado = estado; }
void Equipo::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }
void Equipo::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

// METODOS DE NEGOCIO 

bool Equipo::esValido() const {
    return (std::strlen(nombre) > 0 && std::strlen(ciudad) > 0 &&
            std::strlen(entrenador) > 0 && !eliminado);
}

void Equipo::actualizarEstadisticas(int golesFavor, int golesContra, int resultado) {
    golesAFavor += golesFavor;
    golesEnContra += golesContra;
    if (resultado > 0) {
        victorias++;
        puntos += 3;
    } else if (resultado == 0) {
        empates++;
        puntos += 1;
    } else {
        derrotas++;
    }
}

void Equipo::mostrarBasico() const {
    std::cout << "ID: " << id << " | " << nombre << " (" << ciudad << ") | PTS: " << puntos
              << " | V: " << victorias << " | E: " << empates << " | D: " << derrotas
              << " | GF: " << golesAFavor << " | GC: " << golesEnContra;
}

void Equipo::mostrarCompleto() const {
    std::cout << "\n========== FICHA DEL EQUIPO ==========" << std::endl;
    std::cout << "ID           : " << id << std::endl;
    std::cout << "Nombre       : " << nombre << std::endl;
    std::cout << "Ciudad       : " << ciudad << std::endl;
    std::cout << "Entrenador   : " << entrenador << std::endl;
    std::cout << "Puntos       : " << puntos << std::endl;
    std::cout << "Victorias    : " << victorias << std::endl;
    std::cout << "Empates      : " << empates << std::endl;
    std::cout << "Derrotas     : " << derrotas << std::endl;
    std::cout << "Goles a favor: " << golesAFavor << std::endl;
    std::cout << "Goles en contra: " << golesEnContra << std::endl;
    std::cout << "Partidos jugados: " << cantidadPartidos << std::endl;
    std::cout << "Eliminado    : " << (eliminado ? "Sí" : "No") << std::endl;
    std::cout << "Creado       : " << timeToStringLocal(fechaCreacion) << std::endl;
    std::cout << "Última modif.: " << timeToStringLocal(fechaUltimaModificacion) << std::endl;
    std::cout << "======================================" << std::endl;
}

size_t Equipo::obtenerTamano() {
    return sizeof(Equipo);
}