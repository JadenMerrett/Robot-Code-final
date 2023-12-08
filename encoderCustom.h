#include "CrcLib.h"
#ifndef ENCODERCUSTOM_H
#define ENCODERCUSTOM_H

void encoderLoop();
void encoderSetup();
void pieceHeight();
void analogCrane();
void towerLoop();
void backtobase();
void slowCrane();

const int encoder = CRC_PWM_6;

#endif