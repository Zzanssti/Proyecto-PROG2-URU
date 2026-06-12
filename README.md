🏆 Sistema de Gestión de Torneos Deportivos (URU)
"Solución robusta para la administración eficiente de ligas y torneos."

📋 Descripción del Proyecto
Este sistema ha sido diseñado como una herramienta de gestión integral para el Torneo URU. El software permite administrar de forma dinámica las tres entidades pilares de cualquier liga deportiva: Equipos, Jugadores y Partidos.

Desarrollado en C++, el proyecto prioriza la eficiencia en la gestión de memoria y la integridad de los datos, aplicando principios de Arquitectura de Software para asegurar que el sistema sea escalable y fácil de mantener.

🛠️ Arquitectura Técnica
El proyecto se basa en una Arquitectura de Tres Capas para separar responsabilidades:

Capa de Datos (Modelos): Estructuras (struct) optimizadas para la gestión en el Heap.

Capa de Lógica: Funciones "puras" que manipulan el estado del torneo sin dependencias de entrada/salida.

Capa de Presentación: Interfaz de usuario intuitiva basada en menús interactivos y validaciones robustas.

Diagrama de Relaciones
🚀 Cómo Empezar
Requisitos
Compilador de C++ (GCC, Clang o MSVC).

Compilación y Ejecución
Clonar/Descargar el proyecto.

Abrir la terminal en el directorio raíz.

Compilar:

Bash
g++ Proyectoprog2.cpp -o SistemaTorneo
Ejecutar:

Bash
./SistemaTorneo
✨ Características Principales
Gestión Dinámica de Memoria: Uso inteligente del Heap para redimensionar colecciones automáticamente ante el crecimiento del torneo.

Algoritmo de Ordenamiento: Implementación propia de Bubble Sort con criterios jerárquicos: Puntos > Dif. Goles > Goles a Favor.

Integridad Referencial: validaciones estrictas para evitar registros huérfanos y duplicados (IDs únicos, dorsales, cruces de partidos).

Resiliencia: Manejo de errores en la entrada de datos (cin.fail()) para evitar comportamientos inesperados.
