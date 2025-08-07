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
		return 1;

	int map_x = floor(x / game->tile_size);
	int map_y = floor(y / game->tile_size);

	return (map[map_y][map_x] != 0);
}

int in_bounds(double x, double y, t_game *game)
{
	return (
		x >= 0 &&
		x < game->width * game->tile_size &&
		y >= 0 &&
		y < game->height * game->tile_size);
}

double distance_bet_points(double x0, double y0, double x1, double y1)
{
	return (sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0)));
}

void castRay(t_game *game, t_player *player, t_ray *ray, int column)
{
	double x_intercept, y_intercept;
	double step_x, step_y;

	double horz_wall_hit_x, horz_wall_hit_y;
	int found_horz_wall_hit = false;

	//
	// finding the horizontal intersections
	//
	// find the closest horizontal y intersection point
	y_intercept = floor(player->py / game->tile_size) * game->tile_size;
	if (ray->is_ray_facing_down)
		y_intercept += game->tile_size;

	// find the closest horizontal x intersection point
	x_intercept = player->px + (y_intercept - player->py) / tan(ray->ray_angle);

	// calculate the increament xstep and ystep
	step_y = game->tile_size;
	step_y *= ray->is_ray_facing_up ? -1 : 1;

	step_x = game->tile_size / tan(ray->ray_angle); // ! replace tilesize with step y for checking
	step_x *= (ray->is_ray_facing_left && step_x > 0) ? -1 : 1;
	step_x *= (ray->is_ray_facing_right && step_x < 0) ? -1 : 1;

	double next_horz_x = x_intercept;
	double next_horz_y = y_intercept;

	while (in_bounds(next_horz_x, next_horz_y, game))
	{
		if (hasWallAt(game, player, next_horz_x, next_horz_y - (ray->is_ray_facing_up ? 1 : 0)))
		{
			found_horz_wall_hit = true;
			horz_wall_hit_x = next_horz_x;
			horz_wall_hit_y = next_horz_y;
			break;
		}
		else
		{
			next_horz_x += step_x;
			next_horz_y += step_y;
		}
	}

	double vert_wall_hit_x, vert_wall_hit_y;
	int found_vert_wall_hit = false;
	//
	// finding the horizontal intersections
	//
	// find the closest horizontal y intersection point
	x_intercept = floor(player->px / game->tile_size) * game->tile_size;
	if (ray->is_ray_facing_right)
		x_intercept += game->tile_size;

	// find the closest horizontal x intersection point
	y_intercept = player->py + (x_intercept - player->px) * tan(ray->ray_angle);

	// calculate the increament xstep and ystep
	step_x = game->tile_size;
	step_x *= ray->is_ray_facing_left ? -1 : 1;

	step_y = game->tile_size * tan(ray->ray_angle);
	step_y *= (ray->is_ray_facing_up && step_y > 0) ? -1 : 1;
	step_y *= (ray->is_ray_facing_down && step_y < 0) ? -1 : 1;

	double next_vert_x = x_intercept;
	double next_vert_y = y_intercept;

	while (in_bounds(next_vert_x, next_vert_y, game))
	{
		if (hasWallAt(game, player, next_vert_x - (ray->is_ray_facing_left ? 1 : 0), next_vert_y))
		{
			found_vert_wall_hit = true;
			vert_wall_hit_x = next_vert_x;
			vert_wall_hit_y = next_vert_y;
			break;
		}
		else
		{
			next_vert_x += step_x;
			next_vert_y += step_y;
		}
	}
	double horz_hit_distance = (found_horz_wall_hit) ? distance_bet_points(player->px, player->py, horz_wall_hit_x, horz_wall_hit_y) : INT_MAX;
	double vert_hit_distance = (found_vert_wall_hit) ? distance_bet_points(player->px, player->py, vert_wall_hit_x, vert_wall_hit_y) : INT_MAX;

	ray->wall_hit_x = (horz_hit_distance < vert_hit_distance) ? horz_wall_hit_x : vert_wall_hit_x;
	ray->wall_hit_y = (horz_hit_distance < vert_hit_distance) ? horz_wall_hit_y : vert_wall_hit_y;
	ray->distance = (horz_hit_distance < vert_hit_distance) ? horz_hit_distance : vert_hit_distance;
	ray->was_hit_vertical = vert_hit_distance < horz_hit_distance;
}

void init_ray(t_ray *ray, double ray_angle)
{
	ray->ray_angle = ray_angle;
	ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle < PI;
	ray->is_ray_facing_up = !ray->is_ray_facing_down;
	ray->is_ray_facing_right = ray->ray_angle < PI * 0.5 || ray->ray_angle > PI * 1.5;
	ray->is_ray_facing_left = !ray->is_ray_facing_right;
	ray->distance = 0;
	ray->was_hit_vertical = false;
	ray->wall_hit_x = 0;
	ray->wall_hit_y = 0;
}

void cast_all_rays(t_game *game, t_player *player)
{
	double fov = 60.0 * (PI / 180);
	double ray_angle = player->rotationAngle - (fov / 2.0);
	int column = 0;
	int i = -1;

	int wall_striple_width = 1;
	int num_of_rays = game->width / wall_striple_width;

	game->rays = malloc(sizeof(t_ray) * num_of_rays);
	if (!game->rays)
		exit(1);
	while (++i < num_of_rays)
	{
		ray_angle = normalizeAngle(ray_angle);
		init_ray(&game->rays[i], ray_angle);
		castRay(game, &game->player, &game->rays[i], column);
		drawLine(game, player->px, player->py, game->rays[i].wall_hit_x, game->rays[i].wall_hit_y, COLOR_RED);
		ray_angle += fov / num_of_rays;
		column++;
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
	// double end_x = player->px + (cos(player->rotationAngle) * 30);
	// double end_y = player->py + (sin(player->rotationAngle) * 30);
	// drawLine(game, player->px, player->py, end_x, end_y, COLOR_RED);
}

void draw_rec(t_game *game, int x, int y, int size, int color)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == 0 || i == game->tile_size - 1 && j == 0 || j == game->tile_size - 1)
				my_mlx_pixel_put(game, x + i, y + j, COLOR_BLACK);
			else
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
	cast_all_rays(game, &game->player);
	drawPlayer(game, &game->player, COLOR_RED);
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
	game.tile_size = 64;
	game.width = game.tile_size * game.mapCols;
	game.height = game.tile_size * game.mapRows;

	// player
	game.player.px = game.width / 2;
	game.player.py = game.height / 2;
	game.player.moveSpeed = 2.00;
	game.player.radius = 4;
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
