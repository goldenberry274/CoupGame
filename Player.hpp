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
        string last_arrest;        // Prevent arresting the same player twice in a row
        size_t extra_turns = 0;         // Accumulated via the bribe action
		bool is_alive = true;
	
    public:
        Player(string cpy_name, string cpy_role);
        Player(const Player& copy);
        virtual ~Player();

        // === Getters ===
        size_t coins() const;
        string name() const;
        string role() const;
        bool is_sanctioned() const;
        string last_player_arrested() const;
        bool is_arrest_blocked() const;
        bool alive() const;

        // === Setters ===
        void set_sanction(bool new_sanction);
        void set_arrest_block(bool new_arrest_blocked);

        // === Coin management ===
        // If `add` is true, adds coins; otherwise attempts to subtract.
        //If there are too many coins to subtract, sets the coins to 0
        bool change_coins(size_t coins, bool add);

        // === Action availability checks ===
        //Returns true if player has 4 or more coins
        bool can_bribe() const;
        //Returns true if arrested->name() != last_arrest and arrested has enough coins for arrest (2 for Merchant, 1 for everybody else)
        bool can_arrest(const Player* arrested) const;
        //Returns true if user has enough coins to sanction (3 for regular players, an extra if the target is a Judge) and if the target isn't already sanctioned
        bool can_sanction(const Player* player) const;
        //Returns true if player has 7 or more coins
        bool can_coup() const;

        // === Turn lifecycle ===
        //Takes care of subracting extra turns at the start of the turn when there are
        //Overriden in Merchant.hpp 
        virtual void turn_cycle_start(); 
        //If sanctioned == true, set it to false. 
        void turn_end();
        // === Basic actions ===
        void gather();                  // +1 coin (Doesn't work if sanctioned)
        virtual void tax();             // +2 coins (Doesn't work if sanctioned, can be blocked by Governor). If Governor, +3 coins
        void bribe();                   // -4 coins, +2 extra turns (can be blocked)
        
        // Get number of extra turns
        size_t get_extra_turns();   
        // Used when the Player used an extra turn. -1 extra turns.      
        void subtract_turn();           
        // === Interactions with other players ===
        // +1 coin, can't target same player twice in a row. 
        // No coins gained if the target is a Merchhant (See Merchant.hpp)
        void arrest(const Player* arrested);   
        // -1 coin, unless Merchant. (See Merchant.hpp and Merchant.cpp for override) 
        virtual void when_arrested();           
        // -3 coins, prevents economic actions (Gather and tax). If sanctioned is Judge, -1 extra coin
        void sanction(const Player* sanctioned);
        virtual void when_sanctioned();         // Sets the "sanction" field to "true". If Baron, get 1 coin (See Baron.hpp and Baron.cpp)
        
        void coup();                            // -7 coins, eliminates a player (Elemination logic in Game class) 
        void when_couped();                     // Sets is_alive to false
        bool must_coup();                       //Returns true when the player has more than 10 coins
        // === Role-specific turn abilities. By default most players don't have them, only the Spy and Baron do===
        virtual bool can_use_turn_ability() const;
        //By default returns nothing. See Baron.hpp, Baron.cpp and Spy.hpp, Spy.cpp for more detail
        virtual void turn_ability();
        //In Player.hpp, returns 0. See Spy.hpp for the overriding of this method
        virtual size_t turn_ability(const Player* player);
        //Role specific realtime abilities. Called when certian actions can be prevented. The Spy, General, Governor and Judge have such abilities
        //(See Spy.hpp, General.hpp and Governor.hpp for more detail)
        virtual bool can_use_realtime_ability() const;
        //Returns nothing
        //See General.hpp, Spy.hpp and Judge.hpp for the overridings of this method
        virtual void realtime_ability() ;

        //Functions that uare called when an action is undone (Undones the action that was performed) 
        virtual void undo_tax();
        //If is_arrested = true, adds the amount of money taken from the arrest. If is_arrested = false, subctract the money that was taken at arrest 
        virtual void undo_arrest(bool is_arrested, Player* target);
        //Sets extra_turns to 0
        void undo_bribe();
        
    };

} 

#endif 
