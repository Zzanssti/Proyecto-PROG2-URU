#ifndef GOL_HPP
#define GOL_HPP

class Gol {
private:
    int idJugador;   // ID del jugador que anotó (0 si autogol o desconocido)
    int minuto;      // Minuto del partido en que se anotó (0-120)
    int equipo;      // 0 = local, 1 = visitante

public:
    //  Constructores 
    Gol();                                    // Por defecto (todos a 0)
    Gol(int idJugador, int minuto, int equipo); // Parametrizado
    Gol(const Gol& otro);                     // Copia
    ~Gol();                                   // Destructor (vacío)

    //  Getters
    int getIdJugador() const;
    int getMinuto() const;
    int getEquipo() const;

    // Setters con validaciones 
    void setIdJugador(int id);                // Acepta >= 0 (0 = desconocido)
    bool setMinuto(int minuto);              // Rango 0-120
    bool setEquipo(int equipo);              // Solo 0 o 1

    // Metodos de negocio
    bool esValido() const;                   // Verifica minuto y equipo válidos
    void mostrar() const;                    // Presentación en una línea

    // Metodo estático 
    static size_t obtenerTamano();           // sizeof(Gol)
};

#endif