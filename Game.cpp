#include <iostream>
#include <vector>
#include <string>

#include "Game.hpp"
#include "Player.hpp"
#include "Spy.hpp"
#include "Judge.hpp"
#include "Governor.hpp"
#include "Merchant.hpp"
#include "General.hpp"
#include "Baron.hpp"

#define MIN_PLAYERS 2
#define MAX_PLAYERS 6

//Actions to check if they are blocked or not by some classes

#define TAX "Tax"
#define BRIBE "Bribe"
#define COUP "Coup"
#define ARREST "Arrest"


using namespace std;
using namespace coup;

Game::Game(size_t new_player_amount) : player_amount(new_player_amount){
	if (player_amount < MIN_PLAYERS || player_amount > MAX_PLAYERS )
		throw "Invalid arguments: Game construction";

	game_players = {};
	current_player = nullptr;
	curr_turn = 0;
	game_winner = "";
}

Game::Game(const Game& other)
    : player_amount(other.player_amount),
      curr_turn(other.curr_turn),
      game_winner(other.game_winner),
      current_player(nullptr)
{
    for (Player* p : other.game_players) {
		Player* new_p;
		if (p->role() == "Spy"){
			new_p = new Spy(*(p));
		}
		else if (p->role() == "Judge")
			new_p = new Judge(*(p));
		else if (p->role() == "Baron")
			new_p = new Baron(*(p));
		else if (p->role() == "Merchant")
			new_p = new Merchant(*(p));
		else if (p->role() == "General")
			new_p = new General(*(p));
		else
			new_p = new Governor(*(p));
        game_players.push_back(new_p);
        
        // Set current_player if it matches
        if (p == other.current_player) {
            current_player = new_p;
        }
    }
}



Game::~Game(){
	for (Player* player : game_players) {
        delete player;
    }
	game_players.clear();
}

//General getters
vector<string> Game::players(){
	vector<string> names;
	if (game_players.empty())
		return names;

	
	for (Player* play: game_players){
		if (play->alive())
			names.push_back(play->name());
	}
	return names;
}

vector<Player*> Game::player_vector() const{
	return this->game_players;
}
Player* Game::get_current_player(){
	size_t tries = 0;
    while (tries < game_players.size()) {
        size_t index = curr_turn % player_amount;
        if (game_players[index]->alive()) {
			current_player = game_players[index];
            return current_player;
        }
        curr_turn++;
        tries++;
    }
	
    return nullptr;
}

string Game::turn(){
	if (game_players.empty()){
		return "";
	}

	size_t index = curr_turn % player_amount;
	return game_players.at(index)->name();
}

bool Game::is_there_a_winner(){
	
	return winner() != "";
}

string Game::winner() {
    int aliveCount = 0;
    Player* lastAlive = nullptr;

    for (Player* p : game_players) {
        if (p->alive()) {
            aliveCount++;
            lastAlive = p;
        }
    }

    if (aliveCount == 1 && lastAlive) {
        game_winner = lastAlive->name();
    } else {
        game_winner = ""; // No winner yet
    }

    return game_winner;
}
void Game::add_player(Player* player){
	if (player == nullptr)
		throw "Invalid argument: null player";
	if (game_players.size() == player_amount)
		throw "Invalid argument: there are enough players already";
	if (player->name() == "")
		throw "Invalid argument: Missing name";
	
	for (size_t i = 0; i < game_players.size(); i++){
		if (game_players[i]->name() == player->name()){
			throw "Invalid argument: A player with this name already exists";
		}
			
	}

	game_players.push_back(player);
}

void Game::update_turn(){
	if (get_current_player()->get_extra_turns() > 0){
		get_current_player()->turn_cycle_start();
		return;
	}

	get_current_player()->turn_end();

	size_t tries = 0;
	do{
		curr_turn = (curr_turn + 1) % game_players.size();
		tries++;
	} while ((!game_players[curr_turn]->alive()) && tries < game_players.size());
	
	game_players[curr_turn]->turn_cycle_start();

}
void Game::gather(Player* player){
	if (player == nullptr)
        throw "Invalid argument: null player";
	if (player != get_current_player())
		throw "Not your turn";
	if (player->must_coup())
		throw "Illegal action: Player must perofrm a Coup";
	if (player->is_sanctioned()){
		throw "Illegal action: player sanctioned";
	}
	
	player->gather();
}

void Game::tax(Player* player){
	if (player == nullptr)
        throw "Invalid argument: null player";
	if (player != get_current_player())
		throw "Not your turn";
	if (player->must_coup())
		throw "Illegal action: Player must perofrm a Coup";
    if (player->is_sanctioned()){
        throw "Illegal action: player sanctioned";
    }
	player->tax();
}

void Game::bribe(Player* player){
	if (player == nullptr)
		throw "Invalid argument";
	if (!player->can_bribe())
		throw "Illegal action";
	if (player->must_coup())
		throw "Illegal action: Player must perofrm a Coup";
	if (player != get_current_player())
		throw "Not your turn";
	player->bribe();
}

