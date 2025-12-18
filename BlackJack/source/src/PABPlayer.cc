#include "..\Includes/PABPlayer.h"
#include "assert.h" 
#include "..\Interfaces/ITable.h"

const std::vector<std::vector<char> > PABPlayer::actiontable = {
    {'H',  'H',  'H',  'H',  'H',  'H',  'H',  'H',  'H',  'H'},
	{'H',  'D',  'D',  'D',  'D',  'H',  'H',  'H',  'H',  'H'},
	{'D',  'D',  'D',  'D',  'D',  'D',  'D',  'D',  'H',  'H'},
	{'D',  'D',  'D',  'D',  'D',  'D',  'D',  'D',  'D',  'H'},
	{'H',  'H',  'S',  'S',  'S',  'H',  'H',  'H',  'H',  'H'},
	{'S',  'S',  'S',  'S',  'S',  'H',  'H',  'H',  'H',  'H'},
	{'S',  'S',  'S',  'S',  'S',  'H',  'H',  'H',  'H',  'H'},
	{'S',  'S',  'S',  'S',  'S',  'H',  'H',  'H',  'H',  'H'},
	{'S',  'S',  'S',  'S',  'S',  'H',  'H',  'H',  'H',  'H'},
	{'S',  'S',  'S',  'S',  'S',  'S',  'S',  'S',  'S',  'S'},
	{'H',  'H',  'H',  'D',  'D',  'H',  'H',  'H',  'H',  'H'},
	{'H',  'H',  'H',  'D',  'D',  'H',  'H',  'H',  'H',  'H'},
	{'H',  'H',  'D',  'D',  'D',  'H',  'H',  'H',  'H',  'H'},
	{'H',  'H',  'D',  'D',  'D',  'H',  'H',  'H',  'H',  'H'},
	{'H',  'D',  'D',  'D',  'D',  'H',  'H',  'H',  'H',  'H'},
	{'S',  'D',  'D',  'D',  'D',  'S',  'S',  'H',  'H',  'H'},
	{'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P'},
	{'H',  'H',  'P',  'P',  'P',  'P',  'H',  'H',  'H',  'H'},
	{'H',  'H',  'P',  'P',  'P',  'P',  'H',  'H',  'H',  'H'},
	{'H',  'H',  'H',  'H',  'H',  'H',  'H',  'H',  'H',  'H'},
	{'D',  'D',  'D',  'D',  'D',  'D',  'D',  'D',  'H',  'H'},
	{'P',  'P',  'P',  'P',  'P',  'H',  'H',  'H',  'H',  'H'},
	{'P',  'P',  'P',  'P',  'P',  'P',  'H',  'H',  'H',  'H'},
	{'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P'},
	{'P',  'P',  'P',  'P',  'P',  'S',  'P',  'P',  'S',  'S'},
	{'S',  'S',  'P',  'P',  'P',  'P',  'S',  'S',  'S',  'S'},
};

int PABPlayer::CalculateHandValue(ITable::Hand player_hand) const {
	int total_hand_value = 0;
	int aces = 0;

	for (auto v : player_hand) {
		if (static_cast<int>(v.value_) >= 10) {
			total_hand_value += 10;
		}
		else if (static_cast<int>(v.value_) == 1) {
			aces++;
		}
		else {
			total_hand_value += static_cast<int>(v.value_);
		}
	}

	int win_point = Rules.GetWinPoint();
	int tmp_aces = aces;
	while (tmp_aces != 0) {
		if (total_hand_value + 11 <= win_point) {
			total_hand_value += 11;
		}
		else {
			total_hand_value += 1;
		}
		tmp_aces--;
	}

	return total_hand_value;
}

