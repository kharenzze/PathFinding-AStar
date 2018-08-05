//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2018                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __BODY_H__
#define __BODY_H__ 1

#include <sprite.h>
#include <defines.h>
#include <mathlib/vec2.h>
#include <node.h>

class Agent;
class AgentGroup;

class Body {
  public:
    enum class Color {
      Green,
      Blue,
      Purple,
      Red,
    };

    enum class Type {
      Autonomous,
      Manual,
    };

    enum class SteeringMode {
      Kinematic_Seek,         //1       Kinematics
      Kinematic_Flee,         //2
      Kinematic_Arrive,       //3
      Kinematic_Wander,       //4
      Seek,                   //q       Steering Basics
      Flee,                   //w
      Arrive,                 //e
      Align,                  //r
      Velocity_Matching,      //t
      Pursue,                 //a       Steering Delegated
      Face,                   //s
      LookGoing,              //d
      Wander,                 //f
      Separation,             //z
      Cohesion,               //x
      Alignment,              //c
      Flocking,               //v
      Pathfind,               //v
    };

    Body() {};
    ~Body() {};

    void init(const Color color, const Type type);
    void update(const uint32_t dt);
    void render() const;

    void setTarget(Agent* target);
    void setAgentGroup(AgentGroup* ag) { agentGroup_ = ag; };
    void setSteering(const SteeringMode mode) { steering_mode_ = mode; };
    const KinematicStatus* getKinematic() const { return &state_; }
    KinematicStatus* getKinematic() { return &state_; }
    void resetStep();
  private:
    void updateManual(const uint32_t);
    void setOrientation(const MathLib::Vec2& velocity);
    void keepInSpeed();
    void keepInBounds();

    void applyKinematicSteering(const KinematicSteering& steering, const uint32_t ms);
    void applySteering(const Steering& steering, const uint32_t ms);
    
    void kinematicSeek(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const;
    void kinematicFlee(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const;
    void kinematicArrive(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const;
    void kinematicWandering(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const;

    void seek(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void flee(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void arrive(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void align(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void velocityMatching(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void pursue(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void face(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void lookGoing(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void wander(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) const;
    void separation(const KinematicStatus& character, AgentGroup* agentGroup, Steering* steering) const;
    void cohesion(const KinematicStatus& character, AgentGroup* agentGroup, Steering* steering) const;
    void alignment(const KinematicStatus& character, AgentGroup* agentGroup, Steering* steering) const;
    void flocking(const KinematicStatus& character, AgentGroup* agentGroup, const KinematicStatus* target, Steering* steering) const;
    void pathfind(Agent* me, Steering* steering) const;


    Sprite sprite_;
    Type type_;
    Color color_;
    SteeringMode steering_mode_;
    Agent* target_;
    AgentGroup * agentGroup_;

    const float max_speed_ = 100.0f;

    struct {
      struct {
        MathLib::Vec2 pos;
        MathLib::Vec2 v;
      } green, red, blue;
    } dd;

    KinematicStatus state_;
};

#endif