#include "operacionesEquipos.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../jugadores/Jugador.hpp"
#include "../utilidades/Formatos.hpp"
#include <cstring>

namespace OperacionesEquipos {

const char* RUTA_EQUIPOS = "datos/equipos.bin";

// CRUD basico 
bool guardarEquipo(Equipo& equipo) {
    return GestorArchivos::guardarRegistro(RUTA_EQUIPOS, equipo);
}

bool leerEquipoPorID(int id, Equipo& resultado) {
    return GestorArchivos::leerRegistroPorID(RUTA_EQUIPOS, id, resultado);
}

bool actualizarEquipo(const Equipo& equipo) {
    return GestorArchivos::actualizarRegistro(RUTA_EQUIPOS, equipo);
}

bool eliminarEquipoLogico(int id) {
    return GestorArchivos::eliminarRegistroLogico<Equipo>(RUTA_EQUIPOS, id);
}

int contarEquiposActivos() {
    return GestorArchivos::contarRegistrosActivos(RUTA_EQUIPOS);
}

// Busquedas

int buscarEquiposPorNombre(const char* subcadena, Equipo resultados[], int maxResultados) {
    int contador = 0;
    GestorArchivos::recorrerRegistros<Equipo>(RUTA_EQUIPOS, [&](const Equipo& eq) {
        if (contador < maxResultados && Formatos::contieneSubcadena(eq.getNombre(), subcadena)) {
            resultados[contador++] = eq;
        }
    });
    return contador;
}

int listarEquiposOrdenados(Equipo resultados[], int maxResultados) {
    // Primero recolectamos todos los equipos activos
    Equipo* temp = new Equipo[contarEquiposActivos()];
    int count = 0;
    GestorArchivos::recorrerRegistros<Equipo>(RUTA_EQUIPOS, [&](const Equipo& eq) {
        temp[count++] = eq;
    });

    // Ordenar por puntos, diferencia de goles , goles a favor 
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            bool intercambiar = false;
            if (temp[j].getPuntos() < temp[j+1].getPuntos()) {
                intercambiar = true;
            } else if (temp[j].getPuntos() == temp[j+1].getPuntos()) {
                int dif1 = temp[j].getGolesAFavor() - temp[j].getGolesEnContra();
                int dif2 = temp[j+1].getGolesAFavor() - temp[j+1].getGolesEnContra();
                if (dif1 < dif2) intercambiar = true;
                else if (dif1 == dif2 && temp[j].getGolesAFavor() < temp[j+1].getGolesAFavor()) {
                    intercambiar = true;
                }
            }
            if (intercambiar) {
                Equipo aux = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = aux;
            }
        }
    }

    int copiar = (count < maxResultados) ? count : maxResultados;
    for (int i = 0; i < copiar; ++i) {
        resultados[i] = temp[i];
    }
    delete[] temp;
    return copiar;
}

// Operaciones de negocio 

bool agregarEquipo(const char* nombre, const char* ciudad, const char* entrenador, Equipo& nuevoEquipo) {
    // Verificar duplicado de nombre
    bool duplicado = false;
    GestorArchivos::recorrerRegistros<Equipo>(RUTA_EQUIPOS, [&](const Equipo& eq) {
        if (!duplicado && std::strcmp(eq.getNombre(), nombre) == 0) {
            duplicado = true;
        }
    });
    if (duplicado) return false;

    Equipo equipo(nombre, ciudad, entrenador);
    if (!guardarEquipo(equipo)) return false;

    // Recuperar el ID asignado
    ArchivoHeader header = GestorArchivos::leerHeader(RUTA_EQUIPOS);
    equipo.setId(header.proximoID - 1);
    nuevoEquipo = equipo;
    return true;
}

bool actualizarEquipoLogica(int id, const char* nombre, const char* ciudad, const char* entrenador) {
    Equipo eq;
    if (!leerEquipoPorID(id, eq)) return false;

    // Verificar nombre duplicado (si cambia)
    if (std::strcmp(eq.getNombre(), nombre) != 0) {
        bool duplicado = false;
        GestorArchivos::recorrerRegistros<Equipo>(RUTA_EQUIPOS, [&](const Equipo& e) {
            if (!duplicado && e.getId() != id && std::strcmp(e.getNombre(), nombre) == 0) {
                duplicado = true;
            }
        });
        if (duplicado) return false;
    }

    eq.setNombre(nombre);
    eq.setCiudad(ciudad);
    eq.setEntrenador(entrenador);
    eq.setFechaUltimaModificacion(time(nullptr));
    return actualizarEquipo(eq);
}

bool eliminarEquipoLogica(int idEquipo) {
    // Primero, desvincular jugadores (poner idEquipo = 0)
    GestorArchivos::recorrerRegistros<Jugador>("datos/jugadores.bin", [&](const Jugador& j) {
        if (j.getIdEquipo() == idEquipo) {
            Jugador jMod = j;
            jMod.setIdEquipo(0);
            jMod.setFechaUltimaModificacion(time(nullptr));
            GestorArchivos::actualizarRegistro("datos/jugadores.bin", jMod);
        }
    });
    return eliminarEquipoLogico(idEquipo);
}

} // namespace OperacionesEquipos