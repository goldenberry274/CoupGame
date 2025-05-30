#include <wx/wx.h>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Game.hpp"
#include "Player.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "Governor.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace coup;

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    Game game;
    Player* currentPlayer;

    wxPanel* panel;
    wxBoxSizer* mainSizer;
    wxTextCtrl* logOutput;
    wxStaticText* playerInfo;
    wxChoice* targetChoice;

    void InitializePlayers();

    void OnGather(wxCommandEvent& event);
    void OnTax(wxCommandEvent& event);
    void UpdatePlayerInfo();
    void LogAction(const std::string& msg);
    void OnBribe(wxCommandEvent& event);
    void OnArrest(wxCommandEvent& event);
    void OnSanction(wxCommandEvent& event);
    void OnCoup(wxCommandEvent& event);
    void OnAbility(wxCommandEvent& event);
    void SkipTurn(wxCommandEvent& event);
    
    Player* GetTargetPlayer();

    void RefreshTargetList();
    void CheckForWinner();
    void DisableAllActionButtons();

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_BTN_GATHER = 1,
    ID_BTN_TAX,
    ID_BTN_ABILITY,
    ID_BTN_BRIBE,
    ID_BTN_ARREST,
    ID_BTN_SANCTION,
    ID_BTN_COUP,
    ID_CHOICE_TARGET,
    ID_BTN_SKIP_TURN
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_BTN_GATHER, MainFrame::OnGather)
    EVT_BUTTON(ID_BTN_TAX, MainFrame::OnTax)
    EVT_BUTTON(ID_BTN_BRIBE, MainFrame::OnBribe)
    EVT_BUTTON(ID_BTN_ARREST, MainFrame::OnArrest)
    EVT_BUTTON(ID_BTN_SANCTION, MainFrame::OnSanction)
    EVT_BUTTON(ID_BTN_COUP, MainFrame::OnCoup)
    EVT_BUTTON(ID_BTN_ABILITY, MainFrame::OnAbility)
    EVT_BUTTON(ID_BTN_SKIP_TURN, MainFrame::SkipTurn)

wxEND_EVENT_TABLE()

class GameApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(GameApp);

