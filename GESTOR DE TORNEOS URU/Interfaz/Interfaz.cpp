#include "Interfaz.hpp"
#include "../Persistencia/GestorArchivos.hpp"
#include "../Equipos/operacionesEquipos.hpp"
#include "../Jugadores/operacionesJugadores.hpp"
#include "../Partidos/operacionesPartidos.hpp"
#include "../Torneo/operacionesTorneo.hpp"
#include "../Utilidades/Validaciones.hpp"
#include "../Utilidades/Formatos.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

// Constructor
Interfaz::Interfaz() {
    // Inicializar archivos (lo hace main.cpp, pero por si acaso)
    // No hacemos nada aquí, la inicialización ya está en main
}

// Ejecuta el bucle principal
void Interfaz::ejecutar() {
    // Cargar torneo si existe
    Torneo torneo;
    if (OperacionesTorneo::existeTorneo()) {
        OperacionesTorneo::leerTorneo(torneo);
        cout << "\n==========================================" << endl;
        cout << "      SISTEMA DE GESTIÓN DE TORNEOS URU   " << endl;
        cout << "      Torneo: " << torneo.getNombre() << endl;
        cout << "      Deporte: " << torneo.getDeporte() << " (" << torneo.getFormato() << ")" << endl;
        cout << "==========================================" << endl;
    } else {
        cout << "\n==========================================" << endl;
        cout << "      SISTEMA DE GESTIÓN DE TORNEOS URU   " << endl;
        cout << "      (Sin torneo configurado)" << endl;
        cout << "==========================================" << endl;
    }

    menuPrincipal();
}

// --- Menú Principal ---
void Interfaz::menuPrincipal() {
    int opcion;
    do {
        cout << "\n==========================================" << endl;
        cout << "1. Gestion de Equipos" << endl;
        cout << "2. Gestion de Jugadores" << endl;
        cout << "3. Gestion de Partidos" << endl;
        cout << "4. Tabla de Posiciones" << endl;
        cout << "5. Reportes" << endl;
        cout << "6. Mantenimiento" << endl;
        cout << "0. Salir" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";

        opcion = leerOpcion(0, 6);

        switch (opcion) {
            case 1: menuEquipos(); break;
            case 2: menuJugadores(); break;
            case 3: menuPartidos(); break;
            case 4: {
                Equipo resultados[100];
                int cant = OperacionesEquipos::listarEquiposOrdenados(resultados, 100);
                if (cant == 0) {
                    mostrarError("No hay equipos registrados.");
                } else {
                    cout << "\n=========================================================================" << endl;
                    cout << "                  TABLA DE POSICIONES OFICIAL (URU)                      " << endl;
                    cout << "=========================================================================" << endl;
                    for (int i = 0; i < cant; ++i) {
                        cout << " # " << (i+1) << " -> ";
                        resultados[i].mostrarBasico();
                        cout << endl;
                    }
                }
                esperarEnter();
                break;
            }
            case 5: menuReportes(); break;
            case 6: menuMantenimiento(); break;
            case 0:
                cout << "\n[SISTEMA]: Programa finalizado. Los datos están guardados en disco.\n";
                break;
        }
    } while (opcion != 0);
}

