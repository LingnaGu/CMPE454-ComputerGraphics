// world.cpp


#include "world.h"
#include "ship.h"
#include "staticShip.h"
#include "main.h"
#include "gpuProgram.h"
#include "strokefont.h"

#include <sstream>


void World::start()

{
  // Create a ship at the centre of the world

  ship = new Ship( 0.5 * (worldMin+worldMax) );

  // Create some large asteroids

  // Pick a random position at least 20% away from the origin (which
  // is where the ship will be).

  asteroids.clear();
  shells.clear();
  staticShips.clear();

  for (int i=0; i<NUM_INITIAL_ASTEROIDS; i++) {

    vec3 pos;
    do {
      pos = vec3( randIn01(), randIn01(), 0 );
    } while ((pos - vec3( 0.5, 0.5, 0 )).length() < 0.20);

    asteroids.push_back( new Asteroid( pos % (worldMax - worldMin) + worldMin ) );
  }

  // Increase asteroid speed in later rounds

  for (unsigned int  i=0; i<asteroids.size(); i++) {
    asteroids[i]->velocity = ((1+round) * ASTEROID_SPEED_ROUND_FACTOR) * asteroids[i]->velocity;
    asteroids[i]->angularVelocity = ((1+round) * ASTEROID_SPEED_ROUND_FACTOR) * asteroids[i]->angularVelocity;
  }
  
  if(numLives == 0)
  {
  	numLives = 3;  
  	highscore = score;
  	score = 0;
  }
  else if(numLives != 3 && numLives != 2 && numLives != 1)
  {
  	  numLives = 3;
  }
  
  // Draw ships representing how many lives the player has remaining
  cout << numLives << endl;
  for(int i=0; i<numLives; i++)
  {
    vec3 pos = vec3(30+i*30, SCREEN_WIDTH/SCREEN_ASPECT-30, 0);
    staticShips.push_back( new StaticShip( pos ) );
    staticShips[i]->scaleFactor = 2;
  }

  state = RUNNING;
}


void World::updateState( float elapsedTime )

{
  if (state == PAUSED)
    return;

  if (asteroids.size() == 0) {
    round++;
    start();
    return;
  }

  // See if any keys are pressed for thrust

  if (state == RUNNING) {

    if (rightKey == DOWN)
      ship->rotateCW( elapsedTime );

    if (leftKey == DOWN)
      ship->rotateCCW( elapsedTime );

    if (upKey == DOWN)
      ship->addThrust( elapsedTime );
  }

  // Update the ship

  ship->updatePose( elapsedTime );

  // Update the asteroids (check for asteroid collisions)

  for (unsigned int i=0; i<asteroids.size(); i++) {
    asteroids[i]->updatePose( elapsedTime );
    if (state == RUNNING && ship->intersects( *asteroids[i] ))
    {
      numLives--;
      
      gameOver();
    }
  }

  // Update the shells (check for asteroid collisions)

  for (unsigned int i=0; i<shells.size(); i++) {
    shells[i]->elapsedTime += elapsedTime;

    if (shells[i]->elapsedTime > SHELL_MAX_TIME) { // remove an old shell

      shells.erase( shells.begin() + i );
      i--;

    } else { // move a not-too-old shell

      vec3 prevPosition = shells[i]->centrePosition();
      shells[i]->updatePose( elapsedTime );

      // Check for shell/asteroid collision
      Segment path( prevPosition, shells[i]->centrePosition() );

	  // YOUR CODE HERE
      //
      // Check each of the asteroids to see if it has intersected the
      // shell's path.  If so, either (a) remove the asteroid if it is
      // too small or (b) break the asteroids into two.  Also
      // increment the score according to the asteroid's scoreValue.
      //
      // - An asteroid is removed if (asteroids->scaleFactor * ASTEROID_SCALE_FACTOR_REDUCTION < MIN_ASTEROID_SCALE_FACTOR).
      //
      // - A split asteroid should add velocities to the two
      //   sub-asteroids in opposite directions perpendicular to the
      //   direction of the shell.
      // 
      // - the sub-asteroid scaleFactor and scoreValue should be
      //   modified from those of the parent asteroid.

	  // Update the number of asteroids added to the vector. 
	  // This ensures that we don't check if the newly created asteroids 
	  // collide with the shell (results in an infinite loop).
	  int numNewAsteroids = 0; 

	  // Iterate through each asteroid to see if it intersects with this shell's path
	  for (unsigned int j = 0; j < asteroids.size()-numNewAsteroids; j++) 
	  {
		  if ( state == RUNNING && asteroids[j]->intersects(path) )
		  {
			  if (asteroids[j]->scaleFactor * ASTEROID_SCALE_FACTOR_REDUCTION < MIN_ASTEROID_SCALE_FACTOR)
			  {
				  // Remove this asteroid
				  asteroids.erase(asteroids.begin() + j);	// Remove asteroid from list
				  j--;										// After removing an asteroid from the list, we must update the value used to index
				  score += asteroids[j]->scoreValue;		// Update the player's score
			  }
			  else
			  {
				  // Break this asteroid up

				  // Define two new subasteroids in the same position as the parent asteroid
				  Asteroid* ast1 = new Asteroid( asteroids[j]->position );
				  Asteroid* ast2 = new Asteroid( asteroids[j]->position );

				  // Use direction of shell to dictate the orientation of subasteroids
				  // The subasteroids will move in opposite directions perpendicular to the shell
				  float theta1 = shells[i]->orientation.angle() - (2*M_PI)/4; // Shell angle - pi/4
				  float theta2 = shells[i]->orientation.angle() + (2*M_PI)/4; // Shell angle + pi/4

				  // Wrap around both ways to keep radians within (0,2pi)
				  if (theta2 > (2 * M_PI))
				  {
					  theta2 = fmod(theta2, 2 * M_PI);
				  }
				  if (theta1 < 0)
				  {
					  theta1 += (2 * M_PI);
				  }

				  // Define new orientations of subasteroids based on above angles
				  quaternion subAstOrientation1 = quaternion(theta1, vec3(0, 0, 1));
				  quaternion subAstOrientation2 = quaternion(theta2, vec3(0, 0, 1));

				  // Calculate subasteroid velocities based on the angle calculated above
				  float asteroidVelocity = sqrt( asteroids[j]->velocity.squaredLength() );

				  vec3 subAstVelocity1 = { -asteroidVelocity * sin(theta1), asteroidVelocity * cos(theta1), 0 };
				  vec3 subAstVelocity2 = { -asteroidVelocity * sin(theta2), asteroidVelocity * cos(theta2), 0 };

				  ast1->velocity	= subAstVelocity1;
				  ast1->orientation = subAstOrientation1;
				  ast2->velocity	= subAstVelocity2;
				  ast2->orientation = subAstOrientation2;

				  // Set new score value and scaleFactor of subasteroids
				  ast1->scoreValue /= 3;
				  ast2->scoreValue /= 3;
				  ast1->scaleFactor /= 3;
				  ast2->scaleFactor /= 3;

				  // Add to list of asteroids
				  asteroids.push_back(ast1);
				  asteroids.push_back(ast2);

				  // Remove this asteroid
				  asteroids.erase(asteroids.begin() + j);
				  j--;

				  // Update player's score
				  score += asteroids[i]->scoreValue;

				  // Update the number of asteroids added to the vector. 
				  // This ensures that we don't check if the newly created asteroids 
				  // collide with the shell (results in an infinite loop).
				  numNewAsteroids += 2;
			  }
		  }
	  }

    }
  }
}


