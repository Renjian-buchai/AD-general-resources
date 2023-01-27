int stateTime = 0;
int gameState = 0;

const int refreshRate = 40;
const int normalisingFactor = 5.0f;
const int maxSpeed = 100;

#pragma region fck

int modulo(int value, int divisor)
{
    if (value == divisor)
    {
        return value;
    }
    return value % divisor;
}

int isColour(int r, int g, int b)
{

    r = modulo(abs(r), 255); // Modulo operator. Ensures that r, g and b are all between 0 and 255.
    g = modulo(abs(g), 255);
    b = modulo(abs(b), 255);

    if (
        CS_R > r - 10 && // Checks if colour is between 10 RGB values off from intended
        CS_R < r + 10 &&
        CS_G > g - 10 &&
        CS_G < g + 10 &&
        CS_B > b - 10 &&
        CS_B < b + 10)
    {
        return true;
    }
    return false;
}

// not case

int notColour(int r, int g, int b)
{

    r = modulo(abs(r), 255);
    b = modulo(abs(b), 255);

    if (
        CS_R > r - 10 && // Checks if colour is between 10 RGB values off from intended
        CS_R < r + 10 &&
        CS_G > g - 10 &&
        CS_G < g + 10 &&
        CS_B > b - 10 &&
        CS_B < b + 10)
    {
        return false; // Reversed for early return
    }
    return true;
}

// Format for individual colours:
//
// is case:
//   int is<Colour>()
//   {
//       return isColour(R, G, B);
//   }
//
// not case:
//   int not<Colour>()
//   {
//      return notColour(R, G, B);
//   }

// Check if direction is within 3 degrees of the intended direction.

int isDir(int intended)
{
    int error = 3;
    if (RotationZ > (intended - error) && RotationZ < (intended + error))
    {
        return true;
    }
    return false;
}

// not case

int notDir(int intended)
{
    intended = modulo(abs(intended), 360);

    int error = 3;
    if (RotationZ > (intended - error) && RotationZ < (intended + error))
    {
        return false;
    }
    return true;
}

// Check if direction is within <error> degrees of the intended directions for fine tuning

int isDirection(int intended, int error)
{
    intended = modulo(abs(intended), 360);
    error = modulo(abs(error), 360);

    if (RotationZ > (intended - error) && RotationZ < (intended + error))
    {
        return true;
    }
    return false;
}

// not case

int notDirection(int intended, int error)
{
    if (RotationZ > (intended - error) && RotationZ < (intended + error))
    {
        return false;
    }
    return true;
}

// 4 cardinal directions
// ! The bearings are counterclockwise
// ! North might not be the direction you start in

int isNorth()
{
    return isDir(180);
}

int isSouth()
{
    return isDir(0);
}

int isEast()
{
    return isDir(90);
}

int isWest()
{
    return isDir(270);
}

// not cardinal directions

int notNorth()
{
    return notDir(180);
}

int notSouth()
{
    return notDir(0);
}

int notEast()
{
    return notDir(90);
}

int notWest()
{
    return notDir(270);
}

int isTime(int timeInSeconds) // ! Remember to add stateTime++ to game0, else it WILL NOT WORK.
{
    if (stateTime >= (timeInSeconds * refreshRate)) // runs at 40Hz, hence the *40
    {
        return true;
    }
    return false;
}

// not case
int notTime(int timeInSeconds) // ! Once again, remember to add stateTime++ to game0
{
    if (stateTime >= (timeInSeconds * refreshRate))
    {
        return false;
    }
    return true;
}

float getPosBlack()
{
    const int normalise = 5;
    float pos = 0;
    float count = 0;

    if (IR_L3 == 0) // Getting the IR input from software.
    {               // Then averaging it out for movements.
        pos += -5;
        count++;
    }
    if (IR_L2 == 0)
    {
        pos += -3;
        count++;
    }
    if (IR_L1 == 0)
    {
        pos += -1;
        count++;
    }
    if (IR_R1 == 0)
    {
        pos += 1;
        count++;
    }
    if (IR_R2 == 0)
    {
        pos += 3;
        count++;
    }
    if (IR_R3 == 0)
    {
        pos += 5;
        count++;
    }

    if (count > 0)
    {
        pos = pos / count;
    }
    return pos;
}

