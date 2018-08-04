//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2018                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include <body.h>
#include <agent.h>
#include <defines.h>
#include <debug_draw.h>

const float SQUARED_RADIUS = 25.0f;
const float TIME_TO_TARGET = 0.5f;

void Body::init(const Color color, const Type type) {
  type_ = type;
  color_ = color;

  switch(color) {
    case Color::Green: sprite_.loadFromFile(AGENT_GREEN_PATH); break;
    case Color::Blue: sprite_.loadFromFile(AGENT_BLUE_PATH); break;
    case Color::Purple: sprite_.loadFromFile(AGENT_PURPLE_PATH); break;
    case Color::Red: sprite_.loadFromFile(AGENT_RED_PATH); break;
    default: sprite_.loadFromFile(AGENT_GREEN_PATH);
  }

  steering_mode_ = SteeringMode::Kinematic_Seek;
}

void Body::update(const uint32_t dt) {
  KinematicSteering kinematicSteering;
  Steering steering;
  bool isKinematic = false;
  if (type_ == Type::Autonomous) {
    switch (this->steering_mode_) {
    case Body::SteeringMode::Kinematic_Seek: 
      this->kinematicSeek(state_, target_->getKinematic(), &kinematicSteering);
      isKinematic = true;
      break;
    case Body::SteeringMode::Kinematic_Flee: 
      this->kinematicFlee(state_, target_->getKinematic(), &kinematicSteering);
      isKinematic = true;
      break;
    case Body::SteeringMode::Kinematic_Arrive: 
      this->kinematicArrive(state_, target_->getKinematic(), &kinematicSteering);
      isKinematic = true;
      break;
    case Body::SteeringMode::Kinematic_Wander: 
      this->kinematicWandering(state_, target_->getKinematic(), &kinematicSteering);
      isKinematic = true;
      break;
    case Body::SteeringMode::Seek: 
      this->seek(state_, target_->getKinematic(), &steering);
      break;
    case Body::SteeringMode::Flee: 
      this->flee(state_, target_->getKinematic(), &steering);
      break;
    case Body::SteeringMode::Arrive: 
      this->arrive(state_, target_->getKinematic(), &steering);
      break;
    case Body::SteeringMode::Align: 
      this->align(state_, target_->getKinematic(), &steering);
      break;
    case Body::SteeringMode::Velocity_Matching: 
      this->velocityMatching(state_, target_->getKinematic(), &steering);
      break;
    case Body::SteeringMode::Pursue: 
      this->pursue(state_, target_->getKinematic(), &steering);
      break;
    case Body::SteeringMode::Face: 
      this->face(state_, target_->getKinematic(), &steering);
      break;
    case Body::SteeringMode::LookGoing: 
      this->lookGoing(state_, target_->getKinematic(), &steering);
      break;
    }
    if (isKinematic) {
      this->applyKinematicSteering(kinematicSteering, dt);
    } else {
      this->applySteering(steering, dt);
    }
  } else {
    updateManual(dt);
  }



  sprite_.setPosition(state_.position.x(), state_.position.y());
  sprite_.setRotation(state_.orientation);

}

void Body::applyKinematicSteering(const KinematicSteering& steering, const uint32_t ms) {
  const float dt = ms * 0.001;
  state_.velocity = steering.velocity;
  state_.speed = state_.velocity.length();
  state_.position += state_.velocity * dt;
  state_.orientation = state_.orientation + steering.rotation * dt;

  keepInSpeed();
  keepInBounds();

  dd.green.pos = state_.position;
  dd.green.v = state_.velocity;
}

void Body::applySteering(const Steering& steering, const uint32_t ms) {
  const float dt = ms * 0.001;
  state_.velocity += steering.linear * dt;
  state_.speed = state_.velocity.length();
  keepInSpeed();
  state_.position += state_.velocity * dt;
  keepInBounds();

  state_.rotation += steering.angular * dt;
  state_.orientation += state_.rotation * dt;

  dd.green.pos = state_.position;
  dd.green.v = state_.velocity;
}

void Body::render() const {
  sprite_.render();

  DebugDraw::drawVector(dd.red.pos, dd.red.v, 0xFF, 0x00, 0x00, 0xFF);
  DebugDraw::drawVector(dd.green.pos, dd.green.v, 0x00, 0x50, 0x00, 0xFF);
  DebugDraw::drawVector(dd.blue.pos, dd.blue.v, 0x00, 0x00, 0xFF, 0xFF);
  DebugDraw::drawPositionHist(state_.position);
}

void Body::setTarget(Agent* target) {
  target_ = target;
}


void Body::updateManual(const uint32_t dt) {
  float time = dt * 0.001f;             //dt comes in miliseconds

  MathLib::Vec2 orientation;
  orientation.fromPolar(1.0f, state_.orientation);
  state_.velocity = orientation.normalized() * state_.speed;
  state_.position += state_.velocity * time;

  keepInSpeed();
  keepInBounds();

  dd.green.pos = state_.position;
  dd.green.v = state_.velocity;
}

void Body::setOrientation(const Vec2& velocity) {
  if (velocity.length2() > 0) {
    state_.orientation = atan2(velocity.y(), velocity.x());
  }
}

