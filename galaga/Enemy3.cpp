// Comments in this file were AI-generated (Claude); code written by the team.
#include "Enemy3.h"
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
#include "NotificationManager.h"
// Creates a 30x30 enemy centered at loc
void Enemy3::Initialize(CMPUT350::GameContext* context)
{
    rot

}
bool Enemy3::Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target)
{
    if(mStates == EnemyStates::kWaiting){
        t = 0;
        //formationSpot = location;
        std::vector<CMPUT350::Point2D> path = {
            formationSpot,
            CMPUT350::Point2D(600, 30),
            CMPUT350::Point2D(250, 100),
            CMPUT350::Point2D(500, target.y + 700),
            CMPUT350::Point2D(650, 400),
            CMPUT350::Point2D(target.x, target.y),
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

void Enemy3::LateUpdate(CMPUT350::GameContext* context)
{
    if(!mAlive){
        if(!(mStates == EnemyStates::kWaiting)) context->mNotificationManager->Notify("Enemy3DestroyedInFormation");
        else context->mNotificationManager->Notify("Enemy3DestroyedInFormation");
    }
}


// Draws the enemy as a red square
void Enemy3::RenderForeground(CMPUT350::GameContext* context)
{
    //context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
    if(mHealth == 3){
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 10), CMPUT350::Colors::blue);
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 10, 30), CMPUT350::Colors::blue);
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(10, -15), 10, 30), CMPUT350::Colors::blue);
    }
    if(mHealth == 2){
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 10), CMPUT350::Colors::green);
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 10, 30), CMPUT350::Colors::green);
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(10, -15), 10, 30), CMPUT350::Colors::green);
    }
    if(mHealth ==1){
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 10), CMPUT350::Colors::red);
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-10, -15), 10, 30), CMPUT350::Colors::red);
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(10, -15), 10, 30), CMPUT350::Colors::red);
    }
}

