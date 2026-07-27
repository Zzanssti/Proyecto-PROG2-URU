#ifndef JUGADOR_HPP
#define JUGADOR_HPP

#include <ctime>
#include <cstring>

class Jugador {
private:
    // Atributos en el mismo orden que el struct original
    int id;                     // 4 bytes
    int idEquipo;               // 4
    char nombre[100];           // 100
    char cedula[20];            // 20
    char posicion[20];          // 20
    int edad;                   // 4
    int numeroDorsal;           // 4
    int golesAnotados;          // 4
    int tarjetasAmarillas;      // 4
    int tarjetasRojas;          // 4
    bool eliminado;             // 1 (con padding)
    time_t fechaCreacion;       // 8
    time_t fechaUltimaModificacion; // 8

public:
    //  Constructores 
    Jugador();                                    // Por defecto
    Jugador(int idEquipo, const char* nombre, const char* cedula,
            const char* posicion, int edad, int dorsal); // Parametrizado
    Jugador(const Jugador& otro);                 // Copia
    ~Jugador();                                   // Destructor (vacío)

    // Getters 
    int getId() const;
    int getIdEquipo() const;
    const char* getNombre() const;
    const char* getCedula() const;
    const char* getPosicion() const;
    int getEdad() const;
    int getNumeroDorsal() const;
    int getGolesAnotados() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    bool getEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    // Setters con validaciones 
    void setId(int id);
    bool setIdEquipo(int idEquipo);              // Rechaza negativos
    void setNombre(const char* nombre);           // Valida que no esté vacío
    bool setCedula(const char* cedula);          // Valida longitud y dígitos
    void setPosicion(const char* posicion);      // Valida que no esté vacío
    bool setEdad(int edad);                      // Rango 14-60
    bool setNumeroDorsal(int dorsal);            // Rango 1-99
    bool setGolesAnotados(int goles);            // Rechaza negativos
    bool setTarjetasAmarillas(int amarillas);    // Rechaza negativos
    bool setTarjetasRojas(int rojas);            // Rechaza negativos
    void setEliminado(bool estado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    // Metodos de negocio 
    bool esValido() const;                       // Verifica todos los campos
    void incrementarGoles();                     // Aumenta en 1
    void incrementarAmarilla();                  // Aumenta en 1
    void incrementarRoja();                      // Aumenta en 1

    //Metodos de presentacion
    void mostrarBasico() const;   // Linea resumida
    void mostrarCompleto() const; // Ficha detallada

    //Metodo estático para templates 
    static size_t obtenerTamano();  // Retorna sizeof(Jugador)
};

#endif