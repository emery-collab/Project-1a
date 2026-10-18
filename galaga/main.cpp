#include "GameEngine.h"
#include "Player.h"
#include "Galaga.h"
#include "GraphicsObject.h"

int main()
{

    CMPUT350::GameEngine engine(768, 1024, "Galaga");
    std::shared_ptr<CMPUT350::GameObject> game = std::make_shared<CMPUT350::Galaga>(768, 1024);
    engine.AddGameObject(game);

    engine.Run();
    
    return 0;
}
