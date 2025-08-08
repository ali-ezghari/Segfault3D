#include "parser.h"

void exit_error(int stauts, char *message)
{
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

int init_directions(char *line, char *dir)
{
    int fd;
    char *tmp = ft_strchr(line, '.');

    if (tmp && tmp[ft_strlen(tmp) - 1] == '\n')
            tmp[ft_strlen(tmp) - 1] = '\0';
    
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

int init_color(char *line, t_color_data *color)
{
    int i = 0;

    if (line == NULL)
        return (0);
        
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
        // data->map[i] = malloc(len + 1);
        data->map[i] = ft_strdup(gnl_ret);
        if (!data->map[i])
            break;
        // ft_strlcpy(data->map[i], gnl_ret, len);
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
        ptr = tmp; // keep original for free
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

        free(tmp); // free original pointer
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

int flood_fill(char **map, char targt, int x, int y)
{

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

int check_if_map_valid(char **map, int len, t_map_info *data)
{
    // char start_dir;

    data->s_dir.dir = '\0';
    int  dir_check = '\0';
    
    int i = 0;
    dir_check = 0;
    while (map[i])
    {
        if (i == 0 || i == len - 1)
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
                    return (1);
                data->s_dir.dir = map[i][j];
                data->s_dir.x = i;
                data->s_dir.y = j;
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

    char next_char = data->s_dir.dir;
    int x, y;
    while (find_position(map, next_char, &x, &y))
    {        
        if (flood_fill(map, next_char, x, y))
            return (1);
        next_char = '0';
    }
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

            int total_len = total_lines(data->map);
            // printf("map lines : $[%d]\n", map_lines);
            // printf("total len : $[%d]\n", total_len);
            char **map_copy = copy_array(data->map, total_len);

            if (check_if_map_valid(map_copy, total_len, data))
                exit_error(2, "Invalid map\n");
            free_str_array(map_copy);
        }
        else if (fd == -1)
        {
            exit_error(2, "Opening file.\n");
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
    // printf("EA : [%s]\n\n", data->dir.EA);

    // printf("Start dir   : [%c]\n", data->s_dir.dir);
    // printf("Start dir x : [%d]\n", data->s_dir.x + 1);
    // printf("Start dir y : [%d]\n", data->s_dir.y + 1);

    // printf("=====================================\n");


    free_str_array(data->map);
    free(data);
}