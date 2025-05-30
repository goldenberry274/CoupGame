#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "Player.hpp"
#include "Game.hpp"
namespace coup{
	
	class Merchant: public Player{
		public:
			Merchant(Game& game, string name);
			Merchant(const Merchant& copy);
			Merchant(const Player& copy);
			~Merchant();
			//If the Merchant has more than 3 coins, they get 1 coin for free at the start of their turn
			void turn_cycle_start() override;
			//When the Merchant is arrested, they lost 2 coins, but don't give any to the arrester
			void when_arrested() override;
			//if arrested = true: Pays back the 2 coins the Merchant lost
			//if arrested = false: Subtracts the coins they took from the other player
			void undo_arrest(bool arrested, Player* target) override;
	};

};

#endif
