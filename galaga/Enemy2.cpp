// Comments in this file were AI-generated (Claude); code written by the team.
#include "Enemy2.h"
#include "Bullet.h"
#include <iostream>
#include "NotificationManager.h"
// Creates a 30x30 enemy centered at loc

void Enemy2::Initialize(CMPUT350::GameContext* context)
{
    mRotation = 
}
bool Enemy2::Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target)
{
    if(mStates == EnemyStates::kWaiting){
        t = 0;
        //formationSpot = location;
        std::vector<CMPUT350::Point2D> path = {
            formationSpot,
            CMPUT350::Point2D(250, -20),
            CMPUT350::Point2D(600, target.y + 50),
            CMPUT350::Point2D(750, 200),
            CMPUT350::Point2D(350, 550),
            CMPUT350::Point2D(target.x + 200, target.y - 200),
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

void Enemy2::LateUpdate(CMPUT350::GameContext* context)
{
    if(!mAlive){
        if(!(mStates == EnemyStates::kWaiting)) context->mNotificationManager->Notify("Enemy2DestroyedInFormation");
        else context->mNotificationManager->Notify("Enemy2DestroyedInFormation");
    }
}


// Draws the enemy as a red square
void Enemy2::RenderForeground(CMPUT350::GameContext* context)
{
    //context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
    if(mHealth == 2){
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 10), CMPUT350::Colors::green);
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-5, -10), 10, 20), CMPUT350::Colors::green);
    }
    if(mHealth ==1){
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 10), CMPUT350::Colors::red);
        context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-5, -10), 10, 20), CMPUT350::Colors::red);
    }
}


