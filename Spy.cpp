#include <string>

#include "Spy.hpp"
#include "Player.hpp"

#define MUST_COUP 10

using namespace std;
using namespace coup;

Spy::Spy(Game& game, string name) : Player(name, "Spy"){
	game.add_player(this);
}
Spy::Spy(const Spy& spy) : Player(spy.name(), "Spy"){
    coin_num = spy.coins(); 
    sanctioned = spy.is_sanctioned();
    extra_turns = spy.extra_turns;
    is_alive = spy.alive();
    last_arrest = spy.last_player_arrested();
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
