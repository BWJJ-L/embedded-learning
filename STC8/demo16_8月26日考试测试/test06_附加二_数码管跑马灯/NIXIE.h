#ifndef __NIXIE_H__
#define __NIXIE_H__

#include "Config.h"

#define NIX_DI  P44
#define NIX_RCK P43
#define NIX_SCK P42

#define NIXIE_GPIO_INIT     P4M1 &= ~0x1C; P4M0 &= ~0x1C;

void Nixie_GPIO_INIT();
void Nixie_Display(u8, u8);
void Nixie_Operation(u8,u8);

#endif