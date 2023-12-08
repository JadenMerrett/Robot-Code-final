//this file controls the claw system and the various parts of the mechanism
#include "claw.h"
#include <Arduino.h>
#include <CrcLib.h>
using namespace Crc;

int joystickRY = 0;

const int gripSpeed = 8;
const int wristSpeed = 127;

bool controlSwitchState = false;

int gripClose = 100;
int gripOpen = 100;
int gripSpeedOpen = 127;
int manualFlipSpeed = 100;
unsigned long opening_time = 1000;
bool wristSwitchState = true;

bool flipButtonPreviousState = false;
bool flipButton = false;
bool flipState = false;
bool openingState = false;
// ^ is true during flip ^

bool gripState = false;
bool openCloseButton = false;
bool openClosePreviousState = false;
bool rightDpadPreviousState = false;

bool rightDpadState = false;

unsigned long startTime = 0;

unsigned long limitSwitchDelayStart = 0;  // Current millis() when you hit the "Flip" button
unsigned long limitSwitchDelayTime = 1000; //Delay when the flip stops spinning

unsigned long hitSwitchDelayStart = 0;
unsigned long closeHitSwitchDelayTime = 450;
unsigned long openHitSwitchDelayTime = 0;
bool hitSwitchDelayState = false;

int wristStopSpeed=30;

void clawSetup()
{
  CrcLib::InitializePwmOutput(grip);
  CrcLib::InitializePwmOutput(wrist);
  CrcLib::SetDigitalPinMode(wristSwitch, INPUT);
}

void wristLoop()
{
  flipLoop();
  gripCloseOpenLoop();
  manualOverDrive();
}

void flipLoop()
{
  flipButton = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
  wristSwitchState = CrcLib::GetDigitalInput(wristSwitch);

  if ((flipButton == true) && (flipButtonPreviousState == 0))
  {
    flipState = true;
    limitSwitchDelayStart = millis();
    CrcLib::SetPwmOutput(wrist, -wristSpeed);

    CrcLib::SetPwmOutput(grip, gripState ? gripSpeed : -gripSpeedOpen);
  }
  if ((wristSwitchState == false) && (millis() - limitSwitchDelayStart > limitSwitchDelayTime) && (flipState == true))
  {

    if (hitSwitchDelayState == false)
    {
      hitSwitchDelayStart = millis();
      hitSwitchDelayState = true;
    }
  }

  if (millis() - hitSwitchDelayStart > (gripState ? closeHitSwitchDelayTime : openHitSwitchDelayTime) && (hitSwitchDelayState == true) && (flipState == true))
  {
    CrcLib::SetPwmOutput(wrist, wristStopSpeed);
    CrcLib::SetPwmOutput(grip, gripState ? gripClose : 0);
    hitSwitchDelayState = false;    
    flipState = false;
  }

  flipButtonPreviousState = flipButton;
  // Serial.println(hitSwitchDelayState);
}

void gripCloseOpenLoop()
{
  // Gripstate = False is open. Gripstate = True is closed.
  openCloseButton = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
  if (openCloseButton && openClosePreviousState == false)
  {
    gripState = !gripState;
    if (gripState)
    {
      CrcLib::SetPwmOutput(grip, gripClose);
    }
    else
    {
      startTime = millis();
      openingState = true;
      CrcLib::SetPwmOutput(grip, -gripOpen);
    }
  }

  if (gripState == false && millis() - startTime > opening_time && openingState == true)
  {
    CrcLib::SetPwmOutput(grip, 0);
    openingState = false;
  }
  openClosePreviousState = openCloseButton;
}

void manualOverDrive()
{
  rightDpadState = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
  // Serial.println(rightDpadState);
  if (rightDpadState == true && rightDpadPreviousState == false)
  {
    CrcLib::SetPwmOutput(wrist, -wristSpeed/2);
    if (gripState == true) {
      CrcLib::SetPwmOutput(grip, gripSpeed/2);
    }
    if (gripState == false) {
      CrcLib::SetPwmOutput(grip, -gripSpeedOpen/2);
    }
    
  }

  else if (rightDpadState == false && rightDpadPreviousState == true)
  {
    CrcLib::SetPwmOutput(wrist, wristStopSpeed);
    if (gripState == true) {
      CrcLib::SetPwmOutput(grip, gripClose);
    }
    if (gripState == false) {
      CrcLib::SetPwmOutput(grip, 0);
    }
  }

  rightDpadPreviousState = rightDpadState;
}