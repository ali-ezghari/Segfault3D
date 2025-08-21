/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:09:32 by aezghari          #+#    #+#             */
/*   Updated: 2025/08/19 21:09:34 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

static void	set_ray_result(t_ray *ray, t_horz *h, t_vert *v, t_player *player)
{
	double	horz_dist;
	double	vert_dist;

	if (h->found)
		horz_dist = _2points_dist(player->px, player->py, h->next_x, h->next_y);
	else
		horz_dist = (double)INT_MAX;
	if (v->found)
		vert_dist = _2points_dist(player->px, player->py, v->next_x, v->next_y);
	else
		vert_dist = (double)INT_MAX;
	if (horz_dist < vert_dist)
	{
		ray->wall_hit_x = h->next_x;
		ray->wall_hit_y = h->next_y;
		ray->distance = horz_dist;
		ray->was_hit_vertical = false;
	}
	else
	{
		ray->wall_hit_x = v->next_x;
		ray->wall_hit_y = v->next_y;
		ray->distance = vert_dist;
		ray->was_hit_vertical = true;
	}
}

static void	cast_ray(t_game *game, t_player *player, t_ray *ray)
{
	t_horz	h;
	t_vert	v;

	memset(&h, 0, sizeof(t_horz));
	memset(&v, 0, sizeof(t_vert));
	init_horz_ray(game, player, ray, &h);
	h.found = find_horz_wall_hit(game, ray, &h);
	init_vert_ray(game, player, ray, &v);
	v.found = find_vert_wall_hit(game, ray, &v);
	set_ray_result(ray, &h, &v, player);
}

void	raycasting(t_game *game, t_player *player)
{
	double	ray_angle;
	int		num_rays;
	int		i;

	num_rays = game->width;
	ray_angle = player->rotation_angle - (game->fov_angle / 2);
	i = 0;
	if (game->rays)
		free(game->rays);
	game->rays = malloc(sizeof(t_ray) * num_rays);
	if (!game->rays)
		cleanup_and_exit(game, EXIT_FAILURE);
	while (i < num_rays)
	{
		init_ray(&game->rays[i], normalize_angle(ray_angle));
		cast_ray(game, player, &game->rays[i]);
		ray_angle += game->fov_angle / num_rays;
		i++;
	}
}
