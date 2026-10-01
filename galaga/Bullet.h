// Comments in this file were AI-generated (Claude); code written by the team.
#ifndef BULLET_H
#define BULLET_H

#include "CollisionObject.h"
#include "GameContext.h"

// A bullet that travels in a straight line; destroys enemies if fired by the player
class Bullet : public CMPUT350::CollisionObject
{
public:
    // location: spawn point, heading: movement per frame, player: true if fired by the player
    Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player);
    // Returns true if the player fired this bullet
    bool IsPlayerBullet();

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
    bool mAlive;
    CMPUT350::Rect mBounds;
    CMPUT350::Line mLine;
    bool isPlayer;
    float length;
    CMPUT350::Point2D starting;    // Position last frame
    CMPUT350::Point2D ending;      // Position this frame
    CMPUT350::Point2D headingAmt;  // Movement per frame

};
#endif // BULLET_H
