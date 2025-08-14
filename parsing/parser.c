/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:51:17 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/13 14:31:42 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*skip_empty_lines(int fd, char *line)
{
	while (line && (ft_strncmp(line, "\n", 1) == 0 || ft_is_all_spaces(line)))
	{
		free(line);
		line = get_next_line(fd);
	}
	return (line);
}

void init_argvs(t_argv_check *argvs)
{
    argvs[0] = (t_argv_check){ "NO", 0 };
    argvs[1] = (t_argv_check){ "SO", 0 };
    argvs[2] = (t_argv_check){ "WE", 0 };
    argvs[3] = (t_argv_check){ "EA", 0 };
    argvs[4] = (t_argv_check){ "F",  0 };
    argvs[5] = (t_argv_check){ "C",  0 };
}


int	process_map(int fd, char *line, t_map_info *data, int total_map_lines)
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

int	init_data(int fd, t_map_info *data, int total_map_lines)
{
	char	*line;
	char	*tmp;
	int		status;
    t_argv_check argvs[6];

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

void replace_char_in_array(char **arr, char tar, char rep)
{
	int i;
	int j;

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

int	parser(int argc, char *file, t_map_info *data)
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
	map_lines = count_map_lines(fd_count);
	if (init_data(fd, data, map_lines))
		exit_error(2, "Invalid file content.\n", data);
	total_len = total_lines(data->map);
	map_copy = copy_array(data->map, total_len);
	check_if_map_valid(map_copy, total_len, data);
	replace_char_in_array(data->map, ' ', '1');
	free_str_array(map_copy);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_map_info	*data;
	data = malloc(sizeof(t_map_info));
	if (!data)
		return (1);
	ft_memset(data, 0, sizeof(t_map_info));

	data->dir.EA = malloc(sizeof(char) * PATH_MAX);
	data->dir.SO = malloc(sizeof(char) * PATH_MAX);
	data->dir.WE = malloc(sizeof(char) * PATH_MAX);
	data->dir.NO = malloc(sizeof(char) * PATH_MAX);
	

	parser(argc, argv[1], data);

	    // /* print after init the value  */
    printf("\n================ map ================\n");
    for (int i = 0; data->map[i] != NULL; i++)
        printf("---> %s\n", data->map[i]);
    printf("\n=============== COLOR ===============\n");
    printf("ceil  : R [%d], G [%d], B[%d]\n", data->color.ceil.RGB[0],
            data->color.ceil.RGB[1], data->color.ceil.RGB[2]);
    printf("floor : R [%d], G [%d], B[%d]\n", data->color.floor.RGB[0],
            data->color.floor.RGB[1], data->color.floor.RGB[2]);
    printf("number of ceil color : [%d]\n", data->color.ceil.num_color);
    printf("number of ceil color : [%d]\n", data->color.floor.num_color);
    printf("\n===============  dir  ===============\n");
    printf("NO : [%s]\n", data->dir.NO);
    printf("WE : [%s]\n", data->dir.WE);
    printf("SO : [%s]\n", data->dir.SO);
    printf("EA : [%s]\n\n", data->dir.EA);
    printf("Start dir   : [%c]\n", data->s_dir.dir);
    printf("Start dir x : [%d]\n", data->s_dir.x + 1);
    printf("Start dir y : [%d]\n", data->s_dir.y + 1);
    printf("=====================================\n");

	exit_error(0, NULL, data);
}
