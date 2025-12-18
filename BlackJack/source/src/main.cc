#include <vector>
#include "..\Includes/PABGame.h"
#include <time.h>
#include <conio.h>


int main() {
    srand((unsigned)time(0));
    bool repeat_rules = true;
    BaseRules* rules;
    while (repeat_rules == true)
    {
        printf("HELLO HELLO WELLCOME TO BLACKJACK\n");
        printf("CHOOSE YOUR GAMEMODE\n");
        printf("1.-ORIGINAL BJ\n");
        printf("2.-EXTREME BJ\n");
        printf("3.-SIMPLIFIED BJ\n");
        char tmp_rules_input;
        tmp_rules_input = getch();
        system("cls");
        switch (tmp_rules_input) {
        case '1':
            rules = new BaseRules;
            repeat_rules = false;
            break;
        case '2':
            rules = new ExtremeRules;
            repeat_rules = false;
            break;
        case '3':
            rules = new Simplified;
            repeat_rules = false;
            break;
        default:
            printf("Please Insert a valid Number\n");
            repeat_rules = true;
            break;
        }
    }
    PABTable table(*rules);
    PABPlayer p1(*rules);
    PABPlayer p2(*rules);
    PABPlayer p3(*rules);
    PABPlayer p4(*rules);
    PABPlayer p5(*rules);
    PABPlayer p6(*rules);

    std::vector<IPlayer*> Players = { &p1,&p2,&p3,&p4,&p5,&p6 };
    PABGame game(table, Players,*rules);

    game.PlayGame();
    delete rules;
}