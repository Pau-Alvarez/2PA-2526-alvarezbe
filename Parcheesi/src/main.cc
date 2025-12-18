#include <windows.h>
#include "pabparcheesi.h"
#include "game.h"
#include "PauPlayer.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    PauPlayer PPlayer1(0);  
    PauPlayer PPlayer2(1);   
    PauPlayer PPlayer3(2);  
    PauPlayer PPlayer4(3);  
    
    pabparcheesi myp;
    const IPlayer* players[4] = {&PPlayer1, &PPlayer2, &PPlayer3, &PPlayer4};
    Game g{myp, players};
    
    while(g.IsGameOver() == IParcheesi::Color::None) {
        g.PlayTurn();
    }
    
    return 0;
}