#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Player.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Governor.hpp"
#include "Game.hpp"

using namespace std;
using namespace coup;

TEST_CASE("Player constructor validation") {
    CHECK_THROWS_AS(Player("John", "InvalidRole"), std::invalid_argument);
}


TEST_CASE("Player copy constructor copies all relevant fields correctly") {
    Player original("Alice", "Merchant");
    
    // Set some state
    original.change_coins(5, true);          
    original.when_sanctioned();     
    original.when_couped();         
    original.bribe();       // -4 coins
    Player* target = new Player("Bob", "Spy");
    target->change_coins(2, true); 
    original.arrest(target); // Simulate arresting someone, +1 coin

    // Perform copy
    Player copy(original);

    // Now assert fields are copied
    CHECK(copy.name() == "Alice");
    CHECK(copy.role() == "Merchant");
    CHECK(copy.coins() == 2);
    CHECK(copy.is_sanctioned() == true);
    CHECK(copy.alive() == false);
    CHECK(copy.get_extra_turns() == 2);
    CHECK(copy.last_player_arrested() == target->name());

    // Check that they are different objects
    CHECK(&copy != &original);
    delete target;
}


TEST_CASE("Basic attributes and getters") {
    Player p("Alice", "Governor");

    CHECK(p.name() == "Alice");
    CHECK(p.role() == "Governor");
    CHECK(p.coins() == 0);
    CHECK(!p.is_sanctioned());
    CHECK(p.get_extra_turns() == 0);
    CHECK(p.alive());
}

TEST_CASE("Spy copy constructor") {
    Game game(2);
    Spy* original = new Spy(game, "OriginalSpy");
    Baron*  addition = new Baron(game, "Add");
    original->change_coins(3, true);
    cout << "Original has " << original->coins() << " coins" << endl;
    original->when_sanctioned();
    Spy* copy = new Spy(*(original));
    cout << "Copy has " << copy->coins() << " coins" << endl;
    CHECK(copy->name() == "OriginalSpy");
    CHECK(copy->role() == "Spy");
    CHECK(copy->coins() == 3);
    CHECK(copy->is_sanctioned() == true);
    CHECK(copy->alive() == true);
    delete copy;
}

TEST_CASE("Baron copy constructor") {
    Game game(2);
    Baron* original = new Baron(game, "OriginalBaron");
    Spy* addition = new Spy(game, "Add");

    original->change_coins(5, true);
    original->when_sanctioned();

    Baron* copy = new Baron(*original);

    CHECK(copy->name() == "OriginalBaron");
    CHECK(copy->role() == "Baron");
    CHECK(copy->coins() == 6);  // when_sanctioned gives +1
    CHECK(copy->is_sanctioned() == true);
    CHECK(copy->alive() == true);
    delete copy;
}

TEST_CASE("Governor copy constructor") {
    Game game(2);
    Governor* original = new Governor(game, "OriginalGovernor");
    Baron* addition = new Baron(game, "Add");

    original->change_coins(2, true);

    Governor* copy = new Governor(*original);

    CHECK(copy->name() == "OriginalGovernor");
    CHECK(copy->role() == "Governor");
    CHECK(copy->coins() == 2);
    CHECK(copy->alive() == true);
    delete copy;
}

TEST_CASE("General copy constructor") {
    Game game(2);
    General* original = new General(game, "OriginalGeneral");
    Baron* addition = new Baron(game, "Add");

    original->change_coins(5, true);

    General* copy = new General(*original);

    CHECK(copy->name() == "OriginalGeneral");
    CHECK(copy->role() == "General");
    CHECK(copy->coins() == 5);
    CHECK(copy->alive() == true);
    delete copy;
}

TEST_CASE("Merchant copy constructor") {
    Game game(2);
    Merchant* original = new Merchant(game, "OriginalMerchant");
    Baron* addition = new Baron(game, "Add");

    original->change_coins(4, true);
    original->bribe();

    Merchant* copy = new Merchant(*original);

    CHECK(copy->name() == "OriginalMerchant");
    CHECK(copy->role() == "Merchant");
    CHECK(copy->coins() == 0);
    CHECK(copy->get_extra_turns() == 2);
    CHECK(copy->alive() == true);
    delete copy;
}

