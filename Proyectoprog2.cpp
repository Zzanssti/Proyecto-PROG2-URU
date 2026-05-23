#include <iostream>
#include <string>

using namespace std;

//estructura de jugadores
struct jugador{
int id;
string nombre;
int edad;
int puntos;
};

//STRC equipos
struct team{
    int id;
    string nombre;

    jugador** jugadores;
    int num_jugadores;
    int capacidadjugadores;//para el redimensionamiento del arreglo

    //estadisticas
    int partidosganados;
    int partidosperdidos;
    int partidoempatados;
    int partidosjugados;
    int puntoscontra;
    int puntosfavor;
    int puntostotales;
};

struct torneo{

    string nombre;
    team** equipos;
    int cantidadequipos;
    int capacidadequipos;//para el redimensionamiento del arreglo





};
