#include "operacionesPartidos.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../equipos/operacionesEquipos.hpp"
#include "../jugadores/operacionesJugadores.hpp"
#include <cstring>

namespace OperacionesPartidos {

const char* RUTA_PARTIDOS = "datos/partidos.bin";

// CRUD basico

bool guardarPartido(Partido& partido) {
    return GestorArchivos::guardarRegistro(RUTA_PARTIDOS, partido);
}

bool leerPartidoPorID(int id, Partido& resultado) {
    return GestorArchivos::leerRegistroPorID(RUTA_PARTIDOS, id, resultado);
}

bool actualizarPartido(const Partido& partido) {
    return GestorArchivos::actualizarRegistro(RUTA_PARTIDOS, partido);
}

bool eliminarPartidoLogico(int id) {
    return GestorArchivos::eliminarRegistroLogico<Partido>(RUTA_PARTIDOS, id);
}

int contarPartidosActivos() {
    return GestorArchivos::contarRegistrosActivos(RUTA_PARTIDOS);
}

// Busquedas

int listarPartidosPorEstado(const char* estado, Partido resultados[], int maxResultados) {
    int contador = 0;
    GestorArchivos::recorrerRegistros<Partido>(RUTA_PARTIDOS, [&](const Partido& p) {
        if (contador < maxResultados && (std::strlen(estado) == 0 || std::strcmp(p.getEstado(), estado) == 0)) {
            resultados[contador++] = p;
        }
    });
    return contador;
}

int listarPartidosPorEquipo(int idEquipo, Partido resultados[], int maxResultados) {
    int contador = 0;
    GestorArchivos::recorrerRegistros<Partido>(RUTA_PARTIDOS, [&](const Partido& p) {
        if (contador < maxResultados &&
            (p.getIdEquipoLocal() == idEquipo || p.getIdEquipoVisitante() == idEquipo)) {
            resultados[contador++] = p;
        }
    });
    return contador;
}

// Operaciones de negocio 

bool programarPartido(int idLocal, int idVisitante, const char* fecha, Partido& nuevoPartido) {
    if (idLocal == idVisitante) return false;
    Equipo el, ev;
    if (!OperacionesEquipos::leerEquipoPorID(idLocal, el)) return false;
    if (!OperacionesEquipos::leerEquipoPorID(idVisitante, ev)) return false;
    if (partidoYaExiste(idLocal, idVisitante)) return false;

    Partido p(idLocal, idVisitante, fecha);
    if (!guardarPartido(p)) return false;
    nuevoPartido = p;
    return true;
}

bool registrarResultadoPartido(int idPartido, int ptsLocal, int ptsVisitante,
                               const Gol goles[], int numGoles) {
    Partido p;
    if (!leerPartidoPorID(idPartido, p)) return false;
    if (!p.estaProgramado()) return false;
    if (numGoles > 22) return false;

    Equipo local, visitante;
    if (!OperacionesEquipos::leerEquipoPorID(p.getIdEquipoLocal(), local)) return false;
    if (!OperacionesEquipos::leerEquipoPorID(p.getIdEquipoVisitante(), visitante)) return false;

    // Verificar jugadores anotadores
    for (int i = 0; i < numGoles; ++i) {
        if (goles[i].getIdJugador() != 0) {
            Jugador j;
            if (!OperacionesJugadores::leerJugadorPorID(goles[i].getIdJugador(), j)) return false;
            if (goles[i].getEquipo() == 0 && j.getIdEquipo() != p.getIdEquipoLocal()) return false;
            if (goles[i].getEquipo() == 1 && j.getIdEquipo() != p.getIdEquipoVisitante()) return false;
        }
    }

    // Actualizar estadisticas de equipos
    int resultadoLocal = (ptsLocal > ptsVisitante) ? 1 : (ptsLocal == ptsVisitante ? 0 : -1);
    local.actualizarEstadisticas(ptsLocal, ptsVisitante, resultadoLocal);
    visitante.actualizarEstadisticas(ptsVisitante, ptsLocal, -resultadoLocal);

    local.agregarPartidoID(p.getId());
    visitante.agregarPartidoID(p.getId());

    if (!OperacionesEquipos::actualizarEquipo(local)) return false;
    if (!OperacionesEquipos::actualizarEquipo(visitante)) return false;

    // Actualizar goles de jugadores
    for (int i = 0; i < numGoles; ++i) {
        if (goles[i].getIdJugador() != 0) {
            Jugador j;
            if (OperacionesJugadores::leerJugadorPorID(goles[i].getIdJugador(), j)) {
                j.incrementarGoles();
                j.setFechaUltimaModificacion(time(nullptr));
                OperacionesJugadores::actualizarJugador(j);
            }
        }
    }

    p.setGolesLocal(ptsLocal);
    p.setGolesVisitante(ptsVisitante);
    p.setEstado("JUGADO");
    for (int i = 0; i < numGoles; ++i) {
        p.agregarGol(goles[i]);
    }
    p.setFechaUltimaModificacion(time(nullptr));

    return actualizarPartido(p);
}

bool cancelarPartidoLogica(int idPartido) {
    Partido p;
    if (!leerPartidoPorID(idPartido, p)) return false;
    if (p.estaCancelado()) return false;

    if (p.estaJugado()) {
        Equipo local, visitante;
        if (!OperacionesEquipos::leerEquipoPorID(p.getIdEquipoLocal(), local)) return false;
        if (!OperacionesEquipos::leerEquipoPorID(p.getIdEquipoVisitante(), visitante)) return false;

        int golesL = p.getGolesLocal();
        int golesV = p.getGolesVisitante();

        // Revertir estadísticas
        if (golesL > golesV) {
            local.setPuntos(local.getPuntos() - 3);
            local.setVictorias(local.getVictorias() - 1);
            visitante.setDerrotas(visitante.getDerrotas() - 1);
        } else if (golesL < golesV) {
            visitante.setPuntos(visitante.getPuntos() - 3);
            visitante.setVictorias(visitante.getVictorias() - 1);
            local.setDerrotas(local.getDerrotas() - 1);
        } else {
            local.setPuntos(local.getPuntos() - 1);
            local.setEmpates(local.getEmpates() - 1);
            visitante.setPuntos(visitante.getPuntos() - 1);
            visitante.setEmpates(visitante.getEmpates() - 1);
        }
        local.setGolesAFavor(local.getGolesAFavor() - golesL);
        local.setGolesEnContra(local.getGolesEnContra() - golesV);
        visitante.setGolesAFavor(visitante.getGolesAFavor() - golesV);
        visitante.setGolesEnContra(visitante.getGolesEnContra() - golesL);

        // Revertir goles de jugadores
        const Gol* goles = p.getGoles();
        int numGoles = p.getNumGoles();
        for (int i = 0; i < numGoles; ++i) {
            if (goles[i].getIdJugador() != 0) {
                Jugador j;
                if (OperacionesJugadores::leerJugadorPorID(goles[i].getIdJugador(), j)) {
                    j.setGolesAnotados(j.getGolesAnotados() - 1);
                    j.setFechaUltimaModificacion(time(nullptr));
                    OperacionesJugadores::actualizarJugador(j);
                }
            }
        }

        local.setFechaUltimaModificacion(time(nullptr));
        visitante.setFechaUltimaModificacion(time(nullptr));
        if (!OperacionesEquipos::actualizarEquipo(local)) return false;
        if (!OperacionesEquipos::actualizarEquipo(visitante)) return false;
    }

    p.setEstado("CANCELADO");
    p.setGolesLocal(0);
    p.setGolesVisitante(0);
    p.setFechaUltimaModificacion(time(nullptr));
    return actualizarPartido(p);
}

bool partidoYaExiste(int idLocal, int idVisitante) {
    bool existe = false;
    GestorArchivos::recorrerRegistros<Partido>(RUTA_PARTIDOS, [&](const Partido& p) {
        if (!existe && !p.estaCancelado() &&
            ((p.getIdEquipoLocal() == idLocal && p.getIdEquipoVisitante() == idVisitante) ||
             (p.getIdEquipoLocal() == idVisitante && p.getIdEquipoVisitante() == idLocal))) {
            existe = true;
        }
    });
    return existe;
}

bool equipoTienePartidosActivos(int idEquipo) {
    bool tiene = false;
    GestorArchivos::recorrerRegistros<Partido>(RUTA_PARTIDOS, [&](const Partido& p) {
        if (!tiene && !p.estaCancelado() &&
            (p.getIdEquipoLocal() == idEquipo || p.getIdEquipoVisitante() == idEquipo)) {
            tiene = true;
        }
    });
    return tiene;
}

} // namespace OperacionesPartidos