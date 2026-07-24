#ifndef EQUIPO_HPP
#define EQUIPO_HPP

#include <ctime>
#include <cstring>

/**
 * @file Equipo.hpp
 * @brief Define la clase Equipo con encapsulamiento.
 * 
 * Convierte la struct Equipo del Proyecto 2 en una clase con atributos privados.
 */

class Equipo {
private:
    // Atributos privados (mismo orden y tipos que el struct original)
    int id;
    char nombre[100];
    char ciudad[100];
    char entrenador[100];
    int puntos;
    int victorias;
    int empates;
    int derrotas;
    int golesAFavor;
    int golesEnContra;
    int partidosIDs[50];
    int cantidadPartidos;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    // CONSTRUCTORES Y DESTRUCTOR 
    
    /**
     * @brief Constructor por defecto. Inicializa todos los atributos a valores neutros.
     */
    Equipo();
    
    /**
     * @brief Constructor parametrizado con los datos principales
     * @param nombre Nombre del equipo
     * @param ciudad Ciudad de origen
     * @param entrenador Nombre del entrenador
     * 
     * Los estadisticas (puntos, partidos, etc.) se inicializan a 0
     */
    Equipo(const char* nombre, const char* ciudad, const char* entrenador);
    
    /**
     * @brief Constructor de copia.
     * @param otro Equipo a copiar.
     */
    Equipo(const Equipo& otro);
    
    /**
     * @brief Destructor. No realiza acciones especiales.
     */
    ~Equipo();

    // GETTERS (acceso de solo lectura) 
    int getId() const;
    const char* getNombre() const;
    const char* getCiudad() const;
    const char* getEntrenador() const;
    int getPuntos() const;
    int getVictorias() const;
    int getEmpates() const;
    int getDerrotas() const;
    int getGolesAFavor() const;
    int getGolesEnContra() const;
    const int* getPartidosIDs() const;   // Retorna puntero al arreglo
    int getCantidadPartidos() const;
    bool getEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    //SETTERS (con validaciones)
    void setId(int id);
    void setNombre(const char* nombre);
    void setCiudad(const char* ciudad);
    void setEntrenador(const char* entrenador);
    bool setPuntos(int pts);            // Retorna false si el valor es negativo
    bool setVictorias(int v);
    bool setEmpates(int e);
    bool setDerrotas(int d);
    bool setGolesAFavor(int gf);
    bool setGolesEnContra(int gc);
    void agregarPartidoID(int idPartido); // Agrega un ID al arreglo si hay espacio
    void setEliminado(bool estado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    //"Metodos de  negocio"
    /**
     * @brief Verifica que los datos del equipo sean validos (nombre, ciudad, entrenador no vacíos).
     * @return true si el equipo es valido y no está eliminado
     */
    bool esValido() const;
    
    /**
     * @brief Actualiza las estadisticas del equipo tras un partido
     * @param golesFavor Goles marcados por el equipo
     * @param golesContra Goles recibidos
     * @param resultado 1 = victoria, 0 = empate, -1 = derrota
     */
    void actualizarEstadisticas(int golesFavor, int golesContra, int resultado);
    
    /**
     * @brief Muestra una linea resumen del equipo (ID, nombre, puntos, etc.)
     */
    void mostrarBasico() const;
    
    /**
     * @brief Muestra todos los datos del equipo de forma detallada.
     */
    void mostrarCompleto() const;

    //Metodo estatico
    /**
      @brief Retorna el tamaño en bytes de la clase (para persistencia).
      @return sizeof(Equipo)
    
    static size_t obtenerTamano();
    **/
}
#endif