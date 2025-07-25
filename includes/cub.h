#ifndef CUB_H
#define CUB_H

#include <string.h>
#include "../minilibx/mlx.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <X11/X.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ESC_KEY 65307
#define RIGHT_ARROW 65363
#define LEFT_ARROW 65361
#define UP_ARROW 65362
#define DOWN_ARROW 65364

#define WIDTH 500
#define HEIGHT 500
#define PLAYER_SIZE 15
#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF
#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_GREY 0x808080

typedef struct s_img
{
    void *img_ptr;
    char *img_pixel_ptr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_img;

typedef struct s_player
{
    double px;
    double py;
} t_player;

typedef struct s_game
{
    void *mlx_connection;
    void *win_window;
    int *map;
    int mapX;
    int mapY;
    int mapS;
    struct s_player player;
    struct s_img img;
} t_game;

#endif