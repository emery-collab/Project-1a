// Comments in this file were AI-generated (Claude); code written by the team.
#ifndef PLAYER_H
#define PLAYER_H

#include "CollisionObject.h"
#include "Bullet.h"

// The player's ship; moves left/right and fires up to two bullets at a time
class Player : public CMPUT350::CollisionObject
{
public:
    // loc: center of the ship
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
    // Rebuilds the ship's bounds and shape around loc
    void BuildShape(CMPUT350::Point2D loc);
    bool mAlive;
    CMPUT350::Point2D location;
    float width = 40;
    float height = 40;
    CMPUT350::Rect mBounds;

    // Rectangles making up the ship's drawing
    CMPUT350::Rect nozzle;
    CMPUT350::Rect nozzle2;
    CMPUT350::Rect body;
    CMPUT350::Rect body2;
    CMPUT350::Rect cockpit;
    CMPUT350::Rect thrusterL;
    CMPUT350::Rect thrusterR;
    CMPUT350::Point2D bulletSpeed;  // Movement per frame of fired bullets

    // Weak pointers so we can tell when a shot is gone without keeping it alive
    std::weak_ptr<Bullet> mShot2;
    std::weak_ptr<Bullet> mShot1;
};

#endif
