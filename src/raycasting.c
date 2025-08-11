#include "../includes/cub.h"

void castRay(t_game *game, t_player *player, t_ray *ray) // ? DDA algorithm
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

	step_x = game->tile_size / tan(ray->ray_angle);
	step_x *= (ray->is_ray_facing_left && step_x > 0) ? -1 : 1;
	step_x *= (ray->is_ray_facing_right && step_x < 0) ? -1 : 1;

	double next_horz_x = x_intercept;
	double next_horz_y = y_intercept;

	while (next_horz_x >= 0 && next_horz_x <= game->width && next_horz_y >= 0 && next_horz_y <= game->height)
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
	// finding the vertical intersections
	//
	// find the closest vertical y intersection point
	x_intercept = floor(player->px / game->tile_size) * game->tile_size;
	if (ray->is_ray_facing_right)
		x_intercept += game->tile_size;

	// find the closest vertical x intersection point
	y_intercept = player->py + (x_intercept - player->px) * tan(ray->ray_angle);

	// calculate the increament xstep and ystep
	step_x = game->tile_size;
	step_x *= ray->is_ray_facing_left ? -1 : 1;

	step_y = game->tile_size * tan(ray->ray_angle);
	step_y *= (ray->is_ray_facing_up && step_y > 0) ? -1 : 1;
	step_y *= (ray->is_ray_facing_down && step_y < 0) ? -1 : 1;

	double next_vert_x = x_intercept;
	double next_vert_y = y_intercept;

	while (next_horz_x >= 0 && next_horz_x <= game->width && next_horz_y >= 0 && next_horz_y <= game->height)
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

void cast_all_rays(t_game *game, t_player *player)
{
	double fov;
	double ray_angle;
	int i;
	int num_of_rays;

	fov = 60.0 * (PI / 180);
	ray_angle = player->rotationAngle - (fov / 2.0);
	i = 0;
	num_of_rays = game->width;
	game->rays = malloc(sizeof(t_ray) * num_of_rays);
	if (!game->rays)
		exit(1);
	while (i < num_of_rays)
	{
		init_ray(&game->rays[i], normalizeAngle(ray_angle));

	
		castRay(game, &game->player, &game->rays[i]);
	
		drawLine(game, player->px * SCALE_FACTOR, player->py * SCALE_FACTOR, game->rays[i].wall_hit_x * SCALE_FACTOR, game->rays[i].wall_hit_y * SCALE_FACTOR, COLOR_RED);
		ray_angle += fov / num_of_rays;
	
		i++;
	}
}