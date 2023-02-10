int stateTime = 0;
int gameState = 0;

const int refreshRate = 40;
const int normalisingFactor = 5.0f;
const int maxSpeed = 100;

int modulo(int value, int divisor) {
  if (value == divisor) {
    return value;
  }
  return value % divisor;
}

int angleHandler(int direction) {
  direction = direction % 360;

  if (direction > 180) {
    if (RotationZ < (direction - 180)) {
      RotationZ += 360;
    }
  } else {
    if (RotationZ > (direction + 180)) {
      RotationZ -= 360;
    }
  }

  return direction;
}

int isColour(int r, int g, int b) {
  r = modulo(abs(r), 255); /* Modulo operator. Ensures that r, g and b are all
                              between 0 and 255. */
  g = modulo(abs(g), 255);
  b = modulo(abs(b), 255);

  if (CS_R > r - 10 && /* Checks if colour is between 10 RGB values off from
                          intended */
      CS_R < r + 10 &&
      CS_G > g - 10 && CS_G < g + 10 && CS_B > b - 10 && CS_B < b + 10) {
    return true;
  }
  return false;
}

/* not case */

int notColour(int r, int g, int b) {
  r = modulo(abs(r), 255);
  b = modulo(abs(b), 255);

  if (CS_R > r - 10 && /* Checks if colour is between 10 RGB values off from
                          intended */
      CS_R < r + 10 &&
      CS_G > g - 10 && CS_G < g + 10 && CS_B > b - 10 && CS_B < b + 10) {
    return false;  // Reversed for early return
  }
  return true;
}

/* Format for individual colours:

is case:
    int is<Colour>()
  {
      return isColour(R, G, B);
  }

not case:
  int not<Colour>()
  {
     return notColour(R, G, B);
  }

Check if direction is within 3 degrees of the intended direction. */

int isDir(int intended) {
  intended = abs(intended) % 360;

  int error = 3;
  if (RotationZ > (intended - error) && RotationZ < (intended + error)) {
    return true;
  }
  return false;
}

// not case

int notDir(int intended) {
  intended = abs(intended) % 360;

  int error = 3;
  if (RotationZ > (intended - error) && RotationZ < (intended + error)) {
    return false;
  }
  return true;
}

// Check if direction is within <error> degrees of the intended directions for
// fine tuning

int isDirection(int intended, int error) {
  intended = modulo(abs(intended), 360);
  error = modulo(abs(error), 360);

  if (RotationZ > (intended - error) && RotationZ < (intended + error)) {
    return true;
  }
  return false;
}

// not case

int notDirection(int intended, int error) {
  if (RotationZ > (intended - error) && RotationZ < (intended + error)) {
    return false;
  }
  return true;
}

// 4 cardinal directions
// ! The bearings are counterclockwise
// ! North might not be the direction you start in

int isNorth() { return isDir(180); }

int isSouth() { return isDir(0); }

int isEast() { return isDir(90); }

int isWest() { return isDir(270); }

// not cardinal directions

int notNorth() { return notDir(180); }

int notSouth() { return notDir(0); }

int notEast() { return notDir(90); }

int notWest() { return notDir(270); }

int isTime(int timeInSeconds)  // ! Remember to add stateTime++ to game0, else
                               // it WILL NOT WORK.
{
  if (stateTime >=
      (timeInSeconds * refreshRate))  // runs at 40Hz, hence the *40
  {
    return true;
  }
  return false;
}

// not case
int notTime(
    float timeInSeconds)  // ! Once again, remember to add stateTime++ to game0
{
  if (stateTime >= (timeInSeconds * refreshRate)) {
    return false;
  }
  return true;
}

int isDistance(int distance) {
  if (US_Front <= distance) {
    return true;
  }
  return false;
}

int notDistance(int distance) {
  if (US_Front <= distance) {
    return false;
  }
  return true;
}

float getPosBlack() {
  const int normalise = 5;
  float pos = 0;
  float count = 0;

  if (IR_L3 == 0)  // Getting the IR input from software.
  {                // Then averaging it out for movements.
    pos += -5;
    count++;
  }
  if (IR_L2 == 0) {
    pos += -3;
    count++;
  }
  if (IR_L1 == 0) {
    pos += -1;
    count++;
  }
  if (IR_R1 == 0) {
    pos += 1;
    count++;
  }
  if (IR_R2 == 0) {
    pos += 3;
    count++;
  }
  if (IR_R3 == 0) {
    pos += 5;
    count++;
  }

  if (count > 0) {
    pos = pos / count;
  }
  return pos;
}

float getPosWhite() {
  float pos = 0;
  float count = 0;

  if (IR_L3 == 1) {
    pos += -5;
    count++;
  }
  if (IR_L2 == 1) {
    pos += -3;
    count++;
  }
  if (IR_L1 == 1) {
    pos += -1;
    count++;
  }
  if (IR_R1 == 1) {
    pos += 1;
    count++;
  }
  if (IR_R2 == 1) {
    pos += 3;
    count++;
  }
  if (IR_R3 == 1) {
    pos += 5;
    count++;
  }

  if (count > 0) {
    pos = pos / count;
  }
  return pos;
}

