//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2018                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __WORLD_H__
#define __WORLD_H__ 1

#include <mathlib/vec2.h>
#include <defines.h>

#include <cstdio>
#include <agent.h>
#include <AgentGroup.h>
#include <sprite.h>

using MathLib::Vec2;

class World {
  public:
    World();
    ~World() {
      target_.shutdown();
      ia_.shutdown();
    };

    void update(const float dt) { target_.update(dt); ia_.update(dt); }
    void render();

    Agent* target() { return &target_; }
    AgentGroup* ia() { return &ia_; }
  private:
    Agent target_;
    AgentGroup ia_;
    Sprite worldImg;
};

#endif