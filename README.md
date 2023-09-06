# 🌟 Proyecto de Screensavers 🌟

Este proyecto es una implementación de un screensaver que muestra caras sonrientes animadas que rebotan alrededor de la pantalla. El proyecto incluye varias versiones, una secuencial y varias paralelas, utilizando OpenMP para la paralelización.

## Descripción General 🚀

El proyecto consiste en una aplicación que genera una animación de caras sonrientes que interactúan en una ventana gráfica. Las caras sonrientes se mueven alrededor de la pantalla, rebotan cuando colisionan y se dibujan utilizando la biblioteca SDL2. El objetivo es proporcionar una visualización atractiva y una demostración del uso de programación en paralelo con OpenMP.

## Funciones Principales 🎯

### `moveCircle` 🔄

Esta función se encarga de mover un círculo (representando una cara sonriente) por la pantalla y garantizar que no se salga de los límites de la ventana.

### `checkOverlap` 🧐

Comprueba si dos círculos (caras sonrientes) se superponen en la pantalla.

### `drawCircle` 🖋️

Dibuja un círculo utilizando el algoritmo de Bresenham para el trazado de círculos.

### `fillCircle` 🌈

Rellena un círculo con un color específico.

### `drawSmileyFace` 😄

Dibuja una cara sonriente compuesta por un círculo grande (cabeza), dos ojos y una sonrisa.

### `overlapsWithAny` 🤔

Verifica si un nuevo círculo se superpone con cualquiera de los círculos existentes en la pantalla.

## Cómo Ejecutar el Programa 🚀

Para ejecutar el programa, sigue estos pasos:

1. Clona el repositorio desde GitHub.

2. Asegúrate de tener las dependencias necesarias instaladas. Debes tener SDL2 y OpenMP habilitado si estás utilizando la versión paralela.

3. Compila el programa utilizando el compilador de C adecuado dependiendo de la versión que desees ejecutar. Para la versión secuencial: `gcc -o secuencial secuencial.c -lm -lSDL2` y para la versión paralela: `gcc -o paralela paralela.c -lm -lSDL2 -fopenmp`.

4. Ejecuta el programa con el número deseado de caras sonrientes como argumento. Por ejemplo, para ejecutar con 10 caras sonrientes: `./secuencial 10` o `./paralela 10`.

## Autores 👥

- Juan Avila - 20090
- Manuel Archila - 161250
- Diego Franco - 20240
