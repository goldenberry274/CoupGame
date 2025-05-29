#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"
#include "Game.hpp"

namespace coup{
	class General: public Player{
		public:
			General(Game& game, string name);
			~General();
			//The General can undo a coup at the cost of 5 coins
			//Returns true if they have enough (More than 5) and false otherwise
			bool can_use_realtime_ability() const override;
			//Subtracts 5 coins from the amount of coins they have
			void realtime_ability() override;
			//The General doesn't lose any coins when they are arrested. This method gets overriden to do nothing
			void when_arrested() override;
			//If the General is arrested, they don't lose any coins. Thus if they arrest is undoe, they don't get any either
			void undo_arrest(bool arrested, Player* target) override;
	};
};

#endif
