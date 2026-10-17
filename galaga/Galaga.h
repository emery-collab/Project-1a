
#ifndef GALAGA_H
#define GALAGA_H

#include <memory>
#include <vector>
#include "NotificationManager.h"
#include "GameObject.h"

enum class States{
    kStartUp, kWaves, kWaitingForGameToBegin, kGamePlay, kGameOver
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

private:
    float mWidth;
    float mHeight;
    States mStates;
    bool mStarSpawned = false;
};

}  // namespace CMPUT350

#endif