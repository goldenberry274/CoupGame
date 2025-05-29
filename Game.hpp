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
			Player* current_player;
        	size_t curr_turn;
	    	string game_winner;				
			const size_t player_amount;

        public:
            Game(size_t new_player_amount = 3);
        	~Game();

        	//General getters

			//Returns the names of all the players in a vector
       		vector<string> players();
			//Returns the pointers to all the players in a vector
			vector<Player*> player_vector() const;
			//Returns the player whose turn it is
      		string turn();	
			//Returns the name of the winner in a string formar 
			string winner();
			//Returns true if there is only one player left
			bool is_there_a_winner(); 
			//Important! The assumption here is that the game is full of players
			Player* get_current_player();
			
			//Adding names
			void add_player(Player* add_player);
			//Updating the turn
			void update_turn();
			
			//All these actions will perform actions on players. First, a check will be performed for the parameters of the players. (coin number, sanctioned or not, etc.) and only after verifying that an action is possible it will occur
			
			//Call the gather() method within the player (See Player.hpp)
			//Throws excpetions if: player is nullptr or player isn't the player whose turn it is or the player is sanctioned or player has to perform a coup (see must_coup at Player.hpp)
			void gather(Player* player); 
			//Call the tax() method within the player (See Player.hpp)
			//Throws excpetions if: player is nullptr or player isn't the player whose turn it is or the player is sanctioned or player has to perform a coup (see must_coup at Player.hpp)
			void tax(Player* player);
			//Call the bribe() method within the player (See Player.hpp)
			//Throws excpetions if: player is nullptr or player isn't the player whose turn it is or the player doesn't have enough coins to bribe or player has to perform a coup (see must_coup at Player.hpp)
			void bribe(Player* player); 
			//Call the arrest(const Player* arrested) method within the arrester and the when_arrested() method within the arrested. (See Player.hpp)
			//Throws exceptions if: arrester or arrested are nullptr, arrester and arrested are the same player,
			//arrester isn't the player whose turn it is or arrester has to perform a coup (see must_coup at Player.hpp)
			void arrest(Player* arrester, Player* arrested); 
			//Call the sanctoin(const Player* sanctoined) method within the sanctioner and the when_sanctioned() method within the sanctioned. (See Player.hpp)
			//Throws exceptions if: sanctioner or sanctoined are nullptr, sanctioner and sanctoined are the same player, 
			//sanctioner isn't the player whose turn it is or sanctioner has to perform a coup (see must_coup at Player.hpp
			void sanction(Player* sanctioner, Player* sanctioned); 
			//Call the coup() method within the does_coup and the when_couped() method within the sanctioned. (See Player.hpp)
			//Throws exceptions if: does_coup or gets_couped are nullptr, does_coup and gets_couped are the same player, 
			//does_coup isn't the player whose turn it is or does_coup has to perform a coup (see must_coup at Player.hpp
			//Important! For elimination, the parameter is_alive inside Player (See Player.hpp) is set to false.
			//Deletion of all the Player*s in the vector game_players occurs only at the destructor
			void coup(Player* does_coup, Player* gets_couped); 
			//Call the turn_ability() method within the player. (See Player.hpp)
			//Throws exceptions if: player is nullptr, player doesn't have a turn ability (See Player.hpp for info on who does) or the player cannot perform it 
			void turn_ability(Player* player);
			//Call the turn_ability(Player* target) method within the player (See Player.hpp)
			//Throws exceptions if: player or target are nullptr, player and target are the same player, player doesn't have an ability that targets players (See Player.hpp)
			//player isn't the player whose turn it is or player has to perform a coup (see must_coup at Player.hpp
			size_t turn_ability(Player* player, Player* target);
			//Check whether a certain player can prevent an action. Collects all the different players that can into a vector and returns them
			//Throws exceptions if: does_action is nullptr or not the player whose turn it is, action isn't a blockable actions (Arrest, Tax, Coup, Bribe)
			//Returns nullptr if no player that can prevent the action is found. Otherwise, returns the vector<Player*> will all the players who can cancel the action
			vector<Player*> prevent_check(Player* does_action, string action);
			//For the actions: Tax, Bribe and Arrest: Will call the appropriate undo method in the player for the action recieved (See Player.hpp)
			//Throws exceptions if: player is nullptr or not the player whose turn it is or if action isn't "Tax", "Bribe" or "Arrest"
			void prevent_action(Player* player, string action);
			//Used to undo an arrest.
			void prevent_arrest(Player* perpetrator, Player* target);
			//For the action coup, will subtract 7 coins from the perpetrator and 5 coins from the preventer
			//Throw exceptions if: perpetrator doesn't have enough coins, the preventer isn't a General or the preventer doesn't have enough coins. (See General.hpp)
			void prevent_coup(Player* perpetrator, Player* preventer);

	};
	
}

#endif