void Body::keepInBounds() {
  if (state_.position.x() > WINDOW_WIDTH) state_.position.x() = 0.0f;
  if (state_.position.x() < 0.0f) state_.position.x() = WINDOW_WIDTH;
  if (state_.position.y() > WINDOW_HEIGHT) state_.position.y() = 0.0f;
  if (state_.position.y() < 0.0f) state_.position.y() = WINDOW_HEIGHT;
}

void Body::keepInSpeed() {
  if (state_.velocity.length() > max_speed_) {
    state_.velocity = state_.velocity.normalized() * max_speed_;
  }
}

void Body::kinematicSeek(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const{
  steering->velocity = (target->position - character.position).normalized() * this->max_speed_;
  steering->rotation = 0.0f;
}

void Body::kinematicFlee(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const{
  steering->velocity = (character.position - target->position).normalized() * this->max_speed_;
  steering->rotation = 0.0f;
}

void Body::kinematicArrive(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const{
  steering->velocity = target->position - character.position;
  if (steering->velocity.length2() < SQUARED_RADIUS) {
    steering->velocity = MathLib::Vec2(0.0f, 0.0f);
  }
  else {
    steering->velocity /= TIME_TO_TARGET;
    if (steering->velocity.length() > max_speed_) {
      steering->velocity = steering->velocity.normalized() * max_speed_;
    }
  }
  steering->rotation = 0.0f;
}

void Body::kinematicWandering(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const{
  const auto _maxSpeed = max_speed_ / 2;
  const float _maxRotation = 3.14f;

  MathLib::Vec2 orientation;
  orientation.fromPolar(1.0f, character.orientation);
  steering->velocity = orientation * _maxSpeed;
  steering->rotation = _maxRotation * (randomFloat(0.0f, 1.0f) - randomFloat(0.0f, 1.0f));
}

void Body::seek(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const {
  constexpr float _maxAcceleration = 100.0f;
  steering->linear = (target->position - character.position).normalized() * _maxAcceleration;
  steering->angular = 0.0f;
}

void Body::flee(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const {
  constexpr float _maxAcceleration = 100.0f;
  steering->linear = (character.position - target->position).normalized() * _maxAcceleration;
  steering->angular = 0.0f;
}

void Body::arrive(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const {
  constexpr float _maxAcceleration = 100.0f;
  constexpr float _slowRadius = 150.0f;
  constexpr float _timeToTarget = 0.5f;
  
  const MathLib::Vec2 dir = target->position - character.position;
  const float distance = dir.length();
  float targetSpeed = max_speed_;
  if (distance < _slowRadius) {
    targetSpeed *= distance / _slowRadius;
  }

  const MathLib::Vec2 targetVelocity = dir.normalized() * targetSpeed;
  steering->linear = (targetVelocity - character.velocity) / _timeToTarget;
  if (steering->linear.length() > _maxAcceleration) { 
    steering->linear = steering->linear.normalized() * _maxAcceleration;
  }
  steering->angular = 0;
}

void Body::align(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const {
  const float _maxAngAcc = 2.0f;
  const float _maxRotation = 2.0f;
  const float _slowRadius = 0.5f;
  const float _timeToTarget = 0.05f;

  const float rotation = wrapAnglePI(target->orientation - character.orientation);
  const float rotationSize = abs(rotation);

  float targetRotation = _maxRotation;
  if (rotationSize < _slowRadius) {
    targetRotation = (_maxRotation * rotationSize) / _slowRadius;
  }

  targetRotation *= sign(rotation);
  steering->angular = (targetRotation - character.rotation) / _timeToTarget;
  if (abs(steering->angular) > _maxAngAcc) {
    steering->angular= sign(steering->angular) * _maxAngAcc;
  }
  steering->linear = MathLib::Vec2(0, 0);
}

void Body::velocityMatching(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const {
  const float _maxAcc = 50.0f;
  const float _timeToTarget = 0.5f;
  steering->linear = (target->velocity - character.velocity) / _timeToTarget;
  if (steering->linear.length() > _maxAcc) {
    steering->linear = steering->linear.normalized() * _maxAcc;
  }
  steering->angular = 0;
}

void Body::pursue(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const {
  const float _maxPrediction = 2.0f;

  const float distance = (target->position - character.position).length();
  float speed = character.velocity.length();

  float prediction = _maxPrediction;
  if (speed > (distance / _maxPrediction)) {
    prediction = distance / speed;
  }

  KinematicStatus _newTarget = *target;
  _newTarget.position += target->velocity * prediction;
  this->seek(character, &_newTarget, steering);
}

void Body::face(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const {
  const MathLib::Vec2 dir = target->position - character.position;

  KinematicStatus _newTarget = *target;
  _newTarget.orientation = atan2(dir.y(), dir.x());
  this->align(character, &_newTarget, steering);
}

void Body::lookGoing(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const {
  if (character.velocity.length() == 0) {
    steering->angular = 0;
    return;
  }

  KinematicStatus _newTarget = *target;
  _newTarget.orientation = atan2(character.velocity.y(), character.velocity.x());
  this->align(character, &_newTarget, steering);
}
