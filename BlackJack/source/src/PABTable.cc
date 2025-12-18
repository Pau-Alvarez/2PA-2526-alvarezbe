#include "..\Includes/PABTable.h"
#include <cassert>
#include <time.h>
#include <algorithm>
#include <random>

ITable::Hand PABTable::GetHand(int player_index, int hand_index)const {
	assert(player_index <= kMaxPlayers);
	assert(hand_index <= 2);

	return Players[player_index].hand_info.hands[hand_index];
}

int PABTable::GetNumberOfHands(int player_index)const {
	assert(player_index <= kMaxRealPlayers);
	if (Players[player_index].hand_info.hands.size() == 2) return 2;
	return 1;
}

int PABTable::GetPlayerInitialBet(int player_index)const {
	assert(player_index <= kMaxRealPlayers);
	return Players[player_index].hand_info.player_bet[0];
}

int PABTable::GetPlayerCurrentBet(int player_index, int hand_index) const {
	assert(player_index <= kMaxRealPlayers);
	return Players[player_index].hand_info.player_bet[hand_index];
}

int PABTable::GetPlayerMoney(int player_index)const {
	assert(player_index <= kMaxRealPlayers);
	return Players[player_index].player_money;
}

ITable::Result PABTable::PlayInitialBet(int player_index, int money) {
	assert(player_index <= kMaxRealPlayers);
	if (money > Players[player_index].player_money)return  ITable::Result::Illegal;
	if (money < rules.MinimumInitialBet())return  ITable::Result::Illegal;
	if (money > rules.MaximumInitialBet())return  ITable::Result::Illegal;
	Players[player_index].hand_info.player_bet[0] = money;
	Players[player_index].player_money -= money;
	Players[6].player_money += money;
	return ITable::Result::Ok;
}

ITable::Card PABTable::GetDealerCard()const {
	return Players[6].hand_info.hands[0][0];
}

int PABTable::DealerMoney() const {
	return Players[6].player_money;
}

void PABTable::FillDeck() {
	Card tmp;
	for (int i = 0; i < rules.NumberOfDecks();i++) {
		for (int j = 0; j < static_cast<int>(ITable::Value::end) - 1; j++) {
			for (int k = 0; k < static_cast<int>(ITable::Suit::end); k++) {
				tmp.value_ = static_cast<ITable::Value>(j + 1);
				tmp.suit_ = static_cast<ITable::Suit>(k);
				Deck.push_back(tmp);
			}
		}
	}
}

void PABTable::ShufleDeck() {
	std::random_device seed;
	std::mt19937 rand(seed());
	if (Deck.capacity() > 0) {
		Deck.clear();
	}
	FillDeck();

	for (int i = 0; i < rules.NumberOfDecks(); i++) {
		std::shuffle(Deck.begin(), Deck.end(), rand);
	}
}

void PABTable::DealCard(int player_index, int hand_index) {
	assert(player_index < kMaxPlayers);
	assert(hand_index <= Players[player_index].hand_info.hands.capacity());
	Players[player_index].hand_info.hands[hand_index].push_back(Deck.back());
	Deck.pop_back();
}

ITable::Result PABTable::PlaySafeBet(int player_index) {
	assert(player_index <= kMaxRealPlayers);
	int tmp = GetPlayerInitialBet(player_index) / 2;
	if (tmp > GetPlayerMoney(player_index)) return Result::Illegal;
	Players[player_index].hand_info.safe_bet = tmp;
	Players[player_index].player_money -= tmp;
	Players[6].player_money += tmp;
	return ITable::Result::Ok;
}

void PABTable::CalculateRealHandValue(int player_index, int hand_index) {
	assert(player_index <= kMaxPlayers);

	if (hand_index >= Players[player_index].hand_info.total_hand_value.size()) {
		Players[player_index].hand_info.total_hand_value.resize(hand_index + 1, 0);
	}

	if (hand_index >= Players[player_index].hand_info.hands.size()) {
		return;
	}

	Players[player_index].hand_info.total_hand_value[hand_index] = 0;
	int aces = 0;
	Hand tmp = GetHand(player_index, hand_index);
	for (auto& v : tmp) {
		if (static_cast<int>(v.value_) >= 10) {
			Players[player_index].hand_info.total_hand_value[hand_index] += 10;
		}
		else if (static_cast<int>(v.value_) == 1) {
			aces++;
		}
		else {
			Players[player_index].hand_info.total_hand_value[hand_index] += static_cast<int>(v.value_);
		}
	}

	int win_point = rules.GetWinPoint();
	while (aces != 0) {
		if (Players[player_index].hand_info.total_hand_value[hand_index] + 11 <= win_point) {
			Players[player_index].hand_info.total_hand_value[hand_index] += 11;
		}
		else {
			Players[player_index].hand_info.total_hand_value[hand_index] += 1;
		}
		aces--;
	}
}

