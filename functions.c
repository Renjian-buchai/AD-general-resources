#pragma region functions

#pragma region declarations

int stateTime = 0;
int gameState = 0;

#pragma endregion declarations

#pragma region colours

int isColour(int r, int g, int b)
{
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

#pragma endregion colours

#pragma region directions

// Check if direction is within 3 degrees of the intended direction.

int isDirection(int intended)
{
    if (RotationZ > (intended - 3) && RotationZ < (intended + 3))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Check if direction is within <error> degrees of the intended directions for smaller tolerances
// (Just in case) You just need to add 2 arguments to switch from the above to this

int isDir(int intended, int error)
{
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
    return isDirection(180);
}

int isSouth()
{
    return isDirection(0);
}

int isEast()
{
    return isDirection(90);
}

int isWest()
{
    return isDirection(270);
}

#pragma endregion directions

#pragma region movements

#pragma region lineFollowing

#pragma region getPosition

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

#pragma endregion getPosition

#pragma region proportional

void lineFollowBlack(float speed, float gain)
{
    float pos = getPosBlack();
    if (pos > 0)
    {
        WheelLeft = speed;                             // Moving to the right
        WheelRight = speed - (gain * speed * pos / 5); // Decreasing the right wheel's speed so the left will move
    }                                                  // further than the right.
    else if (pos < 0)
    {
        WheelLeft = speed - (gain * speed * fabs(pos) / 5); // Moving to the left
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;  // Making it go straight when the position = 0 just as a failsafe
        WheelRight = speed; // (Doesn't affect anything, but explicit > implicit)
    }
    return;
}

void lineFollowWhite(float speed, float gain)
{
    float pos = getPosWhite();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - (gain * speed * pos / 5.0);
    }
    else
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

#pragma endregion proportional

// TODO Verify if this works
// ! Don't use, likely doesn't currently work.
#pragma region differential

void dFollowBlack(float speed, float gain, float derivative) // Such that the closer the position is to 0, the
{                                                            // slower it turns
    float pos = getPosBlack();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - speed * gain * pos / 5 - speed * derivative * pos / 5;
    }
    else if (pos < 0)
    {
        WheelLeft = speed + speed * gain * pos / 5 + speed * derivative * pos / 5;
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;
        WheelRight = speed;
    }
    return;
}

void dFollowWhite(float speed, float proportion, float derivative) // Same as dFollowBlack
{
    float pos = getPosWhite();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - speed * proportion * pos / 5 - speed * derivative * pos / 5;
    }
    else if (pos < 0)
    {
        WheelLeft = speed + speed * proportion * pos / 5 + speed * derivative * pos / 5;
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;
        WheelRight = speed;
    }
    return;
}

#pragma endregion differential

#pragma endregion linefollowing

void stop()
{
    WheelLeft = 0;
    WheelRight = 0;
    return;
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

void stateUp()
{
    stateTime = 0;
    gameState++;
}

#pragma endregion movements

#pragma region time

int isTime(int timeInSeconds) // ! Remember to add stateTime++ to game0, else it WILL NOT WORK.
{
    if (stateTime >= (timeInSeconds * 40)) // runs at 40Hz, hence the *40
    {
        return true;
    }
    else
    {
        return false;
    }
}

#pragma endregion time

#pragma endregion functions