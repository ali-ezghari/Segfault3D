#include "../includes/cub.h"

int handle_keypress(int keycode, t_game *game)
{
	if (keycode == ESC_KEY)
		cleanup_and_exit(game, 0);
	else if (keycode == W_KEY)
		game->player.walkDirection = 1;
	else if (keycode == S_KEY)
		game->player.walkDirection = -1;
	else if (keycode == LEFT_ARROW)
		game->player.turnDirection = -1;
	else if (keycode == RIGHT_ARROW)
		game->player.turnDirection = 1;
	draw(game);
	return (0);
}
int handle_keyrelease(int keycode, t_game *game)
{
	if (keycode == ESC_KEY)
		cleanup_and_exit(game, 0);
	else if (keycode == W_KEY || keycode == S_KEY)
		game->player.walkDirection = 0;
	else if (keycode == LEFT_ARROW || keycode == RIGHT_ARROW)
		game->player.turnDirection = 0;
	return 0;
}

void draw_line(t_game *game, int x0, int y0, int x1, int y1, int color)
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
	my_mlx_pixel_put(game, x1, y1, color);
}
double normalizeAngle(double angle)
{
	angle = fmod(angle, 2 * PI);
	if (angle < 0)
	{
		angle += (2 * PI);
	}
	return (angle);
}

double distance_bet_points(double x1, double y1, double x2, double y2)
{
	return (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

int has_wall_at(t_game *game, int x, int y)
{
	if (x < 0 || y < 0 || y > game->height || x > game->width)
		return true;

	int map_x = floor(x / game->tile_size);
	int map_y = floor(y / game->tile_size);

	return (game->map[map_y][map_x] != '0');
}

void my_mlx_pixel_put(t_game *data, int x, int y, int color)
{
	char *dst;

	// if (x < 0 || x >= data->width || y < 0 || y >= data->height)
	// {
	// 	return;
	// }
	dst = data->img.img_pixel_ptr + (y * data->img.line_length + x * (data->img.bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}