void lineFollowBlack(int speed, float gain) {
  speed = modulo(speed, maxSpeed);

  float pos = getPosBlack();
  if (pos > 0) {
    WheelLeft = speed;  // Moving to the right
    WheelRight = speed - (gain * speed * pos /
                          normalisingFactor);  // Decreasing the right wheel's
                                               // speed so the left will move
  }                                            // further than the right.
  else if (pos < 0) {
    WheelLeft = speed - (gain * speed * fabs(pos) /
                         normalisingFactor);  // Moving to the left
    WheelRight = speed;
  } else {
    WheelLeft = speed;  // Making it go straight when the position = 0 just as a
                        // failsafe
    WheelRight = speed;  // Doesn't affect anything, but just in case
  }
  return;
}

void lineFollowWhite(int speed, float gain) {
  speed = modulo(speed, maxSpeed);

  float pos = getPosWhite();
  if (pos > 0) {
    WheelLeft = speed;
    WheelRight = speed - (gain * speed * pos / normalisingFactor);
  } else if (pos < 0) {
    WheelLeft = speed + (gain * speed * pos / normalisingFactor);
    WheelRight = speed;
  } else {
    WheelLeft = speed;
    WheelRight = speed;
  }
  return;
}

void gFB(int speed, float gain) { int direction = angleHandler(RotationZ); }

// TODO Write docs
void gyroFollow(int angle, int speed, float gain) {
  int error;
  angleHandler(angle);
  error = RotationZ - angle;

  move_steering(gain * error, speed);

  return;
}

void moveMax() {
  WheelLeft = maxSpeed;
  WheelRight = maxSpeed;

  return;
}

// TODO Write docs
void gFBlack(int speed, int direction, float gain) {
  speed = modulo(speed, maxSpeed);
  float pos = getPosBlack();
  direction = angleHandler(direction);

  if (pos > 2) {
    gyroFollow(direction + 20, speed, gain);
  } else if (pos < 2) {
    gyroFollow(direction - 20, speed, gain);
  } else {
    moveMax();
  }

  return;
}

// TODO Write docs
void gFWhite(int speed, int direction, float gain) {
  speed = modulo(speed, maxSpeed);
  float pos = getPosWhite();
  direction = angleHandler(direction);

  if (pos > 2) {
    gyroFollow(direction + 20, speed, gain);
  } else if (pos < 2) {
    gyroFollow(direction - 20, speed, gain);
  } else {
    gyroFollow(direction, speed, gain);
  }

  return;
}

// TODO Write docs
void gFollowBlack(int speed, int direction, float gain, int correction) {
  speed = modulo(speed, maxSpeed);
  float pos = getPosBlack();
  direction = angleHandler(direction);
  correction = abs(correction) % 360;

  if (pos > 2) {
    gyroFollow(direction + correction, speed, gain);
  } else if (pos < 2) {
    gyroFollow(direction - correction, speed, gain);
  } else {
    gyroFollow(direction, speed, gain);
  }

  return;
}

// TODO Write docs
void gFollowWhite(int speed, int direction, float gain, int correction) {
  speed = modulo(speed, maxSpeed);
  float pos = getPosWhite();
  direction = angleHandler(direction);
  correction = abs(correction) % 360;

  if (pos > 2) {
    gyroFollow(direction - correction, speed, gain);
  } else if (pos < 2) {
    gyroFollow(direction - correction, speed, gain);
  } else {
    gyroFollow(direction, speed, gain);
  }

  return;
}

// TODO Write docs
void gyroFollowBlack(int speed, int direction, float gain, int correction,
                     int error) {
  speed = modulo(speed, maxSpeed);
  float pos = getPosWhite();
  direction = angleHandler(direction);
  correction = abs(correction) % 360;
  error = modulo(error, 5);

  if (pos > error) {
    gyroFollow(direction - correction, speed, gain);
  } else if (pos < error) {
    gyroFollow(direction - correction, speed, gain);
  } else {
    gyroFollow(direction, speed, gain);
  }

  return;
}

void stateUp() {
  stateTime = 0;
  gameState++;
}

void stop() {
  WheelLeft = 0;
  WheelRight = 0;
  return;
}

void checkpoint(int state) {
  stop();
  LED_1 = 1;
  if (gameState == state) {
    if (isTime(1)) {
      LED_1 = 0;
      stateUp();
    }
  }
}

void move(int leftWheel, int rightWheel) {
  leftWheel = modulo(leftWheel, maxSpeed);
  rightWheel = modulo(rightWheel, maxSpeed);
  WheelLeft = leftWheel;
  WheelRight = rightWheel;
  return;
}

// TODO Write docs
void moveSteering(float dir, int speed) {
  WheelLeft = speed - (dir * speed);
  WheelRight = speed + (dir * speed);
}