#include <cassert>
#include "Player.h"
#include "Bullet.h"

Player::Player(CMPUT350::Point2D loc)
{
    mAlive = true;
    BuildShape(loc);
    bulletSpeed = CMPUT350::Point2D(0,-10);


}

void Player::Initialize(CMPUT350::GameContext* context)
{
}

void Player::Update(CMPUT350::GameContext* context)
{

}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
}

bool Player::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    if ((key == 'a') || (key == 'A')) {
        //30 is a little bit more than the ship to add a buffer. 
        
        location.x -= 8;
        if(location.x < 30){
            location.x = 30;
        }
        BuildShape(location);
        return true;
    }
    if ((key == 'd') || (key == 'D')) {
        location.x += 8;
        if(location.x > context->ScreenContext->GetWindowWidth()-30){
            location.x =  context->ScreenContext->GetWindowWidth()-30;
        }
        BuildShape(location);
        return true;
    }
    if ((key == ' ')) {
        
        if(mShot1.expired()){
            std::shared_ptr<Bullet> bullet1 = std::make_shared<Bullet>(location,bulletSpeed, true);
            mShot1 = bullet1;
            context->mEngineView->AddGameObject(bullet1);
            
        }
        else if(mShot2.expired()){
            std::shared_ptr<Bullet> bullet2 = std::make_shared<Bullet>(location,bulletSpeed, true);
            mShot2 = bullet2;
            context->mEngineView->AddGameObject(bullet2);
        }
        return true;
    }
    return false;
}

void Player::RenderBackground(CMPUT350::GameContext* context)
{
}

void Player::RenderForeground(CMPUT350::GameContext* context)
{
    context->ScreenContext->FrameRect(mBounds, 3, CMPUT350::Colors::yellow);
    context->ScreenContext->DrawRect(nozzle, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(nozzle2, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(body, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(cockpit, CMPUT350::Colors::blue);
    context->ScreenContext->DrawRect(thrusterL, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(thrusterR, CMPUT350::Colors::red);
}


void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
}

void Player::Kill()
{
    mAlive = false;
    
}

void Player::BuildShape(CMPUT350::Point2D loc){
    location = loc;
    float top = loc.y - height/2;
    float left = loc.x - width/2;
    mBounds = CMPUT350::Rect(CMPUT350::Point2D(left,top), width, height);

    //Design of ship
    float topNozzle = top;
    float leftNozzle = loc.x - width/8;
    float nozzleWidth = 4;
    float nozzleheight = 10;
    nozzle = CMPUT350::Rect(CMPUT350::Point2D(leftNozzle,topNozzle), nozzleWidth, nozzleheight);

    float topNozzle2 = top + height/5;
    float leftNozzle2 = loc.x - width/5 + width/20;
    float nozzle2Width = 8;
    float nozzle2height = 15;
    nozzle2 = CMPUT350::Rect(CMPUT350::Point2D(leftNozzle2,topNozzle2), nozzle2Width, nozzle2height);

    float topThrusterL = loc.y - height/5;
    float leftThrusterL = left;
    float thrusterLWidth = 8;
    float thrusterLheight = 28;
    
    thrusterL = CMPUT350::Rect(CMPUT350::Point2D(leftThrusterL,topThrusterL), thrusterLWidth, thrusterLheight);

    float topThrusterR = loc.y - height/5;
    float leftThrusterR = loc.x + width/2 - width/5;
    float thrusterRWidth = 8;
    float thrusterRheight = 28;
    thrusterR = CMPUT350::Rect(CMPUT350::Point2D(leftThrusterR,topThrusterR), thrusterRWidth, thrusterRheight);
    
    float topBody = loc.y - height/6;
    float leftBody = left;
    float bodyWidth = width;
    float bodyheight = 15;
    body = CMPUT350::Rect(CMPUT350::Point2D(leftBody,topBody), bodyWidth, bodyheight);

    float topCockpit = loc.y + height/10;
    float leftCockpit = loc.x - width/4 + width/10;;
    float cockpitWidth = 5;
    float cockpitheight = 5;
    cockpit = CMPUT350::Rect(CMPUT350::Point2D(leftCockpit,topCockpit), cockpitWidth, cockpitheight);

}

bool Player::IsAlive() const
{
    // TODO: Update code
    return mAlive;
}

const CMPUT350::Rect& Player::GetBounds()
{
    return mBounds;
}
