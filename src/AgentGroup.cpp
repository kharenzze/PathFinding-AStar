#include <defines.h>
#include <AgentGroup.h>
#include <MathLib/vec2.h>

using MathLib::Vec2;

void AgentGroup::init(World* world, const Body::Color color, const Body::Type type) {
  world_ = world;
  for (int i = 0; i < N_AGENTS; i++) {
    agents_[i].init(world, color, type);
    agents_[i].setAgentGroup(this);
    const float x = randomFloat(-10.0f, 10.0f);
    const float y = randomFloat(-10.0f, 10.0f);
    agents_[i].getKinematic()->position = Vec2(WINDOW_WIDTH / 2 + x, WINDOW_HEIGHT / 2 + y);
  }
}

void AgentGroup::shutdown() {
  world_ = nullptr;
}

void AgentGroup::update(const uint32_t dt) {
  for (int i = 0; i < N_AGENTS; i++) {
    agents_[i].update(dt);
  }
}

void AgentGroup::render() const {
  for (int i = 0; i < N_AGENTS; i++) {
    agents_[i].render();
  }
}

void AgentGroup::setSteering(Body::SteeringMode steering) {
  for (int i = 0; i < N_AGENTS; i++) {
    agents_[i].setSteering(steering);
  }
}

Agent* AgentGroup::getAgent(int i) {
  return &agents_[i];
}