// --- Menú Equipos ---
void Interfaz::menuEquipos() {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║          GESTIÓN DE EQUIPOS               ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Registrar equipo                      ║" << endl;
        cout << "║  2. Buscar equipo                         ║" << endl;
        cout << "║  3. Actualizar equipo                     ║" << endl;
        cout << "║  4. Listar equipos                        ║" << endl;
        cout << "║  5. Eliminar equipo                       ║" << endl;
        cout << "║  0. Volver                                ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione: ";

        opcion = leerOpcion(0, 5);

        switch (opcion) {
            case 1: {
                char nombre[100], ciudad[100], entrenador[100];
                cout << "\n--- REGISTRAR NUEVO EQUIPO ---\n";
                leerCadena(nombre, 100, "Nombre del equipo: ");
                while (!Validaciones::esTextoValido(nombre)) {
                    mostrarError("Nombre inválido (solo letras, espacios y puntos).");
                    leerCadena(nombre, 100, "Nombre del equipo: ");
                }
                leerCadena(ciudad, 100, "Ciudad de origen: ");
                while (!Validaciones::esTextoValido(ciudad)) {
                    mostrarError("Ciudad inválida.");
                    leerCadena(ciudad, 100, "Ciudad de origen: ");
                }
                leerCadena(entrenador, 100, "Entrenador: ");
                while (!Validaciones::esTextoValido(entrenador)) {
                    mostrarError("Entrenador inválido.");
                    leerCadena(entrenador, 100, "Entrenador: ");
                }

                Equipo nuevo;
                if (OperacionesEquipos::agregarEquipo(nombre, ciudad, entrenador, nuevo)) {
                    mostrarExito("Equipo guardado con éxito! ID: " + to_string(nuevo.getId()));
                } else {
                    mostrarError("No se pudo guardar (nombre duplicado o error de archivo).");
                }
                esperarEnter();
                break;
            }
            case 2: {
                int sub;
                cout << "\n--- BUSCAR EQUIPO ---\n1. Por ID\n2. Por Nombre Parcial\nSeleccione: ";
                sub = leerOpcion(1, 2);
                if (sub == 1) {
                    int id = leerEntero("ID: ", 1, 9999);
                    Equipo e;
                    if (OperacionesEquipos::leerEquipoPorID(id, e)) {
                        e.mostrarCompleto();
                    } else {
                        mostrarError("Equipo no encontrado.");
                    }
                } else {
                    char criterio[100];
                    leerCadena(criterio, 100, "Texto a buscar: ");
                    Equipo resultados[100];
                    int cant = OperacionesEquipos::buscarEquiposPorNombre(criterio, resultados, 100);
                    if (cant == 0) {
                        mostrarError("No se encontraron equipos.");
                    } else {
                        cout << "\n--- RESULTADOS ---\n";
                        for (int i = 0; i < cant; ++i) {
                            resultados[i].mostrarBasico();
                            cout << endl;
                        }
                    }
                }
                esperarEnter();
                break;
            }
            case 3: {
                int id = leerEntero("ID del equipo a actualizar: ", 1, 9999);
                Equipo e;
                if (!OperacionesEquipos::leerEquipoPorID(id, e)) {
                    mostrarError("Equipo no existe.");
                    esperarEnter();
                    break;
                }

                int sub2;
                do {
                    cout << "\n╔═══════════════════════════════════════════╗" << endl;
                    cout << "║     EDITANDO EQUIPO: " << e.getNombre() << " (ID " << e.getId() << ")" << "   ║" << endl;
                    cout << "╠═══════════════════════════════════════════╣" << endl;
                    cout << "║ 1. Nombre actual : " << e.getNombre() << endl;
                    cout << "║ 2. Ciudad actual : " << e.getCiudad() << endl;
                    cout << "║ 3. Entrenador    : " << e.getEntrenador() << endl;
                    cout << "║ 0. Guardar cambios y salir               ║" << endl;
                    cout << "╚═══════════════════════════════════════════╝" << endl;
                    cout << "Seleccione campo a modificar: ";
                    sub2 = leerOpcion(0, 3);

                    if (sub2 == 0) break;

                    char nuevoValor[100];
                    bool cambio = false;
                    switch (sub2) {
                        case 1:
                            leerCadena(nuevoValor, 100, "Nuevo nombre: ");
                            if (strcmp(e.getNombre(), nuevoValor) != 0) {
                                // Verificar duplicado
                                bool duplicado = false;
                                Equipo aux;
                                GestorArchivos::recorrerRegistros<Equipo>(OperacionesEquipos::RUTA_EQUIPOS, [&](const Equipo& eq) {
                                    if (!duplicado && eq.getId() != id && strcmp(eq.getNombre(), nuevoValor) == 0) {
                                        duplicado = true;
                                    }
                                });
                                if (duplicado) {
                                    mostrarError("Ya existe un equipo con ese nombre.");
                                } else {
                                    e.setNombre(nuevoValor);
                                    cambio = true;
                                }
                            }
                            break;
                        case 2:
                            leerCadena(nuevoValor, 100, "Nueva ciudad: ");
                            e.setCiudad(nuevoValor);
                            cambio = true;
                            break;
                        case 3:
                            leerCadena(nuevoValor, 100, "Nuevo entrenador: ");
                            e.setEntrenador(nuevoValor);
                            cambio = true;
                            break;
                    }

                    if (cambio) {
                        e.setFechaUltimaModificacion(time(nullptr));
                        if (OperacionesEquipos::actualizarEquipo(e)) {
                            mostrarExito("Campo actualizado correctamente.");
                        } else {
                            mostrarError("Error al actualizar.");
                            // Recargar el equipo
                            OperacionesEquipos::leerEquipoPorID(id, e);
                        }
                    }
                } while (sub2 != 0);
                mostrarExito("Edición finalizada.");
                esperarEnter();
                break;
            }
            case 4: {
                Equipo resultados[100];
                int cant = OperacionesEquipos::listarEquiposOrdenados(resultados, 100);
                if (cant == 0) {
                    mostrarError("No hay equipos registrados.");
                } else {
                    cout << "\n--- LISTA DE EQUIPOS (ordenados por puntos) ---\n";
                    for (int i = 0; i < cant; ++i) {
                        cout << (i+1) << ". ";
                        resultados[i].mostrarBasico();
                        cout << endl;
                    }
                }
                esperarEnter();
                break;
            }
            case 5: {
                int id = leerEntero("ID del equipo a eliminar: ", 1, 9999);
                Equipo e;
                if (!OperacionesEquipos::leerEquipoPorID(id, e)) {
                    mostrarError("Equipo no existe.");
                    esperarEnter();
                    break;
                }
                if (OperacionesPartidos::equipoTienePartidosActivos(id)) {
                    char confirm[10];
                    cout << "[ADVERTENCIA]: El equipo tiene partidos activos. Escriba 'SI' para confirmar: ";
                    cin.getline(confirm, 10);
                    if (strcmp(confirm, "SI") != 0) {
                        mostrarError("Eliminación cancelada.");
                        esperarEnter();
                        break;
                    }
                }
                if (OperacionesEquipos::eliminarEquipoLogica(id)) {
                    mostrarExito("Equipo eliminado (borrado lógico).");
                } else {
                    mostrarError("No se pudo eliminar.");
                }
                esperarEnter();
                break;
            }
        }
    } while (opcion != 0);
}

