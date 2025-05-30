#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include "Player.hpp"

#define BRIBE_LIMIT 4
#define SANCTION_LIMIT 3
#define COUP_LIMIT 7

#define TAX_PAYMENT 2
#define GATHER_PAYMENT 1
#define ARREST_PAYMENT 1

#define EXTRA_TURNS_BRIBE 2
#define MUST_COUP_BAR 10

using namespace std;
using namespace coup;

Player::Player(string cpy_name, string cpy_role) {
    
    if (cpy_role != "Governor" && cpy_role != "Judge" && cpy_role != "General" && cpy_role != "Spy" 
		&& cpy_role != "Baron" && cpy_role != "Merchant") {
        throw invalid_argument("Invalid role: " + cpy_role);
    }

    player_name = cpy_name;
    player_role = cpy_role;
    coin_num = 0;
    sanctioned = false;
    last_arrest = "";
    extra_turns = 0;
	is_alive = true;
}

Player::Player(const Player& copy)
    : player_name(copy.name()),
      player_role(copy.role()),
      coin_num(copy.coins()),
      sanctioned(copy.is_sanctioned()),
      last_arrest(copy.last_player_arrested()),
      extra_turns(copy.extra_turns),
      is_alive(copy.alive()) {
}


Player::~Player() = default;

size_t Player::coins() const {
    return coin_num;
}

string Player::name() const {
    return player_name;
}

string Player::role() const {
    return player_role;
}

bool Player::is_sanctioned() const {
    return sanctioned;
}


string Player::last_player_arrested() const{
    return last_arrest;
}
bool Player::alive() const{
    return is_alive;
}

void Player::set_sanction(bool new_sanctioned) {
    sanctioned = new_sanctioned;
}



void Player::turn_cycle_start() {
    if (extra_turns > 0) {
        extra_turns--;
    }
}

void Player::turn_end(){
    if (sanctioned)
        sanctioned = false;
}
bool Player::change_coins(size_t coins, bool add) {
    if (add) {
        coin_num += coins;
        return true;
    } else {
        cout << "Player has " << coin_num  << " coins" << endl;
        if (coins > coin_num) {
            cout << "Setting coins to 0" << endl;
            coin_num = 0;
            return false;
        }
        coin_num -= coins;
        return true;
    }
}

bool Player::can_bribe() const {
    return coin_num >= BRIBE_LIMIT;
}

bool Player::can_arrest(const Player* arrested) const {
    if (arrested == nullptr || arrested == this)
        throw "Invalid argument in can_arrest";
   
    size_t paywall = 1;
    if (arrested->role() == "Merchant")
        paywall++;

    return arrested->name() != last_arrest && arrested->coins() >= paywall;
}

bool Player::can_sanction(const Player* player) const {
    if (player == nullptr || player == this)
        throw "Invalid argument in can_sanction";
    if (player->is_sanctioned())
        return false;
    size_t paywall = SANCTION_LIMIT;
    if (player->role() == "Judge") 
		paywall++;

    return coin_num >= paywall;
}

bool Player::can_coup() const {
    return coin_num >= COUP_LIMIT;
}

void Player::gather() {
    coin_num += GATHER_PAYMENT;
}

void Player::tax() {
    coin_num += TAX_PAYMENT;
}

void Player::bribe() {
    if (!can_bribe())
        throw "Illegal action: Player doesn't have enough coins to bribe";
    coin_num -= BRIBE_LIMIT;
    extra_turns += EXTRA_TURNS_BRIBE;
    
}

void Player::subtract_turn(){
    if (extra_turns < 1)
        return;
    extra_turns--;
}

size_t Player::get_extra_turns() const{
    return extra_turns;
}


void Player::arrest(const Player* arrested) {
    if (arrested == nullptr || arrested == this)
        throw "Invalid argument in arrested";

    last_arrest = arrested->name();
    if (!(arrested->role() == "Merchant"))
        coin_num += ARREST_PAYMENT;
}

void Player::when_arrested() {
    if (coin_num < ARREST_PAYMENT){
        throw "Too little coins for arrest";
    }
    coin_num -= ARREST_PAYMENT;
}

void Player::sanction(const Player* sanctioned) {
    if (sanctioned == nullptr || sanctioned == this)
        throw "Invalid argument";
    int paywall = SANCTION_LIMIT;
    if (sanctioned->role() == "Judge")
        paywall++;
    coin_num -= paywall;
}

void Player::when_sanctioned() {
    sanctioned = true;
}

void Player::coup() {
    coin_num -= COUP_LIMIT;
}

void Player::when_couped(){
    is_alive = false;
}

bool Player::must_coup(){
    return coin_num >= MUST_COUP_BAR;
}
//By default, players don't have an ability: Some classes don't have time abilities or realtime ones. In the case they are needed, these methods are overloaded in the classes that use them differently.
bool Player::can_use_turn_ability() const {
    return false;
}

void Player::turn_ability(){
    return;
}

size_t Player::turn_ability(const Player* player){
    return 0;
}

bool Player::can_use_realtime_ability() const{
    return false;
}

void Player::realtime_ability(){
    return;
}

void Player::undo_tax(){

    coin_num -= TAX_PAYMENT;
}

void Player::undo_arrest(bool arrested, Player* target){
    if (!target)
        throw "Invalid argument: Null pointer";
    if (arrested)
        coin_num++;
    else if (target->role() == "Merchant")
        return;
    else
        change_coins(1, false);
}

void Player::undo_bribe(){
    extra_turns = 0;
}
