#include "../includes/cub.h"

static void init_horz_ray(t_game *game, t_player *player, t_ray *ray, t_horz *h)
{
    h->y_intercept = floor(player->py / game->tile_size) * game->tile_size;
    if (ray->is_ray_facing_down)
        h->y_intercept += game->tile_size;
    h->x_intercept = player->px + (h->y_intercept - player->py) / tan(ray->ray_angle);
    h->step_y = game->tile_size;
    if (ray->is_ray_facing_up)
        h->step_y *= -1;
    h->step_x = game->tile_size / tan(ray->ray_angle);
    if (ray->is_ray_facing_left && h->step_x > 0)
        h->step_x *= -1;
    if (ray->is_ray_facing_right && h->step_x < 0)
        h->step_x *= -1;
}

static bool find_horz_wall_hit(t_game *game,
                               t_ray *ray, t_horz *h)
{
    h->next_x = h->x_intercept;
    h->next_y = h->y_intercept;
    while (h->next_x >= 0 && h->next_x <= game->width && h->next_y >= 0 && h->next_y <= game->height)
    {
        if (has_wall_at(game, h->next_x, h->next_y - ray->is_ray_facing_up))
        {
            h->found = true;
            return (true);
        }
        h->next_x += h->step_x;
        h->next_y += h->step_y;
    }
    return (false);
}

static void init_vert_ray(t_game *game, t_player *player, t_ray *ray, t_vert *v)
{
    v->x_intercept = floor(player->px / game->tile_size) * game->tile_size;
    if (ray->is_ray_facing_right)
        v->x_intercept += game->tile_size;
    v->y_intercept = player->py + (v->x_intercept - player->px) * tan(ray->ray_angle);
    v->step_x = game->tile_size;
    v->step_x *= ray->is_ray_facing_left ? -1 : 1;
    v->step_y = game->tile_size * tan(ray->ray_angle);
    v->step_y *= (ray->is_ray_facing_up && v->step_y > 0) ? -1 : 1;
    v->step_y *= (ray->is_ray_facing_down && v->step_y < 0) ? -1 : 1;
}

static bool find_vert_wall_hit(t_game *game,
                               t_ray *ray, t_vert *v)
{
    v->next_x = v->x_intercept;
    v->next_y = v->y_intercept;
    while (v->next_x >= 0 && v->next_x <= game->width && v->next_y >= 0 && v->next_y <= game->height)
    {
        if (has_wall_at(game, v->next_x - ray->is_ray_facing_left, v->next_y))
        {
            v->found = true;
            return (true);
        }
        v->next_x += v->step_x;
        v->next_y += v->step_y;
    }
    return (false);
}

static void set_ray_result(t_ray *ray, t_horz *h, t_vert *v, t_player *player)
{
    double horz_dist;
    double vert_dist;

    if (h->found)
        horz_dist = distance_bet_points(player->px, player->py, h->next_x, h->next_y);
    else
        horz_dist = INT_MAX;

    if (v->found)
        vert_dist = distance_bet_points(player->px, player->py, v->next_x, v->next_y);
    else
        vert_dist = INT_MAX;
    if (horz_dist < vert_dist)
    {
        ray->wall_hit_x = h->next_x;
        ray->wall_hit_y = h->next_y;
        ray->distance = horz_dist;
        ray->was_hit_vertical = false;
    }
    else
    {
        ray->wall_hit_x = v->next_x;
        ray->wall_hit_y = v->next_y;
        ray->distance = vert_dist;
        ray->was_hit_vertical = true;
    }
}

static void cast_ray(t_game *game, t_player *player, t_ray *ray)
{
    t_horz h = {0};
    t_vert v = {0};

    init_horz_ray(game, player, ray, &h);
    h.found = find_horz_wall_hit(game, ray, &h);
    init_vert_ray(game, player, ray, &v);
    v.found = find_vert_wall_hit(game, ray, &v);
    set_ray_result(ray, &h, &v, player);
}

void raycasting(t_game *game, t_player *player)
{
    double ray_angle;
    int num_rays;
    int i;

    num_rays = game->width;
    ray_angle = player->rotationAngle - (FOV_ANGLE / 2);
    i = 0;
    game->rays = malloc(sizeof(t_ray) * num_rays);
    if (!game->rays)
        cleanup_and_exit(game, EXIT_FAILURE);
    while (i < num_rays)
    {
        init_ray(&game->rays[i], normalizeAngle(ray_angle));
        cast_ray(game, player, &game->rays[i]);
        ray_angle += FOV_ANGLE / num_rays;
        i++;
    }
}