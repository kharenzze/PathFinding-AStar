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

void Mind::setDest(MathLib::Vec2 destPoint) {
  int xDest, yDest;
  world_->mapPosToCostCell(destPoint, &xDest, &yDest);
  int xOrigin, yOrigin;
  world_->mapPosToCostCell(body_->getKinematic()->position, &xOrigin, &yOrigin);

  initNodes();

  const auto origin = &_nodes[xOrigin][yOrigin];
  const auto dest = &_nodes[xDest][yDest];
  auto current = origin;

  while (current != dest) {
    this->discover(current, dest);
    closed.push_back(current);

    std::vector<Node*>::iterator it;
    Node * best = nullptr;
    float bestF;
    for (it = opened.begin(); it != opened.end(); it++) {
      const auto n = *it;
      if (!best) {
        best = n;
        bestF = n->F();
      }
      else {
        const auto _f = n->F();
        if (_f < bestF) {
          best = n;
          bestF = _f;
        }
      }
    }

    current = best;
  }
  std::cout << "done";
}

void Mind::initNodes() {
  for (int i = 0; i < MAP_H; i++) {
    for (int j = 0; i < MAP_H; i++) {
      Node *n = &_nodes[i][j];
      n->pos = MathLib::Vec2(i,j);
      const auto isBlocked = world_->cost[i][j];
      if (isBlocked) {
        n->status = Node::Status::Blocked;
      } else {
        n->status = Node::Status::None;
      }
      n->G = 0;
      n->H = 0;
      n->child = nullptr;
      n->father = nullptr;
    }
  }
}

void Mind::discover(Node * center, Node * goal) {
  int x = center->pos.x();
  int y = center->pos.y();

  int minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  int maxX = x + 1;
  if (maxX >= MAP_H) {
    maxX = MAP_H - 1;
  }

  int minY = y - 1;
  if (minY < 0) {
    minY = 0;
  }
  int maxY = y + 1;
  if (maxY >= MAP_W) {
    maxY = MAP_W - 1;
  }

  for (int i = minX; i <= maxX; i++) {
    for (int j = minY; j <= maxY; j++) {
      Node * n = &_nodes[i][j];
      if (n == center) {
        break;
      }
      const bool isDiagonal = !(i == x || j == y);
      const auto status = n->status;
      if (status == Node::Status::None) {
        n->father = center;
        n->setG(isDiagonal);
        n->setH(goal);
        n->status = Node::Status::Open;
        opened.push_back(n);
      } else if (status == Node::Status::Open) {
        const auto oldG = n->G;
        const auto oldFather = n->father;
        n->father = center; 
        const auto nG = n->setG(isDiagonal);
        if (nG >= oldG) {
          n->father = oldFather;
          n->G = oldG;
        }
      }
    }
  }

}