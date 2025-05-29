#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"
#include "Game.hpp"

namespace coup{

	class Judge: public Player{
		public:
			Judge(Game& game, string name);
			//Important! Since the Game class doesn't allow for duplicate names, this copy constructor doesn't add the class to the game			
			Judge(const Judge& copy);
			~Judge();
			//The judge can prevent a player from using the Bribe action.
			//Returns true
			bool can_use_realtime_ability() const override;
	};
};

#endif
