#include "GameController.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    GameController::Instance().RunGame();
    return 0;
}