// --- Menú Jugadores ---
void Interfaz::menuJugadores() {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║        GESTIÓN DE JUGADORES               ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Registrar jugador                     ║" << endl;
        cout << "║  2. Buscar jugador                        ║" << endl;
        cout << "║  3. Actualizar jugador                    ║" << endl;
        cout << "║  4. Listar todos los jugadores            ║" << endl;
        cout << "║  5. Listar jugadores por equipo           ║" << endl;
        cout << "║  6. Eliminar jugador                      ║" << endl;
        cout << "║  0. Volver                                ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione: ";

        opcion = leerOpcion(0, 6);

        switch (opcion) {
            case 1: {
                int idEquipo, edad, dorsal;
                char nombre[100], cedula[20], posicion[30];
                cout << "\n--- REGISTRAR NUEVO JUGADOR ---\n";
                idEquipo = leerEntero("ID del equipo (obligatorio): ", 1, 9999);
                Equipo eq;
                if (!OperacionesEquipos::leerEquipoPorID(idEquipo, eq)) {
                    mostrarError("Equipo no existe.");
                    esperarEnter();
                    break;
                }
                leerCadena(nombre, 100, "Nombre: ");
                leerCadena(cedula, 20, "Cédula (7-10 dígitos): ");
                while (!Validaciones::esCedulaValida(cedula)) {
                    mostrarError("Cédula inválida (debe tener 7-10 dígitos).");
                    leerCadena(cedula, 20, "Cédula: ");
                }
                strcpy(posicion, "JUGADOR");
                edad = leerEntero("Edad (14-60): ", 14, 60);
                dorsal = leerEntero("Dorsal (1-99): ", 1, 99);

                Jugador nuevo;
                if (OperacionesJugadores::agregarJugador(idEquipo, nombre, cedula, posicion, edad, dorsal, nuevo)) {
                    mostrarExito("Jugador registrado con ID " + to_string(nuevo.getId()));
                } else {
                    mostrarError("Cédula duplicada o dorsal ya usado en el equipo.");
                }
                esperarEnter();
                break;
            }
            case 2: {
                int sub;
                cout << "\n--- BUSCAR JUGADOR ---\n1. Por ID\n2. Por Nombre Parcial\nSeleccione: ";
                sub = leerOpcion(1, 2);
                if (sub == 1) {
                    int id = leerEntero("ID: ", 1, 9999);
                    Jugador j;
                    if (OperacionesJugadores::leerJugadorPorID(id, j)) {
                        j.mostrarCompleto();
                    } else {
                        mostrarError("Jugador no encontrado.");
                    }
                } else {
                    char criterio[100];
                    leerCadena(criterio, 100, "Texto a buscar: ");
                    Jugador resultados[100];
                    int cant = OperacionesJugadores::buscarJugadoresPorNombre(criterio, resultados, 100);
                    if (cant == 0) {
                        mostrarError("Sin coincidencias.");
                    } else {
                        cout << "\n--- RESULTADOS ---\n";
                        for (int i = 0; i < cant; ++i) {
                            resultados[i].mostrarBasico();
                            cout << endl;
                        }
                    }
                }
                esperarEnter();
                break;
            }
            case 3: {
                int id = leerEntero("ID del jugador a actualizar: ", 1, 9999);
                Jugador j;
                if (!OperacionesJugadores::leerJugadorPorID(id, j)) {
                    mostrarError("Jugador no existe.");
                    esperarEnter();
                    break;
                }

                int sub2;
                do {
                    char nombreEquipo[100] = "Sin equipo";
                    if (j.getIdEquipo() != 0) {
                        Equipo eq;
                        if (OperacionesEquipos::leerEquipoPorID(j.getIdEquipo(), eq)) {
                            strcpy(nombreEquipo, eq.getNombre());
                        }
                    }
                    cout << "\n╔═══════════════════════════════════════════╗" << endl;
                    cout << "║     EDITANDO JUGADOR: " << j.getNombre() << " (ID " << j.getId() << ")" << "   ║" << endl;
                    cout << "╠═══════════════════════════════════════════╣" << endl;
                    cout << "║ 1. Nombre   : " << j.getNombre() << endl;
                    cout << "║ 2. Equipo   : " << nombreEquipo << " (ID " << j.getIdEquipo() << ")" << endl;
                    cout << "║ 3. Edad     : " << j.getEdad() << endl;
                    cout << "║ 4. Dorsal   : " << j.getNumeroDorsal() << endl;
                    cout << "║ 0. Guardar cambios y salir               ║" << endl;
                    cout << "╚═══════════════════════════════════════════╝" << endl;
                    cout << "Seleccione campo a modificar: ";
                    sub2 = leerOpcion(0, 4);

                    if (sub2 == 0) break;

                    bool cambio = false;
                    switch (sub2) {
                        case 1: {
                            char nuevoNombre[100];
                            leerCadena(nuevoNombre, 100, "Nuevo nombre: ");
                            j.setNombre(nuevoNombre);
                            cambio = true;
                            break;
                        }
                        case 2: {
                            int nuevoEquipo = leerEntero("Nuevo ID de equipo: ", 1, 9999);
                            Equipo eq;
                            if (!OperacionesEquipos::leerEquipoPorID(nuevoEquipo, eq)) {
                                mostrarError("Equipo no existe.");
                                break;
                            }
                            // Verificar dorsal único en el nuevo equipo
                            bool duplicado = false;
                            GestorArchivos::recorrerRegistros<Jugador>(OperacionesJugadores::RUTA_JUGADORES, [&](const Jugador& aux) {
                                if (!duplicado && aux.getId() != id && aux.getIdEquipo() == nuevoEquipo && aux.getNumeroDorsal() == j.getNumeroDorsal()) {
                                    duplicado = true;
                                }
                            });
                            if (duplicado) {
                                mostrarError("El dorsal " + to_string(j.getNumeroDorsal()) + " ya está usado en " + eq.getNombre());
                                break;
                            }
                            j.setIdEquipo(nuevoEquipo);
                            cambio = true;
                            break;
                        }
                        case 3: {
                            int nuevaEdad = leerEntero("Nueva edad (14-60): ", 14, 60);
                            j.setEdad(nuevaEdad);
                            cambio = true;
                            break;
                        }
                        case 4: {
                            int nuevoDorsal = leerEntero("Nuevo dorsal (1-99): ", 1, 99);
                            // Verificar dorsal único en el mismo equipo
                            bool duplicado = false;
                            GestorArchivos::recorrerRegistros<Jugador>(OperacionesJugadores::RUTA_JUGADORES, [&](const Jugador& aux) {
                                if (!duplicado && aux.getId() != id && aux.getIdEquipo() == j.getIdEquipo() && aux.getNumeroDorsal() == nuevoDorsal) {
                                    duplicado = true;
                                }
                            });
                            if (duplicado) {
                                mostrarError("Dorsal " + to_string(nuevoDorsal) + " ya usado en el equipo actual.");
                                break;
                            }
                            j.setNumeroDorsal(nuevoDorsal);
                            cambio = true;
                            break;
                        }
                    }

                    if (cambio) {
                        j.setFechaUltimaModificacion(time(nullptr));
                        if (OperacionesJugadores::actualizarJugador(j)) {
                            mostrarExito("Campo actualizado correctamente.");
                        } else {
                            mostrarError("Error al actualizar.");
                            OperacionesJugadores::leerJugadorPorID(id, j);
                        }
                    }
                } while (sub2 != 0);
                mostrarExito("Edición finalizada.");
                esperarEnter();
                break;
            }
            case 4: {
                Jugador resultados[100];
                int cant = OperacionesJugadores::listarTodosLosJugadores(resultados, 100);
                if (cant == 0) {
                    mostrarError("No hay jugadores.");
                } else {
                    cout << "\n--- LISTADO GENERAL DE JUGADORES ---\n";
                    for (int i = 0; i < cant; ++i) {
                        resultados[i].mostrarBasico();
                        cout << endl;
                    }
                }
                esperarEnter();
                break;
            }
            case 5: {
                int idEquipo = leerEntero("ID del equipo: ", 1, 9999);
                Equipo eq;
                if (!OperacionesEquipos::leerEquipoPorID(idEquipo, eq)) {
                    mostrarError("Equipo no existe.");
                    esperarEnter();
                    break;
                }
                Jugador resultados[100];
                int cant = OperacionesJugadores::listarJugadoresPorEquipo(idEquipo, resultados, 100);
                cout << "\n--- PLANTILLA DE JUGADORES: " << eq.getNombre() << " ---\n";
                if (cant == 0) {
                    cout << "No tiene jugadores asignados.\n";
                } else {
                    for (int i = 0; i < cant; ++i) {
                        resultados[i].mostrarBasico();
                        cout << endl;
                    }
                }
                esperarEnter();
                break;
            }
            case 6: {
                int id = leerEntero("ID del jugador a eliminar: ", 1, 9999);
                if (OperacionesJugadores::eliminarJugadorLogica(id)) {
                    mostrarExito("Jugador eliminado.");
                } else {
                    mostrarError("ID inexistente.");
                }
                esperarEnter();
                break;
            }
        }
    } while (opcion != 0);
}

