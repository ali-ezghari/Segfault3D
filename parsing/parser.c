#include "parser.h"

void exit_error(int stauts, char *message, t_map_info *data)
{
    if (data->map)
        free_str_array(data->map);
    if (data)
        free(data);
    if (stauts == 2)
        write (2, "Error: ", 7);
    write(stauts, message, ft_strlen(message));
    exit(stauts);
}

int is_valid_name_file(char *argv)
{
    int len;

    len = ft_strlen(argv);
    if (ft_strnstr(argv + (len - 4), ".cub", 4))
        return (1);
    return (0);
}

void init_directions(char *line, char *dir, t_map_info *data)
{
    int fd;
    char *tmp = line + 2;
    
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
        {
            exit_error(2, "Invalid one of texture can't open\n", data);
        }
    }
}

void init_color(char *line, t_color_data *color, t_map_info *data)
{
    int i = 0;
        
   if (line && line[ft_strlen(line) - 1] == '\n')
            line[ft_strlen(line) - 1] = '\0';

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
            exit_error(2, "Invalid color number input\n", data);
        color->RGB[i] = ft_atoi(*tmp);

        if ((color->RGB[i] < 0 || color->RGB[i] > 255))
        {
            exit_error(2, "Invalid color number [0 to 255]\n", data);
        }
        i++;
        tmp++;
    }

    color->num_color = (color->RGB[0] << 16) | (color->RGB[1] << 8) | color->RGB[2];
    free_str_array(rgb);
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
            init_directions(gnl_ret, data->dir.NO, data);
        }
        else if (ft_strncmp(gnl_ret, "SO ", 3) == 0)
        {
            map_argv[1].checked = 1;
            init_directions(gnl_ret, data->dir.SO, data);
        }
        else if (ft_strncmp(gnl_ret, "WE ", 3) == 0)
        {
            map_argv[2].checked = 1;
            init_directions(gnl_ret, data->dir.WE, data);
        }
        else if (ft_strncmp(gnl_ret, "EA ", 3) ==  0)
        {
            map_argv[3].checked = 1;
            init_directions(gnl_ret, data->dir.EA, data);
        }
        else if (ft_strncmp(gnl_ret, "F ", 2) == 0)
        {
            map_argv[4].checked = 1;
            init_color(gnl_ret, &data->color.floor, data);
        }
        else if (ft_strncmp(gnl_ret, "C ", 2) == 0)
        {
            map_argv[5].checked = 1;
            init_color(gnl_ret, &data->color.ceil, data);
        }
        else if (gnl_ret[0] == '\0' || ft_strncmp(gnl_ret, "\n", 1) == 0 || ft_is_all_spaces(gnl_ret))
        {
            gnl_ret = get_next_line(fd);
            if (tmp)
                free(tmp);
            continue;
        }
        else
        {
            free(tmp);
            return (1);
        }
        free(tmp);
        gnl_ret = get_next_line(fd);
    }

    if (gnl_ret == NULL)
        exit_error(2, "Invalid file, file is empty\n", data);
    
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
        data->map[i] = ft_strdup(gnl_ret);
        if (!data->map[i])
            break;
        len = ft_strlen(data->map[i]);
        if (data->map[i] && data->map[i][len - 1] == '\n')
            data->map[i][len - 1] = '\0';

        i++;
        free(gnl_ret);
        gnl_ret = get_next_line(fd);
    }
    data->map[i] = NULL;

    free(gnl_ret);
    return (0);
}

int count_map_lines(int fd)
{
    int count = 0;
    char *tmp;
    char *ptr;

    tmp = get_next_line(fd);
    while (tmp)
    {
        ptr = tmp;
        while (*ptr == ' ')
            ptr++;

        if (!(ft_strncmp(ptr, "NO", 2) == 0 ||
              ft_strncmp(ptr, "SO", 2) == 0 || 
              ft_strncmp(ptr, "WE", 2) == 0 || 
              ft_strncmp(ptr, "EA", 2) == 0 || 
              ft_strncmp(ptr, "F",  1) == 0 ||
              ft_strncmp(ptr, "C",  1) == 0 || 
              ft_strncmp(ptr, "\n", 1) == 0))
        {
            count++;
        }

        free(tmp);
        tmp = get_next_line(fd);
    }
    close(fd);
    return count;
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

void flood_fill(t_map_info *data, char **map, char targt, int x, int y)
{

    if (map[x][y] == '\0' || map[x][y] == ' ')
        exit_error(2, "Invalid map, map must be surrounded by walls\n", data);

    if (map[x][y] == targt)
    {
        map[x][y] = 'x';
        flood_fill(data, map, targt, x + 1, y);
        flood_fill(data, map, targt, x - 1, y);
        flood_fill(data, map, targt, x, y + 1);
        flood_fill(data, map, targt, x, y - 1);
    }    
}

void check_if_map_valid(char **map, int len, t_map_info *data)
{
    data->s_dir.dir = '\0';
    int  dir_check = '\0';
    
    int i = 0;
    dir_check = 0;
    while (map[i])
    {
        if (i == 0 || i == len - 1)
        {
            if (!ft_notmemchar(map[i], '1', 1))
                exit_error(2, "Invalid map, the 1th the last line shuld be only \'1\'\n", data);
        }

        int j = 0;
        while (map[i][j])
        {
            if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' || map[i][j] == 'W')
            {
                if (dir_check)
                    exit_error(2, "Invalid map, only one diriction\n", data);
                data->s_dir.dir = map[i][j];
                data->s_dir.x = i;
                data->s_dir.y = j;
                dir_check = 1;
            }
            else if (!(map[i][j] == '0' || map[i][j] == '1' || map[i][j] == ' '))
                exit_error(2, "Invalid map, only that 6 character [0, 1, N, E, W, S] or space\n", data);
            j++;
        }
        i++;
    }

    if (dir_check == '\0')
        exit_error(2, "Invalid map, Add player's start position\n", data);

    char next_char = data->s_dir.dir;
    int x, y;
    while (find_position(map, next_char, &x, &y))
    {        
        flood_fill(data, map, next_char, x, y);
        next_char = '0';
    }
}

int parser(int argc, char *argv, t_map_info *data)
{
    int fd;
    int count_lines_fd;
    int map_lines = 0;
    
    if (argc == 2)
    {
        if (!is_valid_name_file(argv))
            exit_error(2, "Invalid name of file.\n", data);

        fd = open(argv, O_RDONLY);
        count_lines_fd = open(argv, O_RDONLY);
        map_lines = count_map_lines(count_lines_fd);
        if (fd != -1)
        {
            if (init_data(fd, data, map_lines))
                exit_error(2, "Invalid file content\n", data);

            int total_len = total_lines(data->map);
            char **map_copy = copy_array(data->map, total_len);
            check_if_map_valid(map_copy, total_len, data);
            free_str_array(map_copy);
        }
        else if (fd == -1)
            exit_error(2, "Opening file.\n", data);
    }
    else
        exit_error(1, "Invalid number of arguments.\n", data);
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
    printf("\n================ map ================\n");
    for (int i = 0; data->map[i] != NULL; i++)
        printf("---> %s\n", data->map[i]);

    printf("\n=============== COLOR ===============\n");
    printf("ceil  : R [%d], G [%d], B[%d]\n", data->color.ceil.RGB[0], data->color.ceil.RGB[1], data->color.ceil.RGB[2]);
    printf("floor : R [%d], G [%d], B[%d]\n", data->color.floor.RGB[0], data->color.floor.RGB[1], data->color.floor.RGB[2]);
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


    free_str_array(data->map);
    free(data);
}