#include "Bullet.h"

Bullet::Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player)
{
    isPlayer = player;

    starting = location;

    headingAmt = heading;

    ending = location + headingAmt;

    
    mLine = CMPUT350::Line(location,ending);

    length = mLine.Length();

    mAlive = true;

    mBounds = CMPUT350::Rect(starting, ending);
}

bool Bullet::IsPlayerBullet()
{
    return isPlayer;
}

void Bullet::Initialize(CMPUT350::GameContext* context)
{
}

void Bullet::Update(CMPUT350::GameContext* context)
{
    starting = ending;
    ending = ending + headingAmt;

    mLine = CMPUT350::Line(starting, ending);
    mBounds = CMPUT350::Rect(starting,ending);
    int windowHeight = context->ScreenContext->GetWindowHeight();
    int windowwidth = context->ScreenContext->GetWindowWidth();

    if(starting.x < 0 || starting.y < 0 || starting.y > windowHeight || starting.x > windowwidth){
        Kill();
    }
}

void Bullet::LateUpdate(CMPUT350::GameContext* context)
{
}

bool Bullet::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return false;
}

void Bullet::RenderBackground(CMPUT350::GameContext* context)
{
}

void Bullet::RenderForeground(CMPUT350::GameContext* context)
{
    //Do we want different colors for enemy and player bullets?
    context->ScreenContext->DrawLine(mLine.p1, mLine.p2, 4, CMPUT350::Colors::white);
}

void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
}

void Bullet::Kill()
{
    mAlive = false;
}

bool Bullet::IsAlive() const
{
    return mAlive;
}

const CMPUT350::Rect& Bullet::GetBounds()
{
    return mBounds;
}
