GESTOR DE TORNEOS URU

## Sistema de Gestión de Torneos Deportivos - POO y Modularización

---

## 📋 Índice

1. [Descripción del Proyecto](#-descripción-del-proyecto)
2. [Objetivos de Aprendizaje](#-objetivos-de-aprendizaje)
3. [Arquitectura del Sistema](#-arquitectura-del-sistema)
4. [Estructura del Proyecto](#-estructura-del-proyecto)
5. [Requisitos del Sistema](#-requisitos-del-sistema)
6. [Instalación y Compilación](#-instalación-y-compilación)
7. [Manual de Usuario](#-manual-de-usuario)
8. [Diagrama de Clases](#-diagrama-de-clases)
9. [Principios de Diseño Aplicados](#-principios-de-diseño-aplicados)
10. [Funcionalidades Principales](#-funcionalidades-principales)
11. [Validaciones y Reglas de Negocio](#-validaciones-y-reglas-de-negocio)
12. [Persistencia de Datos](#-persistencia-de-datos)
13. [Mantenimiento y Backup](#-mantenimiento-y-backup)
14. [Autores y Contribuciones](#-autores-y-contribuciones)

---

## 📝 Descripción del Proyecto

**Gestor de Torneos URU** es un sistema de gestión deportiva desarrollado en C++ que permite administrar torneos, equipos, jugadores y partidos. El proyecto representa la evolución de un sistema monolítico hacia una **arquitectura modular con Programación Orientada a Objetos**, aplicando principios de diseño sólidos y buenas prácticas de programación.

### Características Principales

- ✅ Gestión completa de torneos (configuración, fechas, formato)
- ✅ Registro y administración de equipos
- ✅ Gestión de jugadores con cédula y dorsal único
- ✅ Programación de partidos y registro de resultados
- ✅ Tabla de posiciones automática
- ✅ Reporte de goleadores (Top 10)
- ✅ Persistencia en archivos binarios
- ✅ Backup y restauración de datos
- ✅ Verificación de integridad referencial

---

## 🎯 Objetivos de Aprendizaje

Este proyecto fue desarrollado como parte del curso **Programación 2** y tiene como objetivos:

1. **Aplicar Programación Orientada a Objetos (POO)** en C++
   - Convertir `struct` a `class` con encapsulamiento
   - Constructores, destructores y métodos de acceso
   - Composición de clases

2. **Organización Modular**
   - Separar declaraciones (`.hpp`) de implementaciones (`.cpp`)
   - Módulos organizados por dominio
   - Compilación multi-archivo con `Makefile`

3. **Templates y Código Genérico**
   - `GestorArchivos<T>` para operaciones de persistencia
   - Eliminación de código duplicado

4. **Persistencia en Archivos Binarios**
   - Acceso aleatorio con `seekg()` y `seekp()`
   - Headers para metadatos
   - Borrado lógico

5. **Arquitectura de Capas**
   - Presentación (Interfaz)
   - Lógica de negocio (Operaciones)
   - Entidades (Clases POJO)
   - Persistencia (GestorArchivos)

---

## 🏗️ Arquitectura del Sistema

El sistema sigue una **arquitectura por capas** de 4 niveles:

```
┌──────────────────────────────────────────────────────────────┐
│                     CAPA DE PRESENTACIÓN                    │
│                   (Interfaz/Interfaz)                      │
│   - Menús interactivos                                     │
│   - Validación de entrada del usuario                      │
│   - Loop principal del programa                            │
└────────────────────────────┬─────────────────────────────────┘
                             │ (llama a)
┌────────────────────────────▼─────────────────────────────────┐
│                       CAPA DE NEGOCIO                        │
│   (operacionesEquipos, operacionesJugadores,               │
│    operacionesPartidos, operacionesTorneo)                 │
│   - Lógica de negocio                                      │
│   - Validaciones de reglas de negocio                      │
│   - Coordinación entre entidades                           │
└────────────────────────────┬─────────────────────────────────┘
                             │ (usa)
┌────────────────────────────▼─────────────────────────────────┐
│                       CAPA DE ENTIDADES                      │
│   (Equipo, Jugador, Gol, Partido, Torneo)                 │
│   - Encapsulamiento de datos                               │
│   - Métodos de negocio propios                             │
│   - Validaciones internas                                  │
└────────────────────────────┬─────────────────────────────────┘
                             │ (usa)
┌────────────────────────────▼─────────────────────────────────┐
│                      CAPA DE PERSISTENCIA                    │
│   (GestorArchivos, ArchivoHeader)                          │
│   - Lectura/escritura binaria                              │
│   - Templates para operaciones genéricas                   │
│   - Manejo de headers y metadatos                          │
└──────────────────────────────────────────────────────────────┘
```

---

## 📁 Estructura del Proyecto

```
GESTOR DE TORNEOS URU/
│
├── Equipos/                     # Módulo de equipos
│   ├── Equipo.hpp               # Clase Equipo (declaración)
│   ├── Equipo.cpp               # Clase Equipo (implementación)
│   ├── operacionesEquipos.hpp   # Operaciones de negocio (declaración)
│   └── operacionesEquipos.cpp   # Operaciones de negocio (implementación)
│
├── Jugadores/                   # Módulo de jugadores
│   ├── Jugador.hpp              # Clase Jugador (declaración)
│   ├── Jugador.cpp              # Clase Jugador (implementación)
│   ├── operacionesJugadores.hpp # Operaciones de negocio (declaración)
│   └── operacionesJugadores.cpp # Operaciones de negocio (implementación)
│
├── Partidos/                    # Módulo de partidos
│   ├── Gol.hpp                  # Clase Gol (declaración)
│   ├── Gol.cpp                  # Clase Gol (implementación)
│   ├── Partido.hpp              # Clase Partido (declaración)
│   ├── Partido.cpp              # Clase Partido (implementación)
│   ├── operacionesPartidos.hpp  # Operaciones de negocio (declaración)
│   └── operacionesPartidos.cpp  # Operaciones de negocio (implementación)
│
├── Torneo/                      # Módulo de torneo
│   ├── Torneo.hpp               # Clase Torneo (declaración)
│   ├── Torneo.cpp               # Clase Torneo (implementación)
│   ├── operacionesTorneo.hpp    # Operaciones de negocio (declaración)
│   └── operacionesTorneo.cpp    # Operaciones de negocio (implementación)
│
├── Persistencia/                # Módulo de persistencia
│   ├── ArchivoHeader.hpp        # Estructura de metadatos de archivos
│   └── GestorArchivos.hpp       # Clase template para operaciones genéricas
│
├── Utilidades/                  # Módulo de utilidades
│   ├── Validaciones.hpp         # Validaciones de formato (declaración)
│   ├── Validaciones.cpp         # Validaciones de formato (implementación)
│   ├── Formatos.hpp             # Formateo de salida (declaración)
│   └── Formatos.cpp             # Formateo de salida (implementación)
│
├── Interfaz/                    # Módulo de interfaz de usuario
│   ├── Interfaz.hpp             # Clase Interfaz (declaración)
│   └── Interfaz.cpp             # Clase Interfaz (implementación)
│
├── datos/                       # Archivos de datos (persistencia)
│   ├── torneo.bin               # Configuración del torneo
│   ├── equipos.bin              # Datos de equipos
│   ├── jugadores.bin            # Datos de jugadores
│   └── partidos.bin             # Datos de partidos
│
├── main.cpp                     # Punto de entrada del programa
├── Makefile                     # Archivo de compilación
└── README.md                    # Esta documentación
```

---

## 💻 Requisitos del Sistema

### Compilador
- **C++11** o superior
- Recomendado: **g++ (GCC)** versión 7.0 o superior
- Windows: **MinGW-w64** o **MSYS2**
- Linux/Mac: **GCC** o **Clang**

### Herramientas
- **Make** (para compilación automática)
- Sistema operativo: Windows, Linux o macOS

### Espacio en Disco
- Mínimo: ~10 MB (código y datos)

---

## 🔧 Instalación y Compilación

### Opción 1: Compilación con Makefile (Recomendado)

#### Windows (con MinGW)
```bash
# Asegúrate de que MinGW esté en el PATH
mingw32-make

# Si make está instalado
make

# Limpiar archivos objeto
mingw32-make clean

# Compilar y ejecutar
mingw32-make run
```

#### Linux/Mac
```bash
# Compilar
make

# Limpiar
make clean

# Compilar y ejecutar
make run
```

### Opción 2: Compilación Manual

#### Windows (CMD/PowerShell)
```bash
g++ -std=c++11 -I. main.cpp Equipos/Equipo.cpp Equipos/operacionesEquipos.cpp Jugadores/Jugador.cpp Jugadores/operacionesJugadores.cpp Partidos/Gol.cpp Partidos/Partido.cpp Partidos/operacionesPartidos.cpp Torneo/Torneo.cpp Torneo/operacionesTorneo.cpp Utilidades/Formatos.cpp Utilidades/Validaciones.cpp Interfaz/Interfaz.cpp -o torneo.exe
```

#### Linux/Mac
```bash
g++ -std=c++11 -I. main.cpp Equipos/Equipo.cpp Equipos/operacionesEquipos.cpp Jugadores/Jugador.cpp Jugadores/operacionesJugadores.cpp Partidos/Gol.cpp Partidos/Partido.cpp Partidos/operacionesPartidos.cpp Torneo/Torneo.cpp Torneo/operacionesTorneo.cpp Utilidades/Formatos.cpp Utilidades/Validaciones.cpp Interfaz/Interfaz.cpp -o torneo
```

### Ejecución
```bash
# Windows
torneo.exe

# Linux/Mac
./torneo
```

---

## 📖 Manual de Usuario

### Menú Principal
```
╔═══════════════════════════════════════════════╗
║         GESTOR DE TORNEOS URU                ║
╠═══════════════════════════════════════════════╣
║  Torneo: [Nombre del Torneo]
║  Deporte: [Deporte] ([Formato])
╚═══════════════════════════════════════════════╝

1. Gestión de Equipos
2. Gestión de Jugadores
3. Gestión de Partidos
4. Configuración del Torneo
5. Reportes
6. Mantenimiento
0. Salir
```

### 1. Gestión de Equipos
```
╔═══════════════════════════════════════════╗
║          GESTIÓN DE EQUIPOS               ║
╠═══════════════════════════════════════════╣
║  1. Registrar equipo                      ║
║  2. Buscar equipo                         ║
║  3. Actualizar equipo                     ║
║  4. Listar equipos                        ║
║  5. Eliminar equipo                       ║
║  0. Volver                                ║
╚═══════════════════════════════════════════╝
```

### 2. Gestión de Jugadores
```
╔═══════════════════════════════════════════╗
║        GESTIÓN DE JUGADORES               ║
╠═══════════════════════════════════════════╣
║  1. Registrar jugador                     ║
║  2. Buscar jugador                        ║
║  3. Actualizar jugador                    ║
║  4. Listar todos los jugadores            ║
║  5. Listar jugadores por equipo           ║
║  6. Eliminar jugador                      ║
║  0. Volver                                ║
╚═══════════════════════════════════════════╝
```

### 3. Gestión de Partidos
```
╔═══════════════════════════════════════════╗
║        GESTIÓN DE PARTIDOS                ║
╠═══════════════════════════════════════════╣
║  1. Programar partido                     ║
║  2. Registrar resultado                   ║
║  3. Buscar partido                        ║
║  4. Listar partidos                       ║
║  5. Cancelar partido                      ║
║  0. Volver                                ║
╚═══════════════════════════════════════════╝
```

### 4. Reportes
```
╔═══════════════════════════════════════════╗
║              REPORTES                     ║
╠═══════════════════════════════════════════╣
║  1. Tabla de posiciones                   ║
║  2. Tabla de goleadores (Top 10)          ║
║  3. Ficha técnica de partido              ║
║  0. Volver                                ║
╚═══════════════════════════════════════════╝
```

### 5. Mantenimiento
```
╔═══════════════════════════════════════════╗
║           MANTENIMIENTO                   ║
╠═══════════════════════════════════════════╣
║  1. Verificar integridad referencial      ║
║  2. Crear backup de datos                 ║
║  0. Volver                                ║
╚═══════════════════════════════════════════╝
```

---

## 📐 Diagrama de Clases

```
┌─────────────────────────────────────────────────────────────────────┐
│                            GESTOR ARCHIVOS                         │
│  + inicializarArchivo(ruta: const char*): bool                     │
│  + leerHeader(ruta: const char*): ArchivoHeader                    │
│  + actualizarHeader(ruta: const char*, header: ArchivoHeader): bool│
│  + guardarRegistro<T>(ruta: const char*, objeto: T&): bool         │
│  + leerRegistroPorID<T>(ruta: const char*, id: int, resultado: T&): bool│
│  + actualizarRegistro<T>(ruta: const char*, objeto: const T&): bool│
│  + eliminarRegistroLogico<T>(ruta: const char*, id: int): bool     │
│  + contarRegistrosActivos(ruta: const char*): int                  │
│  + buscarIndicePorID<T>(ruta: const char*, id: int): int           │
│  + recorrerRegistros<T>(ruta: const char*, callback: Func)         │
└─────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│                             TORNEO                                 │
│  - nombre: char[100]                                              │
│  - deporte: char[50]                                              │
│  - formato: char[20]                                              │
│  - fechaInicio: char[11]                                          │
│  - fechaFin: char[11]                                             │
│  - fechaCreacion: time_t                                          │
│  - fechaUltimaModificacion: time_t                                │
│  + Torneo()                                                       │
│  + Torneo(nombre, deporte, formato, fechaInicio, fechaFin)        │
│  + getNombre(): const char*                                       │
│  + setNombre(nombre: const char*)                                 │
│  + esValido(): bool                                               │
│  + mostrarBasico(): void                                          │
│  + mostrarCompleto(): void                                        │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                             EQUIPO                                 │
│  - id: int                                                         │
│  - nombre: char[100]                                               │
│  - ciudad: char[100]                                               │
│  - entrenador: char[100]                                           │
│  - puntos: int                                                     │
│  - victorias: int                                                  │
│  - empates: int                                                    │
│  - derrotas: int                                                   │
│  - golesAFavor: int                                                │
│  - golesEnContra: int                                              │
│  - partidosIDs: int[50]                                            │
│  - cantidadPartidos: int                                           │
│  - eliminado: bool                                                 │
│  - fechaCreacion: time_t                                           │
│  - fechaUltimaModificacion: time_t                                 │
│  + Equipo()                                                        │
│  + Equipo(nombre, ciudad, entrenador)                              │
│  + getId(): int                                                    │
│  + getNombre(): const char*                                        │
│  + setNombre(nombre: const char*)                                  │
│  + esValido(): bool                                                │
│  + actualizarEstadisticas(golesFavor, golesContra, resultado)     │
│  + mostrarBasico(): void                                           │
│  + mostrarCompleto(): void                                         │
│  + obtenerTamano(): size_t                                         │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                             JUGADOR                                │
│  - id: int                                                         │
│  - idEquipo: int                                                   │
│  - nombre: char[100]                                               │
│  - cedula: char[20]                                                │
│  - posicion: char[20]                                              │
│  - edad: int                                                       │
│  - numeroDorsal: int                                               │
│  - golesAnotados: int                                              │
│  - tarjetasAmarillas: int                                          │
│  - tarjetasRojas: int                                              │
│  - eliminado: bool                                                 │
│  - fechaCreacion: time_t                                           │
│  - fechaUltimaModificacion: time_t                                 │
│  + Jugador()                                                       │
│  + Jugador(idEquipo, nombre, cedula, posicion, edad, dorsal)      │
│  + getId(): int                                                    │
│  + getNombre(): const char*                                        │
│  + setNombre(nombre: const char*)                                  │
│  + esValido(): bool                                                │
│  + incrementarGoles(): void                                        │
│  + incrementarAmarilla(): void                                     │
│  + incrementarRoja(): void                                         │
│  + mostrarBasico(): void                                           │
│  + mostrarCompleto(): void                                         │
│  + obtenerTamano(): size_t                                         │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                               GOL                                  │
│  - idJugador: int                                                  │
│  - minuto: int                                                     │
│  - equipo: int                                                     │
│  + Gol()                                                           │
│  + Gol(idJugador, minuto, equipo)                                  │
│  + getIdJugador(): int                                             │
│  + getMinuto(): int                                                │
│  + getEquipo(): int                                                │
│  + setMinuto(minuto: int): bool                                    │
│  + setEquipo(equipo: int): bool                                    │
│  + esValido(): bool                                                │
│  + mostrar(): void                                                 │
│  + obtenerTamano(): size_t                                         │
└─────────────────────────────────────────────────────────────────────┘
                                    │
                                    │ Composición (1..*)
                                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│                             PARTIDO                                │
│  - id: int                                                         │
│  - idEquipoLocal: int                                              │
│  - idEquipoVisitante: int                                          │
│  - fecha: char[11]                                                 │
│  - estado: char[12]                                                │
│  - descripcion: char[200]                                          │
│  - golesLocal: int                                                 │
│  - golesVisitante: int                                             │
│  - goles: Gol[22]                                                  │
│  - numGoles: int                                                   │
│  - eliminado: bool                                                 │
│  - fechaCreacion: time_t                                           │
│  - fechaUltimaModificacion: time_t                                 │
│  + Partido()                                                       │
│  + Partido(idLocal, idVisitante, fecha, estado, descripcion)      │
│  + getId(): int                                                    │
│  + getIdEquipoLocal(): int                                         │
│  + getEstado(): const char*                                        │
│  + setEstado(estado: const char*): bool                            │
│  + agregarGol(gol: const Gol&): bool                               │
│  + esValido(): bool                                                │
│  + estaJugado(): bool                                              │
│  + estaProgramado(): bool                                          │
│  + estaCancelado(): bool                                           │
│  + mostrarBasico(): void                                           │
│  + mostrarCompleto(): void                                         │
│  + obtenerTamano(): size_t                                         │
└─────────────────────────────────────────────────────────────────────┘
```

### Módulos de Operaciones (Namespaces)

```
┌─────────────────────────────────────────────────────────────────────┐
│                     OPERACIONES EQUIPOS                            │
│  + guardarEquipo(Equipo&): bool                                    │
│  + leerEquipoPorID(int, Equipo&): bool                             │
│  + actualizarEquipo(const Equipo&): bool                           │
│  + eliminarEquipoLogico(int): bool                                 │
│  + contarEquiposActivos(): int                                     │
│  + buscarEquiposPorNombre(const char*, Equipo[], int): int        │
│  + agregarEquipo(const char*, const char*, const char*, Equipo&): bool│
│  + actualizarEquipoLogica(int, const char*, const char*, const char*): bool│
│  + eliminarEquipoLogica(int): bool                                 │
│  + listarEquiposOrdenados(Equipo[], int): int                      │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                    OPERACIONES JUGADORES                           │
│  + guardarJugador(Jugador&): bool                                  │
│  + leerJugadorPorID(int, Jugador&): bool                           │
│  + actualizarJugador(const Jugador&): bool                         │
│  + eliminarJugadorLogico(int): bool                                │
│  + contarJugadoresActivos(): int                                   │
│  + buscarJugadoresPorNombre(const char*, Jugador[], int): int     │
│  + listarJugadoresPorEquipo(int, Jugador[], int): int             │
│  + listarTodosLosJugadores(Jugador[], int): int                   │
│  + agregarJugador(int, const char*, const char*, const char*, int, int, Jugador&): bool│
│  + actualizarJugadorLogica(int, int, const char*, const char*, int, int): bool│
│  + eliminarJugadorLogica(int): bool                                │
│  + cedulaExiste(const char*): bool                                 │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                    OPERACIONES PARTIDOS                            │
│  + guardarPartido(Partido&): bool                                  │
│  + leerPartidoPorID(int, Partido&): bool                           │
│  + actualizarPartido(const Partido&): bool                         │
│  + eliminarPartidoLogico(int): bool                                │
│  + contarPartidosActivos(): int                                    │
│  + listarPartidosPorEstado(const char*, Partido[], int): int      │
│  + listarPartidosPorEquipo(int, Partido[], int): int              │
│  + programarPartido(int, int, const char*, Partido&): bool        │
│  + registrarResultadoPartido(int, int, int, const Gol[], int): bool│
│  + cancelarPartidoLogica(int): bool                                │
│  + partidoYaExiste(int, int): bool                                 │
│  + equipoTienePartidosActivos(int): bool                           │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                    OPERACIONES TORNEO                              │
│  + guardarTorneo(const Torneo&): bool                              │
│  + leerTorneo(Torneo&): bool                                       │
│  + existeTorneo(): bool                                            │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 🎨 Principios de Diseño Aplicados

### 1. Responsabilidad Única (SRP)
Cada clase y módulo tiene una única razón para existir:

| Clase/Módulo | Responsabilidad |
|--------------|-----------------|
| `Equipo` | Representar datos y lógica de un equipo |
| `Jugador` | Representar datos y lógica de un jugador |
| `Partido` | Representar datos y lógica de un partido |
| `GestorArchivos` | Toda operación de lectura/escritura en archivos |
| `Interfaz` | Menús, navegación y loop principal |
| `Validaciones` | Validaciones de formato reutilizables |
| `Formatos` | Formateo de salida consistente |

### 2. Encapsulamiento
- Todos los atributos son **privados**
- Acceso solo a través de **getters y setters**
- Los setters incluyen **validaciones** de negocio

### 3. Abstracción
- El usuario no necesita saber cómo se guarda un equipo
- `OperacionesEquipos::agregarEquipo()` oculta toda la complejidad

### 4. Composición
```cpp
class Partido {
    Gol goles[22];  // Partido CONTIENE goles
};
```

### 5. Templates (Polimorfismo paramétrico)
```cpp
template <class T>
static bool guardarRegistro(const char* ruta, T& objeto) {
    // Funciona con Equipo, Jugador, Partido, etc.
}
```

### 6. Namespaces (Agrupación lógica)
```cpp
namespace OperacionesEquipos {
    // Todas las funciones relacionadas con equipos
}
```

---

## ⚡ Funcionalidades Principales

### Gestión de Equipos
| Función | Descripción |
|---------|-------------|
| `agregarEquipo()` | Registrar nuevo equipo con nombre único |
| `buscarEquiposPorNombre()` | Búsqueda parcial (insensible a mayúsculas) |
| `actualizarEquipoLogica()` | Modificar datos del equipo |
| `eliminarEquipoLogica()` | Borrado lógico (desvincula jugadores) |
| `listarEquiposOrdenados()` | Tabla de posiciones ordenada por puntos |

### Gestión de Jugadores
| Función | Descripción |
|---------|-------------|
| `agregarJugador()` | Registrar jugador con cédula y dorsal únicos |
| `buscarJugadoresPorNombre()` | Búsqueda parcial |
| `actualizarJugadorLogica()` | Modificar datos del jugador |
| `eliminarJugadorLogica()` | Borrado lógico |
| `listarJugadoresPorEquipo()` | Mostrar plantilla completa |

### Gestión de Partidos
| Función | Descripción |
|---------|-------------|
| `programarPartido()` | Crear nuevo partido entre dos equipos |
| `registrarResultadoPartido()` | Registrar resultado y actualizar estadísticas |
| `cancelarPartidoLogica()` | Cancelar partido (revierte estadísticas) |
| `partidoYaExiste()` | Evitar duplicados |

### Reportes
| Reporte | Descripción |
|---------|-------------|
| Tabla de Posiciones | Equipos ordenados por puntos, diferencia de goles |
| Tabla de Goleadores | Top 10 jugadores con más goles |
| Ficha Técnica | Detalles completos de un partido |

### Mantenimiento
| Función | Descripción |
|---------|-------------|
| `verificarIntegridadReferencial()` | Validar que todas las referencias sean válidas |
| `crearBackup()` | Copia de seguridad de todos los archivos de datos |

---

## ✅ Validaciones y Reglas de Negocio

### Validaciones de Formato (Clase Validaciones)
| Función | Validación |
|---------|------------|
| `esTextoValido()` | Solo letras, espacios y puntos |
| `esCedulaValida()` | 7-10 dígitos |
| `esFechaValida()` | Formato YYYY-MM-DD |
| `esNumeroEnRango()` | Valor dentro de un rango |

### Validaciones de Negocio (Setters)
| Clase | Validación |
|-------|------------|
| `Equipo` | Nombre, ciudad y entrenador no vacíos; puntos no negativos |
| `Jugador` | Edad 14-60; Dorsal 1-99; Cédula única; Dorsal único por equipo |
| `Partido` | Equipos diferentes; Fecha válida; Estado permitido; Max 22 goles |

### Reglas de Negocio (Operaciones)
| Regla | Descripción |
|-------|-------------|
| Nombre único | No pueden existir dos equipos con el mismo nombre |
| Cédula única | No pueden existir dos jugadores con la misma cédula |
| Dorsal único | Un dorsal no puede repetirse en el mismo equipo |
| Partido único | No pueden existir dos partidos entre los mismos equipos (activos) |
| Eliminación | No se puede eliminar un equipo con partidos activos |

---

## 💾 Persistencia de Datos

### Formato de Archivos Binarios

#### Estructura del Archivo
```
+------------------+
| ArchivoHeader    | ← Metadatos (cantidad, próximoID, version)
+------------------+
| Registro 0       | ← Objeto (Equipo, Jugador, Partido, etc.)
+------------------+
| Registro 1       |
+------------------+
| Registro 2       |
+------------------+
| ...              |
+------------------+
```

#### ArchivoHeader
```cpp
struct ArchivoHeader {
    int cantidadRegistros;  // Total de registros escritos
    int proximoID;          // Próximo ID a asignar
    int registrosActivos;   // Registros NO eliminados
    int version;            // Para control de cambios
};
```

### Archivos de Datos
| Archivo | Contenido | Formato |
|---------|-----------|---------|
| `torneo.bin` | Un único objeto `Torneo` | Sin header |
| `equipos.bin` | Múltiples objetos `Equipo` | Con header |
| `jugadores.bin` | Múltiples objetos `Jugador` | Con header |
| `partidos.bin` | Múltiples objetos `Partido` | Con header |

### Operaciones de Persistencia
| Operación | Descripción |
|-----------|-------------|
| `guardarRegistro()` | Escribe al final del archivo, asigna ID automático |
| `leerRegistroPorID()` | Busca por ID, retorna si existe y no está eliminado |
| `actualizarRegistro()` | Sobrescribe en la misma posición |
| `eliminarRegistroLogico()` | Marca el registro como eliminado |
| `recorrerRegistros()` | Itera sobre todos los registros activos |

### Ventajas
- ✅ **Alta performance**: Lectura/escritura directa en binario
- ✅ **Integridad**: Borrado lógico mantiene la integridad referencial
- ✅ **Flexibilidad**: Los templates permiten operar con cualquier tipo
- ✅ **Portabilidad**: Los archivos .bin son independientes del sistema operativo

---

## 🔧 Mantenimiento y Backup

### Verificación de Integridad Referencial
```cpp
void verificarIntegridadReferencial() {
    // Verifica que:
    // 1. Todos los jugadores tengan un equipo existente
    // 2. Todos los partidos tengan equipos existentes
    // 3. Todos los goles tengan jugadores existentes
    // 4. Reporta cualquier inconsistencia
}
```

### Creación de Backup
```cpp
bool crearBackup() {
    // Crea una carpeta con fecha y hora actual
    // Copia todos los archivos .bin
    // backup_2024-01-15_14-30/
    //   ├── torneo.bin
    //   ├── equipos.bin
    //   ├── jugadores.bin
    //   └── partidos.bin
}
```

---

## 👥 Autores y Contribuciones

### Autor Principal
- **Estudiante:** Santiago Torrealba
- **Asignatura:** Programación 2
- **Profesor:** Ing. Victor Kneider
- **Universidad:** Universidad Rafael Urdaneta
- **Trimestre:** 2026-C

### Agradecimientos
- Al profesor **Ing. Victor Kneider** por la guía y los conceptos enseñados
- A la **Universidad Rafael Urdaneta** por el espacio académico
- A todos los compañeros que aportaron feedback durante el desarrollo

---

## 📚 Referencias

### Conceptos Aplicados
- **Programación Orientada a Objetos** - C++ Primer (Lippman)
- **Patrones de Diseño** - Design Patterns (Gamma et al.)
- **C++ Templates** - C++ Templates (Vandevoorde)
- **Persistencia en C++** - Effective Modern C++ (Meyers)

### Enlaces Útiles
- [C++ Reference](https://en.cppreference.com/)
- [GCC Documentation](https://gcc.gnu.org/onlinedocs/)
- [Makefile Tutorial](https://makefiletutorial.com/)
- [MinGW-w64](https://www.mingw-w64.org/)

---

## 📝 Notas de Versión

### Versión 1.0 (2026-C)
- ✅ Sistema completo con todas las funcionalidades
- ✅ Arquitectura modular con POO
-
