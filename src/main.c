#include "../includes/cub.h"


int main()
{

    struct s_game game;
    // loading
    init_game(&game);
    init_mlx(&game);
    init_player(&game, &game.player);

    draw(&game);
    mlx_hook(game.win_window, 2, 1L << 0, handle_keypress, &game);
    mlx_hook(game.win_window, 3, 1L << 1, handle_keyrelease, &game);

    mlx_loop(game.mlx_connection);
    return 0;
}