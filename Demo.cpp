#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdio>
#include <ctime>

#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Judge.hpp"
#include "Spy.hpp"
#include "General.hpp"
#include "Merchant.hpp"
#include "Baron.hpp"

using namespace std;
using namespace coup;

void print_player_status(Player* player) {
    cout << "\nIt's " << player->name() << "'s turn.\n";
    cout << "Role: " << player->role() << "\n";
    cout << "Coins: " << player->coins() << "\n";
    string player_sanctioned = "";
    if (player->is_sanctioned() == true)
        player_sanctioned = "true";
    else
        player_sanctioned = "false";
    cout << "Sanctioned: " << player_sanctioned << "\n";
    cout << "Extra Turns: " << player->get_extra_turns() << "\n\n";
}

void print_action_menu() {
    cout << "Select action:\n";
    cout << "1. Gather\n";
    cout << "2. Tax \n";
    cout << "3. Bribe \n";
    cout << "4. Arrest\n";
    cout << "5. Sanction\n";
    cout << "6. Coup\n";
    cout << "7. Turn Ability (Spy/Baron only)\n";
    cout << "Your choice: ";
}

string select_target(const vector<Player*>& all_players, Player* current) {
    vector<Player*> valid_targets;
    for (Player* p : all_players) {
        if (p->alive() && p != current) {
            valid_targets.push_back(p);
        }
    }

    if (valid_targets.empty()) {
        cout << "No valid targets.\n";
        return "";
    }

    for (size_t i = 0; i < valid_targets.size(); ++i) {
        cout << i << ". " << valid_targets[i]->name() << " (" << valid_targets[i]->role() << ")\n";
    }

    int choice = -1;
    cout << "Choose a target: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice < 0 || static_cast<size_t>(choice) >= valid_targets.size()) {
        cout << "Invalid target.\n";
        return "";
    }

    return valid_targets[choice]->name();
}

