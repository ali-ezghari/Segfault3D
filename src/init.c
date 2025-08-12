#include "../includes/cub.h"

void init_ray(t_ray *ray, double ray_angle)
{
    ray->ray_angle = ray_angle;
    ray->wall_hit_x = 0;
    ray->wall_hit_y = 0;
    ray->distance = 0;

    ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle < PI;
    ray->is_ray_facing_up = !ray->is_ray_facing_down;
    ray->is_ray_facing_right = ray->ray_angle < PI * 0.5 || ray->ray_angle > PI * 1.5;
    ray->was_hit_vertical = false;
    ray->is_ray_facing_left = !ray->is_ray_facing_right;
}

void init_player(t_game *game, t_player *player)
{
    player->px = game->width / 2;
    player->py = game->height / 2;
    player->radius = 5;
    player->moveSpeed = 2.00;
    player->rotationAngle = 0;
    player->turnDirection = 0;
    player->walkDirection = 0;

    player->rotationAngle = PI / 2;
    player->rotationSpeed = 4 * (PI / 180);
}

void init_game(t_game *game)
{
    game->mapRows = 15;
    game->mapCols = 15;
    game->tile_size = 64;
    game->width = game->tile_size * game->mapCols;
    game->height = game->tile_size * game->mapRows;

    game->mlx_connection = NULL;
    game->win_window = NULL;
}

void cleanup_and_exit(t_game *game, int exit_code)
{
    if (game->img.img_ptr)
        mlx_destroy_image(game->mlx_connection, game->img.img_ptr);
    if (game->win_window)
        mlx_destroy_window(game->mlx_connection, game->win_window);
    if (game->mlx_connection)
        mlx_destroy_display(game->mlx_connection);
    if (game->mlx_connection)
        free(game->mlx_connection);
    exit(exit_code);
}

void init_mlx(t_game *game)
{
    game->mlx_connection = mlx_init();
    if (!game->mlx_connection)
        cleanup_and_exit(game, 1);

    game->win_window = mlx_new_window(game->mlx_connection, game->width, game->height, "cub3d");
    if (!game->win_window)
        cleanup_and_exit(game, 1);

    game->img.img_ptr = mlx_new_image(game->mlx_connection, game->width, game->height);
    if (!game->img.img_ptr)
        cleanup_and_exit(game, 1);

    game->img.img_pixel_ptr = mlx_get_data_addr(
        game->img.img_ptr,
        &game->img.bits_per_pixel,
        &game->img.line_length,
        &game->img.endian);
    if (!game->img.img_pixel_ptr)
        cleanup_and_exit(game, 1);
}