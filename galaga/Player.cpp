// Comments in this file were AI-generated (Claude); code written by the team.
#include <cassert>
#include "Player.h"
#include "Bullet.h"
#include "NotificationManager.h"
#include "Keyboard.h"
// Creates the ship centered at loc
Player::Player(CMPUT350::Point2D loc)
{
    mAlive = true;
    BuildShape(loc);
    bulletSpeed = CMPUT350::Point2D(0,-10);
    movingLeft = false;
    movingRight = false;
    firing = false;


}

void Player::Initialize(CMPUT350::GameContext* context)
{

    //Space
    //mKeyUpNotification.insert({40, "Fire"});
    //registers for all key inputs
    context->mNotificationManager->Register(context->CurrObject, "MoveLeft");
    context->mNotificationManager->Register(context->CurrObject, "MoveRight");
    context->mNotificationManager->Register(context->CurrObject, "Fire");
    context->mNotificationManager->Register(context->CurrObject, "StopMoveLeft");
    context->mNotificationManager->Register(context->CurrObject, "StopMoveRight");
}

void Player::Update(CMPUT350::GameContext* context)
{
    if(movingLeft){
        location.x -= 8;
        if(location.x < 30){
            location.x = 30;
        }
        BuildShape(location);
    }
    if(movingRight){
        location.x += 8;
        if(location.x > context->ScreenContext->GetWindowWidth()-30){
            location.x =  context->ScreenContext->GetWindowWidth()-30;
        }
        BuildShape(location);
    }
    if(firing){
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
        firing = false;
    }


}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
}

void Player::ReceiveNotification(const std::string& key){
    if ((key == "MoveLeft")) {
        movingLeft = true;
    }
    if ((key == "MoveRight")) {
        movingRight = true;
    }
    if ((key == "Fire")) {
        firing = true;
    }
    if ((key == "StopMoveLeft")) {
        movingLeft = false;
    }
    if ((key == "StopMoveRight")) {
        movingRight = false;
    }
    
    

}

// A/D moves the ship, space fires if fewer than two bullets are on screen
// Returns true if the key was handled
// Old Part 1a key logic kept below for reference while moving it to the notification system.
/*
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
        
        // A shot slot is free once its bullet has been destroyed
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
*/

void Player::RenderBackground(CMPUT350::GameContext* context)
{
}

// Draws the ship and its bounding box
void Player::RenderForeground(CMPUT350::GameContext* context)
{
    /*
    context->ScreenContext->FrameRect(mBounds, 3, CMPUT350::Colors::yellow);
    context->ScreenContext->DrawRect(nozzle, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(nozzle2, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(body, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(cockpit, CMPUT350::Colors::blue);
    context->ScreenContext->DrawRect(thrusterL, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(thrusterR, CMPUT350::Colors::red);
    */
    CMPUT350::Rect localBounds(CMPUT350::Point2D(-width / 2, -height / 2), width, height);

    context->ScreenContext->FrameRect(localBounds, 3, CMPUT350::Colors::yellow);

    context->ScreenContext->DrawRect(mShapes[0].shape.rect, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(mShapes[1].shape.rect, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(mShapes[2].shape.rect, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(mShapes[3].shape.rect, CMPUT350::Colors::blue);
    context->ScreenContext->DrawRect(mShapes[4].shape.rect, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(mShapes[5].shape.rect, CMPUT350::Colors::red);
}


// Player ignores collisions
void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
}

// Marks the player dead so the engine removes it next frame
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


    mShapes.clear();

    // Convert a world-coordinate rectangle into a local-coordinate rectangle.
    // lambda function
    auto toLocal = [loc](const CMPUT350::Rect& rect) {
        return CMPUT350::Rect(CMPUT350::Point2D(rect.topLeft.x - loc.x, rect.topLeft.y - loc.y), rect.width, rect.height);
    };

    mShapes.push_back(toLocal(nozzle));
    mShapes.push_back(toLocal(nozzle2));
    mShapes.push_back(toLocal(body));
    mShapes.push_back(toLocal(cockpit));
    mShapes.push_back(toLocal(thrusterL));
    mShapes.push_back(toLocal(thrusterR));

}

bool Player::IsAlive() const
{
    // TODO: Update code
    return mAlive;
}

// Returns the bounding box used for collisions
const CMPUT350::Rect& Player::GetBounds()
{
    return mBounds;
}

// PART B PLACEHOLDER: GetLocation/GetRotation are now pure virtual in GraphicsObject.
// Stub values for now, so the class still compiles.
CMPUT350::Point2D Player::GetLocation() const
{
    return location;
}

float Player::GetRotation() const
{
    return 0.0f;
}

const std::vector<CMPUT350::Shape>& Player::GetShapes()
{
    return mShapes;
}