// --- Menú Partidos ---
void Interfaz::menuPartidos() {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║        GESTIÓN DE PARTIDOS                ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Programar partido                     ║" << endl;
        cout << "║  2. Registrar resultado                   ║" << endl;
        cout << "║  3. Buscar partido                        ║" << endl;
        cout << "║  4. Listar partidos                       ║" << endl;
        cout << "║  5. Cancelar partido                      ║" << endl;
        cout << "║  0. Volver                                ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione: ";

        opcion = leerOpcion(0, 5);

        switch (opcion) {
            case 1: {
                int loc = leerEntero("ID Local: ", 1, 9999);
                int vis = leerEntero("ID Visitante: ", 1, 9999);
                if (loc == vis) {
                    mostrarError("No puede ser el mismo equipo.");
                    esperarEnter();
                    break;
                }
                char fecha[11];
                leerCadena(fecha, 11, "Fecha (YYYY-MM-DD): ");
                while (!Validaciones::esFechaValida(fecha)) {
                    mostrarError("Formato inválido. Use YYYY-MM-DD");
                    leerCadena(fecha, 11, "Fecha (YYYY-MM-DD): ");
                }
                Partido p;
                if (OperacionesPartidos::programarPartido(loc, vis, fecha, p)) {
                    mostrarExito("Partido agendado con ID " + to_string(p.getId()));
                } else {
                    mostrarError("IDs inválidos o partido duplicado.");
                }
                esperarEnter();
                break;
            }
            case 2: {
                int idPart = leerEntero("ID Partido: ", 1, 9999);
                Partido p;
                if (!OperacionesPartidos::leerPartidoPorID(idPart, p)) {
                    mostrarError("Partido no existe.");
                    esperarEnter();
                    break;
                }
                if (!p.estaProgramado()) {
                    mostrarError("El partido no está PROGRAMADO.");
                    esperarEnter();
                    break;
                }
                int ptsL = leerEntero("Goles Local: ", 0, 100);
                int ptsV = leerEntero("Goles Visitante: ", 0, 100);
                Gol goles[22];
                int numGoles = 0;
                char resp;
                do {
                    if (numGoles >= 22) {
                        cout << "Límite de 22 goles alcanzado.\n";
                        break;
                    }
                    cout << "¿Registrar gol? (s/n): ";
                    cin >> resp;
                    Formatos::limpiarBuffer();
                    if (resp != 's' && resp != 'S') break;
                    int equipo = leerEntero("Equipo (0=Local, 1=Visitante): ", 0, 1);
                    int minuto = leerEntero("Minuto (0-120): ", 0, 120);
                    int idJug = leerEntero("ID Jugador (0 si desconocido): ", 0, 9999);
                    if (idJug != 0) {
                        Jugador j;
                        if (!OperacionesJugadores::leerJugadorPorID(idJug, j)) {
                            mostrarError("Jugador no existe, se omite.");
                            continue;
                        }
                    }
                    goles[numGoles] = Gol(idJug, minuto, equipo);
                    numGoles++;
                } while (true);

                if (OperacionesPartidos::registrarResultadoPartido(idPart, ptsL, ptsV, goles, numGoles)) {
                    mostrarExito("Resultado registrado exitosamente.");
                } else {
                    mostrarError("Error al registrar el resultado.");
                }
                esperarEnter();
                break;
            }
            case 3: {
                int sub;
                cout << "\n--- BUSCAR PARTIDO ---\n1. Por ID\n2. Por Equipo\nSeleccione: ";
                sub = leerOpcion(1, 2);
                if (sub == 1) {
                    int id = leerEntero("ID: ", 1, 9999);
                    Partido p;
                    if (OperacionesPartidos::leerPartidoPorID(id, p)) {
                        p.mostrarCompleto();
                    } else {
                        mostrarError("No existe.");
                    }
                } else {
                    int idEq = leerEntero("ID Equipo: ", 1, 9999);
                    Partido resultados[100];
                    int cant = OperacionesPartidos::listarPartidosPorEquipo(idEq, resultados, 100);
                    if (cant == 0) {
                        mostrarError("Sin partidos.");
                    } else {
                        cout << "\n--- RESULTADOS ---\n";
                        for (int i = 0; i < cant; ++i) {
                            resultados[i].mostrarBasico();
                            cout << endl;
                        }
                    }
                }
                esperarEnter();
                break;
            }
            case 4: {
                int sub;
                cout << "\n--- LISTAR PARTIDOS ---\n1. Todos\n2. Solo PROGRAMADOS\nSeleccione: ";
                sub = leerOpcion(1, 2);
                Partido resultados[100];
                int cant;
                if (sub == 2) {
                    cant = OperacionesPartidos::listarPartidosPorEstado("PROGRAMADO", resultados, 100);
                } else {
                    cant = OperacionesPartidos::listarPartidosPorEstado("", resultados, 100);
                }
                if (cant == 0) {
                    mostrarError("No hay partidos.");
                } else {
                    cout << "\n--- CALENDARIO DE PARTIDOS ---\n";
                    for (int i = 0; i < cant; ++i) {
                        resultados[i].mostrarBasico();
                        cout << endl;
                    }
                }
                esperarEnter();
                break;
            }
            case 5: {
                int id = leerEntero("ID del partido a cancelar: ", 1, 9999);
                if (OperacionesPartidos::cancelarPartidoLogica(id)) {
                    mostrarExito("Partido cancelado y estadísticas revertidas.");
                } else {
                    mostrarError("No se pudo cancelar (no existe o ya cancelado).");
                }
                esperarEnter();
                break;
            }
        }
    } while (opcion != 0);
}

