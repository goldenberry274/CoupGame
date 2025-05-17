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
			//Braon ability: Invest 3 coins to earn 6
			void role_ability() override;
			bool can_use_ability() const override;

			bool when_sanctioned(Player* player);
	};

}

#endif