TEST_CASE("Judge copy constructor") {
    Game game(2);
    Judge* original = new Judge(game, "OriginalJudge");
    Baron* addition = new Baron(game, "Add");

    original->change_coins(1, true);

    Judge* copy = new Judge(*original);

    CHECK(copy->name() == "OriginalJudge");
    CHECK(copy->role() == "Judge");
    CHECK(copy->coins() == 1);
    CHECK(copy->alive() == true);
    delete copy;
}

TEST_CASE("Game copy constructor") {
    // Original game setup
    Game original_game(3);
    Spy* spy = new Spy(original_game, "SpyPlayer");
    General* general = new General(original_game, "GeneralPlayer");
    Baron* baron = new Baron(original_game, "BaronPlayer");

    spy->change_coins(2, true);
    general->change_coins(5, true);
    baron->change_coins(3, true);
    baron->when_sanctioned();

    

    // Copy the game
    Game copied_game(original_game);

    // Ensure player count is the same
    CHECK(copied_game.player_vector().size() == 3);

    // Get copied players
    std::vector<Player*> copied_players = copied_game.player_vector();

    // Check each player's role and properties
    CHECK(copied_players[0]->name() == "SpyPlayer");
    CHECK(copied_players[0]->role() == "Spy");
    CHECK(copied_players[0]->coins() == 2);

    CHECK(copied_players[1]->name() == "GeneralPlayer");
    CHECK(copied_players[1]->role() == "General");
    CHECK(copied_players[1]->coins() == 5);

    CHECK(copied_players[2]->name() == "BaronPlayer");
    CHECK(copied_players[2]->role() == "Baron");
    CHECK(copied_players[2]->coins() == 4); // when_sanctioned adds 1
    CHECK(copied_players[2]->is_sanctioned() == true);

    // Verify current_player is correctly copied
    CHECK(copied_game.get_current_player()->name() == "SpyPlayer");
}
TEST_CASE("Coin manipulation") {
    Player p("Bob", "General");

    CHECK(p.change_coins(5, true) == true);
    CHECK(p.coins() == 5);

    CHECK(p.change_coins(3, false) == true);
    CHECK(p.coins() == 2);

    CHECK(p.change_coins(10, false) == false); // More than owned
    CHECK(p.coins() == 0); // Should clamp to 0
}

TEST_CASE("Gather, Tax, and Bribe") {
    Player p("Eva", "Judge");
    CHECK_THROWS_AS(p.bribe(), char const* );
    p.gather();
    CHECK(p.coins() == 1);

    p.tax();
    CHECK(p.coins() == 3);

    p.change_coins(2, true); // total = 5
    CHECK(p.can_bribe());

    p.bribe();
    CHECK(p.coins() == 1);
    CHECK(p.get_extra_turns() == 2);

    p.undo_bribe();
    CHECK(p.get_extra_turns() == 0);
}

TEST_CASE("Coup logic") {
    Player p("Kira", "Spy");

    p.change_coins(10, true);
    CHECK(p.can_coup());
    CHECK(p.must_coup());

    p.coup();
    CHECK(p.coins() == 3);

    p.when_couped();
    CHECK(!p.alive());
}

TEST_CASE("Arrest and Sanction mechanics") {
    Player gov("Gov", "Governor");
    Player merch("Mina", "Merchant");

    merch.change_coins(2, true); // Merchant needs at least 2 coins to be arrested
    CHECK(gov.can_arrest(&merch));

    gov.arrest(&merch);
    CHECK(gov.coins() == 0);
    CHECK(gov.last_player_arrested() == "Mina");

    merch.when_arrested();
    CHECK(merch.coins() == 1);

    gov.change_coins(5, true);
    CHECK(gov.can_sanction(&merch));

    gov.sanction(&merch);
    CHECK(gov.coins() == 2);

    merch.when_sanctioned();
    CHECK(merch.is_sanctioned());

    merch.turn_end();
    CHECK(!merch.is_sanctioned());
}

TEST_CASE("Sanity checks for invalid arguments") {
    Player a("A", "Baron");

    CHECK_THROWS(a.can_arrest(nullptr));
    CHECK_THROWS(a.can_sanction(nullptr));
    CHECK_THROWS(a.arrest(nullptr));
    CHECK_THROWS(a.sanction(nullptr));
}

