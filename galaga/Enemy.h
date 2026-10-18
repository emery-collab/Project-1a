// Comments in this file were AI-generated (Claude); code written by the team.
#ifndef ENEMY_H
#define ENEMY_H

#include "CollisionObject.h"
#include "GameContext.h"


enum class EnemyStates{
    kEntering, kWaiting,kAttacking
};
// A stationary enemy drawn as a square; dies when hit by a player bullet
class Enemy : public CMPUT350::CollisionObject
{
public:
    // loc: center of the enemy
    Enemy(CMPUT350::Point2D loc,const std::vector<CMPUT350::Point2D> &path);

    // GameObject Functions
    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    // bool HandleKeyEvent(CMPUT350::GameContext* context, char key) override;
    bool IsAlive() const override;
    void Kill() override;

    // Graphics Object Functions
    void RenderBackground(CMPUT350::GameContext* context) override;
    void RenderForeground(CMPUT350::GameContext* context) override;
    // new virtual functions for partb inherited from collisionobject.h
    CMPUT350::Point2D GetLocation() const override;
    float GetRotation() const override;
    const std::vector<CMPUT350::Shape>& GetShapes() override;


    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    const CMPUT350::Rect& GetBounds() override;

    //Attack
    virtual bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) = 0;
protected:
    bool mAlive;
    CMPUT350::Rect mBounds;
    float length;

    // new member variables
    std::vector<CMPUT350::Shape> mShapes;
    CMPUT350::Point2D location;
    CMPUT350::Point2D formationSpot;
    std::vector<CMPUT350::Point2D> mPath;
    float t;
    float mSpeed;
    float mRotation;
    int mHealth;
    EnemyStates mStates;
};


#endif

