#include "../includes/cub.h"


int main(int argc, char *argv[])
{
    t_info *data;
    struct s_game game;

    data = malloc(sizeof(t_info));
    if (!data)
        return (1);
    ft_memset(data, 0, sizeof(t_info));
    parser(argc, argv[1], data);

    init_game(&game);
    init_mlx(&game);
    init_player(&game, &game.player);
    draw(&game);
    mlx_hook(game.win_window, 2, 1L << 0, handle_keypress, &game);
    mlx_hook(game.win_window, 3, 1L << 1, handle_keyrelease, &game);

    mlx_loop(game.mlx_connection);
    return (0);
}