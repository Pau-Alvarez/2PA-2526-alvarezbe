#pragma once
#ifndef __PABTABLE_H__
#define __PABTABLE_H__ 1
#include "..\Interfaces/ITable.h"
#include "..\Interfaces/Rules.h"
#include <vector>

/**
 * @class PABTable
 * @brief Concrete implementation of the ITable interface for Blackjack game.
 *
 * This class manages the table state including players, cards, bets,
 * and dealer logic. Implements all operations defined in ITable.
 */
class PABTable : public ITable {
public:
	virtual Hand GetHand(int player_index, int hand_index) const;

	virtual int GetNumberOfHands(int player_index) const;

	virtual int GetPlayerCurrentBet(int player_index, int hand_index) const;

	virtual int GetPlayerMoney(int player_index) const;

	virtual int GetPlayerInitialBet(int player_index) const;

	virtual Card GetDealerCard() const;

	virtual int DealerMoney() const;

	virtual void DealCard(int player_index, int hand_index);

	virtual Result PlayInitialBet(int player_index, int money);

	virtual Result PlaySafeBet(int player_index);

	virtual Result ApplyPlayerAction(int player_index, int hand_index,
		Action action);

	virtual void StartRound();

	virtual RoundEndInfo FinishRound();

	virtual void CleanTable();

	virtual ~PABTable() = default;

	/**
	 * @brief Gets the winning point threshold from the rules.
	 * @return int The score needed to win
	 */
	int GetWinPoint() const { return rules.GetWinPoint(); }

	/**
	 * @brief Gets the minimum bet from the rules.
	 * @return int The minimum bet amount
	 */
	int GetMinimumBet() const { return rules.MinimumInitialBet(); }

	/**
	 * @brief Gets the number of initial cards from the rules.
	 * @return int Number of cards dealt initially
	 */
	int GetInitialCards() const { return rules.InitialCards(); }

	/**
	 * @brief Fills the deck with cards according to the configured number of decks.
	 *
	 * Creates all necessary cards for the game based on the rules.
	 * Cards are generated for each value (1-13) and each suit (Hearts, Clubs, etc.).
	 */
	void FillDeck();

	/**
	 * @brief Shuffles the deck of cards using a random shuffle algorithm.
	 *
	 * Uses std::mt19937 to generate a random shuffle of the cards.
	 * First clears the current deck, then fills it with FillDeck() and finally shuffles it.
	 */
	void ShufleDeck();

	/**
	 * @brief Calculates the real value of a specific hand for a player.
	 *
	 * @param player_index Player index (0-5 for players, 6 for dealer)
	 * @param hand_index Hand index (0 or 1 in case of split)
	 *
	 * Calculates the hand value considering that Aces can be 1 or 11,
	 * and J, Q, K cards are worth 10. The calculation is stored in the player's structure.
	 */
	void CalculateRealHandValue(int player_index, int hand_index);

	/**
	 * @brief Initializes the table structure with empty hands and bets.
	 *
	 * Sets up all necessary data structures to start a round,
	 * including creating vectors for hands, bets, and hand values.
	 */
	void CreateTable();

	/**
	 * @brief PABTable constructor.
	 *
	 * @param game_rules Reference to the game rules to be used.
	 *
	 * Initializes the table with the correct number of players (6 players + dealer),
	 * assigns initial money according to the rules, and calls CreateTable() to
	 * set up the data structures.
	 */
	PABTable(BaseRules& game_rules);

private:

	/// Maximum number of real players (excluding dealer)
	static const int kMaxRealPlayers = 6;

	/// Maximum number of cards in a standard deck
	static const int kMaxcards = 52;

	/**
	 * @struct PlayerInfo
	 * @brief Contains all information about a player at the table.
	 */
	struct PlayerInfo {

		/**
		 * @struct HandInfo
		 * @brief Information about a player's hand(s)
		 */
		struct HandInfo {
			std::vector<Hand>  hands;
			std::vector<int>  player_bet;
			int safe_bet = 0;
			std::vector<int> total_hand_value;
		};
		HandInfo hand_info;

		int  player_money = 0;
	};
	std::vector<PlayerInfo> Players;

	BaseRules& rules;

	std::vector<Card> Deck;

	/**
	 * @brief Checks if a hand is a Blackjack.
	 *
	 * @param player_index Player index (0-5 for players, 6 for dealer)
	 * @param hand_index Hand index (0 or 1 in case of split)
	 * @return bool True if the hand is a Blackjack, false otherwise
	 *
	 * A hand is considered Blackjack if it has exactly the initial number of cards
	 * (defined by rules) and the total value equals the winning point.
	 */
	bool IsBlackjack(int player_index, int hand_index) const;

};

#endif //__PABTABLE_H__