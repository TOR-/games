#include <iostream>

#include "Game.h"

int main(int argc, char ** argv)
{
	if (2 != argc)
	{
		std::cerr << "Usage: " << argv[0] << " asset_folder_path" << std::endl;
		return 1;
	}
	Game game(argv[1]);
	bool success = game.Initialise();
	if(success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}

