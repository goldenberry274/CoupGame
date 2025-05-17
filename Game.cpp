#include <iostream>
#include <vector>
#include <string>

#include "Game.hpp"
#include "Player.hpp"

#define GATHER_PAY 1
#define INVESTMENT_PAY 6
#define NORMAL_TAX_PAY 2
#define GOV_TAX_PAY 3
#define ARREST_PAY 1

using namespace std;
using namespace coup;

Game::Game(size_t new_player_amount) : player_amount(new_player_amount){
	game_players = {};
	curr_turn = 0;
	game_winner = "";
}

Game::~Game(){}

//General getters
vector<string> Game::players(){
	if (game_players.empty())
		throw "Invalid argument";

	vector<string> names;
	for (Player* play: game_players){
		names.push_back(play->name());
	}
	return names;
}

string Game::turn(){
	if (game_players.empty())
		throw "Invalid argument";
	size_t index = curr_turn % player_amount;
	return game_players.at(index)->name();
}

void Game::add_player(Player* player){
	if (player == nullptr)
		throw "Invalid argument";
	if (game_players.size() == player_amount)
		throw "Invalid argument";
	game_players.push_back(player);
}

void Game::gather(Player* player){
	if (player == nullptr)
                throw "Invalid argument";
	if (player->is_sanctioned()){
		throw "Illegal action";
	}
	player->gather();
}

void Game::tax(Player* player){
	if (player == nullptr)
                throw "Invalid argument";
    if (player->is_sanctioned()){
            throw "Illegal action";
    }
	player->tax();
}

void Game::bribe(Player* player){
	if (player == nullptr)
		throw "Invalid argument";
	if (!player->can_bribe())
		throw "Illegal action";
	player->bribe();
}
void Game::arrest(Player* arrester, Player* arrested){
	if (arrester == nullptr || arrested == nullptr)
		throw "Invalid argument";

	if (!arrester->can_arrest(arrested))
		throw "Illegal action";
	
	arrester->arrest(arrested);
	arrested->when_arrested();
}

void Game::sanction(Player* sanctioner, Player* sanctioned){
	if (sanctioner == nullptr || sanctioned == nullptr)
		throw "Invalid argument";

	if (!sanctioner->can_sanction(sanctioned))
		throw "Illegal action";
	
	sanctioner->sanction(sanctioned);
	sanctioned->when_sanctioned();
}

