#include "operacionesTorneo.hpp"
#include <fstream>
#include <cstring>

namespace OperacionesTorneo {

bool guardarTorneo(const Torneo& torneo) {
    // Abrir archivo en modo binario para escritura (sobrescribe)
    std::ofstream out("datos/torneo.bin", std::ios::binary);
    if (!out) return false;
    out.write(reinterpret_cast<const char*>(&torneo), sizeof(Torneo));
    out.close();
    return true;
}

bool leerTorneo(Torneo& torneo) {
    std::ifstream in("datos/torneo.bin", std::ios::binary);
    if (!in) return false;
    in.read(reinterpret_cast<char*>(&torneo), sizeof(Torneo));
    if (!in || in.gcount() != static_cast<std::streamsize>(sizeof(Torneo))) {
        in.close();
        return false;
    }
    in.close();
    return true;
}

bool existeTorneo() {
    Torneo aux;
    return leerTorneo(aux) && aux.esValido();
}

} // namespace OperacionesTorneo