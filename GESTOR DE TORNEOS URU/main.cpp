#include "Interfaz/Interfaz.hpp"
#include "Persistencia/GestorArchivos.hpp"
#include "Equipos/operacionesEquipos.hpp"
#include "Jugadores/operacionesJugadores.hpp"
#include "Partidos/operacionesPartidos.hpp"


int main() {
    // Inicializar archivos
    GestorArchivos::inicializarArchivo("datos/torneo.bin");
    GestorArchivos::inicializarArchivo(OperacionesEquipos::RUTA_EQUIPOS);
    GestorArchivos::inicializarArchivo(OperacionesJugadores::RUTA_JUGADORES);
    GestorArchivos::inicializarArchivo(OperacionesPartidos::RUTA_PARTIDOS);

    // Ejecutar interfaz
    Interfaz interfaz;
    interfaz.ejecutar();

    return 0;
}