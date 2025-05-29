#ifndef SPY_HPP
#define SPY_HPP

#include "Game.hpp"
#include "Player.hpp"

namespace coup{
	
	class Spy: public Player{
		public:
			Spy(Game& game, string name);

			~Spy();
			//Spy has a turn ability. Returns true.
			bool can_use_turn_ability() const override;
			//Spy has a realtime ability: When one Player wants to arrest another, Spy can undo the arrest.
			//Prevention logic occurs at Game.cpp at prevent_action
			//returns true
			bool can_use_realtime_ability() const override;
			//Spy's turn ability: They can spy on a target player and see how much coins the target has.
			//Returns the number of coins the Player* player has
			//Throws exepctions in case of the player being nullptr or this
			size_t turn_ability(const Player* player) override;
	};

};

#endif
