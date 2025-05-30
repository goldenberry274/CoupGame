#include <string>

#include "General.hpp"

#define COUP_PREVENT_PRICE 5
using namespace std;
using namespace coup;

General::General(Game& game, string name) : Player(name, "General"){
	game.add_player(this);
}

General::General(const General& copy) : Player(copy.name(), "General"){
    coin_num = copy.coins();
    sanctioned = copy.is_sanctioned();
    last_arrest = copy.last_arrest;
    extra_turns = copy.extra_turns;
	is_alive = copy.alive();
}
General::General(const Player& copy) : Player(copy.name(), "General"){
	coin_num = copy.coins();
    sanctioned = copy.is_sanctioned();
    last_arrest = copy.last_player_arrested();
    extra_turns = copy.get_extra_turns();
	is_alive = copy.alive();
}

General::~General(){}


bool General::can_use_realtime_ability() const{
    
    return coin_num >= COUP_PREVENT_PRICE;
}

void General::realtime_ability(){
    if (!can_use_realtime_ability())
        throw "Invalid action. General must have 5 coins or more to prevent a coup";
    coin_num -= COUP_PREVENT_PRICE;
}

void General::when_arrested(){
    return;
}

void General::undo_arrest(bool arrested, Player* target){
    if (!target)
        throw "Invalid argument: Null pointer";
    if (arrested)
        return;
    if (target->role() == "Merchant")
        return;
    change_coins(1, false);
}
