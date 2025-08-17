#ifndef CUB_H
#define CUB_H

#include <string.h>
#include "../minilibx-linux/mlx.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <X11/X.h>
#include <stdbool.h>
#include <limits.h>

#define PI 3.14159265359

#define ESC_KEY 65307
#define RIGHT_ARROW 65363
#define LEFT_ARROW 65361
#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100

#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF
#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_GREY 0x808080

#define FOV_ANGLE 60 * (PI / 180)
#define SCALE_FACTOR 0.2

typedef struct s_horz {
    double x_intercept;
    double y_intercept;
    double step_x;
    double step_y;
    double next_x;
    double next_y;
    bool found;
} t_horz;

typedef struct s_vert {
    double x_intercept;
    double y_intercept;
    double step_x;
    double step_y;
    double next_x;
    double next_y;
    bool found;
} t_vert;

typedef struct s_img
{
    void *img_ptr;
    char *img_pixel_ptr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_img;

typedef struct s_ray
{
    double wall_hit_x;
    double wall_hit_y;
    double distance;
    int was_hit_vertical;
    double ray_angle;

    bool is_ray_facing_up;
    bool is_ray_facing_down;
    bool is_ray_facing_right;
    bool is_ray_facing_left;

} t_ray;

typedef struct s_player
{
    double px;         // player x
    double py;         // player y
    int radius;        // player circle raduis
    int turnDirection; // 1 if looking right and -1 if looking left
    int walkDirection; // 1 if going upwards , -1 if going downwards

    double rotationAngle; //
    double moveSpeed;     // pixels per player step
    double rotationSpeed; // how many degrees the player will rotates per key press(per frame) the player

    int strafedirection; // 1 if going right, -1 if going left
} t_player;

typedef struct s_game
{
    void *mlx_connection;
    void *win_window;
    char **map;
    int width;
    int height;
    int mapRows;
    int mapCols;
    int mapSize;
    int tile_size;
    struct s_ray *rays;
    struct s_player player;
    struct s_img img;
} t_game;

void my_mlx_pixel_put(t_game *data, int x, int y, int color);
double normalizeAngle(double angle);
int has_wall_at(t_game *game, int x, int y);
double distance_bet_points(double x0, double y0, double x1, double y1);
int handle_keypress(int keycode, t_game *game);
int handle_keyrelease(int keycode, t_game *game);
void draw(t_game *game);


void raycasting(t_game *game, t_player *player);
void draw_line(t_game *game, int x0, int y0, int x1, int y1, int color);
void init_ray(t_ray *ray, double ray_angle);

void cleanup_and_exit(t_game *game, int exit_code);

void draw(t_game *game);
void draw_rec(t_game *game, int x, int y, int size, int color);
void draw_player(t_game *game, t_player *player, int color);

void update(t_game *game, t_player *player);

void init_player(t_game *game, t_player *player);
void init_game(t_game *game);
void init_mlx(t_game *game);


#endif
