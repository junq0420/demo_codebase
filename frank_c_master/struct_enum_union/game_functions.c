#include "game_functions.h"

#include <string.h>
#include <stdio.h>

const char *char_class_string[] = {
	"Warrior",
	"Mage",
	"Rogue"
};

const char *enemy_type_string[] = {
	"Goblin",
	"Troll",
	"Dragon"
};

// functions declaration
void create_player(Player *player, const char *name, CharacterClass char_class) {
	printf("prepare to create a char for you.\n");
	strncpy(player->name, name, sizeof(player->name) - 1);
	player->name[sizeof(player->name) - 1] = '\0';
	printf("creating...\n");
	player->char_class = char_class;
	player->level = 1;
	player->health = 100;
	player->exp = 0;

	switch (player->char_class) {// diffrent type character have diffrent ability
		case Warrior: player->ability.strength = 10; break;
		case Mage: player->ability.mana = 50.0; break;
		case Rogue: player->ability.stealth = 20; break;
	}
}

Enemy generate_enemy(EnemyType enemy_type, int32_t level) {
	Enemy enemy;
	enemy.enemy_type = enemy_type;
	enemy.level = level; 
	enemy.health = level * 20;

	switch (enemy_type) {
		case Goblin: enemy.ability.strength = 10 * level; break;
		case Troll: enemy.ability.strength = 20 * level; break;
		case Dragon: enemy.ability.mana = 100 * level; break;
	}

	return enemy;
}

void player_upgrade(Player *player) {
	if (player->exp >= 100) {
		player->exp -= 100;
		player->level += 1;

		player->ability.strength += 10;
		player->health += 20;

		printf("Congratulates you upgrade to %d level, and needs %d exp to next level.\n", player->level, 100 - player->exp);
	}
}

void battle(Player *player, Enemy *enemy) {
	int32_t enemy_hp_reset = enemy->health;

	printf("Player %s encounters a level %d %s! A battle begins...\n", player->name, enemy->level, 
		enemy_type_string[enemy->enemy_type]);

	while (player->health > 0 && enemy->health > 0) {
		// player always firstly attacks enemy
		enemy->health -= player->ability.strength;
		printf("player's attacks deal %d damage to enemy and enemy's hp: %d\n\n", player->ability.strength, enemy->health);
		if (enemy->health <= 0) break;

		player->health -= enemy->ability.strength;
		printf("enemy's attacks deal %d damage to player and plaer's hp: %d\n\n", enemy->ability.strength, player->health);
		if (player->health <= 0) break;
	}

	if (enemy->health <= 0) {
		enemy->health = enemy_hp_reset;
		player->exp += (50 * enemy->level);
		printf("Player defeats Enemy! And gained %d exp points.\n", (50 * enemy->level));
		player_upgrade(player);
	}
	if (player->health <= 0) {
		player->health = 100;
		printf("Player can't defeats Enemy.\n");
	}
}

void print_player(Player *player) {
	printf("********************************************\n");
	printf("Player Info\n");
	printf("Name: %s | Class: %s | LV: %d | HP: %d | Exp: %d\n", player->name, char_class_string[player->char_class], 
		player->level, player->health, player->exp);
	printf("********************************************\n\n");
}

void print_enemy(Enemy *enemy) {
	printf("********************************************\n");
	printf("Enemy Info\n");
	printf("Type: %s | LV: %d | HP: %d\n", enemy_type_string[enemy->enemy_type], enemy->level, enemy->health);
	printf("********************************************\n\n");
}
