#include "BlackJack.h"
#include "stdio.h"
#include "stdlib.h"

void BlackJack::DrawCard(int current_player){
    int random_card= rand()%deck.actuall_cards;
    DeckNode* aux;
    DeckNode* destroyable_info=nullptr;
    for (int i;i<random_card;i++){
        aux=aux->next;
    }
    if(aux->value>1 && aux->value<=10){
        player_value[current_player]+=aux->value;
    }else if(aux->value<=10){
        player_value[current_player]+=10;
        if(player_figure[current_player][1]>10 &&player_figure[current_player][1]<14){
            player_figure[current_player][2]=aux->value;
        }else{
            player_figure[current_player][1]=aux->value;
        }
    }else{
        player_Aces[current_player]+=1;
        player_value[current_player]+=1;
    }

    destroyable_info=aux;
    free(aux);
    if(destroyable_info->next==nullptr){
        deck.first=destroyable_info->next;
        if(deck.first!=nullptr)deck.first->prev=nullptr;
    }else{
        destroyable_info->prev->next=destroyable_info->next;
        destroyable_info->next->prev=destroyable_info->prev;
    }
    deck.actuall_cards--;
}

void BlackJack::Action(int i){
    while(player_value[i]-10<shown_dealer_value){
        if(player_value[i]<17){
            DrawCard(i);
        }else{
            return;
        }
    }
}

void BlackJack::Turn(){
    for(int i=0;i<MaxNumPlayers_;i++){
        DrawCard(i);
        if(i==0) shown_dealer_value=player_value[i];
        DrawCard(i);
    }
    for(int i=1;i<MaxNumPlayers_;i++){
        Action(i);
    }
    
}