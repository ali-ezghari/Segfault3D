#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
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


typedef struct g_map_info
{
    t_color color;
    t_directions dir;
    char **map;
} t_map_info;

void free_str_array(char **arr)
{
    char **tmp = arr;
    while (*tmp)
        free(*tmp++);
    free(arr);
}

void exit_error(int stauts, char *message)
{
    if (stauts == 2)
        write (2, "Error: ", 7);
    write(stauts, message, strlen(message));
    exit(stauts);
}

int is_valid_name_file(char *argv)
{
    int len;

    len = strlen(argv);
    if (strstr(argv + (len - 4), ".cub"))
        return (1);
    return (0);
}

int init_directions(char *line, char *dir)
{
    int fd;
    char *tmp = ft_strchr(line, '.');

    if (tmp && tmp[strlen(tmp) - 1] == '\n')
            tmp[strlen(tmp) - 1] = '\0';
    
    if (tmp == NULL)
        exit_error(2, "Invalid one of texture\n");
    else
    {
        ft_strlcpy(dir, tmp, PATH_MAX - 1);
        fd = open(dir, O_RDONLY);
        if (fd == -1)
        {
            exit_error(2, "Invalid one of texture can't open\n");
        }
    }
    return (1);
}

int ft_isspace(unsigned char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || 
            c == '\v' || c == '\f' || c == '\r');
}


int ft_is_all_spaces(const char *s)
{
    if (!s || !*s)
        return (0);
    while (*s)
        if (!ft_isspace((unsigned char)*s++))
            return (0);
    return (1);   
}

int digits_count(int n) 
{
    int count = 0;
    if (n == 0) 
        return 1;
    while (n != 0) 
    {
        n /= 10;
        count++;
    }
    return count;
}


int ft_is_all_digits(const char *s)
{
    char *tmp;
    char *iter;

    if (!s || !*s)
        return (0);

    tmp = ft_strtrim(s, " ");
    if (!tmp)
        return (0);

    iter = tmp;
    while (*iter)
    {
        if (!ft_isdigit((unsigned char)*iter))
        {
            free(tmp);
            return (0);
        }
        iter++;
    }
    free(tmp);
    return (1);
}


