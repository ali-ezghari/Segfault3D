/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:50:39 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/15 16:25:45 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void	exit_error(int stauts, char *message, t_info *data)
{
	if (data->v)
		free(data->v);
	if (data->map)
		free_str_array(data->map);
	if (data)
		free(data);
	if (message)
	{
		write(stauts, "Error: \n", 8);
		write(stauts, message, ft_strlen(message));
	}
    if (stauts != -1)
        exit(stauts);
}

void	read_map(int fd, t_info *data, int total_map_lines, char *line)
{
	int (i), (k), (width);
	data->map = malloc(sizeof(char *) * (total_map_lines + 1));
	if (!data->map)
		return ;
	i = 0;
	width = data->map_width;
	while (line && (ft_strncmp(line, "\n", 1) != 0 || !ft_is_all_spaces(line)))
	{
		k = -1;
		data->map[i] = malloc((width + 1) * sizeof(char));
		if (!data->map[i])
			break ;
		while (line[++k] && line[k] != '\n' && k < width)
			data->map[i][k] = line[k];
		while (k < width)
			data->map[i][k++] = ' ';
		data->map[i][k] = '\0';
		i++;
		free(line);
		line = get_next_line(fd);
	}
	data->map[i] = NULL;
	free(line);
}

int	is_valid_name_file(char *argv)
{
	int	len;

	len = ft_strlen(argv);
	if (ft_strnstr(argv + (len - 4), ".cub", 4))
		return (1);
	return (0);
}

void	replace_char_in_array(char **arr, char tar, char rep)
{
	int	i;
	int	j;

	i = 0;
	if (!arr || !(*arr))
		return ;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j])
		{
			if (arr[i][j] == tar)
				arr[i][j] = rep;
			j++;
		}
		i++;
	}
}

char	*skip_empty_lines(int fd, char *line)
{
	while (line && (ft_strncmp(line, "\n", 1) == 0 || ft_is_all_spaces(line)))
	{
		free(line);
		line = get_next_line(fd);
	}
	return (line);
}