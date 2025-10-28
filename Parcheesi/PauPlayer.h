#include "game.h"
#include "pabparcheesi.h"

class PauPlayer : public IPlayer{

    private:

    int this_Index;

    public:

    PauPlayer(int index): this_Index(index) {}
    
    int PauPlayer::DecideMove(const IParcheesi& parcheesi, int dice_roll) const{

        const int maxpieces=4;

        int final_piece_index;

        IParcheesi::Movement PosibleMovement[maxpieces];
        
        for(int i=0;i<maxpieces;i++){
            IParcheesi* tmp = parcheesi.Clone();

            PosibleMovement[i]=tmp->ApplyMovement(i,this_Index,dice_roll);
            
            if(dice_roll==5 && tmp->PiecesAtHome(this_Index)!=0 && PosibleMovement[i]==IParcheesi::Movement::Normal)

            if(PosibleMovement[i]==IParcheesi::Movement::IllegalMustBreakBridge)
                return i;
        }

        for(int i=0;i<maxpieces;i++){
            if((int)PosibleMovement[i]<=(int)PosibleMovement[1]){
                PosibleMovement[1]=PosibleMovement[i];
                final_piece_index=i;
            }
        }
        return final_piece_index;



    }
};