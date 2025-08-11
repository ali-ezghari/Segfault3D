#include "../includes/cub.h"

int map[11][15] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};


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
	cast_all_rays(game, &game->player);
}

int main()
{

	struct s_game game;
	// game
	init_game(&game);
	init_mlx(&game);

	draw(&game);
	mlx_hook(game.win_window, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.win_window, 3, 1L << 1, handle_keyrelease, &game);

	mlx_loop(game.mlx_connection);

	return (0);
}
