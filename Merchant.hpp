#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "Player.hpp"
#include "Game.hpp"
namespace coup{
	
	class Merchant: public Player{
		Merchant(Game& game, string name);
		~Merchant();
		void turn_cycle_start() override;
	};

};

#endif