TEST_CASE("Ability check - Normal players"){
    Player a("A", "Baron");
    CHECK(!a.can_use_realtime_ability());
    CHECK(!a.can_use_realtime_ability());
}
TEST_CASE("Turn ability checks - Spy and Baron"){
    Game game(6);
    Spy* spy = new Spy(game, "Avi");
    Baron* baron = new Baron(game, "Baron");
    CHECK(spy->can_use_turn_ability());
    CHECK(!baron->can_use_turn_ability());
    baron->change_coins(3, true);
    CHECK(baron->can_use_turn_ability());

    CHECK_THROWS(spy->can_arrest(nullptr));
    CHECK_THROWS(spy->can_arrest(spy));
    baron->turn_ability();
    CHECK(baron->coins() == 6);

}

TEST_CASE("Realtime ability checks - General, Judge and Spy"){
    Game game(6);
    Spy* spy = new Spy(game, "Avi");
    General* Gen = new General(game, "Gen");
    Judge* judge = new Judge(game, "Judy");

    CHECK(spy->can_use_realtime_ability());
    CHECK(!Gen->can_use_realtime_ability());
    CHECK(judge->can_use_realtime_ability());
    Gen->change_coins(5, true);
    CHECK(Gen->can_use_realtime_ability());
    
}

TEST_CASE("Game setup and basic functionality") {
    Game game(4);
    Player* alice = new Player("Alice", "Spy");
    Player* bob = new Player("Bob", "General");
    Player* charlie = new Player("Gil", "Merchant");

    game.add_player(alice);
    game.add_player(bob);
    game.add_player(charlie);

    CHECK(game.players().size() == 3);
    CHECK(game.turn() == "Alice");

    SUBCASE("Adding duplicate player name should throw") {
        Player* dupe = new Player("Alice", "Merchant");
        CHECK_THROWS_WITH(game.add_player(dupe), "Invalid argument: A player with this name already exists");
        delete dupe;
    }

    SUBCASE("Adding more players than allowed should throw") {
        Game smallGame(2);
        Player* p1 = new Player("One", "Spy");
        Player* p2 = new Player("Two", "Baron");
        Player* p3 = new Player("Three", "General");

        smallGame.add_player(p1);
        smallGame.add_player(p2);
        CHECK_THROWS_WITH(smallGame.add_player(p3), "Invalid argument: there are enough players already");

        delete p3; // Only p3 not owned by the game
    }
}

TEST_CASE("Gameplay actions and validation") {
    Game game(3);
    Player* p1 = new Player("One", "Spy");
    Player* p2 = new Player("Two", "Baron");
    Player* p3 = new Player("Three", "General");

    game.add_player(p1);
    game.add_player(p2);
    game.add_player(p3);

    SUBCASE("Valid gather action") {
        CHECK_NOTHROW(game.gather(p1));
    }

    SUBCASE("Invalid gather from wrong player") {
        CHECK_THROWS_WITH(game.gather(p2), "Not your turn");
    }

    SUBCASE("Sanctioned player cannot gather") {
        p1->set_sanction(true);
        CHECK_THROWS_WITH(game.gather(p1), "Illegal action: player sanctioned");
    }

    SUBCASE("Must coup prevents gather") {
        p1->change_coins(10, true);
        CHECK_THROWS_WITH(game.gather(p1), "Illegal action: Player must perofrm a Coup");
    }

    SUBCASE("Invalid coup against self") {
        p1->change_coins(10, true); 
        CHECK_THROWS_WITH(game.coup(p1, p1), "Invalid argument: Player cannot perform Coup on themselves: Coup");
    }

    SUBCASE("Prevent check throws on wrong player") {
        CHECK_THROWS_WITH(game.prevent_check(p2, "Tax"), "Invalid argument: Not your turn");
    }

    SUBCASE("Prevent check throws on invalid action") {
        CHECK_THROWS_WITH(game.prevent_check(p1, "InvalidAction"), "Invalid argument: Got action that cannot be canceled");
    }
}

TEST_CASE("Winner detection") {
    Game game(2);
    Player* a = new Player("A", "Baron");
    Player* b = new Player("B", "Spy");

    game.add_player(a);
    game.add_player(b);

    CHECK(game.winner() == "");

    a->when_couped(); 
    CHECK(game.winner() == "B");

   
}

TEST_CASE("Governor class basic functionality") {
    Game game(2);

    Governor* gov = new Governor(game, "Gov");

    CHECK(game.player_vector().size() == 1);
    CHECK(game.player_vector()[0]->name() == "Gov");

    SUBCASE("Tax adds correct amount of coins") {
        size_t initial = gov->coins();
        gov->tax();
        CHECK(gov->coins() ==  3);
    }

    SUBCASE("Undo tax reverses the coin addition") {
        gov->tax();
        int afterTax = gov->coins();
        gov->undo_tax();
        CHECK(gov->coins() == afterTax - 3);
    }

    SUBCASE("can_use_realtime_ability returns true") {
        CHECK(gov->can_use_realtime_ability() == true);
    }
    
}