std::vector<int> PABPlayer::TableIndex(const ITable& table, int player_index, int hand_index) {
	std::vector<int> Result;
	ITable::Card tmp = table.GetDealerCard();
	int dealer_card_value = static_cast<int>(tmp.value_) - 2;
	if (dealer_card_value == -1) {
		Result.push_back(9);
	}
	else if (dealer_card_value >= 9) {
		Result.push_back(8);
	}
	else {
		Result.push_back(dealer_card_value);
	}

	ITable::Hand player_hand = table.GetHand(player_index, hand_index);
	int total_hand_value = CalculateHandValue(player_hand);

	if (player_hand.size() != 2) {
		int aces = 0;
		for (auto v : player_hand) {
			if (static_cast<int>(v.value_) == 1) {
				aces++;
			}
		}

		if (aces == 1 && total_hand_value < 18) {
			int othercards = 0;
			for (auto v : player_hand) {
				if (static_cast<int>(v.value_) != 1) {
					if (static_cast<int>(v.value_) >= 10) {
						othercards += 10;
					}
					else {
						othercards += static_cast<int>(v.value_);
					}
				}
			}
			Result.push_back(9 + othercards);
			return Result;
		}

		if (total_hand_value <= 8) {
			Result.push_back(0);
			return Result;
		}
		if (total_hand_value >= 17) {
			Result.push_back(9);
			return Result;
		}

		switch (total_hand_value) {
		case 9:
			Result.push_back(1);
			return Result;
		case 10:
			Result.push_back(2);
			return Result;
		case 11:
			Result.push_back(3);
			return Result;
		case 12:
			Result.push_back(4);
			return Result;
		case 13:
			Result.push_back(5);
			return Result;
		case 14:
			Result.push_back(6);
			return Result;
		case 15:
			Result.push_back(7);
			return Result;
		case 16:
			Result.push_back(8);
			return Result;
		default:
			Result.push_back(9);
			return Result;
		}
	}

	int aces = 0;
	for (auto v : player_hand) {
		if (static_cast<int>(v.value_) == 1) {
			aces++;
		}
	}

	if (static_cast<int>(player_hand[0].value_) >= 10 &&
		static_cast<int>(player_hand[1].value_) >= 10) {
		Result.push_back(25);
		return Result;
	}

	if (static_cast<int>(player_hand[0].value_) ==
		static_cast<int>(player_hand[1].value_)) {
		Result.push_back(15 + static_cast<int>(player_hand[0].value_));
		return Result;
	}

	if (aces == 1 && total_hand_value < 18) {
		int othercards = 0;
		for (auto v : player_hand) {
			if (static_cast<int>(v.value_) != 1) othercards += static_cast<int>(v.value_);
		}
		Result.push_back(9 + othercards);
		return Result;
	}

	if (total_hand_value <= 8) {
		Result.push_back(0);
		return Result;
	}
	if (total_hand_value >= 17) {
		Result.push_back(9);
		return Result;
	}

	switch (total_hand_value) {
	case 9:
		Result.push_back(1);
		return Result;
	case 10:
		Result.push_back(2);
		return Result;
	case 11:
		Result.push_back(3);
		return Result;
	case 12:
		Result.push_back(4);
		return Result;
	case 13:
		Result.push_back(5);
		return Result;
	case 14:
		Result.push_back(6);
		return Result;
	case 15:
		Result.push_back(7);
		return Result;
	case 16:
		Result.push_back(8);
		return Result;
	default:
		Result.push_back(9);
		return Result;
	}
}

ITable::Action PABPlayer::DecidePlayerAction(const ITable& table, int player_index, int hand_index) {

	std::vector<int> tmp_table_index = TableIndex(table, player_index, hand_index);

	if (tmp_table_index.size() < 2) {
		return ITable::Action::Stand;
	}

	if (tmp_table_index[1] >= 26 || tmp_table_index[1] < 0) {
		return ITable::Action::Stand;
	}

	if (tmp_table_index[0] >= 10 || tmp_table_index[0] < 0) {
		return ITable::Action::Stand;
	}

	char move = PABPlayer::actiontable[tmp_table_index[1]][tmp_table_index[0]];

	ITable::Hand playerhand = table.GetHand(player_index, hand_index);

	switch (move) {
	case 'H':
		return ITable::Action::Hit;
	case 'D':
		if (playerhand.size() > 2) {
			return ITable::Action::Hit;
		}
		return ITable::Action::Double;
	case 'S':
		return ITable::Action::Stand;
	case 'P':
		if (table.GetNumberOfHands(player_index) == 1 && playerhand.size() == 2) {
			return ITable::Action::Split;
		}
		else if (tmp_table_index[1] > 23) {
			return ITable::Action::Stand;
		}
		else {
			return ITable::Action::Hit;
		}
	default:
		return ITable::Action::Stand;
	}
}

int PABPlayer::DecideInitialBet(const ITable& table, int player_index) {
	int player_money = table.GetPlayerMoney(player_index);

	if (player_money < Rules.MinimumInitialBet()) {
		return Rules.MinimumInitialBet();
	}

	int max_bet = player_money / 2;
	if (max_bet < Rules.MinimumInitialBet()) {
		return Rules.MinimumInitialBet();
	}

	int range = max_bet - Rules.MinimumInitialBet();
	if (range <= 0) {
		return Rules.MinimumInitialBet();
	}

	int final_bet = Rules.MinimumInitialBet() + (rand() % range);

	if (final_bet > Rules.MaximumInitialBet()) {
		return Rules.MaximumInitialBet();
	}

	return final_bet;
}

bool PABPlayer::DecideUseSafe(const ITable& table, int player_index) {
	int player_money = table.GetPlayerMoney(player_index);
	int player_bet = table.GetPlayerInitialBet(player_index);
	if (player_bet / 2 > player_money) return false;
	return true;
}