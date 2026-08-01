#include "Gol.hpp"
#include <iostream>

//  Constructores 

Gol::Gol() : idJugador(0), minuto(0), equipo(0) {
    // Inicialización directa con lista de inicializadores
}

Gol::Gol(int idJugador, int minuto, int equipo) {
    setIdJugador(idJugador);
    setMinuto(minuto);
    setEquipo(equipo);
}

Gol::Gol(const Gol& otro) {
    idJugador = otro.idJugador;
    minuto = otro.minuto;
    equipo = otro.equipo;
}

Gol::~Gol() {
    // No hay recursos dinamicos que liberar.
}

//  Getters 

int Gol::getIdJugador() const { return idJugador; }
int Gol::getMinuto() const { return minuto; }
int Gol::getEquipo() const { return equipo; }

//  Setters con validaciones 

void Gol::setIdJugador(int id) {
    // Acepta cualquier valor >= 0 (0 significa desconocido o autogol)
    idJugador = (id >= 0) ? id : 0;
}

bool Gol::setMinuto(int minuto) {
    if (minuto < 0 || minuto > 120) return false; // Permite hasta 120 (incluye tiempo extra)
    this->minuto = minuto;
    return true;
}

bool Gol::setEquipo(int equipo) {
    if (equipo != 0 && equipo != 1) return false;
    this->equipo = equipo;
    return true;
}

//Metodos de negocio 

bool Gol::esValido() const {
    return (minuto >= 0 && minuto <= 120 && (equipo == 0 || equipo == 1));
}

void Gol::mostrar() const {
    std::cout << "Jugador ID: " << idJugador << " | Minuto: " << minuto
              << " | Equipo: " << (equipo == 0 ? "LOCAL" : "VISITANTE");
}

// Metodo estatico

size_t Gol::obtenerTamano() {
    return sizeof(Gol);
}