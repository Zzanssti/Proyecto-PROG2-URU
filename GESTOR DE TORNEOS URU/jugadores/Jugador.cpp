#include "Jugador.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cctype>

// Constructores 

Jugador::Jugador() {
    id = 0;
    idEquipo = 0;
    std::memset(nombre, 0, sizeof(nombre));
    std::memset(cedula, 0, sizeof(cedula));
    std::memset(posicion, 0, sizeof(posicion));
    edad = 0;
    numeroDorsal = 0;
    golesAnotados = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Jugador::Jugador(int idEquipo, const char* nombre, const char* cedula,
                 const char* posicion, int edad, int dorsal) {
    this->id = 0;
    setIdEquipo(idEquipo);
    setNombre(nombre);
    setCedula(cedula);
    setPosicion(posicion);
    setEdad(edad);
    setNumeroDorsal(dorsal);
    golesAnotados = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Jugador::Jugador(const Jugador& otro) {
    id = otro.id;
    idEquipo = otro.idEquipo;
    std::strcpy(nombre, otro.nombre);
    std::strcpy(cedula, otro.cedula);
    std::strcpy(posicion, otro.posicion);
    edad = otro.edad;
    numeroDorsal = otro.numeroDorsal;
    golesAnotados = otro.golesAnotados;
    tarjetasAmarillas = otro.tarjetasAmarillas;
    tarjetasRojas = otro.tarjetasRojas;
    eliminado = otro.eliminado;
    fechaCreacion = otro.fechaCreacion;
    fechaUltimaModificacion = otro.fechaUltimaModificacion;
}

Jugador::~Jugador() {
    // No hay recursos dinámicos que liberar
    // Definido para cumplir con la convencion
}

// -Getters

int Jugador::getId() const { return id; }
int Jugador::getIdEquipo() const { return idEquipo; }
const char* Jugador::getNombre() const { return nombre; }
const char* Jugador::getCedula() const { return cedula; }
const char* Jugador::getPosicion() const { return posicion; }
int Jugador::getEdad() const { return edad; }
int Jugador::getNumeroDorsal() const { return numeroDorsal; }
int Jugador::getGolesAnotados() const { return golesAnotados; }
int Jugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int Jugador::getTarjetasRojas() const { return tarjetasRojas; }
bool Jugador::getEliminado() const { return eliminado; }
time_t Jugador::getFechaCreacion() const { return fechaCreacion; }
time_t Jugador::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// Setters con validaciones -

void Jugador::setId(int id) { this->id = id; }

bool Jugador::setIdEquipo(int idEquipo) {
    if (idEquipo < 0) return false;
    this->idEquipo = idEquipo;
    return true;
}

void Jugador::setNombre(const char* nombre) {
    if (nombre && std::strlen(nombre) > 0 && std::strlen(nombre) < 100) {
        std::strcpy(this->nombre, nombre);
    }
}

bool Jugador::setCedula(const char* cedula) {
    if (!cedula) return false;
    size_t len = std::strlen(cedula);
    if (len < 7 || len > 10) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!std::isdigit(cedula[i])) return false;
    }
    std::strcpy(this->cedula, cedula);
    return true;
}

void Jugador::setPosicion(const char* posicion) {
    if (posicion && std::strlen(posicion) > 0 && std::strlen(posicion) < 20) {
        std::strcpy(this->posicion, posicion);
    }
}

bool Jugador::setEdad(int edad) {
    if (edad < 14 || edad > 60) return false;
    this->edad = edad;
    return true;
}

bool Jugador::setNumeroDorsal(int dorsal) {
    if (dorsal < 1 || dorsal > 99) return false;
    this->numeroDorsal = dorsal;
    return true;
}

bool Jugador::setGolesAnotados(int goles) {
    if (goles < 0) return false;
    golesAnotados = goles;
    return true;
}

bool Jugador::setTarjetasAmarillas(int amarillas) {
    if (amarillas < 0) return false;
    tarjetasAmarillas = amarillas;
    return true;
}

bool Jugador::setTarjetasRojas(int rojas) {
    if (rojas < 0) return false;
    tarjetasRojas = rojas;
    return true;
}

void Jugador::setEliminado(bool estado) { eliminado = estado; }
void Jugador::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }
void Jugador::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

// Metodos de negocio 

bool Jugador::esValido() const {
    return (std::strlen(nombre) > 0 && std::strlen(cedula) > 0 &&
            idEquipo > 0 && edad >= 14 && edad <= 60 &&
            numeroDorsal >= 1 && numeroDorsal <= 99 && !eliminado);
}

void Jugador::incrementarGoles() { golesAnotados++; }
void Jugador::incrementarAmarilla() { tarjetasAmarillas++; }
void Jugador::incrementarRoja() { tarjetasRojas++; }

// Metodos de presentacion 

void Jugador::mostrarBasico() const {
    std::cout << "ID: " << id << " | " << nombre << " | CI: " << cedula
              << " | Dorsal: #" << numeroDorsal << " | Goles: " << golesAnotados;
}

void Jugador::mostrarCompleto() const {
    std::cout << "\n╔═══════════════════════════════════════════╗" << std::endl;
    std::cout << "║           FICHA DEL JUGADOR             ║" << std::endl;
    std::cout << "╠═══════════════════════════════════════════╣" << std::endl;
    std::cout << "║ ID            : " << std::setw(20) << id << " ║" << std::endl;
    std::cout << "║ Equipo ID     : " << std::setw(20) << idEquipo << " ║" << std::endl;
    std::cout << "║ Nombre        : " << std::setw(20) << nombre << " ║" << std::endl;
    std::cout << "║ Cédula        : " << std::setw(20) << cedula << " ║" << std::endl;
    std::cout << "║ Posición      : " << std::setw(20) << posicion << " ║" << std::endl;
    std::cout << "║ Edad          : " << std::setw(20) << edad << " ║" << std::endl;
    std::cout << "║ Dorsal        : " << std::setw(20) << numeroDorsal << " ║" << std::endl;
    std::cout << "║ Goles         : " << std::setw(20) << golesAnotados << " ║" << std::endl;
    std::cout << "║ Amarillas     : " << std::setw(20) << tarjetasAmarillas << " ║" << std::endl;
    std::cout << "║ Rojas         : " << std::setw(20) << tarjetasRojas << " ║" << std::endl;
    std::cout << "║ Eliminado     : " << std::setw(20) << (eliminado ? "Sí" : "No") << " ║" << std::endl;
    std::cout << "║ Creado        : " << std::setw(20) << ctime(&fechaCreacion) << " ║" << std::endl;
    std::cout << "║ Última modif. : " << std::setw(20) << ctime(&fechaUltimaModificacion) << " ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════╝" << std::endl;
}

// Metodo estatico

size_t Jugador::obtenerTamano() {
    return sizeof(Jugador);
}