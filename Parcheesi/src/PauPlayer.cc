#include "PauPlayer.h"

int PauPlayer::DecideMove(const IParcheesi& parcheesi, int dice_roll) const {
    const int maxpieces = 4;
    
    if(dice_roll == 5) {
        if(parcheesi.PiecesAtHome(this_index) > 0) {
            for(int i = 0; i < maxpieces; i++) {
                int piece_idx = this_index * maxpieces + i;
                IParcheesi* tmp = parcheesi.Clone();
                IParcheesi::Movement result = tmp->ApplyMovement(piece_idx, this_index, dice_roll);
                delete tmp;
                
                if(result != IParcheesi::Movement::NoMoves && 
                   result != IParcheesi::Movement::IllegalMustEnterPiece) {
                    return piece_idx;
                }
            }
        }
    }
    
    for(int i = 0; i < maxpieces; i++) {
        int piece_idx = this_index * maxpieces + i;
        IParcheesi* tmp = parcheesi.Clone();
        IParcheesi::Movement result = tmp->ApplyMovement(piece_idx, this_index, dice_roll);
        delete tmp;
        
        if(result == IParcheesi::Movement::Eat) {
            return piece_idx;
        }
    }
    for(int i = 0; i < maxpieces; i++) {
        int piece_idx = this_index * maxpieces + i;
        IParcheesi* tmp = parcheesi.Clone();
        IParcheesi::Movement result = tmp->ApplyMovement(piece_idx, this_index, dice_roll);
        delete tmp;
        
        if(result == IParcheesi::Movement::End || result == IParcheesi::Movement::ReachExit) {
            return piece_idx;
        }
    }
    for(int i = 0; i < maxpieces; i++) {
        int piece_idx = this_index * maxpieces + i;
        IParcheesi* tmp = parcheesi.Clone();
        IParcheesi::Movement result = tmp->ApplyMovement(piece_idx, this_index, dice_roll);
        delete tmp;
        
        if(result == IParcheesi::Movement::Normal) {
            return piece_idx;
        }
    }
    return this_index * maxpieces;
}