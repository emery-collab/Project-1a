#include "Enemy.h"
#include "Bullet.h"

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

void Enemy::RenderForeground(CMPUT350::GameContext* context)
{
    context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
}

void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
}

void Enemy::Kill()
{
    mAlive = false;
}

bool Enemy::IsAlive() const
{
    return mAlive;
}

const CMPUT350::Rect& Enemy::GetBounds()
{
    return mBounds;
}
