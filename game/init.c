/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:06:26 by aezghari          #+#    #+#             */
/*   Updated: 2025/08/19 15:13:33 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void	init_ray(t_ray *ray, double ray_angle)
{
	ray->ray_angle = ray_angle;
	ray->wall_hit_x = 0;
	ray->wall_hit_y = 0;
	ray->distance = 0;
	ray->is_facing_down = ray->ray_angle > 0 && ray->ray_angle < PI;
	ray->is_facing_up = !ray->is_facing_down;
	ray->is_facing_right = ray->ray_angle < PI * 0.5
		||ray->ray_angle > PI * 1.5;
	ray->is_facing_left = !ray->is_facing_right;
	ray->was_hit_vertical = false;
}

static void	init_player_rotation(t_player *player, char spawn_dir)
{
	if (spawn_dir == 'S')
		player->rotation_angle = PI / 2;
	else if (spawn_dir == 'N')
		player->rotation_angle = 3 * PI / 2;
	else if (spawn_dir == 'W')
		player->rotation_angle = PI;
	else if (spawn_dir == 'E')
		player->rotation_angle = 0;
}

void	init_player(t_game *game, t_player *player)
{
	int	x;
	int	y;

	x = game->data->s_dir.y;
	y = game->data->s_dir.x;
	player->radius = 6;
	player->move_speed = 0.3;
	player->turn_dir = 0;
	player->walk_dir = 0;
	player->strafe_dir = 0;
	player->rotation_speed = 0.3 * (PI / 180);
	player->px = x * game->tile_size + game->tile_size / 2;
	player->py = y * game->tile_size + game->tile_size / 2;
	init_player_rotation(player, game->data->s_dir.dir);
	game->map[y][x] = '0';
}

void	init_game(t_game *game)
{
	game->width = 800;
	game->height = 800;
	game->map_rows = game->data->map_length;
	game->map_cols = game->data->map_width;
	game->fov_angle = 60 * (PI / 180);
	game->tile_size = 64;
	game->distance_to_pl = (game->width / 2) / tan(game->fov_angle / 2);
	get_map(game);
	game->mlx_connection = NULL;
	game->win_window = NULL;
	game->rays = NULL;
}

void	init_mlx(t_game *game)
{
	game->mlx_connection = mlx_init();
	if (!game->mlx_connection)
		cleanup_and_exit(game, EXIT_FAILURE);
	game->win_window = mlx_new_window(game->mlx_connection,
			game->width, game->height, "cub3d");
	if (!game->win_window)
		cleanup_and_exit(game, EXIT_FAILURE);
	game->img.img_ptr = mlx_new_image(game->mlx_connection,
			game->width, game->height);
	if (!game->img.img_ptr)
		cleanup_and_exit(game, EXIT_FAILURE);
	game->img.img_pixel_ptr = mlx_get_data_addr(
			game->img.img_ptr,
			&game->img.bits_per_pixel,
			&game->img.line_length,
			&game->img.endian);
	if (!game->img.img_pixel_ptr)
		cleanup_and_exit(game, EXIT_FAILURE);
}
