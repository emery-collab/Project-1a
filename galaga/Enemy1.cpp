// Comments in this file were AI-generated (Claude); code written by the team.
#include "Enemy1.h"
#include "Bullet.h"
#include <iostream>
#include "NotificationManager.h"
// Creates a 30x30 enemy centered at loc


void Enemy1::Initialize(CMPUT350::GameContext* context)
{

}
bool Enemy1::Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target)
{
    if(mStates == EnemyStates::kWaiting){
        t = 0;
        formationSpot = location;
        std::vector<CMPUT350::Point2D> path = {
            formationSpot,
            CMPUT350::Point2D(target.x-100, target.y-500),
            CMPUT350::Point2D(target.x-50, target.y-300),
            CMPUT350::Point2D(target.x, target.y),
            CMPUT350::Point2D(target.x+50, target.y-300),
            CMPUT350::Point2D(target.x+100, target.y-500),
            formationSpot
        };
        mPath = path;
        mStates = EnemyStates::kAttacking;
        return true;
    }
    if(mStates == EnemyStates::kAttacking){
        return false;


    }
    return false;

}

void Enemy1::LateUpdate(CMPUT350::GameContext* context)
{
    if(!mAlive){
        context->mNotificationManager->Notify("EnemyDestroyed");
    }
}


// Draws the enemy as a red square
void Enemy1::RenderForeground(CMPUT350::GameContext* context)
{
    //context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 30), CMPUT350::Colors::red);
}
