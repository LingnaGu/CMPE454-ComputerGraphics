// staticShip.h


#ifndef STATICSHIP_H
#define STATICSHIP_H


#include <vector>
#include "headers.h"
#include "object.h"

#define SHIP_WIDTH 10		 // ship is 10 m wide
#define SHIP_MASS 5000		 // ship is 5000 kg

class StaticShip : public Object {

  static float verts[];

 public:

  StaticShip( vec3 position )
    : Object( position, verts, SHIP_WIDTH, SHIP_MASS )
    {};
};


#endif
