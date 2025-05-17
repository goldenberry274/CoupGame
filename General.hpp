#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"
#include "Game.hpp"

namespace coup{
	class General: public Player{
		General(Game& game, string name);
		~General();
		void when_arrested() override;
	};
};

#endif
