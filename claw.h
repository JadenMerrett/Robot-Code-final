#ifndef GRIPLOOP_H
#define GRIPLOOP_H
#include <CrcLib.h>
using namespace Crc;

const int grip = CRC_PWM_5;
const int wrist = CRC_PWM_9;
const int wristSwitch = CRC_DIG_1;

void clawSetup();
void gripSetup();
void gripLoop();
void wristSetup();
void wristLoop();
void flipLoop();
void gripCloseOpenLoop();
void controlSwitchSetup();
void controlSwitchLoop();
void manualOverDrive();

#endif