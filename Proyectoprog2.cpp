#include <iostream>
#include <cstring>

using namespace std;

//estructura de jugadores
struct jugador{
int id;
char* nombre;   //puntero para el nombre del jugador
int edad;
int puntos;
};

//STRC equipos
struct team{
    int id;
    char* nombre;   //puntero para el nombre del equipo

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
 //estructura principal del torneo
struct torneo{

    char* nombre;      //puntero para el nombre del torneo
    team** equipos;  //puntero para el arreglo de equipos
    int cantidadequipos;
    int capacidadequipos;//para el redimensionamiento del arreglo
};
// inicia la estructura del torneo,asigna el nombre y crea el arreglo de equipos
void inicializartorneo(torneo* &t, const char* nombreTorneo,int capacidadInicial){
    // reservamos la estructura en el heap
    t = new torneo;

    //control de cadenas con char
    int longitudnombre = strlen(nombreTorneo) + 1;
    t->nombre = new char[longitudnombre];
    //se copia el contenido del parametro al nuevo espacio reservado
    strcpy(t->nombre, nombreTorneo);

    //contadores de control
    t->cantidadequipos = 0;
    t->capacidadequipos = capacidadInicial;

    //reservamos el arreglo de equipos
    t->equipos = new team*[capacidadInicial];

    //iniciamos cada casilla en nullptr para limpiar la memoria
    for(int i = 0; i < capacidadInicial; i++){
        t->equipos[i] = nullptr;
    }
}










void liberartorneo(torneo* &t);


int main(){
    //prueba
    torneo* miTorneo= nullptr;
    
    inicializartorneo(miTorneo, "Torneo de Futbol", 2);
    cout<<"prueba de inicio con cahr"<< endl;
    cout<<"torneo creado: "<< miTorneo->nombre << endl;
    cout<<"capacidad inicial de equipos: "<< miTorneo->capacidadequipos << endl;


    return 0;
}