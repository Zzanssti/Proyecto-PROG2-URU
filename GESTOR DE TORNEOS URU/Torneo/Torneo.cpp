#include "Torneo.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cctype>

// Constructores

Torneo::Torneo() {
    std::memset(nombre, 0, sizeof(nombre));
    std::memset(deporte, 0, sizeof(deporte));
    std::memset(formato, 0, sizeof(formato));
    std::memset(fechaInicio, 0, sizeof(fechaInicio));
    std::memset(fechaFin, 0, sizeof(fechaFin));
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Torneo::Torneo(const char* nombre, const char* deporte, const char* formato,
               const char* fechaInicio, const char* fechaFin) {
    setNombre(nombre);
    setDeporte(deporte);
    setFormato(formato);
    setFechaInicio(fechaInicio);
    setFechaFin(fechaFin);
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Torneo::Torneo(const Torneo& otro) {
    std::strcpy(nombre, otro.nombre);
    std::strcpy(deporte, otro.deporte);
    std::strcpy(formato, otro.formato);
    std::strcpy(fechaInicio, otro.fechaInicio);
    std::strcpy(fechaFin, otro.fechaFin);
    fechaCreacion = otro.fechaCreacion;
    fechaUltimaModificacion = otro.fechaUltimaModificacion;
}

Torneo::~Torneo() {
    // No hay recursos dinamicos que liberar
}

// Getters 

const char* Torneo::getNombre() const { return nombre; }
const char* Torneo::getDeporte() const { return deporte; }
const char* Torneo::getFormato() const { return formato; }
const char* Torneo::getFechaInicio() const { return fechaInicio; }
const char* Torneo::getFechaFin() const { return fechaFin; }
time_t Torneo::getFechaCreacion() const { return fechaCreacion; }
time_t Torneo::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

//Setters con validaciones

void Torneo::setNombre(const char* nombre) {
    if (nombre && std::strlen(nombre) > 0 && std::strlen(nombre) < 100) {
        std::strcpy(this->nombre, nombre);
    }
    // Si no es valido, no se modifica
}

void Torneo::setDeporte(const char* deporte) {
    if (deporte && std::strlen(deporte) > 0 && std::strlen(deporte) < 50) {
        std::strcpy(this->deporte, deporte);
    }
}

bool Torneo::setFormato(const char* formato) {
    if (formato && (std::strcmp(formato, "GRUPOS") == 0 ||
                    std::strcmp(formato, "ELIMINATORIA") == 0)) {
        std::strcpy(this->formato, formato);
        return true;
    }
    return false;
}

// Funcion auxiliar para validar fecha YYYY-MM-DD
static bool esFechaValida(const char* fecha) {
    if (!fecha || std::strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(fecha[i])) return false;
    }
    return true;
}

bool Torneo::setFechaInicio(const char* fecha) {
    if (esFechaValida(fecha)) {
        std::strcpy(fechaInicio, fecha);
        return true;
    }
    return false;
}

bool Torneo::setFechaFin(const char* fecha) {
    if (esFechaValida(fecha)) {
        std::strcpy(fechaFin, fecha);
        return true;
    }
    return false;
}

void Torneo::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }
void Torneo::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

// Metodos de negocio 

bool Torneo::esValido() const {
    return (std::strlen(nombre) > 0 && std::strlen(deporte) > 0 &&
            std::strlen(formato) > 0 && std::strlen(fechaInicio) == 10 &&
            std::strlen(fechaFin) == 10);
}

void Torneo::mostrarBasico() const {
    std::cout << "Torneo: " << nombre << " (" << deporte << " - " << formato << ")";
}

void Torneo::mostrarCompleto() const {
    std::cout << "\n╔═══════════════════════════════════════════╗" << std::endl;
    std::cout << "║       CONFIGURACIÓN DEL TORNEO          ║" << std::endl;
    std::cout << "╠═══════════════════════════════════════════╣" << std::endl;
    std::cout << "║ Nombre       : " << std::setw(20) << nombre << " ║" << std::endl;
    std::cout << "║ Deporte      : " << std::setw(20) << deporte << " ║" << std::endl;
    std::cout << "║ Formato      : " << std::setw(20) << formato << " ║" << std::endl;
    std::cout << "║ Fecha inicio : " << std::setw(20) << fechaInicio << " ║" << std::endl;
    std::cout << "║ Fecha fin    : " << std::setw(20) << fechaFin << " ║" << std::endl;
    // Convertir time_t a string
    char buffer[80];
    struct tm* tmInfo = localtime(&fechaCreacion);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmInfo);
    std::cout << "║ Creado       : " << std::setw(20) << buffer << " ║" << std::endl;
    tmInfo = localtime(&fechaUltimaModificacion);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmInfo);
    std::cout << "║ Últ. modif.  : " << std::setw(20) << buffer << " ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════╝" << std::endl;
}

//Metodo estatico

size_t Torneo::obtenerTamano() {
    return sizeof(Torneo);
}