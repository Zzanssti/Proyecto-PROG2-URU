#ifndef OPERACIONES_JUGADORES_HPP
#define OPERACIONES_JUGADORES_HPP

#include "Jugador.hpp"

namespace OperacionesJugadores {

extern const char* RUTA_JUGADORES;

// CRUD basico
bool guardarJugador(Jugador& jugador);
bool leerJugadorPorID(int id, Jugador& resultado);
bool actualizarJugador(const Jugador& jugador);
bool eliminarJugadorLogico(int id);
int contarJugadoresActivos();

// Busquedas
int buscarJugadoresPorNombre(const char* subcadena, Jugador resultados[], int maxResultados);
int listarJugadoresPorEquipo(int idEquipo, Jugador resultados[], int maxResultados);
int listarTodosLosJugadores(Jugador resultados[], int maxResultados);

// Operaciones de negocio
bool agregarJugador(int idEquipo, const char* nombre, const char* cedula,
                    const char* posicion, int edad, int dorsal, Jugador& nuevoJugador);
bool actualizarJugadorLogica(int id, int nuevoIdEquipo, const char* nombre,
                             const char* posicion, int edad, int dorsal);
bool eliminarJugadorLogica(int id);
bool cedulaExiste(const char* cedula);

} // namespace OperacionesJugadores

#endif