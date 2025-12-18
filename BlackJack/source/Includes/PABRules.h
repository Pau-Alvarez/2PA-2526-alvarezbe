#include "..\Interfaces/Rules.h"

/**
 * @class ExtremeRules
 * @brief Extreme Blackjack rules with higher winning points and multiple decks.
 * 
 * This class extends BaseRules with more extreme parameters:
 * - Winning point: 25
 * - 2 decks of cards
 * - 3 initial cards
 * - Dealer initial money: 100,000
 * - Dealer stops at 21
 */
class ExtremeRules : public BaseRules {
public:
    ~ExtremeRules() = default;

    int GetWinPoint() const { return 25; }

    int NumberOfDecks() const { return 2; }

    int InitialCards() const { return 3; }

    int InitialPlayerMoney() const { return 4000; }

    int InitialDealerMoney() const { return 100000; }

    int MinimumInitialBet() const { return 100; }

    int MaximumInitialBet() const { return 10000; }

    int DealerStop() const { return 21; }
};

/**
 * @class Simplified
 * @brief Simplified Blackjack rules with lower winning points.
 * 
 * This class extends BaseRules with simplified parameters:
 * - Winning point: 20
 * - 1 deck of cards
 * - 2 initial cards
 * - Dealer stops at 16
 */
class Simplified : public BaseRules {
public:
    ~Simplified() = default;

    int GetWinPoint() const { return 20; }

    int NumberOfDecks() const { return 1; }

    int InitialCards() const { return 2; }

    int InitialPlayerMoney() const { return 4000; }

    int InitialDealerMoney() const { return 100000; }

    int MinimumInitialBet() const { return 100; }

    int MaximumInitialBet() const { return 10000; }

    int DealerStop() const { return 16; }
};