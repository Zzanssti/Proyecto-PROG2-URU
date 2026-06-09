# Sistema de Gestión de Torneos - Programación II

## 📝 Descripción

Este programa permite llevar el control de un torneo deportivo en C++. Está enfocado en gestionar equipos, los jugadores que los integran y los partidos que se disputan, manteniendo las estadísticas y tablas de posiciones siempre actualizadas. 📈

## 🚀 ¿Qué puede hacer el sistema?

* **Equipos 🛡️:** Crear nuevos equipos, buscarlos por nombre (incluso si escribes solo una parte) y editar o eliminar información.
* **Jugadores 🏃‍♂️:** Registrar jugadores con su cédula, dorsal y posición (Portero, Defensa, etc.). El sistema evita que se repita la cédula o que un jugador use un número de dorsal ya ocupado.
* **Partidos 📅:** Programar encuentros entre equipos, registrar los marcadores finales y cancelar partidos si es necesario.
* **Estadísticas 🏆:** El programa calcula automáticamente los puntos y victorias, ordenando la tabla de posiciones según las reglas del torneo.

## ⚙️ Puntos clave del desarrollo

* **Memoria Dinámica 💾:** Usamos memoria dinámica para que el programa maneje los datos de forma eficiente según lo que necesites.
* **Validaciones ✅:** Incluimos controles para evitar errores, como programar un equipo contra sí mismo o modificar partidos ya finalizados.
* **Ordenamiento 🔢:** La tabla de posiciones se organiza automáticamente comparando puntos, diferencia de puntos y puntos a favor.

## 🛠️ Cómo compilar

Para ejecutar el proyecto en tu máquina, abre una terminal en la carpeta del archivo y escribe:

```bash
g++ Proyectoprog2.cpp -o torneo
./torneo
```

