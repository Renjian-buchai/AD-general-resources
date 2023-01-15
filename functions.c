#pragma region functions

#pragma region declarations

int stateTime = 0;

#pragma endregion declarations

#pragma region movements

#pragma region lineFollowing

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

void lineFollowBlack(float speed, float gain)
{
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

// TODO Verify if this works
// ! Don't use, likely doesn't currently work.
void derivativeFollowBlack(float speed, float gain, float derivative) // Such that the closer the position is to 0, the
{                                                                     // slower it turns
    float pos = getPosBlack();
    if (pos > 0)
    {
        WheelLeft = speed;
        WheelRight = speed - speed;
    }
    else if (pos < 0)
    {
        WheelLeft = (25 * speed + (5 * gain * speed * pos) + (gain * speed * neg(pow(pos, 2)))) / 25;
        WheelRight = speed;
    }
    else
    {
        WheelLeft = speed;
        WheelRight = speed;
    }
    return;
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
    return;
}

#pragma endregion linefollowing

void stop()
{
    WheelLeft = 0;
    WheelRight = 0;
    return;
}

void checkpoint()
{
    while (notTime(1))
    {
        stop();
        LED_1 = 1;
    }
    LED_1 = 0;
}

#pragma endregion movements

#pragma region colours

int notColour(int r, int g, int b)
{
    if (
        CS_R > r - 10 && // Checks if colour is between 10 RGB values off from intended
        CS_R < r + 10 &&
        CS_G > g - 10 &&
        CS_G < g + 10 &&
        CS_B > b - 10 &&
        CS_B < b + 10)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Format for individual colours:
// int not<Colour>()
// {
//     return notColour(R, G, B);
// }

#pragma endregion colours

#pragma region directions

// Check if direction is within 3 degrees of the intended direction.

int notDirection(int intended)
{
    if (RotationZ > (intended - 3) && RotationZ < (intended + 3))
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Check if direction is within <error> degrees of the intended directions for smaller tolerances
// (Just in case) You just need to add 2 arguments to switch from the above to this

int notDirection(int intended, int error)
{
    if (RotationZ > (intended - error) && RotationZ < (intended + 3))
    {
        return false;
    }
    else
    {
        return true;
    }
}

// 4 cardinal directions
// ! The bearings are counterclockwise
// ! North might not be the direction you start in

int notNorth()
{
    return notDirection(180);
}

int notSouth()
{
    return notDirection(0);
}

int notEast()
{
    return notDirection(90);
}

int notWest()
{
    return notDirection(270);
}

#pragma endregion directions

#pragma region time

int notTime(int timeInSeconds) // ! Remember to add stateTime++ to game0, else it WILL NOT WORK.
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