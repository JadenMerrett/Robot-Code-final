//this file controls the forklift and the various parts of the mechanism
#include "encoderCustom.h"
#include <Encoder.h>
#include <PID_v1.h>
#include <Arduino.h>
#include <CrcLib.h>
using namespace Crc;

int towerHeightList[] = {0, 923, 2024, 4964, 7546, 10950, 15400, 20000};
bool towerUpButton = false;
bool towerDownButton = false;
int towerHeightIndex = 0;
bool previousTowerState = false;

int pieceHeightlist[] = {-30, -25, 0, 55, 65, 70, 100};
bool up = false;
bool down = false;
int positionOfPiece = 0;
bool previousState2 = false;

bool righttriggerstate = false;

int downGachetteState = 0;
int upGachetteState = 0;
int outputGachetteDown = 0;
int outputGachetteUp = 0;
int outputGachette = 0;
int speedoffset = 30;
bool analogCraneButton = 0;
int toggleAnalogCrane = 0;

Encoder ForkLiftEncoder(CRC_ENCO_A, CRC_ENCO_B);
double desiredPosition = 0;
double encoderMotorSpeed = 0;
double currentPosition = 0;
float Kp = 1;
float Ki = 0;
float Kd = 0;
PID encPID(&currentPosition, &encoderMotorSpeed, &desiredPosition, 0.2, 0, 0, DIRECT);
unsigned long encStart = 0;
unsigned long encDelay = 2000;

bool toggleAnalogCranePreviousState=false;

bool slowCraneUpState=false;
bool slowCraneDownState=false;

int slowSpeed=32;

void encoderSetup()
{
  CrcLib::InitializePwmOutput(encoder);
  CrcLib::SetPwmOutput(encoder, speedoffset);
  encStart = millis();
  while (millis() - encStart < encDelay){
    CrcLib::Update();
  }
  ForkLiftEncoder.write(0);
  encPID.SetOutputLimits(-128, 127);
  encPID.SetMode(AUTOMATIC);
  
}

void encoderLoop()
{
  towerLoop();
  // pieceHeight();
  analogCrane();
  slowCrane();
}

void towerLoop()
{
  towerUpButton = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
  towerDownButton = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
  currentPosition = ForkLiftEncoder.read();
  if (toggleAnalogCrane==false){

  if ((towerUpButton == true) && (towerHeightIndex < 7) && (previousTowerState == false))
  {
    towerHeightIndex = towerHeightIndex + 1;
    desiredPosition = towerHeightList[towerHeightIndex];
  }

  else if ((towerDownButton == true) && (towerHeightIndex != 0) && (previousTowerState == false))
  {
    towerHeightIndex = towerHeightIndex - 1;
    desiredPosition = towerHeightList[towerHeightIndex];
  }
  previousTowerState = towerUpButton || towerDownButton;
  encPID.Compute();
  CrcLib::SetPwmOutput(encoder, encoderMotorSpeed);
  // Serial.print(desiredPosition);
  // Serial.print(", ");
  // Serial.println(currentPosition);
  // Serial.println(encoderMotorSpeed);
  }
}

void pieceHeight()
{
  up = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
  down = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);

  if ((up == true) && (positionOfPiece < 6) && (previousState2 == false))
  {
    positionOfPiece = positionOfPiece + 1;
    // Serial1.print("1,p");
    // Serial1.println(pieceHeightlist[positionOfPiece]);
    currentPosition = pieceHeightlist[positionOfPiece];
  }

  else if ((down == true) && (positionOfPiece != 0) && (previousState2 == false))
  {
    positionOfPiece = positionOfPiece - 1;
    // Serial1.print("1,p");
    // Serial1.println(pieceHeightlist[positionOfPiece]);
    currentPosition = pieceHeightlist[positionOfPiece];
  }
  previousState2 = up || down;
}

void analogCrane()
{
analogCraneButton = CrcLib::ReadDigitalChannel(BUTTON::L1);

if ((analogCraneButton==true)&&(toggleAnalogCranePreviousState==false)) {
  toggleAnalogCrane=!toggleAnalogCrane;
}

if (toggleAnalogCrane==true) {
  downGachetteState = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);
  outputGachetteDown = map(downGachetteState + 128, 0, 255, 0, 158);

  upGachetteState = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);
  outputGachetteUp = map(upGachetteState + 128, 0, 255, 0, 97);
  outputGachette = speedoffset - outputGachetteDown + outputGachetteUp;

  if ((slowCraneUpState==false)&&(slowCraneDownState==false)) {
    CrcLib::SetPwmOutput(encoder, outputGachette);
    }

  // Serial.println(ForkLiftEncoder.read());
  // Serial1.print("1,p");
  // Serial1.println(currentPosition + outputGachetteUp - outputGachetteDown);
  // Serial.println (analogCraneButton);
  // Serial.println (toggleAnalogCrane);
}

toggleAnalogCranePreviousState=analogCraneButton;
}

void slowCrane () {
  slowCraneUpState=CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
  slowCraneDownState=CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);

  if (slowCraneUpState==true){
    CrcLib::SetPwmOutput(encoder,slowSpeed+40);
  }

  if (slowCraneDownState==true){
    CrcLib::SetPwmOutput(encoder,36-slowSpeed);
  }
}