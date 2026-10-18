// Comments in this file were AI-generated (Claude); code written by the team.
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
#include "NotificationManager.h"
#include "Bezier.h"
#include "Player.h"

// Creates a 30x30 enemy centered at loc

Enemy::Enemy(CMPUT350::Point2D loc,const std::vector<CMPUT350::Point2D> &path )
{
    float h = 30;
    float w = 30;
    mAlive = true;
    float top = loc.y - h/2;
    float left = loc.x - w/2;
    mBounds = CMPUT350::Rect(CMPUT350::Point2D(left,top), w, h);

    // new constructor calls partb
    location = loc;
    formationSpot = loc;
    mShapes.clear();
    mShapes.push_back(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 30));
    mPath = path;
    t = 0;
    mSpeed = 0.01;
    mRotation = (0,0);
    mStates = EnemyStates::kEntering;

};
void Enemy::Initialize(CMPUT350::GameContext* context)
{
}

void Enemy::Update(CMPUT350::GameContext* context)
{
   switch (mStates)
    {

        case(EnemyStates::kEntering):{

            auto slope = CMPUT350::Bezier::GetSlope(mPath,t);
            auto derivative = slope.Distance(CMPUT350::Point2D(0,0));
            mRotation = std::atan2(slope.y, slope.x); 
            if((mPath.size() - 1) / 3 > t+(10/derivative)) t+=(10/derivative);
            else t = (mPath.size() - 1) / 3;
            //std::cerr << "t = " << t << "\n";
            location = CMPUT350::Bezier::GetPoint(mPath, t);
            if(t ==(mPath.size() - 1) / 3 ){
                mStates = EnemyStates::kWaiting;
            } 
            break;
        }
        case(EnemyStates::kWaiting):{
            //location = formationSpot;
            float speed = 0.1;
            auto distance = 20;

            location.x = formationSpot.x + std::sin(context->currentFrame * speed) * distance;
            break;
        }
        case(EnemyStates::kAttacking):
            auto slope = CMPUT350::Bezier::GetSlope(mPath,t);
            auto derivative = slope.Distance(CMPUT350::Point2D(0,0));
            mRotation = std::atan2(slope.y, slope.x); 
            if((mPath.size() - 1) / 3 > t+(10/derivative))  t+=(10/derivative);
            else t = (mPath.size() - 1) / 3;
            //std::cerr << "t = " << t << "\n";
            location = CMPUT350::Bezier::GetPoint(mPath, t);
            if(t ==(mPath.size() - 1) / 3 ){
                mStates = EnemyStates::kWaiting;
                mRotation = 0,0;
            }
            if(context->currentFrame % 60 == 0){
                if(mShot1.expired()){
                std::shared_ptr<Bullet> bullet1 = std::make_shared<Bullet>(location,CMPUT350::Point2D(0,10), false);
                mShot1 = bullet1;
                context->mEngineView->AddGameObject(bullet1);
                
                }
            }

            break;
            

    }


    
}

void Enemy::LateUpdate(CMPUT350::GameContext* context)
{
    if(!mAlive){
        context->mNotificationManager->Notify("EnemyDestroyed");
    }
}

float Enemy::GetRotation() const{
    return mRotation;

}

/*
bool Enemy::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return false;
}
*/

void Enemy::RenderBackground(CMPUT350::GameContext* context)
{
}

// Draws the enemy as a red square
void Enemy::RenderForeground(CMPUT350::GameContext* context)
{
    //context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 30), CMPUT350::Colors::red);
}

// Dies when hit by a player bullet
void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(obj);
    std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(obj);

    if(bullet != nullptr && bullet->IsPlayerBullet()){
        mHealth = mHealth -1;
        if(mHealth <= 0){
            Kill();
        }
        
    }
    if(player != nullptr){
        mHealth = mHealth -1;
        if(mHealth <= 0){
            Kill();
        } 
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

// PART B PLACEHOLDER: GetLocation/GetRotation are now pure virtual in GraphicsObject.
// Stub values for now, so the class still compiles.
CMPUT350::Point2D Enemy::GetLocation() const
{
    return location;
}

const std::vector<CMPUT350::Shape>& Enemy::GetShapes()
{
    return mShapes;
}
