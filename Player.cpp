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
    arrest_blocked = false;
    last_arrest = "";
    extra_turns = 0;
	is_alive = true;
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

bool Player::is_arrest_blocked() const {
    return arrest_blocked;
}

void Player::set_sanction(bool new_sanctioned) {
    sanctioned = new_sanctioned;
}

void Player::set_arrest_block(bool new_arrest_blocked) {
    arrest_blocked = new_arrest_blocked;
}

void Player::turn_cycle_start() {
    sanctioned = false;
    arrest_blocked = false;
    if (extra_turns > 0) {
        extra_turns--;
    }
}

bool Player::add_coins(size_t coins, bool add) {
    if (add) {
        coin_num += coins;
        return true;
    } else {
        if (coins > coin_num) {
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
    return !arrest_blocked && arrested->name() != last_arrest;
}

bool Player::can_sanction(const Player* player) const {
    if (player == nullptr) 
		throw invalid_argument("Null player pointer in can_sanction");

    int paywall = SANCTION_LIMIT;
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
    coin_num -= BRIBE_LIMIT;
    extra_turns += EXTRA_TURNS_BRIBE;
    
}

void Player::arrest(const Player* arrested) {
    if (arrested == nullptr)
        throw "Invalid argument";

    last_arrest = arrested->name();
    if (!(arrested->role() == "Merchant"))
        coin_num += ARREST_PAYMENT;
}

void Player::when_arrested() {
    coin_num -= ARREST_PAYMENT;
}

void Player::sanction(const Player* sancioned) {
    if (sancioned == nullptr)
        throw "Invalid argument";
    int paywall = SANCTION_LIMIT;
    if (sancioned->role() == "Judge")
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
bool Player::can_use_ability() const {
    return true;
}

void Player::role_ability(){
    return;
}
