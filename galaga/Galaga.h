
#ifndef GALAGA_H
#define GALAGA_H

#include <memory>
#include <vector>
#include "NotificationManager.h"
#include "GameObject.h"
namespace CMPUT350 {

class Galaga : public GameObject{

public:

    Galaga(float width, float height);
    virtual void WaitingForGameToBegin();
    virtual void StartUp(int state);
    virtual void Waves(int state);
    virtual void GamePlay(int state);
    virtual void GameOver();

private:
    float mWidth;
    float mHeight;
    int state;
};

}  // namespace CMPUT350

#endif