void World::draw()

{
  // Transform [worldMin,worldMax] to [(-1,-1),(+1,+1)].

  mat4 worldToViewTransform;

  worldToViewTransform
    = translate( -1, -1, 0 )
    * scale( 2.0/(worldMax.x-worldMin.x), 2.0/(worldMax.y-worldMin.y), 1 )
    * translate( -worldMin.x, -worldMin.y, 0 );

  // Draw all world elements, passing in the worldToViewTransform so
  // that they can append their own transforms before passing the
  // complete transform to the vertex shader.

  ship->draw( worldToViewTransform );

  for (unsigned int i=0; i<shells.size(); i++)
    shells[i]->draw( worldToViewTransform );

  for (unsigned int i=0; i<asteroids.size(); i++)
    asteroids[i]->draw( worldToViewTransform );

  // Draw the title

  drawStrokeString( "ASTEROIDS", 0, 0.85, 0.06, glGetUniformLocation( myGPUProgram->id(), "MVP"), CENTRE );

  // Draw messages according to game state

  if (state == BEFORE_GAME) 

    drawStrokeString( "PRESS 's' TO START, 'p' TO PAUSE DURING GAME", 0, -.9, 0.06, glGetUniformLocation( myGPUProgram->id(), "MVP"), CENTRE );

  else {

    // draw score

    stringstream ss;
    ss.setf( ios::fixed, ios::floatfield );
    ss.precision(1);
    ss << "SCORE " << score;
    drawStrokeString( ss.str(), -0.95, 0.75, 0.04, glGetUniformLocation( myGPUProgram->id(), "MVP"), LEFT );
    
    stringstream hs;
    hs.setf( ios::fixed, ios::floatfield );
    hs.precision(1);
    hs << "SESSION HIGHSCORE " << highscore;
    drawStrokeString( hs.str(), -0.95, 0.68, 0.04, glGetUniformLocation( myGPUProgram->id(), "MVP"), LEFT );
    
    // Draw number of lives
    for (unsigned int i=0; i<staticShips.size(); i++)
    	staticShips[i]->draw( worldToViewTransform );

    if (state == AFTER_GAME) {

      // Draw "game over" message

      drawStrokeString( "GAME OVER", 0, 0, 0.12, glGetUniformLocation( myGPUProgram->id(), "MVP"), CENTRE );
      drawStrokeString( "PRESS 's' TO START, 'p' TO PAUSE DURING GAME", 0, -0.9, 0.06, glGetUniformLocation( myGPUProgram->id(), "MVP"), CENTRE );
    }
  }
}


void World::gameOver()

{
  state = AFTER_GAME;
}


void World::togglePause()

{
  if (state == RUNNING)
    state = PAUSED;
  else if (state == PAUSED)
    state = RUNNING;
}
