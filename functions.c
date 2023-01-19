int stateTime = 0;
int gameState = 0;

const int refreshRate = 40;

int isColour(int r, int g, int b)
{
    r = abs(r) % 255; // Modulo operator. Ensures that r, g and b are all between 0 and 255.
    g = abs(g) % 255;
    b = abs(b) % 255;

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
    else
    {
        return false;
    }
}

// Format for individual colours:
// int is<Colour>()
// {
//     return isColour(R, G, B);
// }

// Check if direction is within 3 degrees of the intended direction.

int isDir(int intended)
{
    intended = abs(intended) % 360;

    int error = 3;
    if (RotationZ > (intended - error) && RotationZ < (intended + error))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Check if direction is within <error> degrees of the intended directions for fine tuning

int isDirection(int intended, int error)
{
    intended = abs(intended) % 360;
    error = abs(error) % 360;

    if (RotationZ > (intended - error) && RotationZ < (intended + 3))
    {
        return true;
    }
    else
    {
        return false;
    }
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

float getPosBlack()
{
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
    speed = speed % 100;

    float pos = getPosBlack();
    if (pos > 0)
    {
        WheelLeft = speed;                               // Moving to the right
        WheelRight = speed - (gain * speed * pos / 5.0); // Decreasing the right wheel's speed so the left will move
    }                                                    // further than the right.
    else if (pos < 0)
    {
        WheelLeft = speed - (gain * speed * fabs(pos) / 5.0); // Moving to the left
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;  // Making it go straight when the position = 0 just as a failsafe
        WheelRight = speed; // (Doesn't affect anything, but explicit > implicit)
    }
    return;
}

void lineFollowWhite(int speed, float gain)
{
    speed = speed % 100;

    float pos = getPosWhite();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - (gain * speed * pos / 5.0);
    }
    else if (pos < 0)
    {
        WheelLeft = speed + (gain * speed * pos / 5.0);
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
    speed = speed % 100;

    float pos = getPosBlack();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - speed * gain * pos / 5.0 - speed * derivative * pos / 5.0;
    }
    else if (pos < 0)
    {
        WheelLeft = speed + speed * gain * pos / 5.0 + speed * derivative * pos / 5.0;
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
    speed = speed % 100;

    float pos = getPosWhite();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - speed * proportion * pos / 5.0 - speed * derivative * pos / 5.0;
    }
    else if (pos < 0)
    {
        WheelLeft = speed + speed * proportion * pos / 5.0 + speed * derivative * pos / 5.0;
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;
        WheelRight = speed;
    }
    return;
}

void stop()
{
    WheelLeft = 0;
    WheelRight = 0;
    return;
}

void stateUp()
{
    stateTime = 0;
    gameState++;
}

void checkpoint()
{
    stop();
    LED_1 = 1;
    if (isTime(1))
    {
        LED_1 = 0;
        stateUp();
    }
}

int isTime(int timeInSeconds) // ! Remember to add stateTime++ to game0, else it WILL NOT WORK.
{
    if (stateTime >= (timeInSeconds * refreshRate)) // runs at 40Hz, hence the *40
    {
        return true;
    }
    else
    {
        return false;
    }
}