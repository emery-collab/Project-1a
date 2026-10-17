#include "Galaga.h"
#include "GameObject.h"
#include "GameContext.h"
#include "Stars.h"
#include "Player.h"

#include "Enemy.h"

namespace CMPUT350 {
Galaga::Galaga(float width, float height)
{
    mWidth = width;
    mHeight = height;
    mStates = States::kStartUp;

}

void Galaga::Initialize(CMPUT350::GameContext* context){
    //A
    context->mEngineView->InstallKeyDownNotification(0, "MoveLeft");
    // //left
    context->mEngineView->InstallKeyDownNotification(87, "MoveLeft");
    // //D
    context->mEngineView->InstallKeyDownNotification(3, "MoveRight");
    // //Right
    context->mEngineView->InstallKeyDownNotification(86, "MoveRight");
    // //Space
    context->mEngineView->InstallKeyDownNotification(40, "Fire");
    //A
    context->mEngineView->InstallKeyUpNotification(0, "StopMoveLeft");
    //left
    context->mEngineView->InstallKeyUpNotification(87, "StopMoveLeft");
    //D
    context->mEngineView->InstallKeyUpNotification(3, "StopMoveRight");
    //Right
    context->mEngineView->InstallKeyUpNotification(86, "StopMoveRight");
    //Engine doesn't allow us to call things in initialize yet but it should work when it gets there. 
    //context->mEngineView->AddGameObject(std::make_shared<Stars>(250, CMPUT350::Rect(0, 0, 768, 1024)));
    bool mStarSpawned = false;

}
void Galaga::Update(CMPUT350::GameContext* context){
    
    if(!mStarSpawned){
        //temporary
        context->mEngineView->AddGameObject(std::make_shared<Stars>(100, CMPUT350::Rect(0, 0, 768, 1024)));
        mStarSpawned = true;
    }

    switch (mStates)
    {
        case(States::kWaitingForGameToBegin):
            WaitingForGameToBegin(context);
            break;
        case (States::kStartUp):     
            StartUp(context);     
            break;
        case (States::kGamePlay):
            GamePlay(context);
            break;
        case (States::kWaves):
            Waves(context);
            break;
        case (States::kGameOver):
            GameOver(context);
            break;
    }

}

void Galaga::WaitingForGameToBegin(GameContext* context){

}
void Galaga::StartUp(GameContext* context){
    auto player = std::make_shared<Player>(CMPUT350::Point2D(768 / 2, 900));
    context->mEngineView->AddGameObject(player);
    mStates = States::kWaves;

}
void Galaga::Waves(GameContext* context){
            
    float ySpacing = 60;
    float xSpacing = (768-100)/8;
    for (int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 5; y++){
            auto enemy = std::make_shared<Enemy>(CMPUT350::Point2D(100 + x * xSpacing, y*ySpacing+100));
            context->mEngineView->AddGameObject(enemy);
        }
    }
    mStates = States::kGamePlay;

}
void Galaga::GamePlay(GameContext* context){

}
void Galaga::GameOver(GameContext* context){

}
}