int init_color(char *line, t_color_data *color)
{
    int i = 0;

    if (line == NULL)
        return (0);
        
   if (line && line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

    while (line[i])
    {
        while (ft_isspace(line[i]))
            i++;
        
        if (line[i] == 'F' || line[i] == 'C')
            i++;
        else 
            break;        
    }

    char **rgb = ft_split(line + i, ',');

    i = 0;
    char **tmp = rgb;
    while (*tmp)
    {
        if (!ft_is_all_digits(*tmp))
            exit_error(2, "Invalid color number input\n");
        color->RGB[i] = ft_atoi(*tmp);

        if ((color->RGB[i] < 0 || color->RGB[i] > 255))
        {
            exit_error(2, "Invalid color struct\n");
        }
        i++;
        tmp++;
    }

    color->num_color = (color->RGB[0] << 16) | (color->RGB[1] << 8) | color->RGB[2];
    free_str_array(rgb);
    return (1);
}

int is_all_checked(t_argv_check *map_argv)
{
    int i = 0;
    while (i < 6)
    {
        if (map_argv[i].checked == 0)
            return (0);
        i++;
    }
    return (1);
}
    
int init_data(int fd, t_map_info *data, int total_map_lines)
{
    char *gnl_ret;
    char *tmp;
    t_argv_check map_argv[6] = { {"NO", 0}, {"SO", 0}, {"WE", 0}, {"EA", 0}, {"F", 0}, {"C", 0} };
        
    gnl_ret = get_next_line(fd);
    // gnl_ret = ft_strdup("");
    while (!is_all_checked(map_argv) && gnl_ret)
    {
        tmp = gnl_ret;
        while (ft_isspace(*gnl_ret))
            gnl_ret++;
        
        if (ft_strncmp(gnl_ret, "NO ", 3) == 0)
        {
            map_argv[0].checked = 1;
            init_directions(gnl_ret, data->dir.NO);
        }
        else if (ft_strncmp(gnl_ret, "SO ", 3) == 0)
        {
            map_argv[1].checked = 1;
            init_directions(gnl_ret, data->dir.SO);
        }
        else if (ft_strncmp(gnl_ret, "WE ", 3) == 0)
        {
            map_argv[2].checked = 1;
            init_directions(gnl_ret, data->dir.WE);
        }
        else if (ft_strncmp(gnl_ret, "EA ", 3) ==  0)
        {
            map_argv[3].checked = 1;
            init_directions(gnl_ret, data->dir.EA);
        }
        else if (ft_strncmp(gnl_ret, "F ", 2) == 0)
        {
            map_argv[4].checked = 1;
            init_color(gnl_ret, &data->color.floor);
        }
        else if (ft_strncmp(gnl_ret, "C ", 2) == 0)
        {
            map_argv[5].checked = 1;
            init_color(gnl_ret, &data->color.ceil);
        }
        else if (gnl_ret[0] == '\0' || ft_strncmp(gnl_ret, "\n", 1) == 0 || ft_is_all_spaces(gnl_ret))
        {
            printf("hna\n");
            gnl_ret = get_next_line(fd);
            if (tmp)
                free(tmp);
            continue;
        }
        else 
            return (1);
        free(tmp);
        gnl_ret = get_next_line(fd);
    }

    if (gnl_ret == NULL)
        exit_error(2, "Invalid file, file is empty\n");
    
    if (ft_strncmp(gnl_ret, "\n", 1) == 0 || ft_is_all_spaces(gnl_ret))
    {
        free(gnl_ret);
        gnl_ret = get_next_line(fd);
        while (gnl_ret && (ft_strncmp(gnl_ret, "\n", 1) == 0 || ft_is_all_spaces(gnl_ret)))
        {
            free(gnl_ret);
            gnl_ret = get_next_line(fd);
        }
    }
    else
        return (1);    
        

    int i = 0;
    int len = 0;
    data->map = malloc(sizeof(char *) * (total_map_lines + 1));

    while (gnl_ret && (ft_strncmp(gnl_ret, "\n", 1) != 0 || !ft_is_all_spaces(gnl_ret)))
    {
        // printf("line before store --> [%s]\n", gnl_ret);
        len = ft_strlen(gnl_ret);
        data->map[i] = malloc(len + 1);
        // ft_strlcpy(data->map[i], gnl_ret, len);
        data->map[i] = ft_strdup(gnl_ret);
        if (data->map[i] && data->map[i][strlen(data->map[i]) - 1] == '\n')
            data->map[i][strlen(data->map[i]) - 1] = '\0';

        i++;
        free(gnl_ret);
        gnl_ret = get_next_line(fd);
    }
    data->map[i] = NULL;

            //     printf("\n------------ stor ------------\n\n");
            // for (int i = 0; data->map[i] != NULL; i++)
            //     printf("%s\n", data->map[i]);


    free(gnl_ret);
    return (0);
}

int count_map_lines(int fd)
{
    int count = 0;
    char *tmp;

    tmp = get_next_line(fd);
    while (tmp)
    {
        if ( !(ft_strncmp(tmp, "NO", 2) == 0 ||
               ft_strncmp(tmp, "SO", 2) == 0 || 
               ft_strncmp(tmp, "WE", 2) == 0 || 
               ft_strncmp(tmp, "EA", 2) == 0 || 
               ft_strncmp(tmp, "F",  1) == 0 ||
               ft_strncmp(tmp, "C",  1) == 0 || 
               ft_strncmp(tmp, "\n", 1) == 0 ))
        {
            count++;
        }
        free(tmp);
        tmp = get_next_line(fd);
    }
    free(tmp);
    close(fd);
    return (count);
}

int find_position(char **map, char targt, int *x, int *y)
{
    int i = 0;
    int j = 0;

    while(map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] == targt)
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

int flood_fill(char **map, char targt, int x, int y)
{
    // (void)map;
    // (void)x;
    // (void)y;

    if (map[x][y] == '\0' || map[x][y] == ' ')
        exit_error(2, "Invalid map, map must be surrounded by walls\n");

    if (map[x][y] == targt)
    {
        map[x][y] = 'x';
        flood_fill(map, targt, x + 1, y);
        flood_fill(map, targt, x - 1, y);
        flood_fill(map, targt, x, y + 1);
        flood_fill(map, targt, x, y - 1);
    }    
    return (0);
}

int total_lines(char **arr)
{
    int i = 0;
    int count = 0;

    while (arr[i++])
        count++;
    return (count);
}

int ft_notmemchar(const char *str, char c, int count_sp)
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

int check_if_map_valid(char **map)
{
    char start_dir;
    int  dir_check = '\0';
    int total_len = total_lines(map);

    int i = 0;
    dir_check = 0;
    while (map[i])
    {
        if (i == 0 || i == total_len - 1)
        {
            if (!ft_notmemchar(map[i], '1', 1))
                return (1);
        }

        int j = 0;
        while (map[i][j])
        {
            if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' || map[i][j] == 'W')
            {
                if (dir_check)
                {
                    return (1);
                }
                start_dir = map[i][j];
                dir_check = 1;
            }
            else if (!(map[i][j] == '0' || map[i][j] == '1' || map[i][j] == ' '))
            {
                return (1);
            }
            j++;
        }
        i++;
    }

    if (dir_check == '\0')
        exit_error(2, "Invalid map, Add player's start position\n");

    char next_char = start_dir;
    int x, y;
    while (find_position(map, next_char, &x, &y))
    {        
        if (flood_fill(map, next_char, x, y))
            return (1);
        next_char = '0';
    }

    printf("\n------------ map ------------\n\n");
    for (int i = 0; map[i] != NULL; i++)
        printf("%s\n", map[i]);
    
    printf("\n------------ dir ------------\n");
    printf("start diriction : [%c]\n", start_dir);

    return (0);
}

int parser(int argc, char *argv, t_map_info *data)
{
    int fd;
    int count_lines_fd;
    int map_lines = 0;
    
    if (argc == 2)
    {
        if (!is_valid_name_file(argv))
            exit_error(2, "Invalid name of file.\n");

        fd = open(argv, O_RDONLY);
        count_lines_fd = open(argv, O_RDONLY);
        map_lines = count_map_lines(count_lines_fd);
        if (fd != -1)
        {
            if (init_data(fd, data, map_lines))
                exit_error(2, "Invalid file content\n");

            // printf("\n------------ bev ------------\n\n");
            // for (int i = 0; data->map[i] != NULL; i++)
            //     printf("%s\n", data->map[i]);

            if (check_if_map_valid(data->map))
                exit_error(2, "Invalid map\n");

            // printf("\n------------ aft ------------\n\n");
            // for (int i = 0; data->map[i] != NULL; i++)
            //     printf("%s\n", data->map[i]);

        }
        else if (fd == -1)
        {
            exit_error(errno, "Opening file.\n");
        }
    }
    else
    {
        exit_error(1, "Invalid number of arguments.\n");
    }
    return (0);
}




int main(int argc, char *argv[])
{
    t_map_info *data;
    data = malloc(sizeof(t_map_info));
    if (!data)
        return (1);
    ft_memset(data, 0, sizeof(t_map_info));
    
    parser(argc, argv[1], data);
    
    // /* print after init the value  */
    // printf("\n================ map ================\n");
    // for (int i = 0; data->map[i] != NULL; i++)
    //     printf("---> %s\n", data->map[i]);

    // printf("\n=============== COLOR ===============\n");
    // printf("ceil  : R [%d], G [%d], B[%d]\n", data->color.ceil.RGB[0], data->color.ceil.RGB[1], data->color.ceil.RGB[2]);
    // printf("floor : R [%d], G [%d], B[%d]\n", data->color.floor.RGB[0], data->color.floor.RGB[1], data->color.floor.RGB[2]);
    // printf("number of ceil color : [%d]\n", data->color.ceil.num_color);
    // printf("number of ceil color : [%d]\n", data->color.floor.num_color);

    // printf("\n===============  dir  ===============\n");
    // printf("NO : [%s]\n", data->dir.NO);
    // printf("WE : [%s]\n", data->dir.WE);
    // printf("SO : [%s]\n", data->dir.SO);
    // printf("EA : [%s]\n", data->dir.EA);
    // printf("\n=====================================\n");


    free_str_array(data->map);
    free(data);
}