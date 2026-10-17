#include "Galaga.h"
#include "GameObject.h"
#include "GameContext.h"
#include "Stars.h"

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


}
void Galaga::Update(CMPUT350::GameContext* context){
    bool mStarSpawned = false;
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
            GamePlay(context);
            break;
    }

}

void Galaga::WaitingForGameToBegin(GameContext* context){

}
void Galaga::StartUp(GameContext* context){

}
void Galaga::Waves(GameContext* context){

}
void Galaga::GamePlay(GameContext* context){

}
void Galaga::GameOver(GameContext* context){

}
}