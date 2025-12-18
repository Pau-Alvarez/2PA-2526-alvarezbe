#include "..\Includes/PABGame.h"
#include <conio.h>
#include <algorithm>
#include <utility>
#include <vector>

int PABGame::GetHandvalue(ITable::Hand player_hand) {
    int aces = 0;
    int total_value = 0;

    for (auto v : player_hand) {
        if (static_cast<int>(v.value_) > 10) {
            total_value += 10;
        }
        else if (static_cast<int>(v.value_) == 1) {
            aces++;
        }
        else {
            total_value += static_cast<int>(v.value_);
        }
    }

    // Calcular el mejor valor para los ases según el win_point
    int win_point = rules.GetWinPoint();
    while (aces != 0) {
        // Intentar contar el as como 11 si no nos pasamos del win_point
        if (total_value + 11 <= win_point) {
            total_value += 11;
        }
        else {
            total_value += 1;
        }
        aces--;
    }

    return total_value;
}

char PABGame::GetCardValue(ITable::Card player_card) {
    switch (static_cast<int>(player_card.value_)) {
    case 1:  return 'A';
    case 2:  return '2';
    case 3:  return '3';
    case 4:  return '4';
    case 5:  return '5';
    case 6:  return '6';
    case 7:  return '7';
    case 8:  return '8';
    case 9:  return '9';
    case 10: return '0';
    case 11: return 'J';
    case 12: return 'Q';
    case 13: return 'K';
    default: return '?';
    }
}

void PABGame::PrintHand(ITable::Hand hand, bool show_value) {
    if (show_value) {
        printf("[%d] Cards: ", GetHandvalue(hand));
    }
    else {
        printf("Cards: ");
    }

    for (auto v : hand) {
        if (GetCardValue(v) == '0') {
            printf("[10] ");
        }
        else {
            printf("[%c] ", GetCardValue(v));
        }
    }
}

bool PABGame::IsBlackjack(ITable::Hand hand, int win_point) {
    // Blackjack es tener exactamente el número inicial de cartas y alcanzar el punto ganador
    return hand.size() == static_cast<size_t>(rules.InitialCards()) && GetHandvalue(hand) == win_point;
}

bool PABGame::CheckGameOver(int& active_player_count) {
    if (table.DealerMoney() <= 0) {
        return true;
    }

    active_player_count = 0;
    for (int i = 0; i < 6; i++) {
        if (table.GetPlayerMoney(i) >= rules.MinimumInitialBet()) {
            active_player_count++;
        }
    }

    if (active_player_count == 0) {
        return true;
    }

    return false;
}

bool ComparePlayerMoney(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return a.first > b.first;
}

void PABGame::ShowFinalResults() {
    printf("\n");
    printf("**********************************************\n");
    printf("*           FINAL GAME RESULTS               *\n");
    printf("**********************************************\n\n");

    printf("Dealer final money: %d\n\n", table.DealerMoney());

    std::vector<std::pair<int, int>> player_rankings;

    for (int i = 0; i < 6; i++) {
        player_rankings.push_back(std::make_pair(table.GetPlayerMoney(i), i));
    }

    std::sort(player_rankings.begin(), player_rankings.end(), ComparePlayerMoney);

    printf("=== PLAYER RANKINGS ===\n\n");
    for (int i = 0; i < player_rankings.size(); i++) {
        printf("%d. Player [%d] - Money: %d",
            i + 1,
            player_rankings[i].second,
            player_rankings[i].first);

        if (i == 0 && player_rankings[i].first > 0) {
            printf(" ** WINNER **");
        }
        printf("\n");
    }

    printf("\n");
    if (table.DealerMoney() <= 0) {
        printf("*** THE DEALER IS BANKRUPT! PLAYERS WIN! ***\n");
    }
    else if (player_rankings[0].first <= 0) {
        printf("*** ALL PLAYERS ARE BANKRUPT! DEALER WINS! ***\n");
    }

    printf("\n**********************************************\n\n");
    printf("Game Over. Close the window to exit.\n");
    while (true) {
        _sleep(1000);
    }
}

void PABGame::SolveSplitedHand(int player_index, int hand_index, int win_point) {
    bool end_turn = false;

    do {
        ITable::Hand player_hand = table.GetHand(player_index, hand_index);
        int hand_value = GetHandvalue(player_hand);

        if (hand_value >= win_point) {
            printf("  Hand finished at ");
            PrintHand(player_hand, true);
            printf("\n");
            break;
        }

        ITable::Action player_action = Players[player_index]->DecidePlayerAction(table, player_index, hand_index);
        ITable::Result result = table.ApplyPlayerAction(player_index, hand_index, player_action);

        if (result == ITable::Result::Illegal) {
            printf("  Illegal action, standing instead\n");
            end_turn = true;
            break;
        }

        player_hand = table.GetHand(player_index, hand_index);
        hand_value = GetHandvalue(player_hand);

        switch (player_action) {
        case ITable::Action::Stand:
            printf("  Player stands at ");
            PrintHand(player_hand, true);
            printf("\n");
            end_turn = true;
            break;

        case ITable::Action::Hit:
            printf("  Player hits -> ");
            PrintHand(player_hand, true);
            printf("\n");
            if (hand_value >= win_point) {
                end_turn = true;
            }
            break;

        case ITable::Action::Double:
            printf("  Player doubles -> ");
            PrintHand(player_hand, true);
            printf("\n");
            end_turn = true;
            break;

        case ITable::Action::Split:
            printf("  Split not allowed on split hands\n");
            end_turn = true;
            break;

        default:
            printf("  Unknown action, ending turn\n");
            end_turn = true;
            break;
        }

    } while (!end_turn);
}

