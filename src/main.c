#include "../includes/cub.h"

void draw_player(t_game *game, int tile_size, int color)
{
	int player_sx = tile_size * game->player.px;
	int player_sy = tile_size * game->player.py;

	for (int y = 0; y < PLAYER_SIZE; y++)
	{
		for (int x = 0; x < PLAYER_SIZE; x++)
		{
			mlx_pixel_put(game->mlx_connection, game->win_window, x + player_sx, y + player_sy, color);
		}
	}
}

void draw_tile(t_game *game, int x, int y, int size, int color)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			mlx_pixel_put(game->mlx_connection, game->win_window, x + j, y + i, color);
		}
	}
}

void draw_map(t_game *game)
{
	int tile_x_size = WIDTH / game->mapX;
	int tile_y_size = HEIGHT / game->mapY;
	int tile_size = MIN(tile_x_size, tile_y_size);
	int pixel_x = 0;
	int pixel_y = 0;

	for (int y = 0; y < game->mapY; y++)
	{
		for (int x = 0; x < game->mapX; x++)
		{
			pixel_x = x * tile_size;
			pixel_y = y * tile_size;
			if (game->map[y * game->mapX + x] == 1)
			{
				draw_tile(game, pixel_x, pixel_y, tile_size, COLOR_WHITE);
			}
			else
				draw_tile(game, pixel_x, pixel_y, tile_size, COLOR_GREY);
		}
	}

	draw_player(game, tile_size, COLOR_RED);
}

int handle_keypress(int keycode, t_game *game)
{
	if (keycode == 65307)
		exit(0);
	else if (keycode == 119)
		game->player.py -= 0.1;
	else if (keycode == 115)
		game->player.py += 0.1;
	else if (keycode == 97)
		game->player.px -= 0.1;
	else if (keycode == 100)
		game->player.px += 0.1;
	
	draw_map(game);
	return (0);
}

int main()
{
	int mapp[] = {
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
	};

	struct s_game game;
	game.mlx_connection = mlx_init();
	game.win_window = mlx_new_window(game.mlx_connection, WIDTH, HEIGHT, "cub3d");
	game.img.img_ptr = mlx_new_image(game.mlx_connection, WIDTH, HEIGHT);
	game.img.img_pixel_ptr = mlx_get_data_addr(game.img.img_ptr, &game.img.bits_per_pixel, &game.img.line_length, &game.img.endian);

	game.map = mapp;
	game.mapX = 8;
	game.mapY = 8;
	game.player.px = 2;
	game.player.py = 2;
	game.mapS = game.mapX * game.mapY;

	draw_map(&game);
	mlx_hook(game.win_window, 2, 1L << 0, handle_keypress, &game);

	mlx_loop(game.mlx_connection);
	return (0);
}
