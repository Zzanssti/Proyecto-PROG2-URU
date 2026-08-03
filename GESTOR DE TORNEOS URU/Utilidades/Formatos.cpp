#include "Formatos.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>

std::string Formatos::timeToString(time_t t) {
    struct tm* tiempo = localtime(&t);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tiempo);
    return std::string(buffer);
}

void Formatos::limpiarBuffer() {
    std::cin.ignore(10000, '\n');
}

void Formatos::pausar() {
    std::cout << "Presione Enter para continuar...";
    std::cin.get();
}

void Formatos::convertirMinusculas(const char* origen, char* destino) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = std::tolower(origen[i]);
        i++;
    }
    destino[i] = '\0';
}

bool Formatos::contieneSubcadena(const char* texto, const char* subcadena) {
    if (!texto || !subcadena) return false;
    char textoMin[200], subMin[100];
    convertirMinusculas(texto, textoMin);
    convertirMinusculas(subcadena, subMin);
    return (std::strstr(textoMin, subMin) != nullptr);
}