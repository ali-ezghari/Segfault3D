#include "../includes/cub.h"

void get_map(t_game *game)
{
    int rows;
    int i;

    rows = game->mapRows;
    i = -1;
    game->map = malloc(sizeof(char *) * (rows + 1));
    if (!game->map)
        cleanup_and_exit(game, 1);// ! change to free everything
    while (++i < rows)
        game->map[i] = strdup(game->data->map[i]);
    game->map[rows] = NULL;
}

void init_ray(t_ray *ray, double ray_angle)
{
    ray->ray_angle = ray_angle;
    ray->wall_hit_x = 0;
    ray->wall_hit_y = 0;
    ray->distance = 0;

    ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle < PI;
    ray->is_ray_facing_up = !ray->is_ray_facing_down;
    ray->is_ray_facing_right = ray->ray_angle < PI * 0.5 || ray->ray_angle > PI * 1.5;
    ray->is_ray_facing_left = !ray->is_ray_facing_right;
    ray->was_hit_vertical = false;
}

static void init_player_rotation(t_player *player, char spawn_dir)
{
    if (spawn_dir == 'S')
        player->rotationAngle = PI / 2;
    else if (spawn_dir == 'N')
        player->rotationAngle = 3 * PI / 2;
    else if (spawn_dir == 'W')
        player->rotationAngle = PI;
    else if (spawn_dir == 'E')
        player->rotationAngle = 0;
}

void init_player(t_game *game, t_player *player)
{
    int x;
    int y;

    x = game->data->s_dir.x;
    y = game->data->s_dir.y;
    player->radius = 6;
    player->moveSpeed = 5.00;
    player->turnDirection = 0;
    player->walkDirection = 0;
    player->strafedirection = 0;

    player->rotationSpeed = 4 * (PI / 180);

    player->px = x * game->tile_size + game->tile_size / 2;
    player->py = y * game->tile_size + game->tile_size / 2;
    init_player_rotation(player, game->map[y][x]);
    game->map[y][x] = '0';
}

void init_game(t_game *game)
{
    game->mapRows = game->data->map_lenght;
    game->mapCols = game->data->map_width;
    game->tile_size = 40;
    game->width = game->tile_size * game->mapCols;
    game->height = game->tile_size * game->mapRows;

    get_map(game);

    game->mlx_connection = NULL;
    game->win_window = NULL;
    game->rays = NULL;
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
    if (game->rays)
        free(game->rays);
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