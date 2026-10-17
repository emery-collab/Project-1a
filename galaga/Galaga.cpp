#include "Galaga.h"
#include "GameObject.h"
#include "GameContext.h"
#include "Stars.h"
#include "Player.h"
#include <string> 
#include "Enemy.h"

namespace CMPUT350 {
Galaga::Galaga(float width, float height)
{
    mWidth = width;
    mHeight = height;
    mStates = States::kWaitingForGameToBegin;
    mScore = 0;
    mLives = 3;
    mLevel = 1;
    mStateStartFrame = 0;
    mEnemiesAlive = 0;
    mCredits = 0;
    mGameStart = false;

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

    context->mEngineView->InstallKeyDownNotification(30, "InsertedCredits");
    context->mEngineView->InstallKeyDownNotification(26, "StartGame");
    //A
    context->mEngineView->InstallKeyUpNotification(0, "StopMoveLeft");
    //left
    context->mEngineView->InstallKeyUpNotification(87, "StopMoveLeft");
    //D
    context->mEngineView->InstallKeyUpNotification(3, "StopMoveRight");
    //Right
    context->mEngineView->InstallKeyUpNotification(86, "StopMoveRight");

    context->mEngineView->InstallKeyUpNotification(10, "KillPlayerDebug");
    context->mNotificationManager->Register(context->CurrObject, "EnemyDestroyed");
    context->mNotificationManager->Register(context->CurrObject, "KillPlayerDebug");
    context->mNotificationManager->Register(context->CurrObject, "InsertedCredits");
    context->mNotificationManager->Register(context->CurrObject, "StartGame");
   
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
        case (States::kRespawn):
            Respawn(context);
            break;
        
    }

}

void Galaga::WaitingForGameToBegin(GameContext* context){

    if(mCredits<5) return;
    if (mGameStart) ChangeState(States::kStartUp,context);
    
}
void Galaga::StartUp(GameContext* context){
    //reset the game start boolean to false
    mGameStart = false;
    int elapsed = context->currentFrame - mStateStartFrame;
    //possibly add gui text for a countdown? 
    if (elapsed < 60) return;   

    if(mPlayer.expired()){
        auto player = std::make_shared<Player>(CMPUT350::Point2D(mWidth / 2, 900));
        context->mEngineView->AddGameObject(player);
        mPlayer = player;
    }

    ChangeState(States::kWaves,context);

}
void Galaga::Waves(GameContext* context){
            
    float ySpacing = 60;
    float xSpacing = (768-100)/8;
    for (int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 5; y++){
            auto enemy = std::make_shared<Enemy>(CMPUT350::Point2D(100 + x * xSpacing, y*ySpacing+100));
            context->mEngineView->AddGameObject(enemy);
            mEnemiesAlive++;
        }

    }
    ChangeState(States::kGamePlay,context);
   
    

}
void Galaga::GamePlay(GameContext* context){
    //std::cout<< mEnemiesAlive << "\n";
    if(mPlayer.expired()){
        mLives= mLives -1;
        if(mLives <= 0){
            ChangeState(States::kGameOver, context);
        }
        else{
            ChangeState(States::kRespawn, context);
        }
        
    }
    if(mEnemiesAlive <= 0){
        if(mLives <= 0) return;
        mLevel++;
        ChangeState(States::kStartUp, context);
    } 
    

}
void Galaga::GameOver(GameContext* context){
    int elapsed = context->currentFrame - mStateStartFrame;
    if (elapsed < 90) return;
    for (const auto &obj : *(context->mEngineView)) obj->Kill();
    mEnemiesAlive = 0;
    mLives = 3;
    mLevel = 1;
    mCredits = 0;
    mScore = 0;
    ChangeState(States::kWaitingForGameToBegin, context);

}
void Galaga::Respawn(GameContext* context){
    int elapsed = context->currentFrame - mStateStartFrame;
    if (elapsed < 60) return;

    auto player = std::make_shared<Player>(CMPUT350::Point2D(768 / 2, 900));
    context->mEngineView->AddGameObject(player);
    mPlayer = player;
    ChangeState(States::kGamePlay,context);


}
void Galaga::RenderUI(GameContext *context){
    if(mStates == States::kWaitingForGameToBegin){
        auto displayStart = "Coins: " + std::to_string(mCredits) + "/5";
        int livesSizeGameOver = 30;
        context->GUIContext->DrawText(displayStart, livesSizeGameOver,  Point2D(mWidth/2-175, mHeight/2), CMPUT350::Colors::white);
    }
    else{

        auto displayScore = "SCORE " + std::to_string(mScore);
        int scoreSize = 16;
        context->GUIContext->DrawText(displayScore, scoreSize,  Point2D(10, 10), CMPUT350::Colors::white);

        auto displayLives = "LIVES " + std::to_string(mLives);
        int livesSize = 16;
        context->GUIContext->DrawText(displayLives, livesSize,  Point2D(mWidth-150, 10), CMPUT350::Colors::white);

        auto displayStage = "STAGE " + std::to_string(mLevel);
        int levelSize = 24;
        context->GUIContext->DrawText(displayStage, levelSize,  Point2D(mWidth/2-75, 10), CMPUT350::Colors::white);

        if(mStates == States::kRespawn){
            auto displayStageRespawn = "LIVE(S) " + std::to_string(mLives) + " Ready?";
            int livesSizeRespawn = 30;
            context->GUIContext->DrawText(displayStageRespawn, livesSizeRespawn,  Point2D(mWidth/2-225, mHeight/2), CMPUT350::Colors::white);

        }
        if(mStates == States::kGameOver){
            auto displayStageGameOver = "GAME OVER";
            int livesSizeGameOver = 40;
            context->GUIContext->DrawText(displayStageGameOver, livesSizeGameOver,  Point2D(mWidth/2-175, mHeight/2), CMPUT350::Colors::white);

        }
        
    }


}
void Galaga::ChangeState(States next, GameContext *context){
    mStates = next;
    mStateStartFrame = context->currentFrame;
    

}
void Galaga::ReceiveNotification(const std::string& key){
    if ((key == "EnemyDestroyed") && (mStates ==States::kGamePlay || mStates == States::kRespawn) ){
        mScore+=100;
        mEnemiesAlive= mEnemiesAlive - 1;
    }
    if((key == "KillPlayerDebug")){
        if(auto p = mPlayer.lock()){
            p->Kill();
        }
    }
    if((key == "InsertedCredits")&& mCredits < 5){
        mCredits++;
    }
    if((key == "StartGame") && mCredits >= 5){
        mGameStart = true;
    }

}
}