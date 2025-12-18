#include "game.h"
#include "pabparcheesi.h"

class PauPlayer : public IPlayer{

    private:
        int this_index;

    public:

        PauPlayer(int index) : this_index(index) {};

    int PauPlayer::DecideMove(const IParcheesi& parcheesi, int dice_roll) const;
};