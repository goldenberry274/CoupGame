#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"
#include "Game.hpp"

namespace coup{

	class Judge: public Player{
		public:
			Judge(Game& game, string name);
			Judge(const Judge& copy);
			Judge(const Player& copy);
			~Judge();
			//The judge can prevent a player from using the Bribe action.
			//Returns true
			bool can_use_realtime_ability() const override;
	};
};

#endif
