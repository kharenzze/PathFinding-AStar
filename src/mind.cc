//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2018                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include <mind.h>
#include <body.h>
#include <world.h>

void Mind::init(World* world, Body* body) {
  world_ = world;
  body_ = body;
}

void Mind::update(const uint32_t dt) {
  body_->setTarget(world_->target());
}

void Mind::setDest(MathLib::Vec2 dest) {
  int x, y;
  world_->mapPosToCostCell(dest, &x, &y);
  const auto destCellPos = MathLib::Vec2(x, y);
  world_->mapPosToCostCell(body_->getKinematic()->position, &x, &y);
  const auto originCellPos = MathLib::Vec2(x, y);


}