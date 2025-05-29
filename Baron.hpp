#ifndef BARON_HPP
#define BARON_HPP

#include <string>

#include "Player.hpp"
#include "Game.hpp"

using namespace std;

namespace coup{
	class Baron: public Player{
		public:
			Baron(Game& game, string name);
			~Baron();
			//Subtracts 3 coins and adds 6
			void turn_ability() override;
			//Returns true if the Baron has more than 3 coins
			bool can_use_turn_ability() const override;
			//When sanctioned, the Baron gets 1 coin as conpenscation
			void when_sanctioned() override;
	};

}

#endif