bool GameApp::OnInit() {
    MainFrame* frame = new MainFrame("Coup");
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600)),
      game(6)
{
    SetBackgroundColour(*wxBLACK);
    panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(*wxBLACK);
    currentPlayer = game.get_current_player();

    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxFont uiFont(wxFontInfo(14));
    // === TOP ROW ===
    wxBoxSizer* topRowSizer = new wxBoxSizer(wxHORIZONTAL);

   

    // Player Info Panel (top-left)
    wxStaticBoxSizer* infoBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Current Player Info");
    playerInfo = new wxStaticText(panel, wxID_ANY, "", wxPoint(0,0), wxSize(250, -1), wxALIGN_LEFT);
    playerInfo->SetForegroundColour(*wxWHITE);
    playerInfo->SetFont(uiFont);
    infoBox->Add(playerInfo, 1,  wxALL | wxALIGN_TOP, 10);
    // Placeholder for future left-side content (e.g. player cards, game board)

    topRowSizer->Add(infoBox, 0, wxALL | wxALIGN_TOP, 10);
    topRowSizer->AddStretchSpacer(1); // This pushes remaining space to the right instead

    mainSizer->Add(topRowSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer* midRowSizer = new wxBoxSizer(wxHORIZONTAL);

    // Make the action panel fill the entire horizontal space
    wxBoxSizer* actionPanel = new wxBoxSizer(wxVERTICAL);
    midRowSizer->Add(actionPanel, 1, wxALL | wxEXPAND, 10);  // <- Proportion = 1 and wxEXPAND
    midRowSizer->AddStretchSpacer(1);
    // Actions Group
    wxStaticBoxSizer* actionBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Actions");
    wxGridSizer* grid = new wxGridSizer(2, 4, 5, 6);
    grid->Add(new wxButton(panel, ID_BTN_GATHER, "Gather"), 0, wxEXPAND);
    grid->Add(new wxButton(panel, ID_BTN_TAX, "Tax"), 0, wxEXPAND);
    grid->Add(new wxButton(panel, ID_BTN_BRIBE, "Bribe"), 0, wxEXPAND);
    grid->Add(new wxButton(panel, ID_BTN_ARREST, "Arrest"), 0, wxEXPAND);
    grid->Add(new wxButton(panel, ID_BTN_SANCTION, "Sanction"), 0, wxEXPAND);
    grid->Add(new wxButton(panel, ID_BTN_COUP, "Coup"), 0, wxEXPAND);
    grid->Add(new wxButton(panel, ID_BTN_ABILITY, "Use Ability"), 0, wxEXPAND);
    grid->Add(new wxButton(panel, ID_BTN_SKIP_TURN, "Skip Turn"), 0, wxEXPAND);


    actionBox->Add(grid, 0, wxALL | wxEXPAND, 5);
    actionPanel->Add(actionBox, 0, wxALL | wxEXPAND, 5);

    // Target Choice
    wxStaticBoxSizer* targetBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Target Player");
    targetChoice = new wxChoice(panel, ID_CHOICE_TARGET);
    InitializePlayers();

    targetBox->Add(targetChoice, 0, wxALL | wxEXPAND, 5);
    actionPanel->Add(targetBox, 0, wxALL | wxEXPAND, 5);

    midRowSizer->Add(actionPanel, 0, wxALL | wxALIGN_TOP, 10);
    mainSizer->Add(midRowSizer, 0, wxEXPAND | wxALL, 5);

    // === BOTTOM: Log Output ===
    wxStaticBoxSizer* logBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Action Log");
    logOutput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                           wxTE_MULTILINE | wxTE_READONLY);
    logOutput->SetBackgroundColour(*wxBLACK);
    logOutput->SetForegroundColour(*wxWHITE);
    logOutput->SetFont(uiFont);
    logBox->Add(logOutput, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(logBox, 1, wxALL | wxEXPAND, 10);

    // Finalize
    panel->SetSizer(mainSizer);
    Centre();
    UpdatePlayerInfo();
    RefreshTargetList();
}
void MainFrame::InitializePlayers() {
    wxTextEntryDialog numDialog(this, "Enter number of players (2 to 6):", "Player Setup");
    if (numDialog.ShowModal() != wxID_OK) return;

    long numPlayers;
    if (!numDialog.GetValue().ToLong(&numPlayers) || numPlayers < 2 || numPlayers > 6) {
        wxMessageBox("Invalid number of players. Must be between 2 and 6.", "Error", wxICON_ERROR);
        return;
    }
    srand(time(0));
    for (int i = 0; i < numPlayers; ++i) {
        wxString wxName = wxGetTextFromUser(wxString::Format("Enter name for Player %d:", i + 1));
        std::string name = wxName.ToStdString();
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
            wxMessageBox(wxString::Format("Error creating player: %s", e), "Duplicate Name", wxOK | wxICON_ERROR);
            --i; // retry for this player
        }
    }
    std::vector<Player*> players = game.player_vector();
    if (players.empty()){
        std::cout << "There are no players" << endl;
        return;
    }
    

    currentPlayer = game.get_current_player();
    RefreshTargetList();
    UpdatePlayerInfo();
    wxMessageBox("Players initialized!", "Success", wxICON_INFORMATION);
}
void MainFrame::OnGather(wxCommandEvent& event) {
    try {
        game.gather(currentPlayer);
        LogAction(currentPlayer->name() + " gathered 1 coin.");
        UpdatePlayerInfo();
        game.update_turn();
        currentPlayer = game.get_current_player();
        RefreshTargetList();
        UpdatePlayerInfo();
    } catch (char const* e) {
        wxMessageBox(e, "Error", wxICON_ERROR);
    }
}

void MainFrame::OnTax(wxCommandEvent& event) {
    try {
        game.tax(currentPlayer);
        // Check if anyone can prevent

        vector<Player*> preventers = game.prevent_check(currentPlayer, "Tax");
        if (!preventers.empty()) {
            for (size_t i = 0; i < preventers.size(); i++){
                Player* p = preventers[i]; // Assuming first has the chance
                wxString msg = wxString::Format("%s, do you want to prevent %s's Tax?", 
                                             p->name(), currentPlayer->name());
                int choice = wxMessageBox(msg, "Prevent Tax", wxYES_NO | wxICON_QUESTION);
                if (choice == wxYES) {
                    game.prevent_action(currentPlayer, "Tax");
                    LogAction(p->name() + " prevented " + currentPlayer->name() + "'s Tax!");
                    UpdatePlayerInfo();
                    game.update_turn();
                    currentPlayer = game.get_current_player();
                    RefreshTargetList();
                    UpdatePlayerInfo();
                    return; // Exit early, action was blocked
                }
            }
            
        }

        // No prevention or not prevented
        
        if (currentPlayer->role() == "Governor")
            LogAction(currentPlayer->name() + " gained 3 coins via Tax. (Governor bonus)");
        else
            LogAction(currentPlayer->name() + " gained 2 coins via Tax.");
        UpdatePlayerInfo();
        game.update_turn();
        currentPlayer = game.get_current_player();
        RefreshTargetList();
        UpdatePlayerInfo();
    } catch (char const* e) {
        wxMessageBox(e, "Error", wxICON_ERROR);
    }
}


