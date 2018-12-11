#include <LiquidCrystal.h>
#include <LedControl.h>
#include <math.h>

//lcd
LiquidCrystal lcd(13, 12, 8, 9, 10, 11);

//Menu
unsigned long deltaLCD = 0;
unsigned long deltaMenu = 0;
byte line = 0;
byte menu[] = {
  B10000001,
  B01111110,
  B00100100,
  B00011000,
  B00100100,
  B01111110,
  B10000001,
  B11111111
};

// Joystick
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define JOYSTICK_S A2

// Matrix
#define D_IN         7
#define CLK          6
#define CS           5
#define MAX_IN_USE   1
#define MAP_SIZE     20

LedControl matrix = LedControl(D_IN, CLK, CS, MAX_IN_USE);

// Game
byte scene = 0;
unsigned long past = 0;
unsigned long score = 0;
unsigned long generation = 0;
bool boolMap[MAP_SIZE][MAP_SIZE];

bool isOnMap(unsigned long x, unsigned long y) {
  return (x < MAP_SIZE && y < MAP_SIZE);
}
void lose() {
  if (score < generation) {
    score = generation;
  }
  scene = 2;
  generation = 0;
}
byte count(unsigned long x, unsigned long y) {
  byte sum = 0;
  if (0 < x) {
    sum += (bool)boolMap[y][x - 1];
    if (0 < y) {
      sum += (bool)boolMap[y - 1][x - 1];
    }
    if (y + 1 < MAP_SIZE) {
      sum += (bool)boolMap[y + 1][x - 1];
    }
  }
  if (x + 1 < MAP_SIZE) {
    sum += (bool)boolMap[y][x + 1];
    if (y + 1 < MAP_SIZE) {
      sum += (bool)boolMap[y + 1][x + 1];
    }
  }
  if (0 < y) {
    sum += (bool)boolMap[y - 1][x];
    if (x < MAP_SIZE) {
      sum += (bool)boolMap[y - 1][x + 1];
    }
  }
  if (y + 1 < MAP_SIZE) {
    sum += (bool)boolMap[y + 1][x];
  }
  return sum;
}
// View
unsigned long xView = 0;
unsigned long yView = 0;

void setView(long x, long y) {
  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if (x > MAP_SIZE - 8)
    x = MAP_SIZE - 8;
  if (y > MAP_SIZE - 8)
    y = MAP_SIZE - 8;
  xView = x;
  yView = y;
}

// Player
unsigned long lastMove = 0;
unsigned long playerX = 0;
unsigned long playerY = 0;
bool playerLed = 0;

void nextGeneration() {
  //Counts
  byte counts[MAP_SIZE][MAP_SIZE];
  for (unsigned long i = 0; i < MAP_SIZE; i++) {
    for (unsigned long j = 0; j < MAP_SIZE; j++) {
      counts[i][j] = count(j, i);
    }
  }
  //Entities
  for (unsigned long i = 0; i < MAP_SIZE; i++) {
    for (unsigned long j = 0; j < MAP_SIZE; j++) {
      if (boolMap[i][j]) {
        if (counts[i][j] > 5) {
          boolMap[i][j] = false;
          if (j == playerX && i == playerY) {
            lose();
          }
        }
      } else {
        if (!boolMap[i][j]) {
          if (counts[i][j] == 2) {
            boolMap[i][j] = true;
          }
        }
      }
    }
  }
  // Next
  generation++;
}
void movePlayer(unsigned long x, unsigned long y) {
  lastMove = millis();
  if (!boolMap[playerY + y][playerX + x] && isOnMap(playerX + x, playerY + y) ) {
    // New position
    boolMap[playerY][playerX] = false;
    playerX += x;
    playerY += y;
    boolMap[playerY][playerX] = true;
    setView(playerX - 4, playerY - 4);
    nextGeneration();
  }
}
void initialize() {
  lcd.begin(16, 2);
  scene = 1;
  //Map
  for (unsigned long i = 0; i < MAP_SIZE; i++) {
    for (unsigned long j = 0; j < MAP_SIZE; j++) {
      boolMap[i][j] = 0;
    }
  }
  bool first = 1;
  for (unsigned long i = 0; i < MAP_SIZE; i++) {
    for (unsigned long j = 0; j < MAP_SIZE; j++) {
      if ((i + j) % 11 > 8 && (i * i + 1) % 14 < 2) {
        if (first) {
          first = 0;
          movePlayer(j, i);
        }
        else {
          boolMap[i][j] = true;
        }
      }
    }
  }
}

void setup() {
  // LCD
  lcd.begin(16, 2);
  // Joystick
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_S, INPUT);
  // Matrix
  matrix.shutdown(0, false);
  matrix.setIntensity(0, 7);
  matrix.clearDisplay(0);
}
void loop() {
  // LCD
  if (millis() - deltaLCD > 100) {
    lcd.noDisplay();
    if (scene == 1) {
      // LCD
      lcd.setCursor(0, 0);
      lcd.print("Generation: ");
      lcd.print(generation);
      lcd.setCursor(0, 1);
      lcd.print("x: ");
      lcd.print(playerX + 1);
      lcd.print("   y: ");
      lcd.print(playerY + 1);
    }
    else if (scene == 2) {
      lcd.setCursor(0, 0);
      lcd.print("Best score: ");
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print("Move joystick to restart!");
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("Move joystick");
      lcd.setCursor(0, 1);
      lcd.print("to play!");
    }
    lcd.display();
    deltaLCD = millis();
  }
  // Joystick
  unsigned x, y;
  x = analogRead(A1);
  y = analogRead(A0);
  // Game
  if (scene == 1) {
    // Update game
    if (millis() - lastMove > 400) {
      if (x < 400) {
        movePlayer(1, 0);
      }
      if (x > 600) {
        movePlayer(-1, 0);
      }
      if (y < 400) {
        movePlayer(0, -1);
      }
      if (y > 600) {
        movePlayer(0, 1);
      }
    }
    // Render game
    for (unsigned long i = yView; i < 8 + yView; i++) {
      for (unsigned long j = xView; j < 8 + xView; j++) {
        if (i == playerY && j == playerX) {
          if (millis() - past > 500) {
            past = millis();
            playerLed = !playerLed;
            matrix.setLed(0, i - yView, j - xView, playerLed);
          }
        }
        else {
          matrix.setLed(0, i - yView, j - xView, boolMap[i][j]);
        }
      }
    }
  }
  // Restart
  else if (scene == 2) {
    // Update restart
    if (millis() - lastMove > 400) {
      if (x < 400 || y < 400 || 600 < x || 600 < y) {
        initialize();
      }
    }
    // Render restart
    for (byte i = 0; i < 8; i++) {
      matrix.setRow(0, i, (1 << i) + (128 >> i));
    }

  }
  // Menu
  else {
    // Update menu
    if (millis() - lastMove > 400) {
      if (x < 400 || y < 400 || 600 < x || 600 < y) {
        initialize();
      }
    }
    if (millis() - deltaMenu > 180) {
      line = (line + 1) % 8;
      deltaMenu = millis();
    }
    // Menu rendering
    for (byte i = 0; i < 8; i++) {
      matrix.setRow(0, i, menu[(i + line) % 8] );
    }
  }
}
