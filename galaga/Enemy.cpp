// Comments in this file were AI-generated (Claude); code written by the team.
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
#include "NotificationManager.h"
#include "Bezier.h"
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
    mShapes.clear();
    mShapes.push_back(CMPUT350::Rect(CMPUT350::Point2D(-15, -15), 30, 30));
    mPath = path;
    t = 0;
    mSpeed = 0.01;
    mRotation = (0,0);
};
void Enemy::Initialize(CMPUT350::GameContext* context)
{
}

void Enemy::Update(CMPUT350::GameContext* context)
{
    //t = 3 for three pixels?
    auto slope = CMPUT350::Bezier::GetSlope(mPath,t);
    auto distance = slope.Distance(CMPUT350::Point2D(0,0));
    mRotation = std::atan2(slope.y, slope.x); 
    if((mPath.size() - 1) / 3 > t+mSpeed) t+=mSpeed;
    else t = 1;
    std::cerr << "t = " << t << "\n";
    location = CMPUT350::Bezier::GetPoint(mPath, t);
    
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


    if(bullet != nullptr && bullet->IsPlayerBullet()){
        mhealth = mHealth -1;
        if(mhealth <= 0){
            Kill();
        }
        else{
            if (mHealth == 2){

            }
            if (mHealth == 1){
                
            }
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
