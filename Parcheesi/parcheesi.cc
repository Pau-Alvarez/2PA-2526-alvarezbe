
//Parchis 26/09/2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parcheesi.h"

const int Parcheesi::safePoint[Parcheesi::kSafePoints] = {5, 12, 17, 22, 29, 34, 39, 46, 51, 56, 63, 68};
const int Parcheesi::startPoint[Parcheesi::kMaxPlayers] = {5, 22, 39, 56};
const int Parcheesi::finishPoint[Parcheesi::kMaxPlayers] = {68, 17, 34, 51};

int RollDice(){
  return (rand() % 6) + 1;
}

Parcheesi::Parcheesi(int num_players)
  : num_players_{num_players}
  , current_turn_{0}
  , repeated_sixes_{0}
  , current_player_{kColor_Yellow}
  , player_{kColor_Yellow,kColor_Blue,kColor_Red,kColor_Green} {

}

int Parcheesi::CountPiecesOnBox(int box){
  int occupants = 0;
  for(int i = 0; i < num_players_; i++){
    occupants += player_[i].CountPiecesOnBox(box);
  }
  return occupants;
}

void Parcheesi::NextPlayer(){
  int num_player = static_cast<int>(current_player_);
  num_player = (num_player + 1) % num_players_;
  current_player_ = static_cast<Colors>(num_player);
}

void Parcheesi::PlayTurn(){
  int dice_num;
  dice_num = RollDice();
  for(int i = 0; i < Player::kPieceNum; i++){
    if(dice_num == 5 && player_[current_player_].piece_[i].position == 0 &&
        CountPiecesOnBox(startPoint[current_player_]) < 2){
      player_[current_player_].piece_[i].position = startPoint[current_player_];
      NextPlayer();
      return;
    }
  }
}

MovementOutput Parcheesi::ApplyMovement(int piece, int mov_num){

  for (int i=0 ; i<mov_num ; i++){
    if(CountPiecesOnBox(piece+i+1)==2){
      return kMovementOutput_NoMove;
    }
  }

  if(){
    
  }
}
