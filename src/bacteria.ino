#include <LiquidCrystal.h>
#include <LedControl.h>
#include <math.h>

// Timing
#define DELTA_BUTTON   400
#define DELTA_JOYSTICK 500
#define DELTA_LCD      100
#define DELTA_MENU     200
#define DELTA_PLAYER   500

unsigned long timeButton = 0;
unsigned long timeJoystick = 0;
unsigned long timeLcd = 0;
unsigned long timeMenu = 0;
unsigned long timePlayer = 0;

// Joystick and button
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define JOYSTICK_S A2
#define RESTART     4

// LCD
LiquidCrystal lcd(13, 12, 8, 9, 10, 11);


void resetLcd() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

// Menu
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

// Matrix
#define D_IN         7
#define CLK          6
#define CS           5
#define MAX_IN_USE   1

LedControl matrix = LedControl(D_IN, CLK, CS, MAX_IN_USE);

// Game
#define MAP_SIZE     20
#define MAX_BACTERIA 5
#define MIN_BACTERIA 1

byte scene = 0;
byte equilibrum = 0;
unsigned long score = 0;
unsigned long generation = 0;
byte boolMap[MAP_SIZE][MAP_SIZE];

bool isOnMap(unsigned long x, unsigned long y) {
  return (x < MAP_SIZE && y < MAP_SIZE);
}
void lose() {
  if (score < generation) {
    score = generation;
  }
  scene = 2;
  equilibrum = 0;
  generation = 0;
  resetLcd();
}
void quit() {
  if (score < generation) {
    score = generation;
  }
  scene = 0;
  equilibrum = 0;
  generation = 0;
  resetLcd();
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
bool canBorn(unsigned long x, unsigned long y) {
  return ((count(x, y) < MAX_BACTERIA) && !boolMap[y][x]);
}
void born(unsigned long x, unsigned long y) {
  if (0 < x) {
    if (canBorn(x - 1, y)) {
      boolMap[y][x - 1] = true;
      return ;
    }
    if (0 < y) {
      if (canBorn(x - 1, y - 1)) {
        boolMap[y - 1][x - 1] = true;
        return ;
      }
    }
    if (y + 1 < MAP_SIZE) {
      if (canBorn(x - 1, y + 1)) {
        boolMap[y + 1][x - 1] = true;
        return ;
      }
    }
  }
  if (x + 1 < MAP_SIZE) {
    if (canBorn(x + 1, y)) {
      boolMap[y][x + 1] = true;
      return ;
    }
    if (y + 1 < MAP_SIZE) {
      if (canBorn(x + 1, y + 1)) {
        boolMap[y + 1][x + 1] = true;
        return ;
      }
    }
  }
  if (0 < y) {
    if (canBorn(x, y - 1)) {
      boolMap[y - 1][x] = true;
      return ;
    }
    if (x < MAP_SIZE) {
      if (canBorn(x + 1, y - 1)) {
        boolMap[y - 1][x + 1] = true;
        return ;
      }
    }
  }
  if (y + 1 < MAP_SIZE) {
    if (canBorn(x, y + 1)) {
      boolMap[y + 1][x] = true;
      return ;
    }
  }
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
unsigned long playerX = 0;
unsigned long playerY = 0;
bool playerLed = 0;

void nextGeneration() {
  // Counts
  for (unsigned long i = 0; i < MAP_SIZE; i++) {
    for (unsigned long j = 0; j < MAP_SIZE; j++) {
      if (boolMap[i][j]) {
        boolMap[i][j] = count(j, i);
      }
    }
  }
  bool change = 0;
  // Update
  for (unsigned long i = 0; i < MAP_SIZE; i++) {
    for (unsigned long j = 0; j < MAP_SIZE; j++) {
      // Death bacteria
      if (boolMap[i][j] > MAX_BACTERIA) {
        boolMap[i][j] = false;
        change = 1;
        if (j == playerX && i == playerY) {
          lose();
        }
      }
      // Born bacteria
      else if (boolMap[i][j] <= 2 && boolMap[i][j]) {
        boolMap[i][j] = 3;
        born(j, i);
        change = 1;
      }
    }
  }
  for (unsigned long i = 0; i < MAP_SIZE; i++) {
    for (unsigned long j = 0; j < MAP_SIZE; j++) {
      if (boolMap[i][j] > 0) {
        boolMap[i][j] = 1;
      }
    }
  }
  // Destroy equilibrum
  if (!change) {
    equilibrum = (equilibrum + 1) % MAP_SIZE;
    for (unsigned long i = 0; i < MAP_SIZE; i++) {
      for (unsigned long j = 0; j < MAP_SIZE; j++) {
        // Death bacteria
        if (boolMap[i][j] && (j != playerX || i != playerY)) {
          if ((i + j) % 3 == equilibrum % 3 || (equilibrum * i) % j == 0) {
            boolMap[i][j] = false;
          }
        }
      }
    }
  }
  // Next
  generation++;
}
void movePlayer(unsigned long x, unsigned long y) {
  timeJoystick = millis();
  if (!boolMap[playerY + y][playerX + x] && isOnMap(playerX + x, playerY + y) ) {
    // New position
    nextGeneration();
    boolMap[playerY][playerX] = false;
    playerX += x;
    playerY += y;
    boolMap[playerY][playerX] = true;
    setView(playerX - 4, playerY - 4);
  }
}
void initialize() {
  scene = 1;
  // Map
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
  timeJoystick = millis();
  timePlayer = millis();
  playerLed = 1;
  resetLcd();
}

void setup() {
  // LCD
  lcd.begin(16, 2);
  // Joystick
  pinMode(RESTART, INPUT_PULLUP);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_S, INPUT);
  // Matrix
  matrix.shutdown(0, false);
  matrix.setIntensity(0, 7);
  matrix.clearDisplay(0);
  Serial.begin(9600);
}
void loop() {
  // LCD
  if (millis() > DELTA_LCD + timeLcd) {
    Serial.println(generation);
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
      lcd.print("Move joystick to");
      lcd.setCursor(0, 1);
      lcd.print("play!  Score:");
      lcd.print(score);
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("Move joystick to");
      lcd.setCursor(0, 1);
      lcd.print("play!");
      if (score > 0) {
        lcd.print(" Score:");
        lcd.print(score);
      }
    }
    lcd.display();
    timeLcd = millis();
  }
  // Joystick
  unsigned x, y, s;
  x = analogRead(JOYSTICK_X);
  y = analogRead(JOYSTICK_Y);
  s = analogRead(JOYSTICK_S);
  // Game
  if (scene == 1) {
    // Update game
    if (millis() > DELTA_JOYSTICK + timeJoystick) {
      if (x < 400) {
        movePlayer(1, 0);
      }
      else if (x > 600) {
        movePlayer(-1, 0);
      }
      else if (y < 400) {
        movePlayer(0, 1);
      }
      else if (y > 600) {
        movePlayer(0, -1);
      }
      else if (s < 100) {
        timeJoystick = millis();
        nextGeneration();
      }
    }
    if (millis() > DELTA_BUTTON + timeButton) {
      if (!digitalRead(RESTART)) {
        quit();
        timeButton = millis();
      }
    }
    // Render game
    for (unsigned long i = yView; i < 8 + yView; i++) {
      for (unsigned long j = xView; j < 8 + xView; j++) {
        if (i == playerY && j == playerX) {
          if (millis() > DELTA_PLAYER + timePlayer) {
            timePlayer = millis();
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
    if (millis() > DELTA_JOYSTICK + timeJoystick) {
      if (x < 400 || y < 400 || 600 < x || 600 < y || s < 100) {
        initialize();
      }
    }
    if (millis() > DELTA_BUTTON + timeButton) {
      if (!digitalRead(RESTART)) {
        quit();
        timeButton = millis();
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
    if (millis() > DELTA_JOYSTICK + timeJoystick) {
      if (x < 400 || y < 400 || 600 < x || 600 < y || s < 100) {
        initialize();
      }
    }
    if (millis() > DELTA_BUTTON + timeButton) {
      if (!digitalRead(RESTART)) {
        initialize();
        timeButton = millis();
      }
    }
    if (millis() > DELTA_MENU + timeMenu) {
      line = (line + 1) % 8;
      timeMenu = millis();
    }
    // Menu rendering
    for (byte i = 0; i < 8; i++) {
      matrix.setRow(0, i, menu[(i + line) % 8] );
    }
  }
}
