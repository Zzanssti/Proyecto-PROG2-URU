#include "Validaciones.hpp"

bool Validaciones::esTextoValido(const char* cadena) {
    if (!cadena || std::strlen(cadena) == 0) return false;
    for (size_t i = 0; cadena[i] != '\0'; ++i) {
        char c = cadena[i];
        if (!(std::isalpha(c) || c == ' ' || c == '.')) return false;
    }
    return true;
}

bool Validaciones::esCedulaValida(const char* cedula) {
    if (!cedula) return false;
    size_t len = std::strlen(cedula);
    if (len < 7 || len > 10) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!std::isdigit(cedula[i])) return false;
    }
    return true;
}

bool Validaciones::esFechaValida(const char* fecha) {
    if (!fecha || std::strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(fecha[i])) return false;
    }
    return true;
}

bool Validaciones::esNumeroEnRango(int valor, int min, int max) {
    return (valor >= min && valor <= max);
}