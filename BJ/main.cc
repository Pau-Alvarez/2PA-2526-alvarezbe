#include "BlackJack.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

int main(){
    BlackJack* blackjack;

    srand(time(NULL));

    for (int i=0;i<blackjack->deck.MaxCards_;i++){
        blackjack->deck.InitCard(i);
    }
    blackjack->Turn();
}