TEST_CASE("Spy class behavior") {
    Game game(2);

    Spy* spy = new Spy(game, "Sneaky");
    Governor* gov = new Governor(game, "Target"); // Target player to spy on

    CHECK(spy->role() == "Spy");
    CHECK(game.players().size() == 2);

    SUBCASE("Spy can use turn ability") {
        CHECK(spy->can_use_turn_ability() == true);
    }

    SUBCASE("Spy can use real-time ability") {
        CHECK(spy->can_use_realtime_ability() == true);
    }

    SUBCASE("Spy can see target's coins") {
        gov->change_coins(4, true); // Add 4 coins
        CHECK(spy->turn_ability(gov) == 4);
    }

    SUBCASE("Spy throws on nullptr input") {
        CHECK_THROWS_WITH(spy->turn_ability(nullptr), "Invalid argument: Null player given");
    }

    SUBCASE("Spy throws on spying self") {
        CHECK_THROWS_WITH(spy->turn_ability(spy), "Invliad argument: Cannot spy on yourself");
    }

}

TEST_CASE("Merchant class behavior") {
    Game game(3);

    Merchant* merchant = new Merchant(game, "Marco");
    Spy* spy = new Spy(game, "Sneaky"); // Non-merchant player for testing

    SUBCASE("turn_cycle_start adds coin if condition met") {
        merchant->change_coins(3, true);
        merchant->turn_cycle_start();
        CHECK(merchant->coins() == 3 + 1);
    }
    
    SUBCASE("turn_cycle_start does not add coin if below threshold") {
        merchant->change_coins(3 - 1, true);
        merchant->turn_cycle_start();
        CHECK(merchant->coins() == 3 - 1);
    }



    SUBCASE("when_arrested with not enough coins") {
        merchant->change_coins(3 - 1, true);
        merchant->when_arrested();
        CHECK(merchant->coins() == 0);
    }
    
    SUBCASE("when_arrested with enough coins") {
        merchant->change_coins(2, true);
        merchant->when_arrested();
        CHECK(merchant->coins() == 2 - 2);
    }
    
    SUBCASE("undo_arrest throws on null pointer") {
        CHECK_THROWS_WITH(merchant->undo_arrest(true, nullptr), "Invalid argument: Null pointer");
    }
    
    SUBCASE("undo_arrest when arrested restores ARREST_DEMAND") {
        merchant->change_coins(5, true);
        merchant->undo_arrest(true, spy);
        CHECK(merchant->coins() == 5 + 2);
    }
    
    SUBCASE("undo_arrest on non-arrested target that is a Merchant does nothing") {
        Merchant* otherMerchant = new Merchant(game, "Mia");
        merchant->change_coins(5, true);
        merchant->undo_arrest(false, otherMerchant);
        CHECK(merchant->coins() == 5);
    }
    
    SUBCASE("undo_arrest on non-Merchant target removes 1 coin") {
        merchant->change_coins(5, true);
        merchant->undo_arrest(false, spy);
        CHECK(merchant->coins() == 4);
    }

}

TEST_CASE("General class behavior") {
    Game game(3);

    General* general = new General(game, "Commander");
    Spy* spy = new Spy(game, "Sneaky");

    SUBCASE("can_use_realtime_ability returns false if coins < 5") {
        general->change_coins(4, true);
        CHECK_FALSE(general->can_use_realtime_ability());
    }

    SUBCASE("can_use_realtime_ability returns true if coins >= 5") {
        general->change_coins(5, true);
        CHECK(general->can_use_realtime_ability());
    }

    SUBCASE("realtime_ability throws if not enough coins") {
        general->change_coins(4, true);
        CHECK_THROWS_WITH(general->realtime_ability(), "Invalid action. General must have 5 coins or more to prevent a coup");
    }

    SUBCASE("realtime_ability deducts 5 coins if possible") {
        general->change_coins(6, true);
        general->realtime_ability();
        CHECK(general->coins() == 1);
    }

    SUBCASE("when_arrested does nothing") {
        general->change_coins(3, true);
        general->when_arrested();
        CHECK(general->coins() == 3);
    }

    SUBCASE("undo_arrest throws if null target") {
        CHECK_THROWS_WITH(general->undo_arrest(false, nullptr), "Invalid argument: Null pointer");
    }

    SUBCASE("undo_arrest does nothing if arrested is true") {
        general->change_coins(5, true);
        general->undo_arrest(true, spy);
        CHECK(general->coins() == 5);
    }

    SUBCASE("undo_arrest does nothing if target is Merchant") {
        Merchant* merchant = new Merchant(game, "Mia");
        general->change_coins(5, true);
        general->undo_arrest(false, merchant);
        CHECK(general->coins() == 5);
    }

    SUBCASE("undo_arrest subtracts 1 coin if target is not Merchant and not arrested") {
        general->change_coins(5, true);
        general->undo_arrest(false, spy);
        CHECK(general->coins() == 4);
    }

}

