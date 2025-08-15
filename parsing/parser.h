/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:52:10 by mohalaou          #+#    #+#             */
/*   Updated: 2025/08/15 13:17:37 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"
# define PATH_MAX 4096 

typedef struct g_help_varible
{
	int				i;
	int				j;
	int				x;
	int				y;
	int				dir_set;
	char			next;
	char			c;
}					t_help_varible;

typedef struct s_day_check
{
	char			*named;
	int				checked;
}					t_argv_check;

typedef struct g_directions
{
	char			no[PATH_MAX];
	char			so[PATH_MAX];
	char			we[PATH_MAX];
	char			ea[PATH_MAX];
}					t_directions;

typedef struct g_color_data
{
	int				num_color;
	int				_rgb[3];
}					t_color_data;

typedef struct g_color
{
	t_color_data	ceil;
	t_color_data	floor;
}					t_color;

typedef struct g_start_dir
{
	char			dir;
	int				x;
	int				y;
}					t_start_dir;

typedef struct g_info
{
	t_directions	dir;
	t_color			color;
	t_start_dir		s_dir;
	t_help_varible	*v;
	char			**map;
	int				map_width;
	int				map_lenght;

}					t_info;

/* parser_utils */
int					total_lines(char **arr);
int					ft_isspace(unsigned char c);
int					ft_is_all_spaces(const char *s);
int					ft_is_all_digits(const char *s);
int					ft_notmemchar(const char *str, char c, int count_sp);
void				free_str_array(char **arr);
char				**copy_array(char **original, int rows);

/* init_color_dir */
int					skip_prefix(char *line);
int					parse_line(char *line, t_argv_check *argvs,
						t_info *data);
void				init_directions(char *line, char *dir, t_info *data);
void				init_color(char *line, t_color_data *color,
						t_info *data);
void				validate_and_set_rgb(char **rgb, t_color_data *color,
						t_info *data);

/* check_map */
int					find_position(char **map, char target, int *x, int *y);
void				validate_border(char *line, t_info *data);
void				check_if_map_valid(char **map, int len, t_info *data);
void				validate_char(char c, int *dir_set, t_info *data);
void				flood_fill(t_info *data, char **map, char target, int x,
						int y);

/* parser_utils_two */
int					is_map_line(char *line);
int					count_map_lines(int fd, t_info *data);
int					is_all_checked(t_argv_check *map_argv);
int					ft_notmemchar(const char *str, char c, int count_sp);
char				**copy_array(char **original, int rows);

// /* parser */
// void exit_error(int stauts, char *message, t_info *data);

/* parser_utils_three */
int					is_valid_name_file(char *argv);
void				exit_error(int stauts, char *message, t_info *data);
void				read_map(int fd, t_info *data, int total_map_lines,
						char *line);
void				replace_char_in_array(char **arr, char tar, char rep);
char				*skip_empty_lines(int fd, char *line);

#endif