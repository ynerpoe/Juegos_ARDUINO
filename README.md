
# Juegos_ARDUINO
Este es un conjunto de codigos para juegos minimalistas adaptados para Arduino

# Características generales
Todos los juegos están diseñados para ser minimalistas y funcionar con el mismo hardware sencillo y de bajo costo. El documento incluye el código fuente completo en Arduino para cada juego, con comentarios explicativos en español. Se detallan las funciones principales, la inicialización de hardware, la gestión de entradas y salidas, y la visualización en matrices LED. Se utilizan animaciones y sonidos para mejorar la experiencia de juego.

<img width="218" height="304" alt="image" src="https://github.com/user-attachments/assets/08e346cf-6794-4bf3-ac4e-9e363f7d6efa" />

# Juegos reunidos
TETRIS: Este es el famoso juego de TETRIS adaptado para 4 matrices MAX7219 (32x8) dispuesto de forma que las piezas caigan de manera vertical. El juego es controlado con joystick analógico y botón de rotación. Utiliza la librería LedControl para manejar las matrices LED. El código incluye melodía de Tetris y efectos de sonido con el buzzer. El código también gestiona la lógica de piezas, tablero, colisiones, rotaciones y puntuación. Además, muestra cuenta regresiva y pantalla de “Game Over” con animaciones y puntuación.

SNAKE: Este es el famoso juego SNAKE para 4 matrices MAX7219 dispuesto para usar la totalidad de la matriz led. La matriz está modelada con 8 columnas y 32 filas. Controlado con joystick analógico. Utiliza la librería LedControl para mostrar la serpiente y la comida. Incluye lógica para movimiento, colisiones, crecimiento de la serpiente y puntuación. Efectos de sonido y animaciones para comer comida y fin de juego.

PONG: el clásico juego PONG en una versión minimalista para Arduino, usando 4 matrices LED MAX7219 (32 filas x 8 columnas) y controlado por un joystick analógico. El juego está diseñado para funcionar con hardware sencillo y de bajo coste, y se juega en solitario con la matriz dispuesta verticalmente. Utiliza la librería LedControl para gestionar las matrices LED y el buzzer para efectos de sonido. El código fuente está comentado en español para facilitar su comprensión y modificación.
