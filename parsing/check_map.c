#include "parser.h"

void	validate_border(char *line, t_map_info *data)
{
	if (!ft_notmemchar(line, '1', 1))
		exit_error(2, "Invalid map: first and last lines must contain only '1'.\n", data);
}

void	validate_char(char c, int *dir_set, int i, int j, t_map_info *data)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		if (*dir_set)
			exit_error(2, "Invalid map: only one direction allowed.\n", data);
		data->s_dir.dir = c;
		data->s_dir.x = i;
		data->s_dir.y = j;
		*dir_set = 1;
	}
	else if (!(c == '0' || c == '1' || c == ' '))
		exit_error(2, "Invalid map: allowed characters are [0, 1, N, E, W, S] and space.\n", data);
}

int find_position(char **map, char target, int *x, int *y)
{
    int i = 0;
    int j = 0;

    while(map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] == target)
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

void flood_fill(t_map_info *data, char **map, char target, int x, int y)
{

    if (map[x][y] == '\0' || map[x][y] == ' ')
		exit_error(2, "Invalid map: map must be surrounded by walls.\n", data);

    if (map[x][y] == target)
    {
        map[x][y] = 'x';
        flood_fill(data, map, target, x + 1, y);
        flood_fill(data, map, target, x - 1, y);
        flood_fill(data, map, target, x, y + 1);
        flood_fill(data, map, target, x, y - 1);
    }    
}

void	check_if_map_valid(char **map, int len, t_map_info *data)
{
	t_help_varible v;
	ft_memset(&v, 0, sizeof(t_help_varible));
	data->s_dir.dir = '\0';

	while (map[v.i])
	{
		if (v.i == 0 || v.i == len - 1)
			validate_border(map[v.i], data);
		v.j = 0;
		while (map[v.i][v.j])
        {
			validate_char(map[v.i][v.j], &v.dir_set, v.i, v.j - 1, data);
            v.j++;
        }
		v.i++;
	}
	if (!v.dir_set)
		exit_error(2, "Invalid map: add player's start position.\n", data);
	v.next = data->s_dir.dir;
	while (find_position(map, v.next, &v.x, &v.y))
	{
		flood_fill(data, map, v.next, v.x, v.y);
		v.next = '0';
	}
}
