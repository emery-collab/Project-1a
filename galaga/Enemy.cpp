// Comments in this file were AI-generated (Claude); code written by the team.
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>

// Creates a 30x30 enemy centered at loc
Enemy::Enemy(CMPUT350::Point2D loc)
{
    float h = 30;
    float w = 30;
    mAlive = true;
    float top = loc.y - h/2;
    float left = loc.x - w/2;
    mBounds = CMPUT350::Rect(CMPUT350::Point2D(left,top), w, h);
}

void Enemy::Initialize(CMPUT350::GameContext* context)
{
}

void Enemy::Update(CMPUT350::GameContext* context)
{
}

void Enemy::LateUpdate(CMPUT350::GameContext* context)
{
}

bool Enemy::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return false;
}

void Enemy::RenderBackground(CMPUT350::GameContext* context)
{
}

// Draws the enemy as a red square
void Enemy::RenderForeground(CMPUT350::GameContext* context)
{
    context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
}

// Dies when hit by a player bullet
void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(obj);

    std::cerr << "Did I die?: " << IsAlive() << "\n";

    if(bullet != nullptr && bullet->IsPlayerBullet()){
        Kill();
        
        std::cerr << "Did I die?: " << IsAlive() << "\n";
    }
}

// Marks the enemy dead so the engine removes it next frame
void Enemy::Kill()
{
    mAlive = false;
}

bool Enemy::IsAlive() const
{
    return mAlive;
}

// Returns the bounding box used for collisions
const CMPUT350::Rect& Enemy::GetBounds()
{
    return mBounds;
}
