#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char name[30];
	int id;
} Player;

void update_player_list(Player **player_list, int *cur_size, int new_size, Player new_player);
void print_player(Player *player, int size);

int main(void)
{
	Player *player_list = NULL;
	int current_size = 0;

	Player player1 = { "Tom", 1 };
	update_player_list(&player_list, &current_size, current_size + 1, player1);
	Player player2 = { "Jerry", 2 };
	update_player_list(&player_list, &current_size, current_size + 1, player2);
	Player player3 = { "Frank", 3 };
	update_player_list(&player_list, &current_size, current_size + 1, player3);

	print_player(player_list, current_size);

	free(player_list);

	return 0;
}


void update_player_list(Player **player_list, int *cur_size, int new_size, Player new_player) {

	if (*cur_size < new_size) {
		printf("Current size is smaller than new size, need to expand the player list\n");

		Player *temp = (Player *)realloc(*player_list, new_size * sizeof(Player));
		if (temp == NULL) {
			perror("Failed to realloc for player list");
			exit(EXIT_FAILURE);
		}

		*player_list = temp; // update the player list

		(*player_list)[*cur_size] = new_player; // add new player to list
		*cur_size = new_size;
	}

}

void print_player(Player *player, int size) {
	printf("Current Player: \n");

	for (size_t i = 0; i < size; i++) {
		printf("ID: %d, Name: %s\n", player[i].id, player[i].name);
	}

}


