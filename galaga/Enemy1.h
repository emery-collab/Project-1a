// Comments in this file were AI-generated (Claude); code written by the team.
#ifndef ENEMY1_H
#define ENEMY1_H
#include "Enemy.h"
#include "CollisionObject.h"
#include "GameContext.h"

// 1-health enemy drawn as a red square
class Enemy1 : public Enemy {
public:
    Enemy1(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path)
    :Enemy(loc, path){mHealth = 1;}
    void Initialize(CMPUT350::GameContext *context) override;
    void LateUpdate(CMPUT350::GameContext *context) override;
    bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) override;
    void RenderForeground(CMPUT350::GameContext *context) override;
};


#endif

