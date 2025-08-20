/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:31:47 by aezghari          #+#    #+#             */
/*   Updated: 2025/08/19 21:31:48 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	has_wall_at(t_game *game, int x, int y)
{
	int	map_x;
	int	map_y;

	map_x = x / game->tile_size;
	map_y = y / game->tile_size;
	if (map_x < 0 || map_x >= game->map_cols || 
		map_y < 0 || map_y >= game->map_rows)
		return (1);
	return (game->map[map_y][map_x] == '1');
}

void	my_mlx_pixel_put(t_game *data, int x, int y, int color)
{
	char	*dst;

	dst = data->img.img_pixel_ptr + (y * data->img.line_length + x
			* (data->img.bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	cleanup_and_exit(t_game *game, int exit_code)
{
	if (game->img.img_ptr)
		mlx_destroy_image(game->mlx_connection, game->img.img_ptr);
	if (game->win_window)
		mlx_destroy_window(game->mlx_connection, game->win_window);
	if (game->mlx_connection)
		mlx_destroy_display(game->mlx_connection);
	if (game->mlx_connection)
		free(game->mlx_connection);
	if (game->rays)
		free(game->rays);
	exit(exit_code);
}
