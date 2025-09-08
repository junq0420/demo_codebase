#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "game_structs.h"

// functions declaration
void create_player(Player *player, const char *name, CharacterClass char_class);

Enemy generate_enemy(EnemyType enemy_type, int32_t level);

void battle(Player *player, Enemy *enemy);

void print_player(Player *player);

void print_enemy(Enemy *enemy);

void player_upgrade(Player *player);

#endif // GAME_FUNCTIONS_H
