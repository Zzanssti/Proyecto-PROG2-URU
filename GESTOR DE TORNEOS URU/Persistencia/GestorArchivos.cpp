#include "GestorArchivos.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#ifdef _WIN32
#include <direct.h>
#include <sys/stat.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

bool GestorArchivos::inicializarArchivo(const char* ruta) {
    crearDirectorioDatos();
    std::ifstream in(ruta, std::ios::binary);
    if (!in) {
        std::ofstream out(ruta, std::ios::binary);
        if (!out) return false;
        ArchivoHeader header = {0, 1, 0, 1};
        out.write(reinterpret_cast<const char*>(&header), sizeof(header));
        out.close();
        return true;
    }

    in.seekg(0, std::ios::end);
    std::streampos size = in.tellg();
    in.close();

    if (size < static_cast<std::streampos>(sizeof(ArchivoHeader))) {
        ArchivoHeader header = {0, 1, 0, 1};
        return actualizarHeader(ruta, header);
    }
    return true;
}

ArchivoHeader GestorArchivos::leerHeader(const char* ruta) {
    ArchivoHeader header = {0, 1, 0, 1};
    std::ifstream in(ruta, std::ios::binary);
    if (!in) return header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (in.fail()) {
        in.close();
        return ArchivoHeader{0, 1, 0, 1};
    }
    in.close();
    return header;
}

bool GestorArchivos::actualizarHeader(const char* ruta, const ArchivoHeader& header) {
    std::fstream file(ruta, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.open(ruta, std::ios::out | std::ios::binary);
        if (!file) return false;
    }
    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    if (file.fail()) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

int GestorArchivos::contarRegistrosActivos(const char* ruta) {
    ArchivoHeader header = leerHeader(ruta);
    return header.registrosActivos;
}

void GestorArchivos::crearDirectorioDatos() {
#ifdef _WIN32
    struct _stat info;
    if (_stat("datos", &info) != 0) {
        _mkdir("datos");
    }
#else
    struct stat info;
    if (stat("datos", &info) != 0) {
        mkdir("datos", 0755);
    }
#endif
}
