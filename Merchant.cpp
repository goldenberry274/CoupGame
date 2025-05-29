#include <string.h>

#include "Merchant.hpp"

using namespace coup;
using namespace std;

#define BAR_FOR_EXTRA_COIN 3
#define ARREST_DEMAND 2

Merchant::Merchant(Game& game, string name) : Player(name, "Merchant"){
    game.add_player(this);
}

Merchant::Merchant(const Merchant& copy): Player(copy.name(), "Merchant"){}

Merchant::~Merchant(){}

void Merchant::turn_cycle_start(){
    if (coin_num >= BAR_FOR_EXTRA_COIN)
        coin_num++;
    
    if (extra_turns > 0) {
        extra_turns--;
    }
    
}

void Merchant::when_arrested(){
    if (coin_num < ARREST_DEMAND){
        throw "Too little coins for arrest";
    }
    change_coins(2, false);
}

void Merchant::undo_arrest(bool arrested, Player* target){
    if (!target)
        throw "Invalid argument: Null pointer";
    if (arrested){
        coin_num += ARREST_DEMAND;
        return;
    }
    else if (target->role() == "Merchant")
        return;
    change_coins(1, false);
}