void PABGame::PlayPlayerTurn(int player_index, int win_point) {
    if (table.GetPlayerInitialBet(player_index) == 0) {
        return;
    }

    printf("\n--- PLAYER [%d] TURN ---\n", player_index);

    ITable::Hand player_hand = table.GetHand(player_index, 0);
    int hand_value = GetHandvalue(player_hand);

    printf("Player [%d] initial hand: ", player_index);
    PrintHand(player_hand, true);
    printf("\n");

    if (IsBlackjack(player_hand, win_point)) {
        printf("BLACKJACK! Player [%d] has %d!\n", player_index, win_point);
        return;
    }

    bool end_turn = false;

    do {
        ITable::Action player_action = Players[player_index]->DecidePlayerAction(table, player_index, 0);
        ITable::Result result = table.ApplyPlayerAction(player_index, 0, player_action);

        if (result == ITable::Result::Illegal) {
            printf("Illegal action attempted, standing instead\n");
            end_turn = true;
            break;
        }

        player_hand = table.GetHand(player_index, 0);
        hand_value = GetHandvalue(player_hand);

        switch (player_action) {
        case ITable::Action::Stand:
            printf("Player [%d] stands at ", player_index);
            PrintHand(player_hand, true);
            printf("\n");
            end_turn = true;
            break;

        case ITable::Action::Hit:
            printf("Player [%d] hits -> ", player_index);
            PrintHand(player_hand, true);
            printf("\n");
            if (hand_value >= win_point) {
                if (hand_value == win_point) {
                    printf("Player [%d] reaches %d!\n", player_index, win_point);
                }
                else {
                    printf("Player [%d] BUSTS with %d!\n", player_index, hand_value);
                }
                end_turn = true;
            }
            break;

        case ITable::Action::Double:
            printf("Player [%d] doubles down -> ", player_index);
            PrintHand(player_hand, true);
            printf("\n");
            if (hand_value > win_point) {
                printf("Player [%d] BUSTS with %d!\n", player_index, hand_value);
            }
            end_turn = true;
            break;

        case ITable::Action::Split: {

            printf("\nPlayer [%d] SPLITS their hand!\n", player_index);

            printf("\n>> First hand:\n");
            table.DealCard(player_index, 0);
            ITable::Hand first_hand = table.GetHand(player_index, 0);
            printf("  Starting with ");
            PrintHand(first_hand, true);
            printf("\n");
            SolveSplitedHand(player_index, 0, win_point);

            printf("\n>> Second hand:\n");
            table.DealCard(player_index, 1);
            ITable::Hand second_hand = table.GetHand(player_index, 1);
            printf("  Starting with ");
            PrintHand(second_hand, true);
            printf("\n");
            SolveSplitedHand(player_index, 1, win_point);

            end_turn = true;
            break;

        }
        default:
            printf("Unknown action, ending turn\n");
            end_turn = true;
            break;
        }

    } while (!end_turn);
}

void PABGame::ShowResults(ITable::RoundEndInfo& results, int active_players, int win_point) {
    printf("\n");
    printf("==============================================\n");
    printf("            ROUND RESULTS\n");
    printf("==============================================\n");

    printf("\nDEALER final hand: ");
    PrintHand(results.dealer_hand, true);
    int dealer_value = GetHandvalue(results.dealer_hand);
    if (dealer_value > win_point) {
        printf(" - BUST!\n");
    }
    else {
        printf("\n");
    }

    printf("\n--- PLAYER RESULTS ---\n");

    for (int i = 0; i < active_players; i++) {
        if (table.GetPlayerInitialBet(i) == 0) continue;

        int num_hands = table.GetNumberOfHands(i);
        printf("\nPlayer [%d]:\n", i);

        for (int j = 0; j < num_hands; j++) {
            if (j >= results.winners[i].size()) continue;

            if (num_hands > 1) {
                printf("  Hand %d: ", j + 1);
            }
            else {
                printf("  ");
            }

            ITable::Hand player_hand = table.GetHand(i, j);
            PrintHand(player_hand, true);

            switch (results.winners[i][j]) {
            case ITable::RoundEndInfo::BetResult::Win:
                printf(" - WIN! (+%d)\n", table.GetPlayerCurrentBet(i, j));
                break;
            case ITable::RoundEndInfo::BetResult::Lose:
                printf(" - LOSE (-%d)\n", table.GetPlayerCurrentBet(i, j));
                break;
            case ITable::RoundEndInfo::BetResult::Tie:
                printf(" - PUSH (tie)\n");
                break;
            default:
                printf("\n");
                break;
            }
        }

        printf("  Total money: %d\n", table.GetPlayerMoney(i));
    }

    printf("\nDealer money: %d\n", table.DealerMoney());

    printf("\n==============================================\n\n");
}

