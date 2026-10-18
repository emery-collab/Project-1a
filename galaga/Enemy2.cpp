// Comments in this file were AI-generated (Claude); code written by the team.
#include "Enemy2.h"
#include "Bullet.h"
#include <iostream>
#include "NotificationManager.h"
// Creates a 30x30 enemy centered at loc

void Enemy2::Initialize(CMPUT350::GameContext* context)
{

}
bool Enemy2::Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target)
{

}

void Enemy2::LateUpdate(CMPUT350::GameContext* context)
{
    if(!mAlive){
        context->mNotificationManager->Notify("EnemyDestroyed");
    }
}


// Draws the enemy as a red square
void Enemy2::RenderForeground(CMPUT350::GameContext* context)
{
    //context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 30), CMPUT350::Colors::red);
}

