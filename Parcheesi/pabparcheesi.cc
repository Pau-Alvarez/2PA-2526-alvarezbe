#include "pabparcheesi.h"
#include "iparcheesi.h"

int pabparcheesi::EntryBox(int player_index) const{
    const int max_players = 4;
    const int startPoint[max_players] = {5, 22, 39, 56};
    return startPoint[player_index];
}

int pabparcheesi::ExitBox(int player_index) const{
    const int max_players = 4;
    const int finishPoint[max_players] = {68, 17, 34, 51};
    return finishPoint[player_index];
}

int pabparcheesi::PiecesAtHome(int player_index) const{
    int count=0;
    const int max_players = 4;
    for(int i=0;i<max_players;i++){
        if(pos_[player_index*kMaxpiece+i]==0){
            count++;
        }
    }
    return count;
}

bool pabparcheesi::IsBoxSafe(int box_index) const{
    const int safe_points = 12;
    const int SafePoint[safe_points] = {5, 12, 17, 22, 29, 34, 39, 46, 51, 56, 63, 68};
    for(int i=0;i<safe_points;i++){
        if(box_index==SafePoint[i]) return true;
    }
    return false;
}


IParcheesi::Color pabparcheesi::ColorofPiece(int box_index, int box_piece_index)const {
    bool firstloop=false;
    for(int i=0;i<kMaxpiece*kMaxplayers;i++){
        if(pos_[i]==box_index){
            if(box_piece_index==1){
                int player_id =(int)(i/kMaxplayers);
                switch (player_id)
                {
                case 0 :
                    return IParcheesi::Color::Yellow;
                    break;
                case 1 :
                    return IParcheesi::Color::Blue;
                    break;
                case 2 :
                    return IParcheesi::Color::Red;
                    break;
                case 3 :
                    return IParcheesi::Color::Green;
                    break;
                
                default:
                return IParcheesi::Color::None;
                    break;
                }
            }else if(firstloop==false){
                firstloop==true;
            }else{
                int player_id =(int)(i/kMaxplayers);
                switch (player_id)
                {
                case 0 :
                    return IParcheesi::Color::Yellow;
                    break;
                case 1 :
                    return IParcheesi::Color::Blue;
                    break;
                case 2 :
                    return IParcheesi::Color::Red;
                    break;
                case 3 :
                    return IParcheesi::Color::Green;
                    break;
                
                default:
                return IParcheesi::Color::None;
                    break;
                }
            }
        }
    }
    return IParcheesi::Color::None;
}

void pabparcheesi::SendPieceHome(int piece_index, int player_index){
    pos_[piece_index]=0;
    irrealpos_[piece_index]=0;
}


pabparcheesi* pabparcheesi::Clone() const{
    return new pabparcheesi{*this};
}

IParcheesi::Movement pabparcheesi::ApplyMovement(int piece_index, int player_index, int count){
    IParcheesi::Color actuallcolor=(IParcheesi::Color)player_index;

    if(count==5){
        if(PiecesAtHome(player_index)!=0){
            if(ColorofPiece(EntryBox(player_index),2)!=IParcheesi::Color::None){
                if(ColorofPiece(EntryBox(player_index),1)==actuallcolor && ColorofPiece(EntryBox(player_index),2)==actuallcolor){
                    return IParcheesi::Movement::IllegalEntryBlocked;
                }else{
                    int index;
                    for(int i=0;i<kMaxpiece;i++){
                        if(pos_[(int)ColorofPiece(EntryBox(player_index),2)]*kMaxpiece+i==EntryBox(player_index)){
                            int index=pos_[(int)ColorofPiece(EntryBox(player_index),2)]*kMaxpiece+i;
                        }
                    }
                    SendPieceHome(index,(int)ColorofPiece(EntryBox(player_index),2));
                    pos_[piece_index]=EntryBox(player_index);
                    irrealpos_[piece_index]=0;
                    return IParcheesi::Movement::Eat;
                }
            }else{
                pos_[piece_index]=EntryBox(player_index);
                irrealpos_[piece_index]=0;
                IParcheesi::Movement::Normal;
            }
        }   
    }
    if(pos_[piece_index]==0)return IParcheesi::Movement::IllegalPieceAtHome;
    for(int i=1;i<count-1;i++){
        if(ColorofPiece(pos_[piece_index]+i,2)!=IParcheesi::Color::None){
            return IParcheesi::Movement::IllegalBridge;
        }
    }
    if(ColorofPiece((pos_[piece_index]+count)%IParcheesi::board_size,2)!=IParcheesi::Color::None)
            return IParcheesi::Movement::IllegalBoxFull;
    
    if(irrealpos_[piece_index]+count>IParcheesi::exit_size+IParcheesi::board_size) 
        return IParcheesi::Movement::IllegalPastEnd;

    if(ColorofPiece(pos_[piece_index],2)==ColorofPiece(pos_[piece_index],2))
        return IParcheesi::Movement::IllegalMustBreakBridge;

    if(irrealpos_[piece_index]+count>IParcheesi::board_size){
        pos_[piece_index]=-1;
        irrealpos_[piece_index]+=count;
        return IParcheesi::Movement::ReachExit;
    }

    if(irrealpos_[piece_index]+count==IParcheesi::board_size+IParcheesi::exit_size){
        pos_[piece_index]=-2;
        irrealpos_[piece_index]+=count;
        return IParcheesi::Movement::End;
    }

    int expectedmove=(pos_[piece_index]+count)%IParcheesi::board_size;

    for(int i=0;i<kMaxpiece*kMaxplayers;i++){
        if(expectedmove==pos_[i]){
            pos_[piece_index]=expectedmove;
            irrealpos_[piece_index]+=count;
            int index;
            for(int i=0;i<kMaxpiece;i++){
                if(pos_[(int)ColorofPiece(expectedmove,1)]*kMaxpiece+i==expectedmove){
                    int index=pos_[(int)ColorofPiece(expectedmove,1)]*kMaxpiece+i;
                }
            }
            SendPieceHome(index,(int)ColorofPiece(expectedmove,1));
            return IParcheesi::Movement::Eat;
        }
    }
    pos_[piece_index]=expectedmove;
    irrealpos_[piece_index]+=count;
    return IParcheesi::Movement::Normal;
}
