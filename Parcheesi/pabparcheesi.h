#pragma once
#ifndef PAB_PARCHEESI_H
#define PAB_PARCHEESI_H

#include "iparcheesi.h"

class pabparcheesi : public IParcheesi{
    static const int kMaxplayers=4;
    static const int kMaxpiece=4;
    
    int pos_[kMaxplayers*kMaxpiece];
    int irrealpos_[kMaxplayers*kMaxpiece];

    virtual int EntryBox(int player_index) const;//
    virtual int ExitBox(int player_index) const;//
    virtual int PiecesAtHome(int player_index) const;//
    virtual int PiecesAtEnd(int player_index) const;//
    virtual bool IsBoxSafe(int box_index) const;//
    // higher box_piece_index means the piece moved later
    virtual IParcheesi::Color ColorofPiece(int box_index, int box_piece_index) const ;//
    virtual pabparcheesi* Clone() const;//
    virtual IParcheesi::Movement ApplyMovement(int piece_index, int player_index, int count);
    virtual void SendPieceHome(int piece_index, int player_index);

};





#endif // PARCHEESI_IPARCHEESI_H