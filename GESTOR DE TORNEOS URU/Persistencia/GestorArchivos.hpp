#ifndef GESTORARCHIVOS_HPP
#define GESTORARCHIVOS_HPP

#include <fstream>
#include <iostream>
#include <cstring>
#include "ArchivoHeader.hpp"

using namespace std;

class GestorArchivos {
public:
    // Inicializa el archivo (crea directorio y header si no existe)
    static bool inicializarArchivo(const char* ruta);

    // Lee el header del archivo
    static ArchivoHeader leerHeader(const char* ruta);

    // Actualiza el header (sobrescribe)
    static bool actualizarHeader(const char* ruta, const ArchivoHeader& header);

    //  METODOS TEMPLATE 
    // Guarda un nuevo registro al final del archivo (asigna ID si es 0)
    template <class T>
    static bool guardarRegistro(const char* ruta, T& objeto);

    // Lee un registro por su ID (carga en objeto, retorna true si existe y no eliminado)
    template <class T>
    static bool leerRegistroPorID(const char* ruta, int id, T& resultado);

    // Actualiza un registro existente (sobrescribe)
    template <class T>
    static bool actualizarRegistro(const char* ruta, const T& objeto);

    // Eliminación lógica (marca eliminado = true)
    template <class T>
    static bool eliminarRegistroLogico(const char* ruta, int id);

    // Cuenta registros activos (leyendo header)
    static int contarRegistrosActivos(const char* ruta);

    // Busca índice físico por ID (retorna -1 si no existe o eliminado)
    template <class T>
    static int buscarIndicePorID(const char* ruta, int id);

    // Obtiene el tamaño del registro (sizeof(T))
    template <class T>
    static size_t obtenerTamanoRegistro();

private:
    // Crea el directorio "datos" si no existe
    static void crearDirectorioDatos();
};

//  IMPLEMENTACION DE TEMPLATES 

template <class T>
bool GestorArchivos::guardarRegistro(const char* ruta, T& objeto) {
    // Carga los metadatos del archivo desde el disco a la memoria RAM
    ArchivoHeader header = leerHeader(ruta);
    // Preguntar si el objeto NO tiene ID es decir, es un registro nuevo
    if (objeto.getId() == 0) {
        // Asignar el ID disponible actual (
        objeto.setId(header.proximoID++);
        //
        header.cantidadRegistros++;
        header.registrosActivos++;
        header.version++;
    }
    fstream file(ruta, ios::in | ios::out | ios::binary); //Apertura del archivo en modo lectura/escritura binaria
    if (!file) return false;
    file.seekp(0, ios::end);//Posicionamiento para escritura (append)
    file.write(reinterpret_cast<const char*>(&objeto), sizeof(T));//Escritura binaria del objeto
    if (file.fail()) { file.close(); return false; }//Verificación de errores en la escritura
    file.close();
    return actualizarHeader(ruta, header);
}

template <class T>
bool GestorArchivos::leerRegistroPorID(const char* ruta, int id, T& resultado) {
    
    // Busca el indice del registro con ese ID
    // 'buscarIndicePorID' ya verifica que el registro exista y que NO este eliminado
    int idx = buscarIndicePorID<T>(ruta, id);
    if (idx == -1) return false; // No existe o ya fue borrado logicamente

    //Abrir el archivo en modo SOLO LECTURA binaria
    ifstream in(ruta, ios::binary);
    if (!in) return false; // Si no se puede abrir, falla.

    //Posiciona el puntero de lectura al inicio del registro idx
    in.seekg(sizeof(ArchivoHeader) + idx * sizeof(T), ios::beg);

    // Lee TODO el objeto completo desde esa posicion y lo guarda en resultado
    // Se escribe directamente en la referencia resultado que debe ser una variable ya existente del tipo 
    in.read(reinterpret_cast<char*>(&resultado), sizeof(T));

    // Cierra el archivo
    in.close();

    // Retorna true si el objeto NO está eliminado.
    //'buscarIndicePorID' ya comprobo que no lo esta
    // pero es una doble verificacion de seguridad
    return !resultado.isEliminado();
}

