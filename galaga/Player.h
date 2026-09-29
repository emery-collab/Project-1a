#ifndef PLAYER_H
#define PLAYER_H

#include "CollisionObject.h"
#include "Bullet.h"

class Player : public CMPUT350::CollisionObject
{
public:
    Player(CMPUT350::Point2D loc);

    // GameObject Functions
    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    bool HandleKeyEvent(CMPUT350::GameContext* context, char key) override;
    bool IsAlive() const override;
    void Kill() override;

    // Graphics Object Functions
    void RenderBackground(CMPUT350::GameContext* context) override;
    void RenderForeground(CMPUT350::GameContext* context) override;


    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    const CMPUT350::Rect& GetBounds() override;

    

private:
    void BuildShape(CMPUT350::Point2D loc);
    bool mAlive;
    CMPUT350::Point2D location;
    float width = 40;
    float height = 40;
    CMPUT350::Rect mBounds;

    CMPUT350::Rect nozzle;
    CMPUT350::Rect nozzle2;
    CMPUT350::Rect body;
    CMPUT350::Rect body2;
    CMPUT350::Rect cockpit;
    CMPUT350::Rect thrusterL;
    CMPUT350::Rect thrusterR;
    CMPUT350::Point2D bulletSpeed;

    std::weak_ptr<Bullet> mShot2;
    std::weak_ptr<Bullet> mShot1;
};

#endif
