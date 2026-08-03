#ifndef OPERACIONES_EQUIPOS_HPP
#define OPERACIONES_EQUIPOS_HPP

#include "Equipo.hpp"

 //Ooperaciones de negocio de equipos, incluyendo CRUD y busquedas
namespace OperacionesEquipos {

// Constantes
extern const char* RUTA_EQUIPOS;

// CRUD basico
bool guardarEquipo(Equipo& equipo);
bool leerEquipoPorID(int id, Equipo& resultado);
bool actualizarEquipo(const Equipo& equipo);
bool eliminarEquipoLogico(int id);
int contarEquiposActivos();

// Busquedas
int buscarEquiposPorNombre(const char* subcadena, Equipo resultados[], int maxResultados);
int listarEquiposOrdenados(Equipo resultados[], int maxResultados);

// Operaciones de negocio
bool agregarEquipo(const char* nombre, const char* ciudad, const char* entrenador, Equipo& nuevoEquipo);
bool actualizarEquipoLogica(int id, const char* nombre, const char* ciudad, const char* entrenador);
bool eliminarEquipoLogica(int idEquipo);

} // namespace OperacionesEquipos

#endif