bool PABTable::IsBlackjack(int player_index, int hand_index) const {
	if (hand_index >= Players[player_index].hand_info.hands.size()) {
		return false;
	}

	Hand hand = Players[player_index].hand_info.hands[hand_index];
	int hand_value = Players[player_index].hand_info.total_hand_value[hand_index];
	size_t initial_cards = static_cast<size_t>(rules.InitialCards());
	int win_point = rules.GetWinPoint();

	return hand.size() == initial_cards && hand_value == win_point;
}

ITable::Result PABTable::ApplyPlayerAction(int player_index, int hand_index, Action action) {

	assert(player_index <= kMaxRealPlayers);
	assert(hand_index < Players[player_index].hand_info.hands.capacity());
	switch (action)
	{
	case Action::Hit:
		CalculateRealHandValue(player_index, hand_index);
		if (Players[player_index].hand_info.total_hand_value[hand_index] >= rules.GetWinPoint())
			return ITable::Result::Illegal;
		DealCard(player_index, hand_index);
		return ITable::Result::Ok;
		break;

	case Action::Stand:
		return ITable::Result::Ok;
		break;

	case Action::Double:
		if (GetNumberOfHands(player_index) == 2)
			return ITable::Result::Illegal;

		if (GetPlayerMoney(player_index) < GetPlayerCurrentBet(player_index, hand_index))
			return ITable::Result::Illegal;

		DealCard(player_index, hand_index);
		Players[player_index].player_money -= GetPlayerInitialBet(player_index);
		Players[6].player_money += GetPlayerInitialBet(player_index);
		Players[player_index].hand_info.player_bet[hand_index] *= 2;
		return ITable::Result::Ok;
		break;

	case Action::Split:
		if (GetPlayerMoney(player_index) < GetPlayerInitialBet(player_index))
			return ITable::Result::Illegal;

		{
			Card second_card = Players[player_index].hand_info.hands[hand_index].back();
			Players[player_index].hand_info.hands[hand_index].pop_back();

			Hand new_hand;
			new_hand.push_back(second_card);
			Players[player_index].hand_info.hands.push_back(new_hand);
		}

		Players[player_index].hand_info.player_bet.push_back(GetPlayerInitialBet(player_index));

		Players[player_index].hand_info.total_hand_value.push_back(0);

		Players[player_index].player_money -= GetPlayerInitialBet(player_index);
		Players[6].player_money += GetPlayerInitialBet(player_index);
		return ITable::Result::Ok;
		break;

	default:
		return ITable::Result::Illegal;
		break;
	}
}

void PABTable::StartRound() {
	ShufleDeck();
	if (Players[6].hand_info.hands[0].capacity() != 0)
		Players[6].hand_info.hands[0].clear();

	for (int i = 0; i < rules.InitialCards(); i++) {
		DealCard(6, 0);
	}
}

