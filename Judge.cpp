#include <string>

#include "Judge.hpp"

using namespace std;
using namespace coup;

Judge::Judge(Game& game, string name) : Player(name, "Judge"){
	game.add_player(this);
}

Judge::Judge(const Judge& copy) : Player(copy.name(), "Judge"){
    coin_num = copy.coins();
    sanctioned = copy.is_sanctioned();
    last_arrest = copy.last_arrest;
    extra_turns = copy.extra_turns;
	is_alive = copy.alive();
}
Judge::~Judge(){}

Judge::Judge(const Player& copy) : Player(copy.name(), "Judge"){
	coin_num = copy.coins();
    sanctioned = copy.is_sanctioned();
    last_arrest = copy.last_player_arrested();
    extra_turns = copy.get_extra_turns();
	is_alive = copy.alive();
}


bool Judge::can_use_realtime_ability() const{
	return true;
}