// --- Menú Reportes ---
void Interfaz::menuReportes() {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║              REPORTES                     ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Tabla de posiciones                   ║" << endl;
        cout << "║  2. Tabla de goleadores (Top 10)          ║" << endl;
        cout << "║  3. Ficha técnica de partido              ║" << endl;
        cout << "║  0. Volver                                ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione: ";

        opcion = leerOpcion(0, 3);

        switch (opcion) {
            case 1: {
                Equipo resultados[100];
                int cant = OperacionesEquipos::listarEquiposOrdenados(resultados, 100);
                if (cant == 0) {
                    mostrarError("No hay equipos.");
                } else {
                    cout << "\n╔══════════════════════════════════════════════════════════════╗" << endl;
                    cout << "║                  TABLA DE POSICIONES                       ║" << endl;
                    cout << "╠════╦══════════════════════╦══════╦═══╦═══╦═══╦══════╦══════╣" << endl;
                    cout << "║ #  ║ Equipo               ║  PTS ║ V ║ E ║ D ║  GF  ║  GC  ║" << endl;
                    cout << "╠════╬══════════════════════╬══════╬═══╬═══╬═══╬══════╬══════╣" << endl;
                    for (int i = 0; i < cant; ++i) {
                        cout << "║ " << setw(2) << (i+1) << " ║ " << setw(20) << resultados[i].getNombre()
                             << " ║ " << setw(4) << resultados[i].getPuntos()
                             << " ║ " << setw(2) << resultados[i].getVictorias()
                             << " ║ " << setw(2) << resultados[i].getEmpates()
                             << " ║ " << setw(2) << resultados[i].getDerrotas()
                             << " ║ " << setw(4) << resultados[i].getGolesAFavor()
                             << " ║ " << setw(4) << resultados[i].getGolesEnContra() << " ║" << endl;
                    }
                    cout << "╚════╩══════════════════════╩══════╩═══╩═══╩═══╩══════╩══════╝" << endl;
                }
                esperarEnter();
                break;
            }
            case 2: {
                Jugador jugadores[100];
                int cant = OperacionesJugadores::listarTodosLosJugadores(jugadores, 100);
                // Ordenar por goles descendente
                for (int i = 0; i < cant - 1; ++i) {
                    for (int j = 0; j < cant - i - 1; ++j) {
                        if (jugadores[j].getGolesAnotados() < jugadores[j+1].getGolesAnotados()) {
                            Jugador aux = jugadores[j];
                            jugadores[j] = jugadores[j+1];
                            jugadores[j+1] = aux;
                        }
                    }
                }
                int top = (cant < 10) ? cant : 10;
                cout << "\n╔══════════════════════════════════════════════════════╗" << endl;
                cout << "║            TABLA DE GOLEADORES (TOP 10)            ║" << endl;
                cout << "╠════╦══════════════════╦══════════════╦═══════╗" << endl;
                cout << "║ #  ║ Jugador          ║ Equipo       ║ Goles ║" << endl;
                cout << "╠════╬══════════════════╬══════════════╬═══════╣" << endl;
                for (int i = 0; i < top; ++i) {
                    char equipoNom[100] = "Sin equipo";
                    if (jugadores[i].getIdEquipo() != 0) {
                        Equipo eq;
                        if (OperacionesEquipos::leerEquipoPorID(jugadores[i].getIdEquipo(), eq)) {
                            strcpy(equipoNom, eq.getNombre());
                        }
                    }
                    cout << "║ " << setw(2) << (i+1) << " ║ " << setw(16) << jugadores[i].getNombre()
                         << " ║ " << setw(12) << equipoNom
                         << " ║ " << setw(5) << jugadores[i].getGolesAnotados() << " ║" << endl;
                }
                cout << "╚════╩══════════════════╩══════════════╩═══════╝" << endl;
                esperarEnter();
                break;
            }
            case 3: {
                int id = leerEntero("ID del partido: ", 1, 9999);
                Partido p;
                if (OperacionesPartidos::leerPartidoPorID(id, p)) {
                    p.mostrarCompleto();
                } else {
                    mostrarError("Partido no existe.");
                }
                esperarEnter();
                break;
            }
        }
    } while (opcion != 0);
}

