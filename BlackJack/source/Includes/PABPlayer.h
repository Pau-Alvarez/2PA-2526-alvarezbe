#pragma once
#ifndef __PABPLAYER_H__
#define __PABPLAYER_H__ 1

#include "..\Interfaces/IPlayer.h"
#include "..\Interfaces/Rules.h"

/**
 * @class PABPlayer
 * @brief Concrete implementation of the IPlayer interface for Blackjack.
 * 
 * This class implements a Blackjack player using a strategy table
 * to make decisions about actions, bets, and insurance.
 */
class PABPlayer : public IPlayer {
public:
	virtual ITable::Action DecidePlayerAction(const ITable& table,
		int player_index, int hand_index);

	virtual int DecideInitialBet(const ITable& table, int player_index);

	virtual bool DecideUseSafe(const ITable& table, int player_index);

	virtual ~PABPlayer() = default;

	/**
	 * @brief PABPlayer constructor.
	 * 
	 * @param rules Reference to the game rules to be used by this player.
	 */
	PABPlayer(BaseRules& rules) : Rules{ rules } {}

private:
	/**
	 * @brief Calculates the value of a hand.
	 * 
	 * @param player_hand The hand to calculate
	 * @return int The calculated hand value
	 */
	int CalculateHandValue(ITable::Hand player_hand) const;

	/**
	 * @brief Calculates indices for the strategy table based on the current game state.
	 * 
	 * @param table Reference to the game table
	 * @param player_index Index of the player
	 * @param hand_index Index of the hand
	 * @return std::vector<int> Vector containing row and column indices for the action table
	 * 
	 * Determines the correct indices to look up in the action table based on:
	 * - Dealer's visible card
	 * - Player's hand value and composition
	 * - Special cases like pairs, aces, etc.
	 */
	std::vector<int> TableIndex(const ITable& table, int player_index, int hand_index);

	/// Static action table containing Blackjack strategy decisions
	static const std::vector<std::vector<char> > actiontable;

	BaseRules& Rules;  ///< Reference to the game rules
};

#endif //__PABPLAYER_H__