#include <string>

#include "Governor.hpp"
#include "Game.hpp"
#include "Player.hpp"

#define TAX_PAYMENT 3

using namespace std;
using namespace coup;

Governor::Governor(Game& game, string name) : Player(name, "Governor"){
	game.add_player(this);
}
Governor::~Governor(){}

void Governor::tax() {
	coin_num += TAX_PAYMENT;
}
bool Governor::can_use_realtime_ability() const{
	return true;
} 

void Governor::undo_tax(){
	coin_num -= TAX_PAYMENT;
}

