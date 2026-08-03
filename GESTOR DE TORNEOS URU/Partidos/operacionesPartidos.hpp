#ifndef OPERACIONES_PARTIDOS_HPP
#define OPERACIONES_PARTIDOS_HPP

#include "Partido.hpp"
#include "Gol.hpp"

namespace OperacionesPartidos {

extern const char* RUTA_PARTIDOS;

// CRUD basico
bool guardarPartido(Partido& partido);
bool leerPartidoPorID(int id, Partido& resultado);
bool actualizarPartido(const Partido& partido);
bool eliminarPartidoLogico(int id);
int contarPartidosActivos();

// Busquedas
int listarPartidosPorEstado(const char* estado, Partido resultados[], int maxResultados);
int listarPartidosPorEquipo(int idEquipo, Partido resultados[], int maxResultados);

// Operaciones de negocio
bool programarPartido(int idLocal, int idVisitante, const char* fecha, Partido& nuevoPartido);
bool registrarResultadoPartido(int idPartido, int ptsLocal, int ptsVisitante,
                               const Gol goles[], int numGoles);
bool cancelarPartidoLogica(int idPartido);
bool partidoYaExiste(int idLocal, int idVisitante);
bool equipoTienePartidosActivos(int idEquipo);

} // namespace OperacionesPartidos

#endif