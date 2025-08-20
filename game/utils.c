/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:32:45 by aezghari          #+#    #+#             */
/*   Updated: 2025/08/19 21:32:47 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	handle_keypress(int keycode, t_game *game)
{
	if (keycode == ESC_KEY)
		cleanup_and_exit(game, 0);
	else if (keycode == W_KEY)
		game->player.walk_dir = 1;
	else if (keycode == S_KEY)
		game->player.walk_dir = -1;
	else if (keycode == A_KEY)
		game->player.strafe_dir = -1;
	else if (keycode == D_KEY)
		game->player.strafe_dir = 1;
	else if (keycode == LEFT_ARROW)
		game->player.turn_dir = -1;
	else if (keycode == RIGHT_ARROW)
		game->player.turn_dir = 1;
	draw(game);
	return (0);
}

int	handle_keyrelease(int keycode, t_game *game)
{
	if (keycode == ESC_KEY)
		cleanup_and_exit(game, 0);
	else if (keycode == W_KEY || keycode == S_KEY)
		game->player.walk_dir = 0;
	else if (keycode == A_KEY || keycode == D_KEY)
		game->player.strafe_dir = 0;
	else if (keycode == LEFT_ARROW || keycode == RIGHT_ARROW)
		game->player.turn_dir = 0;
	return (0);
}

double	normalize_angle(double angle)
{
	angle = fmod(angle, 2 * PI);
	if (angle < 0)
		angle += (2 * PI);
	return (angle);
}

double	_2points_dist(double x1, double y1, double x2, double y2)
{
	return (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

void	get_map(t_game *game)
{
	int	rows;
	int	i;

	rows = game->map_rows;
	i = -1;
	game->map = malloc(sizeof(char *) * (rows + 1));
	if (!game->map)
		cleanup_and_exit(game, 1);
	while (++i < rows)
	{
		game->map[i] = strdup(game->data->map[i]);
	}
	game->map[rows] = NULL;
}
