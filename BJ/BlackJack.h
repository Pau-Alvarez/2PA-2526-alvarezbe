#include "Deck.h"


struct BlackJack{

    const int MaxNumPlayers_=4;
    const int MaxDealerValue_ = 16;
    const int BlackJack_= 21; 

    int num_players;
    int current_turn=0;



    int player_value[5]; //0=Dealer
    int player_figure[5][2];
    int player_Aces[5]; //0=Dealer
    int shown_dealer_value;

    Deck deck;

    void DrawCard(int current_player);
    void Turn();
    void Action(int i);
};