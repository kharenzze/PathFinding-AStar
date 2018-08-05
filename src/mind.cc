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

  _nodes.clear();
  opened.clear();
  closed.clear();

  const auto origin = new Node;
  origin->pos = MathLib::Vec2(xOrigin, yOrigin);
  _nodes[xOrigin][yOrigin] = origin;

  const auto dest = new Node;
  dest->pos = MathLib::Vec2(xDest, yDest);
  _nodes[xDest][yDest] = dest;

  auto current = origin;

  while (current != dest) {
    this->discover(current, dest);
    closed.push_back(current);
    current->status = Node::Status::Closed;

    std::vector<Node*>::iterator it;
    Node * best = nullptr;
    float bestF;
    int i = 0, bestI = 0;
    for (it = opened.begin(); it != opened.end(); it++, i++) {
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
          bestI = i;
        }
      }
    }

    current = best;
    opened.erase(opened.begin()+bestI);
  }
  while (current != origin) {
    current->father->child = current;
    current = current->father;
  }
  std::cout << "done";

  body_->resetStep();
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
      Node * n = _nodes[i][j];
      if (n == center) {
        break;
      }
      const auto isBlocked = world_->cost[i][j];
      if (isBlocked) {
        break;
      }
      if (!n) {
        n = new Node();
        n->pos = MathLib::Vec2(i, j);
        _nodes[i][j] = n;
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