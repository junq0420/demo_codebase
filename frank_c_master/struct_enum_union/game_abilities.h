#ifndef GAME_ABILITIES_H
#define GAME_ABILITIES_H

#include <stdint.h>

typedef union {
	int32_t strength;
	float mana;
	int32_t stealth;
} Ability;

#endif // GAME_ABILITIES_H 
