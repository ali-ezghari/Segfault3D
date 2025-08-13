/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:50:39 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/13 11:08:51 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	exit_error(int stauts, char *message, t_map_info *data)
{
	if (data->v)
		free(data->v);
	if (data->map)
		free_str_array(data->map);
	if (data)
		free(data);
	if (stauts != 0)
	{
		write(stauts, "Error: ", 7);
		write(stauts, message, ft_strlen(message));
	}
	exit(stauts);
}

void	read_map(int fd, t_map_info *data, int total_map_lines, char *line)
{
	int	i;
	int	len;

	data->map = malloc(sizeof(char *) * (total_map_lines + 1));
	if (!data->map)
		return ;
	i = 0;
	while (line && (ft_strncmp(line, "\n", 1) != 0 || !ft_is_all_spaces(line)))
	{
		data->map[i] = ft_strdup(line);
		if (!data->map[i])
			break ;
		len = ft_strlen(data->map[i]);
		if (len > 0 && data->map[i][len - 1] == '\n')
			data->map[i][len - 1] = '\0';
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
