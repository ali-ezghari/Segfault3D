/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:38:33 by aezghari          #+#    #+#             */
/*   Updated: 2025/08/19 21:58:57 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void	update(t_game *game, t_player *player)
{
	double	new_player_x;
	double	new_player_y;
	double	move_step;

	player->rotation_angle += player->turn_dir * player->rotation_speed;
	move_step = player->walk_dir * player->move_speed;
	new_player_x = player->px + cos(player->rotation_angle) * move_step;
	new_player_y = player->py + sin(player->rotation_angle) * move_step;
	if (player->strafe_dir != 0)
	{
		new_player_x += cos(player->rotation_angle + (PI / 2))
			* player->strafe_dir * player->move_speed;
		new_player_y += sin(player->rotation_angle + (PI / 2))
			* player->strafe_dir * player->move_speed;
	}
	if (!has_wall_at(game, new_player_x + 2, new_player_y + 2)
		&& !has_wall_at(game, new_player_x - 2, new_player_y - 2)
		&& !has_wall_at(game, new_player_x + 2, new_player_y - 2)
		&& !has_wall_at(game, new_player_x - 2, new_player_y + 2))
	{
		player->px = new_player_x;
		player->py = new_player_y;
	}
}

static void	ft_clear(t_game *game, int width, int height)
{
	int	total_pixels;
	int	i;
	int	*pixels;

	i = 0;
	pixels = (int *)game->img.img_pixel_ptr;
	total_pixels = width * height;
	while (i < total_pixels / 2)
		pixels[i++] = COLOR_GREEN;
	while (i < total_pixels)
		pixels[i++] = COLOR_GREY;
}

void	rec(t_game *game, int x, int y, int height)
{
	int	i;
	int	j;

	i = 0;
	while (i < 1) // 1 => width
	{
		j = 0;
		while (j < height)
		{
			my_mlx_pixel_put(game, x + i, y + j, COLOR_WHITE);
			j++;
		}
		i++;
	}
}

void	render_3d_walls(t_game *game, double fov_angle)
{
	double (dis_to_pl), (proj_wall_h), (wall_height), (corr_distance);
	int		y_start;
	int		i;

	i = 0;
	while (i < game->width)
	{
		corr_distance = game->rays[i].distance
			* cos(game->rays[i].ray_angle - game->player.rotation_angle);
		dis_to_pl = (game->width / 2) / tan(fov_angle / 2);
		proj_wall_h = (game->tile_size / corr_distance)
			* dis_to_pl;
		wall_height = (int)proj_wall_h;
		y_start = (game->height / 2) - (proj_wall_h / 2);
		if (y_start < 0)
			y_start = 0;
		if ((wall_height + y_start) > game->height)
			wall_height = game->height - y_start;
		if (wall_height <= 0)
			continue ;
		rec(game, i, y_start, wall_height);
		i++;
	}
}

void	draw(t_game *game)
{
	ft_clear(game, game->width, game->height);
	update(game, &game->player);
	raycasting(game, &game->player);
	render_3d_walls(game, (60 * (PI / 180)));
	mlx_put_image_to_window(game->mlx_connection, game->win_window,
		game->img.img_ptr, 0, 0);
}
