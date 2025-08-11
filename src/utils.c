#include "../includes/cub.h"


void my_mlx_pixel_put(t_game *data, int x, int y, int color)
{
	char *dst;

	dst = data->img.img_pixel_ptr + (y * data->img.line_length + x * (data->img.bits_per_pixel / 8));
	*(unsigned int *)dst = color;
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
		return true;

	int map_x = floor(x / game->tile_size);
	int map_y = floor(y / game->tile_size);


	return (map[map_y][map_x] != 0);
}

double distance_bet_points(double x0, double y0, double x1, double y1)
{
	return (sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0)));
}
void init_ray(t_ray *ray, double ray_angle)
{
	ray->ray_angle = ray_angle;
	ray->wall_hit_x = 0;
	ray->wall_hit_y = 0;
	ray->distance = 0;

	ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle < PI;
	ray->is_ray_facing_up = !ray->is_ray_facing_down;
	ray->is_ray_facing_right = ray->ray_angle < PI * 0.5 || ray->ray_angle > PI * 1.5;
	ray->was_hit_vertical = false;
	ray->is_ray_facing_left = !ray->is_ray_facing_right;
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