float getPosWhite()
{
    float pos = 0;
    float count = 0;

    if (IR_L3 == 1)
    {
        pos += -5;
        count++;
    }
    if (IR_L2 == 1)
    {
        pos += -3;
        count++;
    }
    if (IR_L1 == 1)
    {
        pos += -1;
        count++;
    }
    if (IR_R1 == 1)
    {
        pos += 1;
        count++;
    }
    if (IR_R2 == 1)
    {
        pos += 3;
        count++;
    }
    if (IR_R3 == 1)
    {
        pos += 5;
        count++;
    }

    if (count > 0)
    {
        pos = pos / count;
    }
    return pos;
}

void lineFollowBlack(int speed, float gain)
{
    speed = modulo(speed, maxSpeed);

    float pos = getPosBlack();
    if (pos > 0)
    {
        WheelLeft = speed;                                             // Moving to the right
        WheelRight = speed - (gain * speed * pos / normalisingFactor); // Decreasing the right wheel's speed so the left will move
    }                                                                  // further than the right.
    else if (pos < 0)
    {
        WheelLeft = speed - (gain * speed * fabs(pos) / normalisingFactor); // Moving to the left
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;  // Making it go straight when the position = 0 just as a failsafe
        WheelRight = speed; // Doesn't affect anything, but just in case
    }
    return;
}

void lineFollowWhite(int speed, float gain)
{
    speed = modulo(speed, maxSpeed);

    float pos = getPosWhite();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - (gain * speed * pos / normalisingFactor);
    }
    else if (pos < 0)
    {
        WheelLeft = speed + (gain * speed * pos / normalisingFactor);
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;
        WheelRight = speed;
    }
    return;
}

// TODO Verify if this works
// ! Don't use, likely doesn't currently work.

void dFollowBlack(int speed, float gain, float derivative) // Such that the closer the position is to 0, the
{                                                          // slower it turns
    speed = modulo(speed, maxSpeed);

    float pos = getPosBlack();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - speed * gain * pos / normalisingFactor - speed * derivative * pos / normalisingFactor;
    }
    else if (pos < 0)
    {
        WheelLeft = speed + speed * gain * pos / normalisingFactor + speed * derivative * pos / normalisingFactor;
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;
        WheelRight = speed;
    }
    return;
}

void dFollowWhite(int speed, float proportion, float derivative) // Same as dFollowBlack
{
    speed = modulo(speed, maxSpeed);

    float pos = getPosWhite();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - speed * proportion * pos / normalisingFactor - speed * derivative * pos / normalisingFactor;
    }
    else if (pos < 0)
    {
        WheelLeft = speed + speed * proportion * pos / normalisingFactor + speed * derivative * pos / normalisingFactor;
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;
        WheelRight = speed;
    }
    return;
}

void stateUp()
{
    stateTime = 0;
    gameState++;
}

void stop()
{
    WheelLeft = 0;
    WheelRight = 0;
    return;
}

void checkpoint()
{
    if (notTime(1))
    {
        LED_1 = 0;
        stateUp();
    }
    stop();
    LED_1 = 1;
}

void move(int leftWheel, int rightWheel)
{
    leftWheel = modulo(leftWheel, maxSpeed);
    rightWheel = modulo(rightWheel, maxSpeed);
    WheelLeft = leftWheel;
    WheelRight = rightWheel;
    return;
}

void moveMax()
{
    WheelLeft = maxSpeed;
    WheelRight = maxSpeed;
    return;
}

#pragma endregion fck

int isOrange()
{ // 'Learn/Advanced/Lift Off Pack/Task-1: Speedway' Orange
    if (isColour(195, 101, 8))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int game0()
{
    stateTime++;

    if (gameState == 0)
    {
        lineFollowBlack(100, 1);
        if (isTime(2))
        {
            stateUp();
        }
    }

    if (gameState == 1)
    {
        lineFollowBlack(100, 1);
        if (isOrange())
        {
            stateUp();
        }
    }

    if (gameState == 2)
    {
        checkpoint();
    }

    if (gameState == 3)
    {
        move(50, -50);
        if (isNorth())
        {
            stateUp();
        }
    }

    if (gameState == 4)
    {
        LED_1 = 1;
        moveMax();
    }

    return 0;
}