#include <string>

#include "Spy.hpp"
#include "Player.hpp"

#define MUST_COUP 10

using namespace std;
using namespace coup;

Spy::Spy(Game& game, string name) : Player(name, "Spy"){
	game.add_player(this);
}


Spy::~Spy(){}

bool Spy::can_use_turn_ability() const{
	return true;
}

bool Spy::can_use_realtime_ability() const{
	return true;
}

size_t Spy::turn_ability(const Player* player){
	if (player == nullptr)
		throw "Invalid argument: Null player given";
	if (player == this)
		throw "Invliad argument: Cannot spy on yourself";
	return player->coins();
}
