//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2018                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __MIND_H__
#define __MIND_H__ 1

#include <cstdint>
#include <mathlib/vec2.h>
#include <node.h>
#include <vector>
#include <map>

#define MAP_H 128
#define MAP_W 128

class Body;
class World;

class Mind {
  public:
    Mind() {};
    ~Mind() {};

    void init(World* world, Body* body);
    void update(const uint32_t dt);
    void setDest(MathLib::Vec2 dest);
    std::vector<Node*> closed, opened;
  private:
    void discover(Node* center, Node* goal);
    World* world_;
    Body* body_;
    std::map<int, std::map<int, Node*>> _nodes;
};

#endif