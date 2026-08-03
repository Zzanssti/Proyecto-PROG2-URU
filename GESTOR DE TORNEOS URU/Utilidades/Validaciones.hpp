#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <cstring>
#include <cctype>

class Validaciones {
public:
    /**
     * Verifica que una cadena contenga solo letras, espacios y puntos
     * para cadena Texto a validar
     * reotrna true si es valido
     */
    static bool esTextoValido(const char* cadena);

    /**
     * Verifica que una cedula tenga 7-10 digitos
     * para cedula Cedula a validar
     * retorna true si es valida
     */
    static bool esCedulaValida(const char* cedula);

    /**
     * Verifica formato YYYY-MM-DD
     * fecha Fecha a validar
     * retorna true si el formato es correcto
     */
    static bool esFechaValida(const char* fecha);

    /**
     * Verifica que un numero este dentro de un rango
     * para valor Numero a verificar
     * para min Limite inferior
     * para max Límite superior
     * retorna true si valor está en [min, max]
     */
    static bool esNumeroEnRango(int valor, int min, int max);
};

#endif