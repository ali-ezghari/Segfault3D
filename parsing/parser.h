#ifndef PARSER_H
#define PARSER_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h> // for printf delete it after finish
#include "../libft/libft.h"
#include "../get_next_line/get_next_line.h"
#define PATH_MAX 4096 // you can call the path max in <limits.h>

typedef struct g_help_varible
{
	int i;
	int j;
	int x;
	int y;
	int dir_set;
	char next;
}			   t_help_varible;

typedef struct s_day_check
{
    char *named; 
    int checked;
}   t_argv_check;

typedef struct  g_directions
{
    char NO[PATH_MAX];
    char SO[PATH_MAX];
    char WE[PATH_MAX];
    char EA[PATH_MAX];
}   t_directions;

typedef struct g_color_data
{
    int num_color;
    int RGB[3];
}   t_color_data;

typedef struct g_color
{
    t_color_data ceil;
    t_color_data floor;
}   t_color;

typedef struct g_start_dir
{
    char dir;
    int x;
    int y;
}              t_start_dir;


typedef struct g_map_info
{
    t_directions dir;
    t_color color;
    t_start_dir s_dir;
    char **map;

} t_map_info;


/* parser_utils */
int     total_lines(char **arr);
int     ft_isspace(unsigned char c);
int     ft_is_all_spaces(const char *s);
int     ft_is_all_digits(const char *s);
int     ft_notmemchar(const char *str, char c, int count_sp);
void    free_str_array(char **arr);
char    **copy_array(char **original, int rows);

/* init_color_dir */
int     skip_prefix(char *line);
int     parse_line(char *line, t_argv_check *argvs, t_map_info *data);
void    init_directions(char *line, char *dir, t_map_info *data);
void    init_color(char *line, t_color_data *color, t_map_info *data);
void    validate_and_set_rgb(char **rgb, t_color_data *color, t_map_info *data);

/* check_map */
int     find_position(char **map, char target, int *x, int *y);
void	validate_border(char *line, t_map_info *data);
void	check_if_map_valid(char **map, int len, t_map_info *data);
void	validate_char(char c, int *dir_set, int i, int j, t_map_info *data);
void    flood_fill(t_map_info *data, char **map, char target, int x, int y);

/* parser_utils_two */
int     count_map_lines(int fd);
int     is_map_line(char *line);
int     is_all_checked(t_argv_check *map_argv);
int     ft_notmemchar(const char *str, char c, int count_sp);
char    **copy_array(char **original, int rows);

// /* parser */
// void exit_error(int stauts, char *message, t_map_info *data);

/* parser_utils_three */
int     is_valid_name_file(char *argv);
void    exit_error(int stauts, char *message, t_map_info *data);
void    read_map(int fd, t_map_info *data, int total_map_lines, char *line);


#endif