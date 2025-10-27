/* Juego SNAKE minimalista para 4 matrices MAX7219 (32x8)
 * Controlado con joystick analógico.
 * Utiliza la librería LedControl para manejar las matrices LED.
 * Adaptado de varios ejemplos de Snake en Arduino.
 * by YnerPoe
 * octubre/2025
 */ 
 
#include <LedControl.h>               // Librería para controlar matrices LED con MAX7219

// Pines para el MAX7219
#define DIN_PIN 4                     // Data IN del MAX7219
#define CS_PIN 5                      // Chip Select / LOAD del MAX7219
#define CLK_PIN 6                     // Clock del MAX7219
#define NUM_OF_MATRIX 4               // Número de matrices encadenadas (cada una 8x8)

// Pines del joystick y buzzer
#define JOY_X A5                      // Entrada analógica X del joystick
#define JOY_Y A4                      // Entrada analógica Y del joystick
#define JOY_SW 3                      // Botón del joystick (switch)
#define BUZZER 12                     // Pin para el buzzer (sonidos)

// Dimensiones de la pantalla total (8 columnas x 32 filas)
const int screenWidth = 8;            // Ancho físico (columnas) de la pantalla total
const int screenHeight = 32;          // Alto lógico combinado (4 matrices * 8 filas)

// Variables del juego
int snakeX, snakeY;                   // Coordenadas de la cabeza de la serpiente (x=col, y=row global)
int foodX, foodY;                     // Coordenadas de la comida
int score = 0;                        // Puntuación acumulada
int rapidez = 100;                    // Ajusta de rapidez del juego
int snakeSize = 1;                    // Tamaño inicial de la serpiente (número de segmentos)
int velocidad = 100;                  // Ajusta la rapidez del juego
int tailX[100], tailY[100];           // Arrays con coordenadas de los segmentos de la cola
char direction = 'd';                 // Dirección actual: 'l','r','u','d' (inicial: 'd' = abajo)
bool isGameOver = false;              // Indicador de fin de juego
bool hasShownCountdown = false;  // Controla si ya se mostró la cuenta regresiva

// Instancia del controlador de matrices LED
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);

void setup() {
  setupPins();                        // Configurar pines de entrada/salida
  setupLedBoard();                    // Inicializar las matrices LED
  setupSnakePosition();               // Posicionar la serpiente al inicio
  setupFoodPosition();                // Colocar la primera comida aleatoria
}

void loop() {
  if (isGameOver) {
    playGameOverSong();
    showGameOverScreen();
  } else {
    if (!hasShownCountdown) {
      showCountdown();
      hasShownCountdown = true;
    }
    startGame();
  }
}

// Configura los pines del joystick
void setupPins() {
  pinMode(JOY_SW, INPUT);
  digitalWrite(JOY_SW, HIGH);
}

// Inicializa todas las matrices LED
void setupLedBoard() {
  for (int i = 0; i < NUM_OF_MATRIX; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 8);
    lc.clearDisplay(i);
  }
}

// Posición inicial de la serpiente
void setupSnakePosition() {
  snakeX = 4;
  snakeY = 4;
}

// Genera una nueva posición aleatoria para la comida
void setupFoodPosition() {
  foodX = random(screenWidth);
  foodY = random(screenHeight);
}

// Lógica principal del juego
void startGame() {
  manageGameOver();
  setJoystickDirection();
  changeSnakeDirection();
  manageSnakeOutOfBounds();
  manageEatenFood();
  manageSnakeTailCoordinates();
  drawSnake();
  delay(velocidad);
}

// Verifica si la serpiente colisionó consigo misma
void manageGameOver() {
  for (int i = 1; i < snakeSize; i++) {
    if (tailX[i] == snakeX && tailY[i] == snakeY) {
      isGameOver = true;
      hasShownCountdown = false;
    }
  }
}

// Evita que la serpiente salga de los límites de la pantalla
void manageSnakeOutOfBounds() {
  if (snakeX >= screenWidth) snakeX = 0;
  else if (snakeX < 0) snakeX = screenWidth - 1;
  if (snakeY >= screenHeight) snakeY = 0;
  else if (snakeY < 0) snakeY = screenHeight - 1;
}

// Verifica si la serpiente comió la comida
void manageEatenFood() {
  if (snakeX == foodX && snakeY == foodY) {
    playFoodEatenSong();
    score++;
    snakeSize++;
    setupFoodPosition();
  }
}

// Actualiza las coordenadas de la cola de la serpiente
void manageSnakeTailCoordinates() {
  int previousX = tailX[0];
  int previousY = tailY[0];
  tailX[0] = snakeX;
  tailY[0] = snakeY;
  for (int i = 1; i < snakeSize && i < 100; i++) {
    int prevX = tailX[i];
    int prevY = tailY[i];
    tailX[i] = previousX;
    tailY[i] = previousY;
    previousX = prevX;
    previousY = prevY;
  }
}

