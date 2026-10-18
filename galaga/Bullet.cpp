// Comments in this file were AI-generated (Claude); code written by the team.
#include "Bullet.h"
#include "Enemy.h"

// Creates a bullet at location that moves by heading each frame
Bullet::Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player)
{
    isPlayer = player;

    starting = location;

    headingAmt = heading;

    ending = location + headingAmt;

    mLine = CMPUT350::Line(location,ending);

    length = mLine.Length();

    mAlive = true;

    // Widen bounds by 4px so a vertical bullet doesn't have 0 width (0 width never collides)
    mBounds = CMPUT350::Rect(starting+ CMPUT350::Point2D(-2, 0), ending+ CMPUT350::Point2D(2, 0));
    
}

// Returns true if the player fired this bullet
bool Bullet::IsPlayerBullet()
{
    return isPlayer;
}

void Bullet::Initialize(CMPUT350::GameContext* context)
{
}

// Moves the bullet one step along its heading and kills it once it leaves the screen
void Bullet::Update(CMPUT350::GameContext* context)
{
    // new check to ensure explosion sticks around for atleast 6 frames before unaliving the object
    if (mExploded) {
        mExplosionFrames++;
        if (mExplosionFrames >= 6) 
            Kill();
        return;
    }

    // normal update logic if not exploded
    starting = ending;
    ending = ending + headingAmt;

    mLine = CMPUT350::Line(starting, ending);
    // Bounds cover the previous and current positions so the bullet can't skip past an enemy
    mBounds = CMPUT350::Rect(starting+ CMPUT350::Point2D(-2, 0),ending + CMPUT350::Point2D(2, 0));
    int windowHeight = context->ScreenContext->GetWindowHeight();
    int windowwidth = context->ScreenContext->GetWindowWidth();

    if(starting.x < 0 || starting.y < 0 || starting.y > windowHeight || starting.x > windowwidth){
        Kill();
    }


}

void Bullet::LateUpdate(CMPUT350::GameContext* context)
{
}

/*
bool Bullet::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return false;
}
*/

void Bullet::RenderBackground(CMPUT350::GameContext* context)
{
}

// Draws the bullet as a white line
void Bullet::RenderForeground(CMPUT350::GameContext* context)
{
    //Do we want different colors for enemy and player bullets?
    //context->ScreenContext->DrawLine(mLine.p1, mLine.p2, 4, CMPUT350::Colors::white);

    // Draw the bullet in local coordinates.
    // GameEngine applies the bullet's world position.

    if (mExploded) {
        context->ScreenContext->DrawCircle(mExplosionPoint, 7, CMPUT350::Colors::yellow);
    } else {
        context->ScreenContext->DrawLine(CMPUT350::Point2D(0, 0), headingAmt, 4, isPlayer ? CMPUT350::Colors::white : CMPUT350::Colors::red);
    }

    
}

// Player bullets are destroyed when they hit an enemy
void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    // added check for explosion
    if (mExploded)
        return;

    std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);

    if(enemy != nullptr && isPlayer){
        Kill();
    }
}

void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj, const CMPUT350::Point2D& collisionPoint) 
{
    // Function for obj and collision point

    // check if exploded return largest Y
    if (mExploded) {
        if (collisionPoint.y > mExplosionPoint.y)
            mExplosionPoint = collisionPoint;
        return;
    }

    std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);

    if(enemy != nullptr && isPlayer){
        mExploded = true;
        mExplosionPoint = collisionPoint;
        mLine = CMPUT350::Line(collisionPoint, collisionPoint);
        mBounds = CMPUT350::Rect(collisionPoint, 0, 0);
    }

}

// Marks the bullet dead so the engine removes it next frame
void Bullet::Kill()
{
    mAlive = false;
}

bool Bullet::IsAlive() const
{
    return mAlive;
}

// Returns the bounding box used for collisions
const CMPUT350::Rect& Bullet::GetBounds()
{
    return mBounds;
}

// PART B PLACEHOLDER: GetLocation/GetRotation are now pure virtual in GraphicsObject.
// Stub values for now, so the class still compiles.
CMPUT350::Point2D Bullet::GetLocation() const
{
    return starting;
}

float Bullet::GetRotation() const
{
    return 0.0f;
}

const std::vector<CMPUT350::Shape>& Bullet::GetShapes() 
{
    mShapes.clear();
    // The bullet's location is starting.
    // Represent the swept line relative to that location.

    // also, now we have to check if the bullet is exploded so that it doesnt instakill everything it lingers on while its an explosion for 6 or so frames
    if (!mExploded)
        mShapes.push_back(CMPUT350::Line(CMPUT350::Point2D(0, 0), headingAmt));

    return mShapes;
}


