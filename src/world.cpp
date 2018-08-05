#include <world.h>

World::World() {
  std::cout << "start;";
  loadCostMap();
  target_.init(this, Body::Color::Red, Body::Type::Autonomous);
  target_.setSteering(Body::SteeringMode::Pathfind);
  ia_.init(this, Body::Color::Green, Body::Type::Autonomous);
  worldImg.loadFromFile(WORLD_MAP);
}

void World::render() {
  worldImg.render();
  target_.render();
  ia_.render();
}

void World::loadCostMap() {
  SDL_Surface* map_image = nullptr;
  if ((map_image = SDL_LoadBMP("COSTS_MAP")) == false)
    return;

  SDL_LockSurface(map_image);

  uint8_t stride = map_image->pitch >> 2;

  {
    uint32_t* pixels = (uint32_t*)map_image->pixels;
    for (int i = 0; i < MAP_L1_HEIGHT; ++i) {
      uint32_t* step_pixels = pixels;
      for (int j = 0; j < MAP_L1_WIDTH; ++j) {
        if ((*step_pixels & 0x00ffffff) == 0) {
          cost[j][i] = 1;     //no walkable
        }
        else {
          cost[j][i] = 0;      //walkable
        }

        step_pixels++;
      }
      pixels += stride;
    }
  }

  SDL_UnlockSurface(map_image);
}
      
void World::mapPosToCostCell(MathLib::Vec2 pos, int *x, int *y) const {
  *x = (int)pos.x() % 8;
  *y = (int)pos.y() % 8;
}
