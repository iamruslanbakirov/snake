#include <LiquidCrystal.h>
LiquidCrystal lcd (2, 3, 4, 5, 6, 7);
const int SPEED = 100;
// joystick
const int X_AXIS_PIN = 0;
const int Y_AXIS_PIN = 1;

//setupable
const int minCol = 0;
const int minRow = 0;
const int heightCell = 8;
const int widthCell = 5;
const int maxCol = 16;
const int maxRow = 2;

const int xMin = minCol*widthCell;
const int yMin = minRow*heightCell;
const int xMax = (widthCell*maxCol)-1;
const int yMax = (heightCell*maxRow)-1;
const int xPosArray[5] = {16, 8, 4, 2, 1};

int currentCol = 0;
int currentRow = 0;

int snakeX = xMin;
int snakeY = yMin;

byte nextChar[heightCell] = {};

void setup() {
  lcd.begin(maxCol, maxRow);
  Serial.begin(9600);
}

void loop() {

  int xJoyStick = readJoystick(X_AXIS_PIN);
  int yJoyStick = readJoystick(Y_AXIS_PIN);

  moveSnake(xJoyStick, yJoyStick);

  detectOverflowPositions(snakeX, snakeY);

  setColRow(snakeX, snakeY);
  createNextChar(snakeX, snakeY);

  writeOnLcd();

  delay(SPEED);
}

void writeOnLcd(){
  lcd.clear();
  lcd.createChar(0, nextChar);
  lcd.setCursor(currentCol, currentRow);
  lcd.write((byte) 0);
}

void setColRow(int x, int y) {
  currentCol = int(x / widthCell);

  if (currentCol > maxCol || currentCol < minCol) {
    currentCol = minCol;
  }

  currentRow = int(y / heightCell);

  if(currentRow < minRow || currentRow > maxRow) {
    currentRow = minRow;
  }
}

void createNextChar(int x, int y) {
  
  int xPos = xPosArray[x % widthCell];
  int yPos = y % heightCell;

  for (int i = 0; i < heightCell; i++) {
    nextChar[i] = yPos == i ? byte(xPos) : byte(0);
  }
}

void detectOverflowPositions(int x, int y) {
  if (x > xMax) {
    snakeX = xMin;
  }

  if (y > yMax) {
    snakeY = yMin;
  }

  if (x < xMin) {
    snakeX = xMax;
  }

  if (y < yMin) {
    snakeY = yMax;
  }
}

int readJoystick(int axis) {
  int val = analogRead(axis);
  val = map(val, 0, 1023, -10, 10);

  if (val <= 2 && val >= -2) {
    val = 0;
  }

  return val;
}

void moveSnake(int xAxis, int yAxis) {
  if (xAxis > 0 && yAxis == 0) {
    snakeX += 1;
  } else if (xAxis < 0 && yAxis == 0) {
    snakeX -= 1;
  } else if (yAxis < 0 && xAxis == 0) {
    snakeY -= 1;
  } else if (yAxis > 0 && xAxis == 0) {
    snakeY += 1;
  } else if (xAxis > 0 && yAxis > 0) {
    snakeX += 1;
    snakeY += 1;
  } else if (xAxis < 0 && yAxis < 0) {
    snakeX -= 1;
    snakeY -= 1;
  } else if (xAxis > 0 && yAxis < 0) {
    snakeX += 1;
    snakeY -= 1;
  } else if (xAxis < 0 && yAxis > 0) {
    snakeX -= 1;
    snakeY += 1;
  }
}
