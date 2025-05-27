# CoupGame
An implementation of the game Coup.
Libaries used: wxWidgets, SFML.
To install wxWidgets on Ubuntu, run this command in the terminal:
sudo apt-get install libwxgtk3.0-dev 
To install wxWidgets on Ubuntu, run this command in the terminal:
sudo apt install libsfml-dev

# Project Overview:
This project is a game of Coup designed to be played locally on s aingle computer between multiple people (2-6).
Contains both a GUI playable version (Main.cpp) and a Demo that can be played via the terminal (Demo.cpp).
# Project Structure:
There are several classes in the game (Spy, Baron, Governor, Judge, Merchant and General).
All of these inherit from the Player class, with each adding their passive, realtime or turn abilities (See section classes).
The Game class manages the internal affairs of them game such as actions and eleminations. It doesn't rely on input from the trminal or otherwise, classes that do rely on input have to manage it themselves and based on said input, tell gam to perform certain actions.
Upon starting the game and selecting how many players (Within the appropriate range), the players will then enter their names and each of them will randomly get a class.
(All classes inherit from Player.hpp)
The goal of the game is to remain the last one standing. 
In each players turn they can perform an action:

# General Actions: Can be used by all players, regardless of class:
```
Gather - +1 coin (Doesn't work if sanctioned)
Tax - +2 coins (Doesn't work if sanctioned, can be blocked by Governor). If Governor, +3 coins
Bribe - -4 coins, +2 extra turns (can be blocked)
Arrest - +1 coins to self, -1 coins to target. If target is a Merchant, -2 coins to them but 0 coins to self
Sanction - -3 coins, prevents economic actions to another player(Gather and tax)
Coup -7 coins, eliminates a player (Elemination logic in Game class)
```
# Classes: These are the classes and their passive and active abilities.
```
Baron: 
Passive ability: Gets one coin when sanctioned as conpenscation. 
Realtime ability: N/E.
Turn ability: Invest 3 coisn to earn 6. (Costs a turn). 
Spy: 
Passive ability: N/E.
Realtime ability: Can prevent one player from using Arrest on another.
Turn ability: Can find out the number of coins another player has (Doesn't cost a turn).
General:
Passive ability: Doesn't lose a coin when arrested.
Realtime ability: Can prevent one player from using Coup on another at the cost of 5 coins.
Turn ability: N/E.
Governor:
Passive ability: Gets 3 coins when taxing instead of 2.
Realtime ability: Can prevent a player from using Tax.
Turn ability: N/E.
Merchant:
Passive abilities: If turn starts with 3 or more coins, get a coin for free. When arrested, lose 2 coins but don't pay to the arrester.
Realtime ability: N/E.
Turn ability: N/E.
Judge:
Passive ability: When sanctioned, the perpetrator has to pay an extra coin.
Realtime ability: Can prevent a player from using Bribe.
Turn ability: N/E.
```
Makefile Commands:
```
main: Compiles the main game with the GUI and runs it.
demo: Compiles the demo file (The terminal version of the game) and runs it.
test: Compiles the test file and runs it.
valgrind: runs the valgrind command on the main game with the GUI.
valgrind-demo: runs the valgrind command on the demo game.
```
