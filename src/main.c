#include "../includes/cub.h"

void my_mlx_pixel_put(t_game *data, int x, int y, int color)
{
	char *dst;

	dst = data->img.img_pixel_ptr + (y * data->img.line_length + x * (data->img.bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void drawLine(t_game *game, int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (x0 != x1 || y0 != y1)
	{
		my_mlx_pixel_put(game, x0, y0, color);

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
	my_mlx_pixel_put(game, x1, y1, color); // Make sure to draw the last point
}

int hasWallAt(t_game *game, t_player *player, int x, int y)
{
	if (x < 0 || y < 0 || y > game->height || x > game->width)
		return 1;

	int check_x = x + cos(player->rotationAngle) * player->radius;
	int check_y = y + sin(player->rotationAngle) * player->radius;

	int map_x = check_x / game->tile_size;
	int map_y = check_y / game->tile_size;

	return (map[map_y][map_x] != 0);
}

void castRay(t_game *game, t_player *player, double rayAngle)
{

	double rayDirX = cos(rayAngle);
	double rayDirY = sin(rayAngle);
	double stepSize = 0.5;
	double maxRayLen = 40;
	double rayLen = 0;
	double nextX = player->px;
	double nextY = player->py;

	while (rayLen < maxRayLen)
	{
		nextX += rayDirX * stepSize;
		nextY +=  rayDirY * stepSize;

		if (hasWallAt(game, &game->player, nextX, nextY))
		{
			break;
		}
		
	}

	drawLine(game, player->px, player->py, nextX, nextY, COLOR_RED);
}

void bismiLah(t_game *game, t_player *player)
{
	double fov = 60.0 * (PI / 180);
	int column = 0;
	double rayAngle = player->rotationAngle - (fov / 2.0);
	int i = 0;
	int raysNum = 320;

	while (i < raysNum)
	{
		castRay(game, &game->player, rayAngle);
		rayAngle += fov / raysNum;
		column++;
		i++;
	}
}

void update(t_game *game, t_player *player)
{
	//? rotating the player based on keypress right(2px to the right) left(2px to the left)
	player->rotationAngle += player->turnDirection * player->rotationSpeed;

	// ? move the player forward or backwards based on the walkDirection
	double moveStep = player->walkDirection * player->moveSpeed;

	double newPlayerX = player->px + cos(player->rotationAngle) * moveStep;
	double newPlayerY = player->py + sin(player->rotationAngle) * moveStep;

	//? prevent the player from going through walls
	if (!hasWallAt(game, player, newPlayerX, newPlayerY))
	{
		player->px = newPlayerX;
		player->py = newPlayerY;
	}
}

void drawPlayer(t_game *game, t_player *player, int color)
{
	update(game, &game->player);
	for (int y = -player->radius; y <= player->radius; y++)
	{
		for (int x = -player->radius; x <= player->radius; x++)
		{
			if (x * x + y * y <= player->radius * player->radius)
			{
				my_mlx_pixel_put(game, player->px + x, player->py + y, color);
			}
		}
	}

	// calculates the distance between player and the desired point that is 30pixel away
	double end_x = player->px + (cos(player->rotationAngle) * 30);
	double end_y = player->py + (sin(player->rotationAngle) * 30);
	drawLine(game, player->px, player->py, end_x, end_y, COLOR_RED);
}

void draw_rec(t_game *game, int x, int y, int size, int color)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			my_mlx_pixel_put(game, x + i, y + j, color);
		}
	}
}

void draw(t_game *game)
{
	for (int x = 0; x < game->mapRows; x++)
	{
		for (int y = 0; y < game->mapCols; y++)
		{
			int tile_x = y * game->tile_size;
			int tile_y = x * game->tile_size;
			int tile_color = map[x][y] == 1 ? COLOR_WHITE : COLOR_BLACK;
			draw_rec(game, tile_x, tile_y, game->tile_size, tile_color);
		}
	}
	drawPlayer(game, &game->player, COLOR_RED);
	bismiLah(game, &game->player);
	mlx_put_image_to_window(game->mlx_connection, game->win_window, game->img.img_ptr, 0, 0);
}

int handle_keypress(int keycode, t_game *game)
{
	if (keycode == ESC_KEY)
	{
		exit(1);
	}

	if (keycode == UP_ARROW)
		game->player.walkDirection = 1;
	else if (keycode == DOWN_ARROW)
		game->player.walkDirection = -1;
	else if (keycode == RIGHT_ARROW)
		game->player.turnDirection = 1;
	else if (keycode == LEFT_ARROW)
		game->player.turnDirection = -1;
	draw(game);
	return (0);
}
int handle_keyrelease(int keycode, t_game *game)
{
	if (keycode == LEFT_ARROW || keycode == RIGHT_ARROW)
		game->player.turnDirection = 0;
	if (keycode == UP_ARROW || keycode == DOWN_ARROW)
		game->player.walkDirection = 0;

	return (0);
}

int main()
{

	struct s_game game;
	// game
	game.mapRows = 11;
	game.mapCols = 15;
	game.mapSize = game.mapRows * game.mapCols;
	game.tile_size = 32;
	game.width = game.tile_size * game.mapCols;
	game.height = game.tile_size * game.mapRows;

	// player
	game.player.px = game.width / 2;
	game.player.py = game.height / 2;
	game.player.moveSpeed = 2.00;
	game.player.radius = 3;
	game.player.walkDirection = 0;
	game.player.turnDirection = 0;
	game.player.rotationAngle = PI / 2;
	game.player.rotationSpeed = 4 * (PI / 180);

	game.mlx_connection = mlx_init();
	game.win_window = mlx_new_window(game.mlx_connection, game.width, game.height, "cub3d");
	game.img.img_ptr = mlx_new_image(game.mlx_connection, game.width, game.height);
	game.img.img_pixel_ptr = mlx_get_data_addr(game.img.img_ptr, &game.img.bits_per_pixel, &game.img.line_length, &game.img.endian);

	draw(&game);
	mlx_hook(game.win_window, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.win_window, 3, 1L << 1, handle_keyrelease, &game);

	mlx_loop(game.mlx_connection);

	return (0);
}
