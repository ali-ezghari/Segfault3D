#include "../includes/cub.h"

void render_rays(t_game *game, t_player *player)
{
    int i;
    t_ray ray;

    i = 0;
    while (i < game->width)
    {
        ray = game->rays[i];
        draw_line(game, player->px * SCALE_FACTOR, player->py * SCALE_FACTOR, ray.wall_hit_x * SCALE_FACTOR, ray.wall_hit_y * SCALE_FACTOR, COLOR_RED);
        i++;
    }
}

void update(t_game *game, t_player *player)
{
    double new_player_x;
    double new_player_y;
    double move_step;

    //? rotating the player based on keypress right(2px to the right) left(2px to the left)
    player->rotationAngle += player->turnDirection * player->rotationSpeed;

    // ? move the player forward or backwards based on the walkDirection
    move_step = player->walkDirection * player->moveSpeed;

    new_player_x = player->px + cos(player->rotationAngle) * move_step;
    new_player_y = player->py + sin(player->rotationAngle) * move_step;

    //? prevent the player from going through walls

    if (player->strafedirection != 0)
    {
        new_player_x += cos(player->rotationAngle + (PI / 2)) * player->strafedirection * player->moveSpeed;
        new_player_y += sin(player->rotationAngle + (PI / 2)) * player->strafedirection * player->moveSpeed;
    }
    if (!has_wall_at(game, new_player_x + 2, new_player_y + 2) &&
        !has_wall_at(game, new_player_x - 2, new_player_y - 2) &&
        !has_wall_at(game, new_player_x + 2, new_player_y - 2) &&
        !has_wall_at(game, new_player_x - 2, new_player_y + 2))
    {
        player->px = new_player_x;
        player->py = new_player_y;
    }
}

static void ft_clear(t_img *img, int width, int height)
{
    int total_pixels;
    int i;
    int *pixels;

    pixels = (int *)img->img_pixel_ptr;
    total_pixels = width * height;

    i = 0;
    while (i < total_pixels / 2)
        pixels[i++] = COLOR_GREEN;
    while (i < total_pixels)
        pixels[i++] = COLOR_GREY;
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
    double distance_to_pl;
    double proj_wall_height;
    double wall_height;
    int y_start;
    double corrected_distance;
    int i;

    i = 0;
    while (i < game->width) // game->width => Num of rays
    {
        corrected_distance = game->rays[i].distance * cos(game->rays[i].ray_angle - game->player.rotationAngle);

        distance_to_pl = (game->width / 2) / tan(FOV_ANGLE / 2);
        proj_wall_height = (game->tile_size / corrected_distance) * distance_to_pl;

        wall_height = (int)proj_wall_height;

        y_start = (game->height / 2) - (proj_wall_height / 2);

        if (y_start < 0)
            y_start = 0;
        wall_height = ((wall_height + y_start) > game->height) ? game->height - y_start : wall_height;
        if (wall_height <= 0)
            continue;
        rec(game, i, y_start, 1, wall_height, COLOR_WHITE);
        i++;
    }
}

void render_2d_map(t_game *game)
{
    int x;
    int y;
    int tile_color;
    int tile_x, tile_y;

    x = 0;
    while (x < game->mapRows)
    {
        y = 0;
        while (y < game->mapCols)
        {
            tile_x = y * game->tile_size;
            tile_y = x * game->tile_size;
            if (game->map[x][y] == '1')
                tile_color = COLOR_WHITE;
            else
                tile_color = COLOR_BLACK;
            draw_rec(game, tile_x, tile_y, game->tile_size, tile_color);
            y++;
        }
        x++;
    }
}

void draw(t_game *game)
{
    ft_clear(&game->img, game->width, game->height);
    update(game, &game->player);
    raycasting(game, &game->player);
    render_3d_walls(game);
    render_2d_map(game);
    render_rays(game, &game->player);
    draw_player(game, &game->player, COLOR_RED);
    mlx_put_image_to_window(game->mlx_connection, game->win_window, game->img.img_ptr, 0, 0);
}
