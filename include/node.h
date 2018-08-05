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

  Node() {}
  ~Node() {}

  MathLib::Vec2 pos;
  float G = 0, H = 0;
  Status status = Status::None;
  Node * father = nullptr;
  Node * child = nullptr; 

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
