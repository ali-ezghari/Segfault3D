#ifndef CUB_H
#define CUB_H

int map[11][15] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

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

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ESC_KEY 65307
#define RIGHT_ARROW 65363
#define LEFT_ARROW 65361
#define UP_ARROW 65362
#define DOWN_ARROW 65364

// #define WIDTH 600
// #define HEIGHT 600
#define PLAYER_SIZE 15
#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF
#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_GREY 0x808080

#define SCALE_FACTOR 0.2
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
} t_player;

typedef struct s_game
{
    void *mlx_connection;
    void *win_window;
    int *map;
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

#endif
