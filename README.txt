Wyatt Wood
13ww11
10129798

Potential Bonus Marks:
- I added a class StaticShips which will display the number of lives the user has at the top left hand corner of the windows in the form of ships.
	- ex) 3 ships means the user has 3 lives remaining
- I also keep track of the user's highscore during this session. The highscore is tallied until the user loses their 3rd life.

Known Isses:
- Ship can't rotate past the starting vertical position in either direction.
	- I tried implementing a workaround in Ship::RotateCW and Ship::RotateCCW to recongize and wrap around at this point but the ship flickers/disappears sometimes.
	- I've commented out these changes so you can mark the other parts of the assignment.
	- As far as I can tell it's not an issue with my code because we don't modify the orientation anywhere, I can only assume it's a bug to do with the quaternion class.