ITable::RoundEndInfo PABTable::FinishRound() {
	ITable::RoundEndInfo Rinfo;

	if (Players[6].hand_info.hands[0].empty()) {
		printf("ERROR: Dealer has no cards!\n");
		return Rinfo;
	}

	CalculateRealHandValue(6, 0);

	printf("Dealer starts with value %d\n", Players[6].hand_info.total_hand_value[0]);

	int dealer_stop = rules.DealerStop();
	int win_point = rules.GetWinPoint();

	while (Players[6].hand_info.total_hand_value[0] < dealer_stop) {
		DealCard(6, 0);
		CalculateRealHandValue(6, 0);
		printf("Dealer draws -> Total: %d\n", Players[6].hand_info.total_hand_value[0]);

		if (Deck.empty()) {
			printf("ERROR: No more cards in deck!\n");
			break;
		}

		if (Players[6].hand_info.total_hand_value[0] > win_point) {
			break;
		}
	}

	Rinfo.dealer_hand = Players[6].hand_info.hands[0];
	int dealer_value = Players[6].hand_info.total_hand_value[0];
	bool dealer_has_blackjack = IsBlackjack(6, 0);

	if (dealer_value > win_point) {
		printf("Dealer BUSTS with %d!\n", dealer_value);
	}
	else {
		printf("Dealer stands at %d\n", dealer_value);
	}

	Rinfo.winners.resize(kMaxRealPlayers);
	Rinfo.player_money_delta.resize(kMaxRealPlayers, 0);
	Rinfo.dealer_money_delta = 0;

	if (dealer_value <= win_point) {
		for (int i = 0; i < kMaxRealPlayers; i++) {
			int num_hands = GetNumberOfHands(i);
			Rinfo.winners[i].resize(num_hands);

			for (int j = 0; j < num_hands; j++) {
				CalculateRealHandValue(i, j);
				int player_value = Players[i].hand_info.total_hand_value[j];
				bool player_has_blackjack = IsBlackjack(i, j);

				if (player_value > win_point) {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Lose;
					Rinfo.player_money_delta[i] -= GetPlayerCurrentBet(i, j);
					Rinfo.dealer_money_delta += GetPlayerCurrentBet(i, j);
				}
				else if (player_has_blackjack && dealer_has_blackjack) {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Tie;
					Players[i].player_money += GetPlayerCurrentBet(i, j);
					Players[6].player_money -= GetPlayerCurrentBet(i, j);
				}
				else if (player_has_blackjack) {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Win;
					Rinfo.player_money_delta[i] += GetPlayerCurrentBet(i, j);
					Rinfo.dealer_money_delta -= GetPlayerCurrentBet(i, j);
					Players[i].player_money += GetPlayerCurrentBet(i, j) * 2;
					Players[6].player_money -= GetPlayerCurrentBet(i, j) * 2;
				}
				else if (dealer_has_blackjack) {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Lose;
					Rinfo.player_money_delta[i] -= GetPlayerCurrentBet(i, j);
					Rinfo.dealer_money_delta += GetPlayerCurrentBet(i, j);
				}
				else if (player_value > dealer_value) {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Win;
					Rinfo.player_money_delta[i] += GetPlayerCurrentBet(i, j);
					Rinfo.dealer_money_delta -= GetPlayerCurrentBet(i, j);
					Players[i].player_money += GetPlayerCurrentBet(i, j) * 2;
					Players[6].player_money -= GetPlayerCurrentBet(i, j) * 2;
				}
				else if (player_value == dealer_value) {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Tie;
					Players[i].player_money += GetPlayerCurrentBet(i, j);
					Players[6].player_money -= GetPlayerCurrentBet(i, j);
				}
				else {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Lose;
					Rinfo.player_money_delta[i] -= GetPlayerCurrentBet(i, j);
					Rinfo.dealer_money_delta += GetPlayerCurrentBet(i, j);
				}
			}
		}
	}
	else {
		for (int i = 0; i < kMaxRealPlayers; i++) {
			int num_hands = GetNumberOfHands(i);
			Rinfo.winners[i].resize(num_hands);

			for (int j = 0; j < num_hands; j++) {
				CalculateRealHandValue(i, j);
				int player_value = Players[i].hand_info.total_hand_value[j];

				if (player_value <= win_point) {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Win;
					Rinfo.player_money_delta[i] += GetPlayerCurrentBet(i, j);
					Rinfo.dealer_money_delta -= GetPlayerCurrentBet(i, j);
					Players[i].player_money += GetPlayerCurrentBet(i, j) * 2;
					Players[6].player_money -= GetPlayerCurrentBet(i, j) * 2;
				}
				else {
					Rinfo.winners[i][j] = ITable::RoundEndInfo::BetResult::Lose;
					Rinfo.player_money_delta[i] -= GetPlayerCurrentBet(i, j);
					Rinfo.dealer_money_delta += GetPlayerCurrentBet(i, j);
				}
			}
		}
	}

	if (dealer_has_blackjack) {
		for (int i = 0; i < kMaxRealPlayers; i++) {
			if (Players[i].hand_info.safe_bet != 0) {
				Rinfo.player_money_delta[i] += Players[i].hand_info.safe_bet;
				Rinfo.dealer_money_delta -= Players[i].hand_info.safe_bet;
				Players[i].player_money += Players[i].hand_info.safe_bet * 2;
				Players[6].player_money -= Players[i].hand_info.safe_bet * 2;
			}
		}
	}
	else {
		for (int i = 0; i < kMaxRealPlayers; i++) {
			if (Players[i].hand_info.safe_bet != 0) {
				Rinfo.player_money_delta[i] -= Players[i].hand_info.safe_bet;
				Rinfo.dealer_money_delta += Players[i].hand_info.safe_bet;
			}
		}
	}

	return Rinfo;
}

void PABTable::CreateTable() {
	for (int i = 0; i < Players.size(); i++) {
		Players[i].hand_info.hands.resize(1);
		Players[i].hand_info.player_bet.resize(1, 0);
		Players[i].hand_info.total_hand_value.resize(1, 0);
	}
}

void PABTable::CleanTable() {
	for (auto& v : Players) {
		v.hand_info.hands.clear();
		v.hand_info.player_bet.clear();
		v.hand_info.total_hand_value.clear();
		v.hand_info.safe_bet = 0;
	}
	CreateTable();
}

PABTable::PABTable(BaseRules& game_rules) : rules(game_rules) {
	Players.resize(7);
	for (int i = 0; i < Players.size(); i++) {
		Players[i].player_money = rules.InitialPlayerMoney();
	}
	CreateTable();

	Players[6].player_money = rules.InitialDealerMoney();
}