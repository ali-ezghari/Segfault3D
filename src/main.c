#include "../includes/cub.h"

void	my_mlx_pixel_put(t_game *data, int x, int y, int color)
{
	char	*dst;

	dst = data->img.img_pixel_ptr + (y * data->img.line_length + x * (data->img.bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void draw_player(t_game *game, int tile_size, int color)
{
	int player_sx = tile_size * game->player.px;
	int player_sy = tile_size * game->player.py;

	for (int y = 0; y < PLAYER_SIZE; y++)
	{
		for (int x = 0; x < PLAYER_SIZE; x++)
		{
			my_mlx_pixel_put(game, x + player_sx, y + player_sy, color);
		}
	}
}

void draw_tile(t_game *game, int x, int y, int size, int color)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == 0 || i == game->tile_size - 1 && j == 0 || j == game->tile_size - 1)
				my_mlx_pixel_put(game, x + j, y + i, COLOR_BLACK); // draw the gap between the tiles
			else
				my_mlx_pixel_put(game, x + j, y + i, color);
		}
	}
}

void draw_map(t_game *game)
{
	int pixel_x = 0;
	int pixel_y = 0;

	for (int y = 0; y < game->mapY; y++)
	{
		for (int x = 0; x < game->mapX; x++)
		{
			pixel_x = x * game->tile_size;
			pixel_y = y * game->tile_size;
			if (game->map[y * game->mapX + x] == 1)
				draw_tile(game, pixel_x, pixel_y, game->tile_size , COLOR_WHITE);
			else
				draw_tile(game, pixel_x, pixel_y, game->tile_size, COLOR_BLACK);
		}
	}

	draw_player(game, game->tile_size, COLOR_RED);
	mlx_put_image_to_window(game->mlx_connection, game->win_window, game->img.img_ptr, 0 ,0 );

}

int handle_keypress(int keycode, t_game *game)
{	
	double px = game->player.px;
	double py = game->player.py;
	
	if (keycode == 65307)
		exit(0);
	else if (keycode == 119)
		py -= 0.1;
	else if (keycode == 115)
		py += 0.1;
	else if (keycode == 97)
		px -= 0.1;
	else if (keycode == 100)
		px += 0.1;
	

	int tile_x = (int)px;
	int tile_y = (int)py;

	if (game->map[tile_y * game->mapX + tile_x] == 0)
	{
		game->player.px = px;
		game->player.py = py;
	}
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
	game.tile_size = MIN((WIDTH / game.mapX), (HEIGHT / game.mapY));
	draw_map(&game);
	mlx_hook(game.win_window, 2, 1L << 0, handle_keypress, &game);

	mlx_loop(game.mlx_connection);
	return (0);
}
