#include <string>

#include "Spy.hpp"
#include "Player.hpp"

#define MUST_COUP 10

using namespace std;
using namespace coup;

Spy::Spy(Game& game, string name) : Player(name, "Spy"){
	game.add_player(this);
}

Spy::Spy(const Spy& copy) : Player(copy.name(), "Spy"){
    coin_num = 0;
    sanctioned = copy.is_sanctioned();
    last_arrest = copy.last_arrest;
    extra_turns = copy.extra_turns;
	is_alive = copy.alive();
}

Spy::~Spy(){}

Spy::Spy(const Player& copy) : Player(copy.name(), "Spy"){
	coin_num = copy.coins();
    sanctioned = copy.is_sanctioned();
    last_arrest = copy.last_player_arrested();
    extra_turns = copy.get_extra_turns();
	is_alive = copy.alive();
}

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
