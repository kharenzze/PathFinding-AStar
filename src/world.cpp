#include <world.h>

World::World() {
  target_.init(this, Body::Color::Red, Body::Type::Manual);
  ia_.init(this, Body::Color::Green, Body::Type::Autonomous);
  worldImg.loadFromFile("../media/mapa.bmp");
}

void World::render() {
  worldImg.render();
  target_.render();
  ia_.render();
}


      
    