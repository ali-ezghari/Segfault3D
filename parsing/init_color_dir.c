/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_color_dir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:49:10 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/13 10:28:06 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

void	init_directions(char *line, char *dir, t_map_info *data)
{
	int		fd;
	char	*tmp;

	tmp = line + 2;
	while (ft_isspace(*tmp))
		tmp++;
	if (tmp && tmp[ft_strlen(tmp) - 1] == '\n')
		tmp[ft_strlen(tmp) - 1] = '\0';
	if (tmp == NULL || ft_strncmp(tmp, "./", 2) || ft_strlen(tmp) <= 2)
		exit_error(2, "Invalid one of texture, ronge texture path\n", data);
	else
	{
		ft_strlcpy(dir, tmp, PATH_MAX - 1);
		fd = open(dir, O_RDONLY);
		if (fd == -1)
			exit_error(2, "Invalid one of texture can't open\n", data);
	}
}

int	skip_prefix(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (line[i] == 'F' || line[i] == 'C')
			i++;
		else
			break ;
	}
	return (i);
}

void	validate_and_set_rgb(char **rgb, t_color_data *color, t_map_info *data)
{
	int	i;

	i = 0;
	while (rgb[i])
	{
		if (!ft_is_all_digits(rgb[i]))
			exit_error(2, "Invalid color input: must be digits only.\n", data);
		color->RGB[i] = ft_atoi(rgb[i]);
		if (color->RGB[i] < 0 || color->RGB[i] > 255)
			exit_error(2, "Invalid color number: must be between 0 and 255.\n",
				data);
		i++;
	}
}

void	init_color(char *line, t_color_data *color, t_map_info *data)
{
	int		i;
	char	**rgb;

	if (line && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	i = skip_prefix(line);
	rgb = ft_split(line + i, ',');
	if (!rgb)
		exit_error(2, "Memory allocation error\n", data);
	validate_and_set_rgb(rgb, color, data);
	color->num_color = (color->RGB[0] << 16)
		| (color->RGB[1] << 8)
		| color->RGB[2];
	free_str_array(rgb);
}

int	parse_line(char *line, t_argv_check *argvs, t_map_info *data)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (argvs[0].checked = 1, init_directions(line, data->dir.NO, data),
			0);
	if (ft_strncmp(line, "SO ", 3) == 0)
		return (argvs[1].checked = 1, init_directions(line, data->dir.SO, data),
			0);
	if (ft_strncmp(line, "WE ", 3) == 0)
		return (argvs[2].checked = 1, init_directions(line, data->dir.WE, data),
			0);
	if (ft_strncmp(line, "EA ", 3) == 0)
		return (argvs[3].checked = 1, init_directions(line, data->dir.EA, data),
			0);
	if (ft_strncmp(line, "F ", 2) == 0)
		return (argvs[4].checked = 1, init_color(line, &data->color.floor,
				data), 0);
	if (ft_strncmp(line, "C ", 2) == 0)
		return (argvs[5].checked = 1, init_color(line, &data->color.ceil, data),
			0);
	if (line[0] == '\0' || ft_strncmp(line, "\n", 1) == 0
		|| ft_is_all_spaces(line))
		return (2);
	return (1);
}
