

struct DeckNode{
    DeckNode* next;
    int value;
    DeckNode* prev;
};
struct Deck{
    DeckNode* first=nullptr;
    const int MaxCards_ = 52*4;
    int actuall_cards=MaxCards_;
    void InitCard(int i);
};



enum CardType{
    As =1,
    Jack = 11,
    Queen,
    King,
};