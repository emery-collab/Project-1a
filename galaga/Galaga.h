
#ifndef GALAGA_H
#define GALAGA_H

#include <memory>
#include <vector>
#include "NotificationManager.h"
#include "GameObject.h"
#include "Player.h"


enum class States{
    kStartUp, kWaves, kWaitingForGameToBegin, kGamePlay, kGameOver,
    kRespawn
};
namespace CMPUT350 {

class Galaga : public GameObject{

public:

    Galaga(float width, float height);

    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    virtual void WaitingForGameToBegin(GameContext* context);
    virtual void StartUp(GameContext* context);
    virtual void Waves(GameContext* context);
    virtual void GamePlay(GameContext* context);
    virtual void GameOver(GameContext* context);
    void RenderUI(GameContext *context) override;
    void ChangeState(States next, GameContext *context);
    void ReceiveNotification(const std::string& key)override;
    virtual void Respawn(GameContext* context);

private:
    float mWidth;
    float mHeight;
    States mStates;
    bool mStarSpawned = false;
    int mScore;
    int mLives;
    int mLevel;
    int mStateStartFrame;
    int mEnemiesAlive;
    int mCredits;
    bool mGameStart;
    std::weak_ptr<Player> mPlayer;
    int offset;
    int waveNumber;
};

}  // namespace CMPUT350

#endif