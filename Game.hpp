#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>

#include "Player.hpp"

using namespace std;

namespace coup{

	class Game{

        protected:
        	vector<Player*> game_players;
        	size_t curr_turn;
	    	string game_winner;				
		const size_t player_amount;

        public:
            	Game(size_t new_player_amount = 3);
        	~Game();

        	//General getters
       		vector<string> players();
      		string turn();	
		string winner();

		//Adding names
		void add_player(Player* add_player);
		void update_turn();
			
		//All these actions will perform actions on players. First, a check will be performed for the pa			rameters of the players. (coin number, sanctioned or not, etc.) and only after verifying that an 			action is possible it will occur
			
		void gather(Player* player);
		void tax(Player* player);
		void bribe(Player* player);
		//If the arrested has 0 coins, the arrester's turn will still get used up and the game will continue. 
		void arrest(Player* arrester, Player* arrested);
		void sanction(Player* sanctioner, Player* sanctioned);
		void coup(Player* does_coup, Player* gets_couped);

			
	};
	
};

#endif
