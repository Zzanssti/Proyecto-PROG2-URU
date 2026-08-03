#include "operacionesJugadores.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Formatos.hpp"
#include "../equipos/operacionesEquipos.hpp"
#include <cstring>

namespace OperacionesJugadores {

const char* RUTA_JUGADORES = "datos/jugadores.bin";

// CRUD basico 

bool guardarJugador(Jugador& jugador) {
    return GestorArchivos::guardarRegistro(RUTA_JUGADORES, jugador);
}

bool leerJugadorPorID(int id, Jugador& resultado) {
    return GestorArchivos::leerRegistroPorID(RUTA_JUGADORES, id, resultado);
}

bool actualizarJugador(const Jugador& jugador) {
    return GestorArchivos::actualizarRegistro(RUTA_JUGADORES, jugador);
}

bool eliminarJugadorLogico(int id) {
    return GestorArchivos::eliminarRegistroLogico<Jugador>(RUTA_JUGADORES, id);
}

int contarJugadoresActivos() {
    return GestorArchivos::contarRegistrosActivos(RUTA_JUGADORES);
}

// Busquedas 

int buscarJugadoresPorNombre(const char* subcadena, Jugador resultados[], int maxResultados) {
    int contador = 0;
    GestorArchivos::recorrerRegistros<Jugador>(RUTA_JUGADORES, [&](const Jugador& j) {
        if (contador < maxResultados && Formatos::contieneSubcadena(j.getNombre(), subcadena)) {
            resultados[contador++] = j;
        }
    });
    return contador;
}

int listarJugadoresPorEquipo(int idEquipo, Jugador resultados[], int maxResultados) {
    int contador = 0;
    GestorArchivos::recorrerRegistros<Jugador>(RUTA_JUGADORES, [&](const Jugador& j) {
        if (contador < maxResultados && j.getIdEquipo() == idEquipo) {
            resultados[contador++] = j;
        }
    });
    return contador;
}

int listarTodosLosJugadores(Jugador resultados[], int maxResultados) {
    int contador = 0;
    GestorArchivos::recorrerRegistros<Jugador>(RUTA_JUGADORES, [&](const Jugador& j) {
        if (contador < maxResultados) {
            resultados[contador++] = j;
        }
    });
    return contador;
}

// Operaciones de negocio 

bool cedulaExiste(const char* cedula) {
    bool existe = false;
    GestorArchivos::recorrerRegistros<Jugador>(RUTA_JUGADORES, [&](const Jugador& j) {
        if (!existe && std::strcmp(j.getCedula(), cedula) == 0) {
            existe = true;
        }
    });
    return existe;
}

bool agregarJugador(int idEquipo, const char* nombre, const char* cedula,
                    const char* posicion, int edad, int dorsal, Jugador& nuevoJugador) {
    if (idEquipo == 0) return false;
    Equipo eq;
    if (!OperacionesEquipos::leerEquipoPorID(idEquipo, eq)) return false;

    if (cedulaExiste(cedula)) return false;

    bool dorsalDuplicado = false;
    GestorArchivos::recorrerRegistros<Jugador>(RUTA_JUGADORES, [&](const Jugador& j) {
        if (!dorsalDuplicado && j.getIdEquipo() == idEquipo && j.getNumeroDorsal() == dorsal) {
            dorsalDuplicado = true;
        }
    });
    if (dorsalDuplicado) return false;

    Jugador jugador(idEquipo, nombre, cedula, posicion, edad, dorsal);
    if (!guardarJugador(jugador)) return false;

    ArchivoHeader header = GestorArchivos::leerHeader(RUTA_JUGADORES);
    jugador.setId(header.proximoID - 1);
    nuevoJugador = jugador;
    return true;
}

bool actualizarJugadorLogica(int id, int nuevoIdEquipo, const char* nombre,
                             const char* posicion, int edad, int dorsal) {
    if (nuevoIdEquipo == 0) return false;
    Jugador j;
    if (!leerJugadorPorID(id, j)) return false;

    Equipo eq;
    if (!OperacionesEquipos::leerEquipoPorID(nuevoIdEquipo, eq)) return false;

    bool dorsalDuplicado = false;
    GestorArchivos::recorrerRegistros<Jugador>(RUTA_JUGADORES, [&](const Jugador& aux) {
        if (!dorsalDuplicado && aux.getId() != id && aux.getIdEquipo() == nuevoIdEquipo && aux.getNumeroDorsal() == dorsal) {
            dorsalDuplicado = true;
        }
    });
    if (dorsalDuplicado) return false;

    j.setIdEquipo(nuevoIdEquipo);
    j.setNombre(nombre);
    j.setPosicion(posicion);
    j.setEdad(edad);
    j.setNumeroDorsal(dorsal);
    j.setFechaUltimaModificacion(time(nullptr));
    return actualizarJugador(j);
}

bool eliminarJugadorLogica(int id) {
    return eliminarJugadorLogico(id);
}

} // namespace OperacionesJugadores