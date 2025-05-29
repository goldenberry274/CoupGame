
#include "Baron.hpp"
#include "Game.hpp"
#include "Player.hpp"

#define INVESTMENT_PAYMENT 3
#define INVESTMENT_PROFIT 6

using namespace std;
using namespace coup;

Baron::Baron(Game& game, string name) : Player(name, "Baron"){
	game.add_player(this);
}

Baron::~Baron(){}

bool Baron::can_use_turn_ability() const{
    
    return coin_num >= INVESTMENT_PAYMENT;
    
}
void Baron::turn_ability(){
    if (!can_use_turn_ability())
        throw "Not enough coins to use ability";
    coin_num += INVESTMENT_PROFIT - INVESTMENT_PAYMENT;
}

void Baron::when_sanctioned(){
    sanctioned = true;
    coin_num++;
}
