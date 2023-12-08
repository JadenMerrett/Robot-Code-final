#ifndef DRIVELOOP_H
#define DRIVELOOP_H
#include <CrcLib.h>
using namespace Crc;

const int frontRight = CRC_PWM_3;
const int frontLeft = CRC_PWM_4;
const int backRight = CRC_PWM_1;
const int backLeft = CRC_PWM_2;

void driveSetup();
void driveLoop();

#endif