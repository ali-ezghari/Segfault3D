/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:29:23 by aezghari          #+#    #+#             */
/*   Updated: 2025/08/19 21:29:56 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void	init_horz_ray(t_game *game, t_player *player, t_ray *ray, t_horz *h)
{
	h->y_intercept = floor(player->py / game->tile_size) * game->tile_size;
	if (ray->is_facing_down)
		h->y_intercept += game->tile_size;
	h->x_intercept = player->px + (h->y_intercept - player->py)
		/ tan(ray->ray_angle);
	h->step_y = game->tile_size;
	if (ray->is_facing_up)
		h->step_y *= -1;
	h->step_x = game->tile_size / tan(ray->ray_angle);
	if (ray->is_facing_left && h->step_x > 0)
		h->step_x *= -1;
	if (ray->is_facing_right && h->step_x < 0)
		h->step_x *= -1;
}

bool	find_horz_wall_hit(t_game *game, t_ray *ray, t_horz *h)
{
	int	map_x;
	int	map_y;

	h->next_x = h->x_intercept;
	h->next_y = h->y_intercept;
	while (1)
	{
		map_x = (int)(h->next_x / game->tile_size);
		map_y = (int)(h->next_y / game->tile_size);
		if (map_x < 0 || map_y < 0 || map_x >= game->map_cols
			|| map_y >= game->map_rows)
			return (false);
		if (has_wall_at(game, h->next_x, h->next_y - ray->is_facing_up))
		{
			h->found = true;
			return (true);
		}
		h->next_x += h->step_x;
		h->next_y += h->step_y;
	}
}

void	init_vert_ray(t_game *game, t_player *player, t_ray *ray, t_vert *v)
{
	v->x_intercept = floor(player->px / game->tile_size) * game->tile_size;
	if (ray->is_facing_right)
		v->x_intercept += game->tile_size;
	v->y_intercept = player->py + (v->x_intercept - player->px)
		* tan(ray->ray_angle);
	v->step_x = game->tile_size;
	if (ray->is_facing_left)
		v->step_x *= -1;
	v->step_y = game->tile_size * tan(ray->ray_angle);
	if (ray->is_facing_up && v->step_y > 0)
		v->step_y *= -1;
	if (ray->is_facing_down && v->step_y < 0)
		v->step_y *= -1;
}

bool	find_vert_wall_hit(t_game *game, t_ray *ray, t_vert *v)
{
	int	map_x;
	int	map_y;

	v->next_x = v->x_intercept;
	v->next_y = v->y_intercept;
	while (1)
	{
		map_x = (int)(v->next_x / game->tile_size);
		map_y = (int)(v->next_y / game->tile_size);
		if (map_x < 0 || map_y < 0 || map_x >= game->map_cols
			|| map_y >= game->map_rows)
			return (false);
		if (has_wall_at(game, v->next_x - ray->is_facing_left, v->next_y))
		{
			v->found = true;
			return (true);
		}
		v->next_x += v->step_x;
		v->next_y += v->step_y;
	}
}
