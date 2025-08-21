/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <mohalaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:38:38 by aezghari          #+#    #+#             */
/*   Updated: 2025/08/20 18:32:16 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include <string.h>
# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"
# include "../minilibx-linux/mlx.h"
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <X11/X.h>
# include <stdbool.h>
# include <limits.h>

# define PATH_MAX 4096 

# define PI 3.14159265359

# define ESC_KEY 65307
# define RIGHT_ARROW 65363
# define LEFT_ARROW 65361
# define W_KEY 119
# define A_KEY 97
# define S_KEY 115
# define D_KEY 100

# define COLOR_RED 0xFF0000
# define COLOR_GREEN 0x00FF00
# define COLOR_BLUE 0x0000FF
# define COLOR_WHITE 0xFFFFFF
# define COLOR_BLACK 0x000000
# define COLOR_GREY 0x808080

# define SCALE_FACTOR 0.2

typedef struct s_horz
{
	double	x_intercept;
	double	y_intercept;
	double	step_x;
	double	step_y;
	double	next_x;
	double	next_y;
	bool	found;
}			t_horz;

typedef struct s_vert
{
	double	x_intercept;
	double	y_intercept;
	double	step_x;
	double	step_y;
	double	next_x;
	double	next_y;
	bool	found;
}			t_vert;

typedef struct s_img
{
	void	*img_ptr;
	char	*img_pixel_ptr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_ray
{
	double	wall_hit_x;
	double	wall_hit_y;
	double	distance;
	int		was_hit_vertical;
	double	ray_angle;
	bool	is_facing_up;
	bool	is_facing_down;
	bool	is_facing_right;
	bool	is_facing_left;
}			t_ray;

typedef struct s_player
{
	double	px;
	double	py;
	int		radius;
	int		turn_dir;
	int		walk_dir;
	double	rotation_angle;
	double	move_speed;
	double	rotation_speed;
	int		strafe_dir;
}			t_player;

typedef struct s_game
{
	void			*mlx_connection;
	void			*win_window;
	char			**map;
	double			distance_to_pl;
	int				width;
	int				height;
	int				map_rows;
	int				map_cols;
	double			fov_angle;
	int				tile_size;
	struct s_ray	*rays;
	struct s_player	player;
	struct s_img	img;
	struct g_info	*data;
}					t_game;

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
	char			*hexa_color;
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
	int				map_length;

}					t_info;

/* parser_utils */
int					parser(int argc, char *file, t_info *data);
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
// void exit_error(int status, char *message, t_info *data);

/* parser_utils_three */
int					is_valid_name_file(char *argv);
void				exit_error(int status, char *message, t_info *data);
void				read_map(int fd, t_info *data, int total_map_lines,
						char *line);
void				replace_char_in_array(char **arr, char tar, char rep);
char				*skip_empty_lines(int fd, char *line);

// game functions 

bool				find_vert_wall_hit(t_game *game, t_ray *ray, t_vert *v);
void				init_vert_ray(t_game *game, t_player *player,
						t_ray *ray, t_vert *v);
void				init_horz_ray(t_game *game, t_player *player,
						t_ray *ray, t_horz *h);
bool				find_horz_wall_hit(t_game *game, t_ray *ray, t_horz *h);

void				my_mlx_pixel_put(t_game *data, int x, int y, int color);
double				normalize_angle(double angle);
int					has_wall_at(t_game *game, int x, int y);
double				_2points_dist(double x0, double y0, double x1, double y1);
int					handle_keypress(int keycode, t_game *game);
int					handle_keyrelease(int keycode, t_game *game);
int					draw(t_game *game);
void				get_map(t_game *game);

void				raycasting(t_game *game, t_player *player);
void				init_ray(t_ray *ray, double ray_angle);

void				cleanup_and_exit(t_game *game, int exit_code);

void				update(t_game *game, t_player *player);
void				init_player(t_game *game, t_player *player);
void				init_game(t_game *game);
void				init_mlx(t_game *game);
#endif
