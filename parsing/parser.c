/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:51:17 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/15 16:15:51 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void	init_argvs(t_argv_check *argvs)
{
	argvs[0] = (t_argv_check){"NO", 0};
	argvs[1] = (t_argv_check){"SO", 0};
	argvs[2] = (t_argv_check){"WE", 0};
	argvs[3] = (t_argv_check){"EA", 0};
	argvs[4] = (t_argv_check){"F", 0};
	argvs[5] = (t_argv_check){"C", 0};
}

int	process_map(int fd, char *line, t_info *data, int total_map_lines)
{
	if (!line)
		exit_error(2, "Invalid file: file is empty.\n", data);
	if (!(ft_strncmp(line, "\n", 1) == 0 || ft_is_all_spaces(line)))
		return (free(line), 1);
	free(line);
	line = skip_empty_lines(fd, get_next_line(fd));
	read_map(fd, data, total_map_lines, line);
	return (0);
}

int	init_data(int fd, t_info *data, int total_map_lines)
{
	char			*line;
	char			*tmp;
	int				status;
	t_argv_check	argvs[6];

	init_argvs(argvs);
	line = get_next_line(fd);
	while (!is_all_checked(argvs) && line)
	{
		tmp = line;
		while (ft_isspace(*line))
			line++;
		status = parse_line(line, argvs, data);
		if (status == 1)
			return (free(tmp), 1);
		if (status == 2)
		{
			free(tmp);
			line = get_next_line(fd);
			continue ;
		}
		free(tmp);
		line = get_next_line(fd);
	}
	return (process_map(fd, line, data, total_map_lines));
}

int	parser(int argc, char *file, t_info *data)
{
	int		fd;
	int		fd_count;
	int		map_lines;
	int		total_len;
	char	**map_copy;

	if (argc != 2)
		exit_error(1, "Invalid number of arguments.\n", data);
	if (!is_valid_name_file(file))
		exit_error(2, "Invalid file name.\n", data);
	fd = open(file, O_RDONLY);
	fd_count = open(file, O_RDONLY);
	if (fd == -1)
		exit_error(2, "opening file.\n", data);
	map_lines = count_map_lines(fd_count, data);
	if (init_data(fd, data, map_lines))
		exit_error(2, "Invalid file content.\n", data);
	total_len = total_lines(data->map);
	map_copy = copy_array(data->map, total_len);
	check_if_map_valid(map_copy, total_len, data);
	replace_char_in_array(data->map, ' ', '1');
	free_str_array(map_copy);
	return (0);
}
