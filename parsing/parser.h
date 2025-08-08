#ifndef PARSER_H
#define PARSER_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h> // for printf delete it after finish
#include "../libft/libft.h"
#include "../get_next_line/get_next_line.h"
#define PATH_MAX 4096 // you can call the path max in <limits.h>

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

int ft_notmemchar(const char *str, char c, int count_sp);
char **copy_array(char **original, int rows);
int total_lines(char **arr);
int ft_is_all_digits(const char *s);
int ft_is_all_spaces(const char *s);
int ft_isspace(unsigned char c);
void free_str_array(char **arr);


#endif