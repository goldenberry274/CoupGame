#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

using namespace std;
namespace coup {

    class Player {
    protected:
        string player_name;        // Any name is valid
        string player_role;        // Must be one of the valid roles (Governor, Spy, etc.)
        size_t coin_num = 0;
        bool sanctioned = false;
        bool arrest_blocked = false;    // Blocked from arresting in the current turn
        string last_arrest;        // Prevent arresting the same player twice in a row
        size_t extra_turns = 0;         // Accumulated via the bribe action
		bool is_alive = true;
	
    public:
        Player(string cpy_name, string cpy_role);
        virtual ~Player();

        // === Getters ===
        size_t coins() const;
        string name() const;
        string role() const;
        bool is_sanctioned() const;
        string last_player_arrested() const;
        bool is_arrest_blocked() const;

        // === Setters ===
        void set_sanction(bool new_sanction);
        void set_arrest_block(bool new_arrest_blocked);

        // === Coin management ===
        // If `add` is true, adds coins; otherwise attempts to subtract.
        bool add_coins(size_t coins, bool add);

        // === Action availability checks ===
        bool can_bribe() const;
        bool can_arrest(const Player* arrested) const;
        bool can_sanction(const Player* player) const;
        bool can_coup() const;
        virtual bool can_use_ability() const;

        // === Turn lifecycle ===
        virtual void turn_cycle_start();

        // === Basic actions ===
        void gather();                  // +1 coin (can be blocked)
        virtual void tax();                     // +2 coins (can be blocked)
        void bribe();                   // -4 coins, +2 extra turns (can be blocked)

        // === Interactions with other players ===
        virtual void arrest(const Player* arrested);              // +1 coin, can't target same player twice
        virtual void when_arrested();           // -1 coin

        void sanction(const Player* sanctioned);            // -3 coins, prevents economic actions
        virtual void when_sanctioned();         // Effect of being sanctioned

        virtual void coup();                              // -7 coins, eliminates a player
        virtual void when_couped();
        // === Role-specific ability ===
        virtual void role_ability();
    };

} 

#endif 