TEST_CASE("Baron class behavior") {
    Game game(2);
    Baron* baron = new Baron(game, "Wealthy");

    SUBCASE("can_use_turn_ability returns false if coins < INVESTMENT_PAYMENT") {
        baron->change_coins(2, true);
        CHECK_FALSE(baron->can_use_turn_ability());
    }

    SUBCASE("can_use_turn_ability returns true if coins >= INVESTMENT_PAYMENT") {
        baron->change_coins(3, true);
        CHECK(baron->can_use_turn_ability());
    }

    SUBCASE("turn_ability adds correct profit minus cost") {
        CHECK_THROWS_WITH(baron->turn_ability(), "Not enough coins to use ability");
        baron->change_coins(3, true);
        size_t before = baron->coins();
        baron->turn_ability();
        CHECK(baron->coins() == before + 6 - 3);
    }

    SUBCASE("when_sanctioned sets sanctioned and adds 1 coin") {
        int before = baron->coins();
        baron->when_sanctioned();
        CHECK(baron->coins() == before + 1);
        CHECK(baron->is_sanctioned() == true);
    }

}


TEST_CASE("Game::prevent_arrest - valid and invalid cases") {
    Game game(2);
    Judge* judge = new Judge(game, "Judge1");
    Spy* spy = new Spy(game, "Spy1");

    // Give current player (Judge) coins and simulate arrest
    judge->change_coins(5, true); 
    spy->change_coins(5, true);

    // Spy arrests judge
    
    spy->arrest(judge);

    // Now prevent arrest
    game.arrest(judge, spy);
    CHECK_NOTHROW(game.prevent_arrest(judge, spy));

    // Invalid: nullptr input
    CHECK_THROWS_WITH(game.prevent_arrest(nullptr, judge), "Invalid argument: Null pointer: prevent_arrest");
    CHECK_THROWS_WITH(game.prevent_arrest(spy, nullptr), "Invalid argument: Null pointer: prevent_arrest");

    // Invalid: wrong turn

    CHECK_THROWS_WITH(game.prevent_arrest(spy, judge), "Not your turn");
}

TEST_CASE("Game::prevent_coup - valid and invalid cases") {
    Game game(3);
    Spy* spy = new Spy(game, "Spy1");

    General* general = new General(game, "General1");

    spy->change_coins(7, true);  // enough to coup
    general->change_coins(5, true);  // enough to prevent

    // Valid prevention
    CHECK_NOTHROW(game.prevent_coup(spy, general));
    CHECK(general->coins() == 0); // spent 5 to prevent

    // Invalid: nullptrs
    CHECK_THROWS_WITH(game.prevent_coup(nullptr, general), "Invalid arguemnt: Null pointer given");
    CHECK_THROWS_WITH(game.prevent_coup(spy, nullptr), "Invalid arguemnt: Null pointer given");

    // Invalid: wrong turn
    game.update_turn();
    CHECK_THROWS_WITH(game.prevent_coup(spy, general), "Invalid argument: Not your turn");

    game.update_turn();
    // Invalid: not enough coins
    spy->change_coins(7, true);
    general->change_coins(3, true);
    CHECK_THROWS_WITH(game.prevent_coup(spy, general), "Invalid argument: Preventer doesn't have enough coins to prevent the coup");

    // Invalid: wrong role
    Merchant* merchant = new Merchant(game, "Merchant1");
    merchant->change_coins(10, true);
    CHECK_THROWS_WITH(game.prevent_coup(spy, merchant), "Invalid argument: Only a General can prevent a coup");
}
