#include <stdio.h>

#include "game_structs.h"
#include "game_functions.h"

int main(void) {
	printf("Welcom to Text RPG game!\n");
	Player player;

	create_player(&player, "Hero", Warrior);
	print_player(&player);

	Enemy enemy = generate_enemy(Goblin, 1);
	print_enemy(&enemy);

	battle(&player, &enemy);
	print_player(&player);

	enemy = generate_enemy(Goblin, 2);

	battle(&player, &enemy);
	print_player(&player);

	enemy = generate_enemy(Troll, 1);

	battle(&player, &enemy);
	print_player(&player);

	battle(&player, &enemy);
	print_player(&player);

	enemy = generate_enemy(Troll, 2);

	battle(&player, &enemy);
	print_player(&player);

	enemy = generate_enemy(Dragon, 1);
	battle(&player, &enemy);
	print_player(&player);
	enemy = generate_enemy(Dragon, 2);
	battle(&player, &enemy);
	print_player(&player);
	enemy = generate_enemy(Dragon, 3);
	battle(&player, &enemy);
	print_player(&player);

	return 0;
}
