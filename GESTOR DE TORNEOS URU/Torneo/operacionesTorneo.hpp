#ifndef OPERACIONES_TORNEO_HPP
#define OPERACIONES_TORNEO_HPP

#include "Torneo.hpp"

namespace OperacionesTorneo {

bool guardarTorneo(const Torneo& torneo);
bool leerTorneo(Torneo& torneo);
bool existeTorneo();

} // namespace OperacionesTorneo

#endif