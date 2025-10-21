#include "Deck.h"
#include "stdio.h"
#include "stdlib.h"



void Deck::InitCard(int i){
    DeckNode* new_node = (DeckNode*) malloc(sizeof(DeckNode)*1);
    new_node->value=(i%13)+1;
    new_node->next=first;
    if (new_node->next != NULL) new_node->next->prev = new_node;
    new_node->prev = NULL;
    first = new_node;
}