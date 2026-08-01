#ifndef PARTIDO_HPP
#define PARTIDO_HPP

#include <ctime>
#include <cstring>
#include "Gol.hpp"

class Partido {
private:
    int id;                     // 4
    int idEquipoLocal;          // 4
    int idEquipoVisitante;      // 4
    char fecha[11];             // 11
    char estado[12];            // 12 ("PROGRAMADO", "JUGADO", "CANCELADO")
    char descripcion[200];      // 200
    int golesLocal;             // 4
    int golesVisitante;         // 4
    Gol goles[22];              // 22 * sizeof(Gol)
    int numGoles;               // 4
    bool eliminado;             // 1 (con padding)
    time_t fechaCreacion;       // 8
    time_t fechaUltimaModificacion; // 8

public:
    // Constructores 
    Partido();                                    // Por defecto
    Partido(int idLocal, int idVisitante, const char* fecha,
            const char* estado = "PROGRAMADO", const char* descripcion = "Fase General");
    Partido(const Partido& otro);                 // Copia
    ~Partido();                                   // Destructor

    // Getters 
    int getId() const;
    int getIdEquipoLocal() const;
    int getIdEquipoVisitante() const;
    const char* getFecha() const;
    const char* getEstado() const;
    const char* getDescripcion() const;
    int getGolesLocal() const;
    int getGolesVisitante() const;
    const Gol* getGoles() const;   // Devuelve puntero al arreglo (lectura)
    int getNumGoles() const;
    bool getEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    // Setters con validaciones 
    void setId(int id);
    bool setIdEquipoLocal(int id);                // Rechaza <=0
    bool setIdEquipoVisitante(int id);            // Rechaza <=0
    void setFecha(const char* fecha);             // Valida formato YYYY-MM-DD (solo longitud y guiones)
    bool setEstado(const char* estado);           // Solo PROGRAMADO, JUGADO, CANCELADO
    void setDescripcion(const char* desc);
    bool setGolesLocal(int goles);                // Rechaza negativos
    bool setGolesVisitante(int goles);            // Rechaza negativos
    bool agregarGol(const Gol& gol);              // Añade gol si hay espacio y es válido
    void setEliminado(bool estado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    // Metodos de negocio 
    bool esValido() const;                        // Verifica equipos, fecha, estado
    bool estaJugado() const;
    bool estaProgramado() const;
    bool estaCancelado() const;

    //  Metodos de presentacion 
    void mostrarBasico() const;
    void mostrarCompleto() const;

    //  Metodo estatico 
    static size_t obtenerTamano();                // sizeof(Partido)
};

#endif