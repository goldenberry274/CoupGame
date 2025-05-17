#include <string.h>

#include "Merchant.hpp"

using namespace coup;
using namespace std;
#define BAR_FOR_EXTRA_COIN 3

Merchant::Merchant(Game& game, string name) : Player(name, "Merchant"){
    game.add_player(this);
}

Merchant::~Merchant(){}

void Merchant::turn_cycle_start(){
    if (coin_num >= BAR_FOR_EXTRA_COIN)
        coin_num++;
    
    sanctioned = false;
    arrest_blocked = false;

    if (extra_turns > 0) {
        extra_turns--;
    }
    
}