template <class T>
bool GestorArchivos::actualizarRegistro(const char* ruta, const T& objeto) {
    
    // Busca la posicion inicial del registro con el ID del objeto.
    // La función buscarIndicePorID ya comprueba que el registro exista
    // y que NO esté marcado como eliminado logicamente
    int idx = buscarIndicePorID<T>(ruta, objeto.getId());
    if (idx == -1) return false; // No existe o está eliminado no se puede actualizar

    // Abrir el archivo en modo lectura/escritura binaria
    // Necesitamos permisos de escritura para sobrescribir el registro
    fstream file(ruta, ios::in | ios::out | ios::binary);
    if (!file) return false;

    //Posiciona el puntero de escritura al inicio del registro idx
    file.seekp(sizeof(ArchivoHeader) + idx * sizeof(T), ios::beg);

    // Sobrescribe TODO el registro con el nuevo objeto.
    // El objeto se escribe en binario, tal cual está en memoria.
    file.write(reinterpret_cast<const char*>(&objeto), sizeof(T));

    //Verifica si hubo error en la escritura 
    if (file.fail()) { 
        file.close(); 
        return false; 
    }

    // Cierra el archivo antes de actualizar la cabecera
    file.close();

    // Lee la cabecera mas reciente desde el disco
    // Esto asegura que trabajamos con la versión más actualizada
    ArchivoHeader header = leerHeader(ruta);

    // Incrementa el numero de version del archivo
    // Cualquier modificacion estructural (como una actualizacion de datos)
    header.version++;

    // Guarda la cabecera actualizada de vuelta al principio del archivo
    return actualizarHeader(ruta, header);
}
template <class T>
bool GestorArchivos::eliminarRegistroLogico(const char* ruta, int id) {
    
    // 1. Busca el indice del registro con ese ID
    // Esta función ya verifica que el registro exista y NO esté eliminado
    int idx = buscarIndicePorID<T>(ruta, id);
    if (idx == -1) return false; // No existe o ya fue eliminado  no se puede borrar

    // Crear un objeto temporal y leer TODO su contenido desde el disco
    // Se asume que leerRegistroPorID es otra funcion que carga el objeto completo
    T objeto;
    if (!leerRegistroPorID<T>(ruta, id, objeto)) return false; // Fallo en lectura 

    // Marcar el objeto como eliminado lógicamente
    objeto.setEliminado(true);

    // Abrir el archivo en modo lectura/escritura binaria para sobrescribir solo ese registro
    fstream file(ruta, ios::in | ios::out | ios::binary);
    if (!file) return false;

    // Posicionar el puntero de escritura exactamente al inicio del registro idx
    file.seekp(sizeof(ArchivoHeader) + idx * sizeof(T), ios::beg);

    //Sobrescribir el registro completo con la nueva version del objeto marcado como eliminado
    file.write(reinterpret_cast<const char*>(&objeto), sizeof(T));
    file.close(); // Cierra el archivo antes de actualizar la cabecera.

    // Volver a leer la cabecera del disco para obtener la version mas reciente
    // ya que podría haber sido modificada por otro proceso mientras tanto
    ArchivoHeader header = leerHeader(ruta);

    // Decrementar el contador de registros activos
    header.registrosActivos--;

    //Incrementa la version del archivo para avisar de este cambio estructural
    header.version++;

    // Escribir la cabecera modificada de vuelta al principio del archivo
    //Si falla, retorna false y si funciona, retorna true
    return actualizarHeader(ruta, header);
}

template <class T>
int GestorArchivos::buscarIndicePorID(const char* ruta, int id) {
    
    // Abre el archivo en modo SOLO Lectura
    ifstream in(ruta, ios::binary);
    if (!in) return -1; // Si no existe o no se puede abrir retorna 

    // Lee la cabecera (metadatos) del principio del archivo.
    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    
    // Si el contador histórico dice que nunca se ha escrito nada, cierra y retorna
    if (header.cantidadRegistros == 0) { in.close(); return -1; }
    
    // Calcula el tamaño fijo en bytes que ocupa cada registro de tipo T
    size_t tam = sizeof(T);

    // Recorre TODOS los registros que se han escrito alguna vez (incluyendo los eliminados)
    // Se usa cantidadRegistros y NO registrosActivos para poder encontrar
    //  registros aunque estén marcados como borrados
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        
        // Mueve el puntero de lectura al inicio del registro 
        in.seekg(sizeof(header) + i * tam, ios::beg);
        
        // LEE SOLO LOS PRIMEROS 4 BYTES (int) de ese registro.
        // SUPONE que el campo id es el PRIMER miembro de la clase/estructura T.
        int idActual;
        in.read(reinterpret_cast<char*>(&idActual), sizeof(int));
        if (in.fail()) break; // Si falla la lectura, sale del bucle.

        // Si el ID leído coincide con el que buscamos
        if (idActual == id) {
            
            //  Vuelve a posicionarse al INICIO del mismo registro 
            in.seekg(sizeof(header) + i * tam, ios::beg);
            
            //  Reserva memoria DINAMICA para copiar todo el registro completo
            char* buffer = new char[tam];
            
            //  Lee el registro ENTERO en ese buffer
            in.read(buffer, tam);
            
        
            //     Asume que el ULTIMO byte del objeto  contiene un flag 
            //     que indica si el registro está marcado como eliminado lógicamente
            bool eliminado = buffer[tam - 1]; 
            
            //Libera la memoria del buffer.
            delete[] buffer;
            
            // Si NO está eliminado el registro es valido
            if (!eliminado) {
                in.close();
                return i;
            } else {
                

    
                break; 
            }
        }
    }
    
    in.close();
    return -1;
}

template <class T>
size_t GestorArchivos::obtenerTamanoRegistro() {
    return sizeof(T);
}

#endif