void MainFrame::OnAbility(wxCommandEvent& event) {
    try {
        if (currentPlayer->role() == "Baron") {
            game.turn_ability(currentPlayer);
            LogAction(currentPlayer->name() + " invested 3 coins and earned 6! (Baron ability)");
        } else if (currentPlayer->role() == "Spy"){
            Player* target = GetTargetPlayer();
            if (!target || target == currentPlayer) {
                wxMessageBox("Select a valid target.", "Invalid", wxICON_WARNING);
                return;
            }
            std::string message = currentPlayer->name() + " Spied on " + target->name() + "! " + target->name()
                                    + " has " + std::to_string(game.turn_ability(currentPlayer, target)) + " coins";
            LogAction(message);
            return;
        } 
        else {
            wxMessageBox("This player has no turn ability.", "Invalid", wxICON_WARNING);
            return;
        }

        UpdatePlayerInfo();
        game.update_turn();
        currentPlayer = game.get_current_player();
        RefreshTargetList();
        UpdatePlayerInfo();
    } catch (char const* e) {
        wxMessageBox(e, "Error", wxICON_ERROR);
    }
}


void MainFrame::LogAction(const std::string& msg){
    if (logOutput){
        wxString currentLog = logOutput->GetValue();
        currentLog += msg + "\n";
        logOutput->SetValue(currentLog);
        logOutput->ShowPosition(logOutput->GetLastPosition());
    }
}

void MainFrame::UpdatePlayerInfo() {
    std::string info = "Name: " + currentPlayer->name() +
                       "\nRole: " + currentPlayer->role() +
                       "\nCoins: " + std::to_string(currentPlayer->coins()) +
                       "\nSanctioned: " + std::string(currentPlayer->is_sanctioned() ? "Yes" : "No") +
                       "\nExtra Turns: " + std::to_string(currentPlayer->get_extra_turns());
    playerInfo->SetLabel(info);
}

Player* MainFrame::GetTargetPlayer() {
    
    wxString idx = targetChoice->GetStringSelection();
    if (idx == "") return nullptr;
    Player* target = nullptr;
    for (size_t i = 0; i < game.player_vector().size();i++){
        if (game.player_vector().at(i)->name() == idx)
            target = game.player_vector().at(i);
    }
    return target;
}

void MainFrame::OnBribe(wxCommandEvent& event) {
    try {
        game.bribe(currentPlayer);
        vector<Player*> preventers = game.prevent_check(currentPlayer, "Bribe");
        if (!preventers.empty()) {
            for (size_t i = 0; i < preventers.size(); i++){
                Player* p = preventers[i]; // Assuming first has the chance
                wxString msg = wxString::Format("%s, do you want to prevent %s's Bribe?", 
                                             p->name(), currentPlayer->name());
                int choice = wxMessageBox(msg, "Prevent Bribe", wxYES_NO | wxICON_QUESTION);
                if (choice == wxYES) {
                    game.prevent_action(currentPlayer, "Bribe");
                    LogAction(p->name() + " prevented " + currentPlayer->name() + "'s Bribe!");
                    UpdatePlayerInfo();
                    game.update_turn();
                    currentPlayer = game.get_current_player();
                    RefreshTargetList();
                    UpdatePlayerInfo();
                    return; // Exit early, action was blocked
                }
            }
        }
        LogAction(currentPlayer->name() + " used Bribe (+2 extra turns).");
        UpdatePlayerInfo();
        game.update_turn();
        currentPlayer = game.get_current_player();
        RefreshTargetList();
        UpdatePlayerInfo();
    } catch (char const* error) {
        wxMessageBox(error, "Error", wxICON_ERROR);
    }
}

void MainFrame::OnArrest(wxCommandEvent& event) {
    try {
        if (!targetChoice->IsEnabled() || targetChoice->GetSelection() == wxNOT_FOUND) {
            wxMessageBox("Select a valid target to arrest.", "No Target", wxICON_WARNING);
            return;
        }

        
        Player* target = GetTargetPlayer();
        game.arrest(currentPlayer, target);

        vector<Player*> preventers = game.prevent_check(currentPlayer, "Arrest");
        if (!preventers.empty()) {
            for (size_t i = 0; i < preventers.size(); i++){
                Player* p = preventers[i]; // Assuming first has the chance
                wxString msg = wxString::Format("%s, do you want to prevent %s's Arrest?", 
                                             p->name(), target->name());
                int choice = wxMessageBox(msg, "Prevent Arrest", wxYES_NO | wxICON_QUESTION);
                if (choice == wxYES) {
                    game.prevent_arrest(currentPlayer, target);
                    LogAction(p->name() + " prevented " + target->name() + "'s Arrest!");
                    UpdatePlayerInfo();
                    game.update_turn();
                    currentPlayer = game.get_current_player();
                    RefreshTargetList();
                    UpdatePlayerInfo();
                    return; // Exit early, action was blocked
                }
            }
        }

        LogAction(currentPlayer->name() + " arrested " + target->name() + ".");
        UpdatePlayerInfo();
        game.update_turn();
        currentPlayer = game.get_current_player();
        RefreshTargetList();
        UpdatePlayerInfo();
    } catch (char const* e) {
        wxMessageBox(e, "Error", wxICON_ERROR);
    }
}


