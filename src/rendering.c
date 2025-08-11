#include "../includes/cub.h"

void    ft_clear(t_img *img, int width, int height, int color)
{
    int     total_pixels;
    int     i;
    int    *pixels;

    pixels = (int *)img->img_pixel_ptr;
    total_pixels = width * height;

    i = 0;
    while (i < total_pixels)
        pixels[i++] = color;
}


static void rec(t_game *game, int x, int y, int width, int height, int color)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            my_mlx_pixel_put(game, x + i, y + j, color);
        }
    }
}
void render_3d_walls(t_game *game)
{
    double fov;
    double distance_to_pl;
    double proj_wall_height;

    fov = 60.0 * (PI / 180);
    for (int i = 0; i < game->width ; i++) // game->width => Num of rays
    {
        double correct_distance = game->rays[i].distance * cos(game->rays[i].ray_angle - game->player.rotationAngle);
        distance_to_pl = (game->width / 2) / tan(fov / 2);
        proj_wall_height = (game->tile_size / correct_distance) * distance_to_pl;
        rec(game, i, (game->height / 2) - (proj_wall_height / 2), 1, proj_wall_height, COLOR_WHITE);
    }
}
void draw_rec(t_game *game, int x, int y, int size, int color)
{
    int scaled_x;
    int scaled_y;
    int scaled_size;
    int i;
    int j;

    i = 0;
    j = 0;
    scaled_x = (int)(x * SCALE_FACTOR);
    scaled_y = (int)(y * SCALE_FACTOR);
    scaled_size = (int)(size * SCALE_FACTOR);

    while (i <= scaled_size)
    {
        j = 0;
        while (j <= scaled_size)
        {
            if (i == 0 || i == scaled_size || j == 0 || j == scaled_size)
                my_mlx_pixel_put(game, scaled_x + i, scaled_y + j, COLOR_BLACK);
            else
                my_mlx_pixel_put(game, scaled_x + i, scaled_y + j, color);
            j++;
        }
        i++;
    }
}

void draw_player(t_game *game, t_player *player, int color)
{
    int y;
    int x;
    int scaled_x;
    int scaled_y;

    update(game, &game->player);
    y = -player->radius;
    while (y <= player->radius)
    {
        x = -player->radius;
        while (x <= player->radius)
        {
            if (x * x + y * y <= player->radius * player->radius)
            {
                scaled_x = SCALE_FACTOR * (player->px + x);
                scaled_y = SCALE_FACTOR * (player->py + y);
                my_mlx_pixel_put(game, scaled_x, scaled_y, color);
            }
            x++;
        }
        y++;
    }
}

void draw(t_game *game)
{
    int x;
    int y;

    x = 0;
    ft_clear(&game->img, game->width, game->height, COLOR_GREY);
    update(game, &game->player);
    render_3d_walls(game);
    while (x < game->mapRows)
    {
        y = 0;
        while (y < game->mapCols)
        {
            int tile_x = y * game->tile_size;
            int tile_y = x * game->tile_size;
            int tile_color = map[x][y] == 1 ? COLOR_WHITE : COLOR_BLACK;
            draw_rec(game, tile_x, tile_y, game->tile_size, tile_color);
            y++;
        }
        x++;
    }
    draw_player(game, &game->player, COLOR_RED);
    mlx_put_image_to_window(game->mlx_connection, game->win_window, game->img.img_ptr, 0, 0);
}
