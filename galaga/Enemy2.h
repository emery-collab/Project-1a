// Comments in this file were AI-generated (Claude); code written by the team.
#ifndef ENEMY2_H
#define ENEMY2_H

#include "CollisionObject.h"
#include "GameContext.h"
#include "Enemy.h"
// 2-health enemy; green, turns red when hit
class Enemy2 : public Enemy {
public:
    Enemy2(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path)
    :Enemy(loc, path){mHealth =2;}
    void Initialize(CMPUT350::GameContext *context) override;
    void LateUpdate(CMPUT350::GameContext *context) override;
    bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) override;
    void RenderForeground(CMPUT350::GameContext *context) override;
};


#endif