// --- Menú Mantenimiento ---
void Interfaz::menuMantenimiento() {
    int opcion;
    do {
        cout << "\n╔═══════════════════════════════════════════╗" << endl;
        cout << "║           MANTENIMIENTO                   ║" << endl;
        cout << "╠═══════════════════════════════════════════╣" << endl;
        cout << "║  1. Verificar integridad referencial      ║" << endl;
        cout << "║  2. Crear backup de datos                 ║" << endl;
        cout << "║  3. Configurar torneo                     ║" << endl;
        cout << "║  0. Volver                                ║" << endl;
        cout << "╚═══════════════════════════════════════════╝" << endl;
        cout << "Seleccione: ";

        opcion = leerOpcion(0, 3);

        switch (opcion) {
            case 1: {
                // Verificar integridad referencial
                cout << "\n╔══════════════════════════════════════════════════════╗" << endl;
                cout << "║      REPORTE DE INTEGRIDAD REFERENCIAL              ║" << endl;
                cout << "╠══════════════════════════════════════════════════════╣" << endl;
                int errores = 0;

                // Verificar jugadores
                int jugadoresVerificados = 0;
                GestorArchivos::recorrerRegistros<Jugador>(OperacionesJugadores::RUTA_JUGADORES, [&](const Jugador& j) {
                    jugadoresVerificados++;
                    if (j.getIdEquipo() != 0) {
                        Equipo eq;
                        if (!OperacionesEquipos::leerEquipoPorID(j.getIdEquipo(), eq)) {
                            cout << "ROTO: Jugador ID " << j.getId() << " apunta a Equipo ID " << j.getIdEquipo() << " (no existe)\n";
                            errores++;
                        }
                    }
                });

                // Verificar partidos
                int partidosVerificados = 0;
                GestorArchivos::recorrerRegistros<Partido>(OperacionesPartidos::RUTA_PARTIDOS, [&](const Partido& p) {
                    partidosVerificados++;
                    Equipo el, ev;
                    if (!OperacionesEquipos::leerEquipoPorID(p.getIdEquipoLocal(), el)) {
                        cout << "ROTO: Partido ID " << p.getId() << " Local ID " << p.getIdEquipoLocal() << " no existe\n";
                        errores++;
                    }
                    if (!OperacionesEquipos::leerEquipoPorID(p.getIdEquipoVisitante(), ev)) {
                        cout << "ROTO: Partido ID " << p.getId() << " Visitante ID " << p.getIdEquipoVisitante() << " no existe\n";
                        errores++;
                    }
                    const Gol* goles = p.getGoles();
                    for (int i = 0; i < p.getNumGoles(); ++i) {
                        if (goles[i].getIdJugador() != 0) {
                            Jugador j;
                            if (!OperacionesJugadores::leerJugadorPorID(goles[i].getIdJugador(), j)) {
                                cout << "ROTO: Gol en Partido ID " << p.getId() << " Jugador ID " << goles[i].getIdJugador() << " no existe\n";
                                errores++;
                            }
                        }
                    }
                });

                int equiposActivos = OperacionesEquipos::contarEquiposActivos();
                cout << "Equipos activos    : " << equiposActivos << "\n";
                cout << "Jugadores verificados : " << jugadoresVerificados << "\n";
                cout << "Partidos verificados  : " << partidosVerificados << "\n";
                cout << "Referencias rotas: " << errores << "\n";
                if (errores == 0) {
                    cout << "Estado: ✓ SISTEMA ÍNTEGRO\n";
                } else {
                    cout << "Estado: ✗ SE ENCONTRARON INCONSISTENCIAS\n";
                }
                cout << "╚══════════════════════════════════════════════════════╝\n";
                esperarEnter();
                break;
            }
            case 2: {
                // Crear backup
                time_t ahora = time(nullptr);
                struct tm* tiempo = localtime(&ahora);
                char nombreCarpeta[100];
                strftime(nombreCarpeta, sizeof(nombreCarpeta), "backup_%Y-%m-%d_%H-%M", tiempo);
#ifdef _WIN32
                string comando = "mkdir " + string(nombreCarpeta);
#else
                string comando = "mkdir -p " + string(nombreCarpeta);
#endif
                system(comando.c_str());

                const char* archivos[] = {"datos/torneo.bin", "datos/equipos.bin", "datos/jugadores.bin", "datos/partidos.bin"};
                const char* nombres[] = {"torneo.bin", "equipos.bin", "jugadores.bin", "partidos.bin"};
                bool ok = true;
                for (int i = 0; i < 4; ++i) {
                    ifstream src(archivos[i], ios::binary);
                    if (!src) {
                        cout << "No se pudo abrir " << archivos[i] << "\n";
                        ok = false;
                        continue;
                    }
                    string destPath = string(nombreCarpeta) + "/" + nombres[i];
                    ofstream dst(destPath, ios::binary);
                    if (!dst) {
                        cout << "No se pudo crear " << destPath << "\n";
                        ok = false;
                        src.close();
                        continue;
                    }
                    dst << src.rdbuf();
                    src.close();
                    dst.close();
                }
                if (ok) {
                    mostrarExito("Backup creado en: " + string(nombreCarpeta));
                } else {
                    mostrarError("Hubo errores durante el backup.");
                }
                esperarEnter();
                break;
            }
            case 3: {
                // Configurar torneo
                Torneo t;
                cout << "==========================================\n";
                cout << "   CONFIGURACION DEL TORNEO URU           \n";
                cout << "==========================================\n";
                if (OperacionesTorneo::existeTorneo()) {
                    OperacionesTorneo::leerTorneo(t);
                    cout << "Datos actuales:\n";
                    t.mostrarBasico();
                    cout << "\n¿Desea modificarlos? (s/n): ";
                    char resp;
                    cin >> resp;
                    Formatos::limpiarBuffer();
                    if (resp != 's' && resp != 'S') {
                        cout << "Configuración cancelada.\n";
                        esperarEnter();
                        break;
                    }
                }
                char nombreTorneo[100];
                char deporteTorneo[50];
                leerCadena(nombreTorneo, 100, "Nombre del torneo: ");
                leerCadena(deporteTorneo, 50, "Deporte (ej. Futbol, Baloncesto): ");
                t.setNombre(nombreTorneo);
                t.setDeporte(deporteTorneo);
                int opcFormato;
                cout << "Seleccione el formato:\n1. GRUPOS\n2. ELIMINATORIA\nOpcion: ";
                opcFormato = leerOpcion(1, 2);
                t.setFormato(opcFormato == 1 ? "GRUPOS" : "ELIMINATORIA");
                char fecha[11];
                leerCadena(fecha, 11, "Fecha inicio (YYYY-MM-DD): ");
                while (!Validaciones::esFechaValida(fecha)) {
                    mostrarError("Formato inválido.");
                    leerCadena(fecha, 11, "Fecha inicio (YYYY-MM-DD): ");
                }
                t.setFechaInicio(fecha);
                leerCadena(fecha, 11, "Fecha fin (YYYY-MM-DD): ");
                while (!Validaciones::esFechaValida(fecha)) {
                    mostrarError("Formato inválido.");
                    leerCadena(fecha, 11, "Fecha fin (YYYY-MM-DD): ");
                }
                t.setFechaFin(fecha);
                t.setFechaCreacion(time(nullptr));
                t.setFechaUltimaModificacion(time(nullptr));
                if (OperacionesTorneo::guardarTorneo(t)) {
                    mostrarExito("Torneo configurado exitosamente.");
                } else {
                    mostrarError("Error al guardar el torneo.");
                }
                esperarEnter();
                break;
            }
        }
    } while (opcion != 0);
}