void MainFrame::OnSanction(wxCommandEvent& event) {
    Player* target = GetTargetPlayer();
    if (!target || target == currentPlayer) {
        wxMessageBox("Select a valid target.", "Invalid", wxICON_WARNING);
        return;
    }

    try {
        game.sanction(currentPlayer, target);
        LogAction(currentPlayer->name() + " sanctioned " + target->name() + ".");
        UpdatePlayerInfo();
        game.update_turn();
        currentPlayer = game.get_current_player();
        if (target->role() == "Baron")
            LogAction(target->name() + " got 1 coin as conpenscation! (Baron ability)");
        RefreshTargetList();
        UpdatePlayerInfo();
    } catch (char const* e) {
        wxMessageBox(e, "Error", wxICON_ERROR);
    }
}

void MainFrame::OnCoup(wxCommandEvent& event) {
    Player* target = GetTargetPlayer();
    if (!target || target == currentPlayer) {
        wxMessageBox("Select a valid target.", "Invalid", wxICON_WARNING);
        return;
    }

    try {
        Player* target = GetTargetPlayer();

        vector<Player*> preventers = game.prevent_check(currentPlayer, "Coup");
        if (!preventers.empty()) {
            for (size_t i = 0; i < preventers.size(); i++){
                Player* p = preventers[i]; 
                wxString msg = wxString::Format("%s, do you want to prevent %s's Coup on %s?", 
                                             p->name(), currentPlayer->name(), target->name());
                int choice = wxMessageBox(msg, "Prevent Coup", wxYES_NO | wxICON_QUESTION);
                if (choice == wxYES) {
                    game.prevent_coup(currentPlayer, p);
                    LogAction(p->name() + " prevented " + currentPlayer->name() + "'s Coup on " + target->name() + "!");
                    UpdatePlayerInfo();
                    game.update_turn();
                    currentPlayer = game.get_current_player();
                    RefreshTargetList();
                    UpdatePlayerInfo();
                    return; // Exit early, action was blocked
                }
            }
        }
        game.coup(currentPlayer, target);

        LogAction(currentPlayer->name() + " performed a Coup on " + target->name() + ". " + target->name() + " has been eliminated from play");
        UpdatePlayerInfo();
        game.update_turn();
        currentPlayer = game.get_current_player();
        RefreshTargetList();
        UpdatePlayerInfo();
        CheckForWinner();

    } catch (char const* e) {
        wxMessageBox(e, "Error", wxICON_ERROR);
    }
}
void MainFrame::SkipTurn(wxCommandEvent& event) {
    
    LogAction(currentPlayer->name() + " skipped their turn.");
    game.update_turn();
    currentPlayer = game.get_current_player();
    RefreshTargetList();
    UpdatePlayerInfo();
        
    
}

void MainFrame::RefreshTargetList() {
    wxArrayString names;
    for (Player* p : game.player_vector()) {
        if (p->alive() && p != currentPlayer) {
            names.Add(p->name());
        }
    }
    
    std::vector<std::string> playerNames = game.players();
    
    targetChoice->Clear();
    
    for (std::string name : playerNames) {
        
        if (name != currentPlayer->name()){
            targetChoice->Append(name);
        }
    }
}

void MainFrame::CheckForWinner() {
    std::string winner = game.winner();
    if (winner != "") {
        wxMessageBox(winner + " won the game!", "Game Over", wxOK | wxICON_INFORMATION);
        LogAction(winner + " is the last one standing and the winner of the game!");
        DisableAllActionButtons(); // Optional: disable buttons to end the game
        sf::Music music;
        if (!music.openFromFile("Victory.ogg")) {
            std::cout << "Failed to open Victory.ogg\n";
            return;
        }

        music.play();

        // Wait while music is playing
        while (music.getStatus() == sf::Music::Playing) {
            sf::sleep(sf::milliseconds(100));
        }

        
    }
}
void MainFrame::DisableAllActionButtons() {
    for (wxWindow* child : panel->GetChildren()) {
        wxButton* btn = dynamic_cast<wxButton*>(child);
        if (btn) {
            btn->Disable();
        }
    }
    targetChoice->Disable();
}
