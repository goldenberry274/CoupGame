#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"
#include "Game.hpp"

namespace coup{

	class Judge: public Player{
		Judge(Game& game, string name);
		~Judge();
	};
};

#endif
