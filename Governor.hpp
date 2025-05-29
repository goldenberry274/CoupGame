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
			//Important! Since the Game class doesn't allow for duplicate names, this copy constructor doesn't add the class to the game
			Governor(const Governor& copy);
			~Governor();
			//The Governor's passive ability is that they gain 3 coins instead of 2 when taxing
			//Adds 3 to the coin number
			void tax() override;
			//The Governor can prevent a player from using Tax.
			//Returns true
			bool can_use_realtime_ability() const override;
			//Since the governor gets a different amount of coins from a player when taxing, they must also 
			//Subtracts 3 from the coin number
			void undo_tax() override;
	
	};

};

#endif
