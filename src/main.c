#include "lib.h"

#define SCREEN_WIDTH 127
#define SCREEN_HEIGHT 63

void draw_rect(Rect rect, int color) {
	drect(rect.v1.x, rect.v1.y, rect.v2.x, rect.v2.y, color);
}

struct Player {
	uint8_t x_pos;
	float y_pos;
	bool is_falling;
	Rect rect;
};

typedef struct {
	Rect upper, lower;
	float x_pos;
	uint8_t select;
} Column;

int hit(struct Player player, Column column) {
	if (player.x_pos >= (uint8_t) column.x_pos && player.x_pos <= (uint8_t) column.x_pos + 10) {
		if ((uint8_t) player.y_pos <= column.upper.v2.y || (uint8_t) player.y_pos >= column.lower.v1.y) {
			return 1;
		}
	}

	return 0;
}

int main() {
	//game variables
	uint64_t timer = 0;
	uint8_t key;
	float falling_timer = 0;
	uint32_t score = 0;

	//initialize player	
	struct Player player;
	player.is_falling = true;
	player.x_pos = 20;
	player.y_pos = SCREEN_HEIGHT / 2;
	
	//initialize columns
	const uint8_t column_height[4] = {10, 20, 30, 40};
	
	Column first_col;
	first_col.x_pos = SCREEN_WIDTH - 10;
	
	Column second_col;
	second_col.x_pos = (SCREEN_WIDTH - 10) * 2 - SCREEN_WIDTH / 2;
	
	while ((key = pollevent().key) != KEY_DEL) {
		timer += 1;
		
		//column mouvement
		if ((first_col.x_pos -= 0.02) <= 1) {
			first_col.x_pos = SCREEN_WIDTH - 10;
			first_col.select = timer % 4;
		}
		if ((second_col.x_pos -= 0.02) <= 1) {
			second_col.x_pos = SCREEN_WIDTH - 10;
			second_col.select = (timer + 3) % 4;
		}

		//player movement
		if (key == KEY_EXE && player.is_falling == true) player.is_falling = false;

		if (player.is_falling == true) {
			player.y_pos += 0.015;
		}
		else {
			player.y_pos -= 0.02;
			falling_timer += 0.003;
		}

		if (falling_timer >= 1) {
			player.is_falling = true;
			falling_timer = 0;
		}
		
		//add score
		if (player.x_pos == (uint8_t) first_col.x_pos + 5) score += 1;
		if (player.x_pos == (uint8_t) second_col.x_pos + 5) score += 1;
		 
		//create player_rect
		player.rect = new_rect(
			new_point(player.x_pos, player.y_pos),
			new_point(player.x_pos + 5, player.y_pos + 2)
		);

		//create columns
		first_col.upper = new_rect(
			new_point(first_col.x_pos, 0),
			new_point(first_col.x_pos + 10, column_height[first_col.select])
		);

		first_col.lower = new_rect(
			new_point(first_col.x_pos, column_height[first_col.select] + 20),
			new_point(first_col.x_pos + 10, SCREEN_HEIGHT)
		);
		
		second_col.upper = new_rect(
			new_point(second_col.x_pos, 0),
			new_point(second_col.x_pos + 10, column_height[second_col.select])
		);

		second_col.lower = new_rect(
			new_point(second_col.x_pos, column_height[second_col.select] + 20),
			new_point(second_col.x_pos + 10, SCREEN_HEIGHT)
		);
		
		if (hit(player, first_col) == 1) break;
		if (hit(player, second_col) == 1) break;

		dclear(C_WHITE);
		draw_rect(first_col.upper, C_BLACK);
		draw_rect(first_col.lower, C_BLACK);

		draw_rect(second_col.upper, C_BLACK);
		draw_rect(second_col.lower, C_BLACK);
		draw_rect(player.rect, C_BLACK);
		
		dprint(1, 1, C_BLACK, "%d", score);
		dupdate();
		key = 0;
	} 

	dclear(C_WHITE);
	dtext(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3, C_BLACK, "GAME OVER!");
	dprint(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, C_BLACK, "YOUR SCORE: %d", score);
	dupdate();
	getkey();

	return 1;
}