
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

bool Baron::can_use_ability() const{
    
    return coin_num >= INVESTMENT_PAYMENT;
    
}
void Baron::role_ability(){
    coin_num += INVESTMENT_PROFIT - INVESTMENT_PAYMENT;
}

bool Baron::when_sanctioned(Player* player){
    sanctioned = true;
    coin_num++;
    return true;
}