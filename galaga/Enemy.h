// Comments in this file were AI-generated (Claude); code written by the team.
#ifndef ENEMY_H
#define ENEMY_H

#include "CollisionObject.h"
#include "GameContext.h"
#include "Bullet.h"


// Flying in, in formation, or attacking
enum class EnemyStates{
    kEntering, kWaiting,kAttacking
};
// Base class for all enemies: follows Bezier paths, hovers in formation, fires while attacking
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
    EnemyStates GetState() const;   // Lets Galaga check if the enemy is attacking
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
    CMPUT350::Point2D formationSpot;        // Home spot in the formation
    std::vector<CMPUT350::Point2D> mPath;   // Current entry or attack path
    float t;                                // Progress along mPath
    float mSpeed;
    float mRotation;
    int mHealth;
    EnemyStates mStates;
    std::weak_ptr<Bullet> mShot1;           // Last bullet fired
};


#endif

