#include <iostream>

#include "Game.h"

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usáid: " << argv[0] 
			<< " #imreoir(1 or 2) #liathróid(1-∞) " << std::endl;
		return -1;
	}
	int np = std::stoi(std::string(argv[1]));
	if ( np != 1 && np != 2 ) return -1;
	int nb = std::stoi(std::string(argv[2]));
	if ( nb < 1 ) return -1;
	std::cout << "Ag tosnú PONG le " << np << " imreoir agus "
		<< nb << " liathróid." << std::endl;
	Game game(np, nb);
	bool success = game.Initialise();
	if(success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}

