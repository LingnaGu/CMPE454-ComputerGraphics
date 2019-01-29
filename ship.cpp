// ship.cpp


#include "ship.h"


void Ship::rotateCW( float deltaT )
{
  if (orientation.q.w > 0.9999)
  {
	  orientation.q.w = -1;
  }
  else if (orientation.q.w < -0.9999)
  {
	  orientation.q.w = 1;
  }

  orientation = quaternion( - SHIP_ROTATION_SPEED * deltaT, vec3(0,0,1) ) * orientation;
}


void Ship::rotateCCW( float deltaT )
{
  if (orientation.q.w > 0.9999)
  {
    orientation.q.w = -1;
  }
  else if (orientation.q.w < -0.9999)
  {
	  orientation.q.w = 1;
  }

  orientation = quaternion( + SHIP_ROTATION_SPEED * deltaT, vec3(0,0,1) ) * orientation;
}


void Ship::addThrust( float deltaT )

{
  // Thrust is in the ship's +y direction.  Make sure to change the
  // velocity in that direction in the *world* coordinate system,
  // since the object velocity is in the world coordinate system.

  // YOUR CODE HERE
  velocity.x -= SHIP_THRUST_ACCEL * sin( orientation.angle() ) * deltaT;
  velocity.y += SHIP_THRUST_ACCEL * cos( orientation.angle() ) * deltaT;
}


Shell * Ship::fireShell()

{
  // YOUR CODE HERE (below, find the correct position, velocity, and orientation for the shell)
  float shipSpeed = 100; // m/s
  vec3 shellVelocity = { -shipSpeed * sin(orientation.angle()), shipSpeed * cos(orientation.angle()), 0 };
  return new Shell( position, shellVelocity, orientation );
}


// Ship model consisting of line segments
//
// These are in a ARBITRARY coordinate system and get remapped to the
// world coordinate system (with the centre of the ship at (0,0) and
// width SHIP_WIDTH) when the VAO is set up.


float Ship::verts[] = {

   3,0,  0,9,
   0,9, -3,0, 
  -3,0,  0,1,
   0,1,  3,0,

  9999
};