// --- Funciones auxiliares ---

void Interfaz::mostrarTitulo(const char* titulo) {
    cout << "\n==========================================" << endl;
    cout << "   " << titulo << endl;
    cout << "==========================================" << endl;
}

void Interfaz::mostrarError(const std::string& mensaje) {
    cout << "[ERROR]: " << mensaje << endl;
}

void Interfaz::mostrarExito(const std::string& mensaje) {
    cout << "[SISTEMA]: " << mensaje << endl;
}

void Interfaz::esperarEnter() {
    cout << "Presione Enter para continuar...";
    cin.get();
}

int Interfaz::leerOpcion(int min, int max) {
    int opcion;
    cin >> opcion;
    while (cin.fail() || opcion < min || opcion > max) {
        cin.clear();
        Formatos::limpiarBuffer();
        cout << "Opción inválida. Intente de nuevo (" << min << "-" << max << "): ";
        cin >> opcion;
    }
    Formatos::limpiarBuffer();
    return opcion;
}

void Interfaz::leerCadena(char* buffer, int tamaño, const char* mensaje) {
    cout << mensaje;
    cin.getline(buffer, tamaño);
    while (cin.fail()) {
        cin.clear();
        Formatos::limpiarBuffer();
        cout << "Entrada demasiado larga. " << mensaje;
        cin.getline(buffer, tamaño);
    }
}

int Interfaz::leerEntero(const char* mensaje, int min, int max) {
    int valor;
    cout << mensaje;
    cin >> valor;
    while (cin.fail() || valor < min || valor > max) {
        cin.clear();
        Formatos::limpiarBuffer();
        cout << "Valor inválido (debe estar entre " << min << " y " << max << "). " << mensaje;
        cin >> valor;
    }
    Formatos::limpiarBuffer();
    return valor;
}