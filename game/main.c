/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:38:19 by aezghari          #+#    #+#             */
/*   Updated: 2025/08/19 17:39:59 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	main(int argc, char *argv[])
{
	t_info			*data;
	struct s_game	game;

	data = malloc(sizeof(t_info));
	if (!data)
		return (1);
	ft_memset(data, 0, sizeof(t_info));
	parser(argc, argv[1], data);
	game.data = data;
	init_game(&game);
	init_mlx(&game);
	init_player(&game, &game.player);
	draw(&game);
	mlx_hook(game.win_window, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.win_window, 3, 1L << 1, handle_keyrelease, &game);
	mlx_loop(game.mlx_connection);
	return (0);
}
