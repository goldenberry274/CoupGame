#include <string>

#include "General.hpp"

using namespace std;
using namespace coup;

General::General(Game& game, string name) : Player(name, "General"){
	game.add_player(this);
}

General::~General(){}

void General::when_arrested(){
    return;
}