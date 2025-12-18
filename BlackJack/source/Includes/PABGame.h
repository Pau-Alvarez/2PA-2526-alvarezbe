#pragma once
#ifndef __PABGAME_H__
#define __PABGAME_H__ 1

#include "..\Interfaces/IGames.h"
#include "..\Interfaces/IPlayer.h"
#include "..\Includes/PABTable.h"
#include "..\Includes/PABPlayer.h"
#include "..\Includes/PABRules.h"
#include <vector>
#include <utility>

/**
 * @class PABGame
 * @brief Main game controller implementing the IGame interface.
 * 
 * This class orchestrates the entire Blackjack game flow, including:
 * - Round management
 * - Player turns
 * - Betting phases
 * - Game state display
 * - End game calculations
 */
class PABGame : public IGame {
public:
    /**
     * @brief PABGame constructor.
     * 
     * @param Table Reference to the game table
     * @param players Vector of pointers to player objects
     */
    PABGame(ITable& Table, std::vector<IPlayer*> players, BaseRules& Rules)
        : table{ Table }, Players{ players }, rules{Rules} {}
    

    virtual void PlayGame() override;

private:
    ITable& table;                    
    std::vector<IPlayer*> Players;   
    BaseRules& rules; 

    /**
     * @brief Calculates the total value of a Blackjack hand.
     * 
     * @param player_hand The hand to calculate value for
     * @return int Total hand value (Aces counted as 1 or 11 optimally)
     */
    int GetHandvalue(ITable::Hand player_hand);
    
    /**
     * @brief Converts a card value to a display character.
     * 
     * @param player_card The card to convert
     * @return char Single character representation ('A', '2', ..., 'K', '0' for 10)
     */
    char GetCardValue(ITable::Card player_card);
    
    /**
     * @brief Handles a player's turn for a split hand.
     * 
     * @param player_index Index of the player
     * @param hand_index Index of the split hand (0 or 1)
     * @param win_point The winning point threshold for this game variant
     * 
     * Manages the decision-making process for a hand that resulted from a split.
     */
    void SolveSplitedHand(int player_index, int hand_index, int win_point);
    
    /**
     * @brief Manages a complete player turn.
     * 
     * @param player_index Index of the player whose turn it is
     * @param win_point The winning point threshold for this game variant
     * 
     * Handles all phases of a player's turn including:
     * - Displaying initial hand
     * - Processing player actions (hit, stand, double, split)
     * - Managing split hands
     */
    void PlayPlayerTurn(int player_index, int win_point);
    
    /**
     * @brief Displays the results of a completed round.
     * 
     * @param results RoundEndInfo containing round results
     * @param active_players Number of active players in the round
     * @param win_point The winning point threshold for this game variant
     * 
     * Shows dealer's final hand, each player's results, and money changes.
     */
    void ShowResults(ITable::RoundEndInfo& results, int active_players, int win_point);
    
    /**
     * @brief Prints a hand of cards to the console.
     * 
     * @param hand The hand to display
     * @param show_value Whether to show the calculated hand value
     * 
     * Displays cards in format: [Value][Value]... with optional total value.
     */
    void PrintHand(ITable::Hand hand, bool show_value = true);
    
    /**
     * @brief Checks if a hand is a Blackjack (natural win with initial cards).
     * 
     * @param hand The hand to check
     * @param win_point The winning point threshold for this game variant
     * @return bool True if the hand is a Blackjack, false otherwise
     */
    bool IsBlackjack(ITable::Hand hand, int win_point);
    
    /**
     * @brief Checks if the game should end.
     * 
     * @param active_player_count Output parameter for number of active players
     * @return bool True if game should end, false otherwise
     * 
     * Game ends when dealer is bankrupt or all players are bankrupt.
     */
    bool CheckGameOver(int& active_player_count);
    
    /**
     * @brief Displays final game results and rankings.
     * 
     * Shows final money amounts, ranks players by money,
     * and declares the winner(s).
     */
    void ShowFinalResults();
};

#endif  //__PABGAME_H__