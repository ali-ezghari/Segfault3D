#include "../includes/cub.h"

void update(t_game *game, t_player *player)
{
    //? rotating the player based on keypress right(2px to the right) left(2px to the left)
    player->rotationAngle += player->turnDirection * player->rotationSpeed;

    // ? move the player forward or backwards based on the walkDirection
    double moveStep = player->walkDirection * player->moveSpeed;

    double newPlayerX = player->px + cos(player->rotationAngle) * moveStep;
    double newPlayerY = player->py + sin(player->rotationAngle) * moveStep;

    //? prevent the player from going through walls
    if (!hasWallAt(game, player, newPlayerX, newPlayerY))
    {
        player->px = newPlayerX;
        player->py = newPlayerY;
    }
}

static void ft_clear(t_img *img, int width, int height, int color)
{
    int total_pixels;
    int i;
    int *pixels;

    pixels = (int *)img->img_pixel_ptr;
    total_pixels = width * height;

    i = 0;
    while (i < total_pixels)
        pixels[i++] = color;
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
void rec(t_game *game, int x, int y, int width, int height, int color)
{
    int i;
    int j;

    i = 0;
    while (i < width)
    {
        j = 0;
        while (j < height)
        {
            my_mlx_pixel_put(game, x + i, y + j, color);
            j++;
        }
        i++;
    }
}

void render_3d_walls(t_game *game)
{
    double fov;
    double distance_to_pl;
    double proj_wall_height;
    double wall_height;
    int y_start;

    fov = 60.0 * (PI / 180);
    for (int i = 0; i < game->width; i++) // game->width => Num of rays
    {
        double correct_distance = game->rays[i].distance * cos(game->rays[i].ray_angle - game->player.rotationAngle);

        distance_to_pl = (game->width / 2) / tan(fov / 2);
        proj_wall_height = (game->tile_size / correct_distance) * distance_to_pl;

        wall_height = (int)proj_wall_height;

        y_start = (game->height / 2) - (proj_wall_height / 2);
        if (y_start < 0)
            y_start = 0;
        wall_height = ((wall_height + y_start) > game->height) ? game->height - y_start : wall_height;
        if (wall_height <= 0)
            continue;
        rec(game, i, y_start, 1, wall_height, COLOR_WHITE);
    }
}

void draw(t_game *game)
{
    int x;
    int y;
    int tile_color;
    int tile_x, tile_y;

    x = 0;
    ft_clear(&game->img, game->width, game->height, COLOR_GREY);
    update(game, &game->player);
    raycasting(game, &game->player);

    render_3d_walls(game);
    while (x < game->mapRows)
    {
        y = 0;
        while (y < game->mapCols)
        {
            tile_x = y * game->tile_size;
            tile_y = x * game->tile_size;
            if (map[x][y] == 1)
                tile_color = COLOR_WHITE;
            else
                tile_color = COLOR_BLACK;
            draw_rec(game, tile_x, tile_y, game->tile_size, tile_color);
            y++;
        }
        x++;
    }

    draw_player(game, &game->player, COLOR_RED);
    mlx_put_image_to_window(game->mlx_connection, game->win_window, game->img.img_ptr, 0, 0);
}
