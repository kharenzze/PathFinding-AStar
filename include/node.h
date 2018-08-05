#pragma once

#include <mathlib/vec2.h>

class Node {
public:
  enum class Status {
    None,
    Open,
    Closed,
    Blocked
  };


  MathLib::Vec2 pos;
  float G, H;
  Status status;
  Node * father;
  Node * child; 

  float F() { return G + H; }
  float setG(bool isDiag) {
    G = father->G + isDiag ? 14 : 10;
    return G;
  }
  float setH(Node * goal) {
    H = (pos - goal->pos).length() * 10;
    return H;
  }

};
