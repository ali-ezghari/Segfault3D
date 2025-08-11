#include "../includes/cub.h"

void init_player(t_game *game, t_player *player)
{
    player->px = game->width / 2;
    player->py = game->height / 2;
    player->radius = 5 * SCALE_FACTOR;
    player->moveSpeed = 2.00;
    player->rotationAngle = 0;
    player->turnDirection = 0;
    player->walkDirection = 0;

    player->rotationAngle = PI / 2;
    player->rotationSpeed = 4 * (PI / 180);
}

void init_game(t_game *game)
{
    game->mapRows = 11;
    game->mapCols = 15;
    game->mapSize = game->mapRows * game->mapCols;
    game->tile_size = 64;
    game->width = game->tile_size * game->mapCols;
    game->height = game->tile_size * game->mapRows;

    game->mlx_connection = NULL;
    game->win_window = NULL;
    init_player(game, &game->player);
}

void init_mlx(t_game *game)
{
    game->mlx_connection = mlx_init();
    if (!game->mlx_connection)
        exit(1);
    game->win_window = mlx_new_window(game->mlx_connection, game->width, game->height, "cub3d");
    if (!game->win_window)
    {
        mlx_destroy_display(game->mlx_connection);
        free(game->mlx_connection);
        exit(1);
    }
    game->img.img_ptr = mlx_new_image(game->mlx_connection, game->width, game->height);
    if (!game->img.img_ptr)
    {
        mlx_destroy_window(game->mlx_connection, game->win_window);
        mlx_destroy_display(game->mlx_connection);
        free(game->mlx_connection);
        exit(1);
    }
    game->img.img_pixel_ptr = mlx_get_data_addr(game->img.img_ptr, &game->img.bits_per_pixel, &game->img.line_length, &game->img.endian);
    if (!game->img.img_pixel_ptr)
    {
        mlx_destroy_image(game->mlx_connection, game->img.img_ptr);
        mlx_destroy_window(game->mlx_connection, game->win_window);
        mlx_destroy_display(game->mlx_connection);
        free(game->mlx_connection);
        exit(1);
    }
}