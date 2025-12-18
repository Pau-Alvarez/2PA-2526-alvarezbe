#include "pabparcheesi.h"
#include "iparcheesi.h"
#include <algorithm>

int pabparcheesi::EntryBox(int player_index) const {
    const int startPoint[4] = {5, 22, 39, 56};
    return startPoint[player_index];
}

int pabparcheesi::ExitBox(int player_index) const {
    const int finishPoint[4] = {68, 17, 34, 51};
    return finishPoint[player_index];
}

int pabparcheesi::PiecesAtHome(int player_index) const {
    int count = 0;
    for(int i = 0; i < kMaxpiece; i++) {
        int piece_idx = player_index * kMaxpiece + i;
        if(pos_[piece_idx] == 0) {
            count++;
        }
    }
    return count;
}

int pabparcheesi::PiecesAtEnd(int player_index) const {
    int count = 0;
    for(int i = 0; i < kMaxpiece; i++) {
        int piece_idx = player_index * kMaxpiece + i;
        if(pos_[piece_idx] == -2) {
            count++;
        }
    }
    return count;
}

bool pabparcheesi::IsBoxSafe(int box_index) const {
    const int SafePoint[12] = {5, 12, 17, 22, 29, 34, 39, 46, 51, 56, 63, 68};
    for(int i = 0; i < 12; i++) {
        if(box_index == SafePoint[i]) return true;
    }
    return false;
}

IParcheesi::Color pabparcheesi::ColorofPiece(int box_index, int box_piece_index) const {
    if(box_index <= 0) return IParcheesi::Color::None;
    
    int count = 0;

    for(int i = 0; i < kMaxplayers * kMaxpiece; i++) {
        if(pos_[i] == box_index) {
            count++;
            if(count == box_piece_index) {
                int player_id = i / kMaxpiece;
                switch(player_id) {
                    case 0: return IParcheesi::Color::Yellow;
                    case 1: return IParcheesi::Color::Blue;
                    case 2: return IParcheesi::Color::Red;
                    case 3: return IParcheesi::Color::Green;
                    default: return IParcheesi::Color::None;
                }
            }
        }
    }
    return IParcheesi::Color::None;
}

void pabparcheesi::SendPieceHome(int piece_index, int player_index) {
    pos_[piece_index] = 0;
    irrealpos_[piece_index] = 0;
}

pabparcheesi* pabparcheesi::Clone() const {
    return new pabparcheesi(*this);
}

IParcheesi::Movement pabparcheesi::ApplyMovement(int piece_index, int player_index, int count) {

    if(piece_index < 0 || piece_index >= kMaxplayers * kMaxpiece) {
        return IParcheesi::Movement::IllegalPass;
    }
    
    if(piece_index / kMaxpiece != player_index) {
        return IParcheesi::Movement::IllegalPass;
    }
    
    int actual_piece_idx = player_index * kMaxpiece + (piece_index % kMaxpiece);
    
    if(pos_[actual_piece_idx] == 0) {
        if(count == 5) {
            if(PiecesAtHome(player_index) > 0) {
                int entry_box = EntryBox(player_index);
                
                if(ColorofPiece(entry_box, 2) != IParcheesi::Color::None) {
                    return IParcheesi::Movement::IllegalEntryBlocked;
                }
                IParcheesi::Color color_at_entry = ColorofPiece(entry_box, 1);
                if(color_at_entry != IParcheesi::Color::None && 
                   color_at_entry != static_cast<IParcheesi::Color>(player_index)) {
                    for(int i = 0; i < kMaxplayers * kMaxpiece; i++) {
                        if(pos_[i] == entry_box) {
                            SendPieceHome(i, static_cast<int>(color_at_entry));
                            break;
                        }
                    }
                    pos_[actual_piece_idx] = entry_box;
                    irrealpos_[actual_piece_idx] = entry_box;
                    return IParcheesi::Movement::Eat;
                }
                
                pos_[actual_piece_idx] = entry_box;
                irrealpos_[actual_piece_idx] = entry_box;
                return IParcheesi::Movement::Normal;
            }
        }
        return IParcheesi::Movement::IllegalMustEnterPiece;
    }
    
    int current_pos = pos_[actual_piece_idx];
    if(current_pos == -2) {
        return IParcheesi::Movement::NoMoves;
    }

    if(current_pos == -1) {
        int current_exit_pos = irrealpos_[actual_piece_idx] - board_size;
        if(current_exit_pos + count > exit_size) {
            return IParcheesi::Movement::IllegalPastEnd;
        }
        
        irrealpos_[actual_piece_idx] += count;
        if(irrealpos_[actual_piece_idx] == board_size + exit_size) {
            pos_[actual_piece_idx] = -2;
            return IParcheesi::Movement::End;
        }
        return IParcheesi::Movement::ReachExit;
    }
    
    int new_pos = current_pos + count;
    
    if(new_pos > board_size) {
        int overshoot = new_pos - board_size;
        if(overshoot > exit_size) {
            return IParcheesi::Movement::IllegalPastEnd;
        }
        
        pos_[actual_piece_idx] = -1;
        irrealpos_[actual_piece_idx] = board_size + overshoot;
        
        if(overshoot == exit_size) {
            pos_[actual_piece_idx] = -2;
            return IParcheesi::Movement::End;
        }
        return IParcheesi::Movement::ReachExit;
    }
    for(int i = 1; i <= count; i++) {
        int intermediate_pos = (current_pos + i - 1) % board_size + 1;
        IParcheesi::Color col1 = ColorofPiece(intermediate_pos, 1);
        IParcheesi::Color col2 = ColorofPiece(intermediate_pos, 2);
        
        if(col1 != IParcheesi::Color::None && col2 != IParcheesi::Color::None &&
           col1 == col2 && col1 == static_cast<IParcheesi::Color>(player_index)) {
            return IParcheesi::Movement::IllegalBridge;
        }
    }
    
    if(ColorofPiece(new_pos, 2) != IParcheesi::Color::None) {
        return IParcheesi::Movement::IllegalBoxFull;
    }
    
    IParcheesi::Color color_at_new_pos = ColorofPiece(new_pos, 1);
    if(color_at_new_pos != IParcheesi::Color::None && 
       color_at_new_pos != static_cast<IParcheesi::Color>(player_index) &&
       !IsBoxSafe(new_pos)) {
        for(int i = 0; i < kMaxplayers * kMaxpiece; i++) {
            if(pos_[i] == new_pos) {
                SendPieceHome(i, static_cast<int>(color_at_new_pos));
                break;
            }
        }
        pos_[actual_piece_idx] = new_pos;
        irrealpos_[actual_piece_idx] = new_pos;
        return IParcheesi::Movement::Eat;
    }
    
    pos_[actual_piece_idx] = new_pos;
    irrealpos_[actual_piece_idx] = new_pos;
    return IParcheesi::Movement::Normal;
}