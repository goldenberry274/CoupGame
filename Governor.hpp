#ifndef GOVERNOR_HPP
#define GEVERNOR_HPP

#include <string>
#include <iostream>
#include "Player.hpp"
#include "Game.hpp"

using namespace std;

namespace coup{

	class Governor: public Player{
		public:
			Governor(Game& game, string name);
			~Governor();

			void tax() override;
			//Add later: accepting user input. Return true if the player wants to cancel tax and false if not
			bool undo_tax(Player* player);	
	
	};

};

#endif
