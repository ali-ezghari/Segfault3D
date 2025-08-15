/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:47:20 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/14 15:39:58 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	validate_border(char *line, t_info *data)
{
	if (!ft_notmemchar(line, '1', 1))
		exit_error(2, "First and last lines must contain only '1'.\n", data);
}

void	validate_char(char c, int *dir_set, t_info *data)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		if (*dir_set)
			exit_error(2, "Invalid map: only one direction allowed.\n", data);
		data->s_dir.dir = c;
		data->s_dir.x = data->v->i;
		data->s_dir.y = data->v->j;
		*dir_set = 1;
	}
	else if (!(c == '0' || c == '1' || c == ' '))
		exit_error(2, "Allowed characters are [0, 1, N, E, W, S, ' ']\n", data);
}

int	find_position(char **map, char target, int *x, int *y)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == target)
			{
				*x = i;
				*y = j;
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

void	flood_fill(t_info *data, char **map, char target, int x, int y)
{
	if (map[x][y] == '\0' || map[x][y] == ' ')
		exit_error(2, "Invalid map: map must be surrounded by walls.\n", data);
	if (map[x][y] == target)
	{
		map[x][y] = 'x';
		flood_fill(data, map, target, x + 1, y);
		flood_fill(data, map, target, x - 1, y);
		flood_fill(data, map, target, x, y + 1);
		flood_fill(data, map, target, x, y - 1);
	}
}

void	check_if_map_valid(char **map, int len, t_info *data)
{
	data->v = malloc(sizeof(t_help_varible));
	if (!data->v)
		exit_error(1, "Memory allocation failed\n", data);
	ft_memset(data->v, 0, sizeof(t_help_varible));
	data->s_dir.dir = '\0';
	while (map[data->v->i])
	{
		if (data->v->i == 0 || data->v->i == len - 1)
			validate_border(map[data->v->i], data);
		data->v->j = 0;
		while (map[data->v->i][data->v->j])
		{
			validate_char(map[data->v->i][data->v->j], &data->v->dir_set, data);
			data->v->j++;
		}
		data->v->i++;
	}
	if (!data->v->dir_set)
		exit_error(2, "Invalid map: add player's start position.\n", data);
	data->v->next = data->s_dir.dir;
	while (find_position(map, data->v->next, &data->v->x, &data->v->y))
	{
		flood_fill(data, map, data->v->next, data->v->x, data->v->y);
		data->v->next = '0';
	}
}
