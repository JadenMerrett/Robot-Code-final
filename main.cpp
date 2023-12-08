//This file is the main file and controlls all other files and libraries
#include <Arduino.h>
#include <CrcLib.h>
#include <Encoder.h>
#include "drive.h"
#include "encodercustom.h"
#include "claw.h"
using namespace Crc;

void setup()
{
  CrcLib::Initialize();
  encoderSetup();
  driveSetup();
  clawSetup();
  Serial.begin(2000000);
  
}

void loop()
{

  CrcLib::Update();

  if (CrcLib::IsCommValid())
  {
    encoderLoop();
    driveLoop();
    wristLoop();
  }

  else
  {
    CrcLib::MoveHolonomic(0, 0, 0, frontLeft, backLeft, frontRight, backRight);
  }
}