void Game::arrest(Player* arrester, Player* arrested){
	if (arrester == nullptr || arrested == nullptr)
		throw "Invalid argument: Arrest";

	if (!arrester->can_arrest(arrested))
		throw "Illegal action: Arrest";
	if (arrester == arrested)
		throw "Cannot arrest yourself";
	if (arrester->must_coup())
		throw "Illegal action: Player must perofrm a Coup";

	if (arrester != get_current_player())
		throw "Not your turn";

	arrester->arrest(arrested);
	arrested->when_arrested();
}

void Game::sanction(Player* sanctioner, Player* sanctioned){
	if (sanctioner == nullptr || sanctioned == nullptr)
		throw "Invalid argument: Null pointer: Sanction";

	if (!sanctioner->can_sanction(sanctioned))
		throw "Cannot sanction this player";
	if (sanctioner == sanctioned)
		throw "Cannot sanction yourself";
	if (sanctioner != get_current_player())
		throw "Not your turn";
	if (sanctioner->must_coup())
		throw "Illegal action: Player must perofrm a Coup";
	if (sanctioned == sanctioner)
		throw "Cannot sanction yourself";
		
	sanctioner->sanction(sanctioned);
	sanctioned->when_sanctioned();
}

void Game::coup(Player* coups, Player* couped){
	if (coups == nullptr || couped == nullptr)
		throw "Invalid argument: Coup";
	if (coups != get_current_player())
		throw "Invalid argument: Not your turn: coup";
	
	if (couped == coups)
		throw "Invalid argument: Player cannot perform Coup on themselves: Coup";

	if (!coups->can_coup())
		throw "Illegal action: Coup";

	coups->coup();
	couped->when_couped();
	
}

void Game::turn_ability(Player* player){
	if (player == nullptr)
		throw "Invalid argument: Turn ability. Null player";

	if (player != get_current_player())
		throw "Invalid agrument: Not your turn";
	
	if (player->role() != "Baron")
		throw "Invalid argument: Turn ability. Invalid player role";

	if (player->must_coup())
		throw "Illegal action: Player must perofrm a Coup";

	if (!player->can_use_turn_ability())
		throw "Illegal action: Cannot use turn ability";

	player->turn_ability();
}

size_t Game::turn_ability(Player* player, Player* target){
	if (player == nullptr || target == nullptr)
		throw "Invalid argument: Turn ability. Null player";
	if (player != get_current_player())
		throw "Invalid agrument: Not your turn";
	if (player == target)
		throw "Invalid agrument: Cannot spy on yourself";
	if (player->must_coup())
		throw "Illegal action: Player must perofrm a Coup";
	if (player->role() != "Spy")
		throw "Invalid argument: Turn ability. None spy player detected";
	

	return player->turn_ability(target);
} 

vector<Player*> Game::prevent_check(Player* does_action, string action){
	if (does_action == nullptr)
		throw "Invalid argument: Null player";
	if (does_action != get_current_player())
		throw "Invalid argument: Not your turn";
	if (action != TAX && action != BRIBE && action != COUP && action != ARREST)
		throw "Invalid argument: Got action that cannot be canceled";

	string preventer_role = "";
	vector<Player*> potential_preventers = {};

	if (action == TAX)
		preventer_role = "Governor";
	else if (action == BRIBE)
		preventer_role = "Judge";
	else if (action == ARREST)
		preventer_role = "Spy";
	else
		preventer_role = "General";
	
	for (Player* player: game_players){
		if (player->role() == preventer_role){
			if (player->can_use_realtime_ability() && does_action != player && player->alive()){				
				potential_preventers.push_back(player);
			}
		}
	}

	return potential_preventers;
}

void Game::prevent_action(Player* player, string action){
	if (player == nullptr)
		throw "Invalid argument: Null pointer: prevent_action";

	if (action != TAX &&  action != BRIBE)
		throw "Uncancellable action";

	if (action == TAX){
		player->undo_tax();
		return;
	}

	if (action == BRIBE){
		player->undo_bribe();
		return;
	}

	
}

void Game::prevent_arrest(Player* perpertrator, Player* target){
	if (!perpertrator || !target)
		throw "Invalid argument: Null pointer: prevent_arrest";
	
	if (perpertrator != get_current_player()){
		throw "Not your turn";
	}

	perpertrator->undo_arrest(true, target);
	target->undo_arrest(false, perpertrator);
	
}
void Game::prevent_coup(Player* perpetrator, Player* preventer){
	if (perpetrator == nullptr || preventer == nullptr){
		throw "Invalid arguemnt: Null pointer given";
	}

	if (perpetrator != get_current_player()){
		throw "Invalid argument: Not your turn";
	}

	if (!perpetrator->can_coup())
		throw "Invalid argument: Perpetrator doesn't have enough coins";

	if (preventer->role() != "General"){
		throw "Invalid argument: Only a General can prevent a coup";
	}

	if (preventer->coins() < 5)
		throw "Invalid argument: Preventer doesn't have enough coins to prevent the coup";

	perpetrator->coup();
	preventer->change_coins(5, false);
}
