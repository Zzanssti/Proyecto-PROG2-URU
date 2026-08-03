#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <ctime>
#include <string>
#include <cstring>
#include <cctype>

/**
 * Formatos.hpp
 * Clase estática con funciones de formateo de salida.
 * 
 * Proporciona utilidades para formatear fechas, limpiar buffers,
 * y manejar cadenas de texto.
 */

class Formatos {
public:

    static std::string timeToString(time_t t);

    static void limpiarBuffer();

    static void pausar();

    static void convertirMinusculas(const char* origen, char* destino);

    static bool contieneSubcadena(const char* texto, const char* subcadena);
};

#endif