int main() {
    cout << "Enter number of players: (2-6)" << endl;

    size_t num_players = 6;
    cin >> num_players;
    while (!(num_players >= 2 && num_players <= 6 )){
        cout << "Enter number of players: (2-6)" << endl;
        cin >> num_players;
    }
    Game game(num_players);

    srand(time(0));
    for (size_t i = 0; i < num_players; ++i) {
        printf("Enter the name for player %d: \n", i + 1);
        std::string name;
        cin >> name;
        std::cout << "Name entered is: " << name << endl;
        int role = rand() % 6 + 1;

        try {
            switch (role) {
                case 1:
                    new Spy(game, name);
                    break;
                case 2:
                    new Baron(game, name);
                    break;
                case 3:
                    new Governor(game, name);
                    break;
                case 4:
                    new General(game, name);
                    break;
                case 5:
                    new Judge(game, name);
                    break;
                case 6:
                    new Merchant(game, name);
                    break;
            }
        } catch (char const* e) {
            cerr << "Duplicate name found. Please enter a unique name" << endl;
            --i; // retry for this player
        }
    }

    

    while (game.winner() == "") {
        Player* current = game.get_current_player();
        vector<Player*> preventers = {};
        print_player_status(current);
        print_action_menu();

        int action;
        cin >> action;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            if (action == 1) {
                game.gather(current);
                cout << current->name() << " gathered! +1 coins" << endl;
            } else if (action == 2) {
                vector<Player*> blockers = game.prevent_check(current, "Tax");
                for (Player* block : blockers) {
                    cout << block->name() << " (" << block->role() << ") can prevent this action! Prevent? (1 = Yes / 0 = No): ";
                    int choice;
                    cin >> choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (choice == 1) {
                        game.prevent_action(current, "act");
                        cout << "Action was prevented by " << block->name() << "!\n";
                        break;
                    }
                }
                game.tax(current);
                
                if (current->role() == "Governor"){
                    cout << current->name() << " taxed! +3 coins (Governor bonus)" << endl;
                }
                else
                    cout << current->name() << " taxed! +2 coins" << endl;

            } else if (action == 3) {
                game.bribe(current);
             } else if (action == 4) {
                cout << "Select target for arrest:\n";
                vector<Player*> players = game.player_vector();
                for (size_t i = 0; i < players.size(); ++i) {
                    if (players[i]->name() != current->name() && players[i]->alive())
                        cout << i << ". " << players[i]->name() << "\n";
                }

                int target_index;
                cin >> target_index;
                Player* target = players[target_index];
                game.arrest(current, target);
                if (target->role() == "Merchant"){
                    cout << current->name() << " arrested " << target->name() << "! -2 coins to " << target->name() << "(Merhcnat ability)" << endl;

                }

                cout << current->name() << " arrested " << target->name() << "! +1 coins to " << current->name() << "-1 coins to " << target->name() << endl;
            } else if (action == 5) {
                cout << "Select target for sanction:\n";
                vector<Player*> players = game.player_vector();
                for (size_t i = 0; i < players.size(); ++i) {
                    if (players[i]->name() != current->name() && players[i]->alive())
                        cout << i << ". " << players[i]->name() << "\n";
                }

                int target_index;
                cin >> target_index;
                Player* target = players[target_index];
                game.sanction(current, target);
                cout << current->name() << " sancitoned " << target->name() << endl;
                if (target->role() == "Baron")
                    cout << target->name() << "Got 1 coin as conpenscation! (Baron ability)" << endl;

            } else if (action == 6) {
                cout << "Select target for coup:\n";
                vector<Player*> players = game.player_vector();
                for (size_t i = 0; i < players.size(); ++i) {
                    if (players[i]->name() != current->name() && players[i]->alive())
                        cout << i << ". " << players[i]->name() << "\n";
                }


                int target_index;
                cin >> target_index;
                vector<Player*> blockers = game.prevent_check(current, "Coup");
                bool prevented = false;
                for (Player* block : blockers) {
                    cout << block->name() << " (" << block->role() << ") can prevent this action! Prevent? (1 = Yes / 0 = No): ";
                    int choice;
                    cin >> choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (choice == 1) {
                        game.prevent_coup(current, block);
                        cout << "Action was prevented by " << block->name() << "!\n";
                        prevented = true;
                        break;
                    }
                }
                if (!prevented){
                    Player* target = players[target_index];
                    game.coup(current, target);
                    cout << current->name() << " performed a coup on " << target->name() << "! " << target->name() << " has been eleminated from play!" << endl;
                }
                
            } else if (action == 7) {
                
                if (current->role() == "Spy") {
                    cout << "Select a target to spy on:\n";
                    vector<Player*> players = game.player_vector();
                    for (size_t i = 0; i < players.size(); ++i) {
                    if (players[i]->alive() && players[i] != current)
                        cout << i << ". " << players[i]->name() << " (" << players[i]->role() << ")\n";
                    }

                    int target_index;
                    cin >> target_index;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (target_index < 0 || static_cast<size_t>(target_index) >= players.size()) {
                        cout << "Invalid target index.\n";
                        continue;
                    }

                    Player* target = players[target_index];
                    try {
                        cout << target->name() << " has " << game.turn_ability(current, target) << " coins\n";
                        continue;
                    } catch (char const* e) {
                        cerr << "Error: " << e << "\n";
                    } catch (const char* msg) {
                        cerr << "Error: " << msg << "\n";
                    }

                } else if (current->role() == "Baron") {
                    try {
                        game.turn_ability(current);
                    } catch (char const* e) {
                        cerr << "Error: " << e << "\n";
                    } catch (const char* msg) {
                        cerr << "Error: " << msg << "\n";
                    }
                } else {
                    cout << "This player has no turn ability.\n";
                    continue;
                }
            }
            
            else {
                cout << "Invalid action!\n";
                continue;
            }

            // Check for possible prevention
            string act =
                (action == 2) ? "Tax" :
                (action == 3) ? "Bribe" :
                (action == 4) ? "Arrest": "";

            if (!act.empty()) {
                vector<Player*> blockers = game.prevent_check(current, act);
                for (Player* block : blockers) {
                    cout << block->name() << " (" << block->role() << ") can prevent this action! Prevent? (1 = Yes / 0 = No): ";
                    int choice;
                    cin >> choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (choice == 1) {
                        game.prevent_action(current, act);
                        cout << "Action was prevented by " << block->name() << "!\n";
                        break;
                    }
                }
            }

        } catch (char const* e) {
            cerr << "Error: " << e << "\n";
            continue;
        } catch (const char* msg) {
            cerr << "Error: " << msg << "\n";
            continue;
        }

        game.update_turn();
    }

    cout << "Game over! Winner is: " << game.winner() << "\n";
    return 0;
}
