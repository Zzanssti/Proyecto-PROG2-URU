#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include <string>

class Interfaz {
public:

    Interfaz();

    void ejecutar();

private:
    // Metodos de menu (cada uno maneja una opcion del menu principal)
    void menuPrincipal();

    // Submenus
    void menuEquipos();
    void menuJugadores();
    void menuPartidos();
    void menuReportes();
    void menuMantenimiento();

    // Funciones auxiliares de presentacion
    void mostrarTitulo(const char* titulo);
    void mostrarError(const std::string& mensaje);
    void mostrarExito(const std::string& mensaje);
    void esperarEnter();

    // Funciones de validacion de entrada
    int leerOpcion(int min, int max);
    void leerCadena(char* buffer, int tamaño, const char* mensaje);
    int leerEntero(const char* mensaje, int min, int max);
};

#endif