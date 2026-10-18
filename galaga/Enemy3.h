// Comments in this file were AI-generated (Claude); code written by the team.
#ifndef ENEMY3_H
#define ENEMY3_H
#include "Enemy.h"
#include "CollisionObject.h"
#include "GameContext.h"

// A stationary enemy drawn as a square; dies when hit by a player bullet
class Enemy3 : public Enemy {
public:
    Enemy3(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path)
    :Enemy(loc, path){mhealth = 3;}
    void Initialize(CMPUT350::GameContext *context) override;
    void LateUpdate(CMPUT350::GameContext *context) override;
    bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) override;
    void RenderForeground(CMPUT350::GameContext *context) override;
};


#endif

