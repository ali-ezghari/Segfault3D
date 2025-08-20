/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:50:50 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/15 16:23:08 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	is_map_line(char *line)
{
	while (*line == ' ')
		line++;
	if (ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0
		|| ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line, "EA", 2) == 0
		|| ft_strncmp(line, "F", 1) == 0 || ft_strncmp(line, "C", 1) == 0
		|| ft_strncmp(line, "\n", 1) == 0)
		return (0);
	return (1);
}

int	count_map_lines(int fd, t_info *data)
{
	size_t	max_widht;
	int		count;
	char	*line;

	count = 0;
	max_widht = 0;
	line = get_next_line(fd);
	while (line && !is_map_line(line))
	{
		free(line);
		line = get_next_line(fd);
	}
	while (line)
	{
		count++;
		if (ft_strncmp(line, "\n", 1) == 0 || ft_is_all_spaces(line))
			exit_error(2, "Invalid map, map shuld be the last think\n", data);
		if (ft_strlen(line) > max_widht)
			max_widht = ft_strlen(line);
		free(line);
		line = get_next_line(fd);
	}
	data->map_width = max_widht;
	data->map_length = count;
	return (close(fd), count);
}

int	ft_notmemchar(const char *str, char c, int count_sp)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (count_sp && *str == ' ')
			;
		else if (*str != c)
			return (0);
		str++;
	}
	return (1);
}

char	**copy_array(char **original, int rows)
{
	int		i;
	char	**copy;

	i = 0;
	copy = malloc(sizeof(char *) * (rows + 1));
	if (!copy)
		return (NULL);
	while (i < rows)
	{
		copy[i] = ft_strdup(original[i]);
		if (!copy[i])
			return (NULL);
		i++;
	}
	copy[rows] = NULL;
	return (copy);
}

int	is_all_checked(t_argv_check *map_argv)
{
	int	i;

	i = 0;
	while (i < 6)
	{
		if (map_argv[i].checked == 0)
			return (0);
		i++;
	}
	return (1);
}