// Lee el joystick y actualiza la dirección de movimiento
void setJoystickDirection() {
  char lastDirection = direction;
  if (analogRead(JOY_X) > 600 && lastDirection != 'u') direction = 'd';
  else if (analogRead(JOY_X) < 400 && lastDirection != 'd') direction = 'u';
  else if (analogRead(JOY_Y) > 600 && lastDirection != 'l') direction = 'r';
  else if (analogRead(JOY_Y) < 400 && lastDirection != 'r') direction = 'l';
}

// Cambia la posición de la cabeza según la dirección
void changeSnakeDirection() {
  switch (direction) {
    case 'l': snakeX--; break;
    case 'r': snakeX++; break;
    case 'u': snakeY--; break;
    case 'd': snakeY++; break;
  }
}

// Reproduce sonido al comer comida
void playFoodEatenSong() {
  tone(BUZZER, 500, 100);
}

// Reproduce secuencia sonora de fin de juego
void playGameOverSong() {
  tone(BUZZER, 100, 100); delay(100);
  tone(BUZZER, 400, 100); delay(100);
  tone(BUZZER, 800, 100); delay(100);
  tone(BUZZER, 1200, 100); delay(100);
}

// Muestra animación de fin de juego encendiendo todos los LEDs
void showGameOverScreen() {
  for (int i = 0; i < screenHeight; i++) {
    for (int j = 0; j < screenWidth; j++) {
      showLed(j, i);
      delay(10);
    }
  }
  fin();
  resetVariables();
}

// Reinicia todas las variables del juego
void resetVariables() {
  setupSnakePosition();
  setupFoodPosition();
  direction = 'd';
  isGameOver = false;
  score = 0;
  snakeSize = 1;
}

// Enciende un LED en la posición (x, y) correspondiente a la matriz
void showLed(int x, int y) {
  int matrixIndex = 3 - (y / 8);  // Inversión vertical: matriz 4 arriba
  int localY = y % 8;
  lc.setLed(matrixIndex, x, localY, true);
}

// Apaga un LED en la posición (x, y)
void hideLed(int x, int y) {
  int matrixIndex = 3 - (y / 8);
  int localY = y % 8;
  lc.setLed(matrixIndex, x, localY, false);
}

// Dibuja la serpiente y la comida en la pantalla
void drawSnake() {
  for (int i = 0; i < screenHeight; i++) {
    for (int j = 0; j < screenWidth; j++) {
      if (i == snakeY && j == snakeX) {
        showLed(snakeX, snakeY);
      } else if (i == foodY && j == foodX) {
        showLed(foodX, foodY);
      } else {
        bool isShown = false;
        for (int k = 0; k < snakeSize; k++) {
          if (tailX[k] == j && tailY[k] == i) {
            showLed(j, i);
            isShown = true;
          }
        }
        if (!isShown) {
          hideLed(j, i);
        }
      }
    }
  }
}

// Muestra una cuenta regresiva antes de iniciar el juego
void showCountdown() {
  for (int n = 3; n >= 1; n--) {
    clearAllMatrices();
    drawDigit(n);
    delay(1000);
  }
  clearAllMatrices();
}

// Limpia todas las matrices
void clearAllMatrices() {
  for (int i = 0; i < NUM_OF_MATRIX; i++) {
    lc.clearDisplay(i);
  }
}

// Dibuja un número grande centrado en las matrices
void drawDigit(int number) {
  byte digits[3][5] = {
    {B111, B100, B111, B100, B111}, // 3
    {B111, B100, B111, B001, B111}, // 2
    {B010, B011, B010, B010, B111}  // 1
  };

  int startX = 2;   // Posición horizontal inicial
  int startY = 9;  // Posición vertical para centrar en 32 filas

  for (int row = 0; row < 5; row++) {
    byte line = digits[3 - number][row];
    for (int col = 0; col < 3; col++) {
      if (bitRead(line, 2 - col)) {
        showLed(startX + col, startY + row);
      }
    }
  }
}

void fin() {
  for (int i = 0; i < screenHeight; i++) {      // Recorrer todas las filas globales
    for (int j = 0; j < screenWidth; j++) {   // Recorrer todas las columnas
      showLed(j, i);                            // Encender cada LED (x = columna, y = fila)
      delay(1);                                 // Pequeña pausa entre LEDs para animación
    }
  }
  clearAllMatrices();
  const byte F[8] = {//F
    B00000000,B01000000,B01001000,B01001000,B01111110,B00000000,B00000000,B00000000
  };
  const byte I[8] = {//I
    B00000000,B00000000,B01000010,B01111110,B01000010,B00000000,B00000000,B00000000
  };
  const byte N[8] = { //N
    B00000000,B01111110,B00001100,B00011000,B00110000,B01111110,B00000000,B00000000
  };

  // Mostrar cada letra en su matriz correspondiente
  for (int row = 0; row < 8; row++) {
    lc.setRow(3, row, F[row]); // Matriz 3: F
    lc.setRow(2, row, I[row]); // Matriz 2: I
    lc.setRow(1, row, N[row]); // Matriz 1: N
  }

  // Efecto de sonido de game over
  for (int i = 0; i < 3; i++) {
    tone(BUZZER, 300 - (i * 50), 300);
    delay(400);
  }
  noTone(BUZZER);
}
