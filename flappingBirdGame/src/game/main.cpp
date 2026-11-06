#include "./headF/Game.h"
#include "./headF/GameSetting.h"

int main()
{
    GameSettings settings;
    Game game(settings);
    game.Run();
    return 0;
}
