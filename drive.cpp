// This file controls the drive of the robot and it's various mechanisms
#include "drive.h"
#include <Arduino.h>
#include <CrcLib.h>
using namespace Crc;

int joyForward = 0;
int joyStrafe = 0;
int joyTurn = 0;
int joyForwardSlow = 0;
bool reverseButton = false;
bool reverseState = false;
float slowMultiplier = 0.25;

const int deadBandMinX = -3;
const int deadBandMaxX = 3;
const int deadBandMinY = -3;
const int deadBandMaxY = 3;

const int minTurningSpeed = 1;
const int maxTurningSpeed = 60;
const int minStrafeSpeed = 1;
const int maxStrafeSpeed = 75;

// max is 127 for strafe ^^^
const int minForwardSpeed = 1;
const int maxForwardSpeed = 127;

int TurningSpeed = 0;
int StrafeSpeed = 0;
int ForwardSpeed = 0;

bool reverseButtonPrevious = false;

void driveSetup()
{
  CrcLib::InitializePwmOutput(frontRight);
  CrcLib::InitializePwmOutput(frontLeft);
  CrcLib::InitializePwmOutput(backRight);
  CrcLib::InitializePwmOutput(backLeft);
}

void driveLoop()
{
  joyForward = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
  joyStrafe = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
  joyTurn = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
  joyForwardSlow = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y);
  reverseButton = CrcLib::ReadDigitalChannel(BUTTON::R1);

  if ((reverseButton == true)&&(reverseButtonPrevious==false)){
    reverseState=!reverseState;
  }

  if (reverseState==true){
    joyForward = constrain(-joyForward, -128, 127);
    joyStrafe = constrain(-joyStrafe, -128, 127);
    joyForwardSlow = constrain(-joyForwardSlow, -128, 127);
  }

  // Serial.println(reverseButton);
  
  StrafeSpeed = map(abs(joyStrafe), deadBandMaxX, 127, minStrafeSpeed, maxStrafeSpeed);
  if (joyStrafe < 0)
  {
    StrafeSpeed = StrafeSpeed * (-1);
    if ((joyStrafe <= deadBandMaxX) && (joyStrafe >= deadBandMinX))
    {
      StrafeSpeed = 0;
    }
  }

  if ((joyForward <= deadBandMaxY) && (joyForward >= deadBandMinY))
  {
    joyForward = 0;
  }
  if ((joyForwardSlow <= deadBandMaxY) && (joyForwardSlow >= deadBandMinY))
  {
    joyForwardSlow = 0;
  }

  if (joyForward != 0)
  {
    ForwardSpeed = map(abs(joyForward), deadBandMaxY, 127, minForwardSpeed, maxForwardSpeed);
    if (joyForward < 0)
    {
      ForwardSpeed = ForwardSpeed * (-1);
    }
  }
  else
  {
    ForwardSpeed = map(abs(joyForwardSlow), deadBandMaxY, 127, minForwardSpeed, maxForwardSpeed * slowMultiplier);
    if (joyForwardSlow < 0)
    {
      ForwardSpeed = ForwardSpeed * (-1);
    }
  }

  TurningSpeed = map(abs(joyTurn), deadBandMaxX, 127, minTurningSpeed, maxTurningSpeed);
  if (joyTurn < 0)
  {
    TurningSpeed = TurningSpeed * (-1);
    if ((joyTurn <= deadBandMaxX) && (joyTurn >= deadBandMinX))
    {
      TurningSpeed = 0;
    }
  }

  reverseButtonPrevious=reverseButton;
  // Serial.println(ForwardSpeed);
  CrcLib::MoveHolonomic(ForwardSpeed, TurningSpeed, StrafeSpeed, frontLeft, backLeft, frontRight, backRight);
}
