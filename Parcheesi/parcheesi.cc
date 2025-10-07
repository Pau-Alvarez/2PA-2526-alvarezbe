
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

MovementOutput Parcheesi::ApplyMovement(int PieceIndex, int mov_num){
 player_[current_player_].piece_[PieceIndex].position=(player_[current_player_].piece_[PieceIndex].position+mov_num)%68;

return kMovementOutput_BasicMove;
}

//IDEAS ANTIGUAS


/* int CheckEat(Parcheesi parchis,int dice,int active_pieces[Player::kPieceNum],int count){
  for(int i=0 ; i<count ; i++){
    for(int j=0 ; j<parchis.num_players-1; j++){
      for(int k=0 ; k<Player::kPieceNum; k++){
        if((parchis.player[parchis.currentPlayer].piece[i].pos + dice) == parchis.player[(parchis.currentPlayer+1+j)%4].piece[k].pos)
          parchis.player[(parchis.currentPlayer+1+j)%4].piece[k].state = kHouse;
          return i;
      }
    }
  }
  return NotFound;
}

int CheckFinalLine(Player player){
  for(int i=0 ; i<Player::kPieceNum ; i++ ){
    if(player.piece[i].state==kFinalLine){
      return i;
    }
  }
  return NotFound;
}

int CheckBridge(Player player,int dice,int active_pieces[Player::kPieceNum],int count){
  for(int i=0 ; i<count ; i++){
    for(int j=0; j<count ; j++){
      if((player.piece[i].pos + dice) == player.piece[j].pos){
        return i;
      }
    } 
  }
  return NotFound;
}

int CheckBigmove(Player player,int active_pieces[Player::kPieceNum],int count){
  int biggest=0;
  int moving_piece=0;
  for (int i=0 ; i<count ; i++){
    if(biggest <= player.piece[i].pos){
        biggest = player.piece[i].pos;
        moving_piece = i;
      }
  }
}

FinalMovement Move(Parcheesi parchis, int dice){
  int active_pieces[Player::kPieceNum];
  int count=0;
  for(int i=0 ; i<Player::kPieceNum ; i++ ){
    if(parchis.player[parchis.currentPlayer].piece[i].state==kActive){
      active_pieces[count]=i;
      count++;
    }
  }
  int eatable=CheckEat(parchis,dice,active_pieces,count);
  if(eatable!=NotFound){
    FinalMovement move = {kEat , eatable};
    return move;
  }
  int finish_line=CheckFinalLine(parchis.player[parchis.currentPlayer]);
  if(finish_line!=NotFound){
    FinalMovement move = {kFinish , finish_line};
    return move;
  }
  int bridge=CheckBridge(parchis.player[parchis.currentPlayer],dice,active_pieces,count);
  if(bridge!=NotFound){
    FinalMovement move = {kBridge , bridge};
    return move;
  }
  FinalMovement move = {kBigadvance , CheckBigmove(parchis.player[parchis.currentPlayer],active_pieces,count)};
  return move;
}

void Turn(Parcheesi parchis){
  const int kSpawnPlayer=5;
  int dice_num=Dicethrow();
  if(dice_num == kSpawnPlayer && CheckHome(parchis.player[parchis.currentPlayer]) && CheckFullStart(parchis.player[parchis.currentPlayer])){
    int count=0;
    while (count<= Player::kPieceNum){
      if(parchis.player[parchis.currentPlayer].piece[count].state==kHouse){
        parchis.player[parchis.currentPlayer].piece[count].pos=Parcheesi::startPoint[parchis.currentPlayer];
        parchis.player[parchis.currentPlayer].piece[count].state=kActive;
        count=Player::kPieceNum;
      }else{
        count++;
      }
    }
  }else if(CheckAlive(parchis.player[parchis.currentPlayer])>0){
    FinalMovement move = Move(parchis, dice_num);
    switch (move.movement)
    {
    case kEat:
      parchis.player[parchis.currentPlayer].piece[move.piece].pos+=20+dice_num;
      if(parchis.player[parchis.currentPlayer].piece[move.piece].pos > 68){
        parchis.player[parchis.currentPlayer].piece[move.piece].pos -=68;
        if(parchis.player[parchis.currentPlayer].piece[move.piece].pos > Parcheesi::finishPoint[parchis.currentPlayer]){
          int finish_pos = parchis.player[parchis.currentPlayer].piece[move.piece].pos - Parcheesi::finishPoint[parchis.currentPlayer];
          parchis.player[parchis.currentPlayer].piece[move.piece].state = kFinalLine;
        }
      }
      break;
    
     case kFinish:
     break;

    default:
      break;
    }
  }
}*/