void PABGame::PlayGame() {
    const int active_players = 6;
    int round_number = 0;
    int active_count = 0;

    // Obtener el punto ganador de las reglas
    int win_point = rules.GetWinPoint();
    int min_bet = rules.MinimumInitialBet();
    int dealer_stop = rules.DealerStop();
    int initial_cards = rules.InitialCards();

    printf("\n");
    printf("**********************************************\n");
    printf("*          BLACKJACK GAME START              *\n");
    printf("**********************************************\n");
    printf("* Win Point: %d                               \n", win_point);
    printf("* Dealer Stops at: %d                         \n", dealer_stop);
    printf("* Initial Cards: %d                           \n", initial_cards);
    printf("* Minimum Bet: %d                            \n", min_bet);
    printf("**********************************************\n");
    printf("\nPress any key to start...\n");
    getch();

    while (!CheckGameOver(active_count)) {
        round_number++;
        printf("\n\n");
        printf("##############################################\n");
        printf("#              ROUND %d                      \n", round_number);
        printf("##############################################\n");

        table.StartRound();

        ITable::Card dealer_card = table.GetDealerCard();
        printf("\nDEALER shows: ");
        if (GetCardValue(dealer_card) == '0') {
            printf("[10]\n");
        }
        else {
            printf("[%c]\n", GetCardValue(dealer_card));
        }

        printf("\n==============================================\n");
        printf("            BETTING PHASE\n");
        printf("==============================================\n");

        for (int i = 0; i < active_players; i++) {
            if (table.GetPlayerMoney(i) < min_bet) {
                printf("Player [%d] ELIMINATED (insufficient funds: %d)\n", i, table.GetPlayerMoney(i));
                continue;
            }

            int bet = Players[i]->DecideInitialBet(table, i);

            if (bet == 0 || bet < min_bet) {
                printf("Player [%d] ELIMINATED (cannot meet minimum bet)\n", i);
                continue;
            }

            ITable::Result result = table.PlayInitialBet(i, bet);

            if (result == ITable::Result::Ok) {
                printf("Player [%d] bets: %d (Remaining: %d)\n", i, bet, table.GetPlayerMoney(i));
            }
            else {
                printf("Player [%d] ELIMINATED (illegal bet)\n", i);
            }
        }

        printf("\n==============================================\n");
        printf("           DEALING INITIAL CARDS\n");
        printf("==============================================\n");

        for (int i = 0; i < active_players; i++) {
            if (table.GetPlayerInitialBet(i) > 0) {
                for (int c = 0; c < initial_cards; c++) {
                    table.DealCard(i, 0);
                }
            }
        }

        for (int i = 0; i < active_players; i++) {
            if (table.GetPlayerInitialBet(i) > 0) {
                ITable::Hand player_hand = table.GetHand(i, 0);
                printf("Player [%d]: ", i);
                PrintHand(player_hand, true);
                printf("\n");
            }
        }

        if (static_cast<int>(dealer_card.value_) == 1) {
            printf("\n==============================================\n");
            printf("     DEALER HAS ACE - INSURANCE AVAILABLE\n");
            printf("==============================================\n");

            for (int i = 0; i < active_players; i++) {
                if (table.GetPlayerMoney(i) > 0 && table.GetPlayerInitialBet(i) > 0) {
                    if (Players[i]->DecideUseSafe(table, i)) {
                        ITable::Result result = table.PlaySafeBet(i);
                        if (result == ITable::Result::Ok) {
                            printf("Player [%d] takes insurance\n", i);
                        }
                        else {
                            printf("Player [%d] cannot afford insurance\n", i);
                        }
                    }
                }
            }
        }

        printf("\n==============================================\n");
        printf("           PLAYERS' TURNS\n");
        printf("==============================================\n");

        for (int i = 0; i < active_players; i++) {
            PlayPlayerTurn(i, win_point);
        }

        printf("\n==============================================\n");
        printf("           DEALER'S TURN\n");
        printf("==============================================\n");

        ITable::RoundEndInfo results = table.FinishRound();

        ShowResults(results, active_players, win_point);

        table.CleanTable();

        printf("\nPress any key to continue to next round...\n");
        getch();
    }

    ShowFinalResults();
}