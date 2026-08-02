
#ifndef TORNEO_HPP
#define TORNEO_HPP
#include <ctime>
#include <cstring>

class Torneo {

    private:
    char nombre[100];               // Nombre del torneo
    char deporte[50];               // Deporte (ej. Fútbol, Baloncesto)
    char formato[20];               // "GRUPOS" o "ELIMINATORIA"
    char fechaInicio[11];           // YYYY-MM-DD
    char fechaFin[11];              // YYYY-MM-DD
    time_t fechaCreacion;           // Fecha de creación del registro
    time_t fechaUltimaModificacion; // Fecha de última modificación

public:
    // Constructores
    Torneo();                                    // Por defecto
    Torneo(const char* nombre, const char* deporte, const char* formato,
           const char* fechaInicio, const char* fechaFin); // Parametrizado
    Torneo(const Torneo& otro);                  // Copia
    ~Torneo();                                   // Destructor (vacío)

    // Getters 
    const char* getNombre() const;
    const char* getDeporte() const;
    const char* getFormato() const;
    const char* getFechaInicio() const;
    const char* getFechaFin() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    // Setters con validaciones
    void setNombre(const char* nombre);          // No vacio y longitud < 100
    void setDeporte(const char* deporte);        // No vacio y longitud < 50
    bool setFormato(const char* formato);        // Solo "GRUPOS" o "ELIMINATORIA"
    bool setFechaInicio(const char* fecha);      // Formato YYYY-MM-DD
    bool setFechaFin(const char* fecha);         // Formato YYYY-MM-DD
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    // Metodos de negocio
    bool esValido() const;                       // Verifica que todos los campos esten completos
    void mostrarBasico() const;
    void mostrarCompleto() const;

    // Metodo estatico
    static size_t obtenerTamano();               // Retorna sizeof(Torneo)
};

#endif