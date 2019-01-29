// segment.h


class Segment {
 public:

  vec3 tail, head;

  Segment( vec3 t, vec3 h )
    { tail = t; head = h; }

  bool intersects( Segment const& s2 ) 
  {
    // YOUR CODE HERE
	bool val = false;
	//cout << "head: (" << head.x << ", " << head.y << ") " << endl;
	//cout << "tail: (" << tail.x << ", " << tail.y << ") " << endl;
	//cout << "s2head: (" << s2.head.x << ", " << s2.head.y << ") " << endl;
	//cout << "s2tail: (" << s2.tail.x << ", " << s2.tail.y << ") " << endl;
	//val = ccw(head, s2.head, s2.tail) != ccw(tail, s2.head, s2.tail) && ccw(head, tail, s2.head) != ccw(head, tail, s2.tail);

	return val;
	/*
	  if (doIntersect(head, tail, s2.head, s2.tail))
	  {
		  cout << "intersect" << endl;
		  return true;
	  }
	  else
	  {
		  return false;
	  }*/
  }

  bool boundingBoxes(vec3 A, vec3 B, vec3 C, vec3 D)
  {
	  return A.x <= D.x
		  && B.x >= C.x
		  && A.y <= D.y
		  && B.y >= C.y;
  }

  bool ccw(vec3 A, vec3 B, vec3 C)
  {
	  bool val = false;

	  val = (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);

	  return val;
  }
  
  bool doIntersect(vec3 p1, vec3 q1, vec3 p2, vec3 q2)
  {
	  // Find the four orientations needed for general and 
	  // special cases 
	  int o1 = getOrientation(p1, q1, p2);
	  int o2 = getOrientation(p1, q1, q2);
	  int o3 = getOrientation(p2, q2, p1);
	  int o4 = getOrientation(p2, q2, q1);

	  // General case 
	  if (o1 != o2 && o3 != o4)
	  {
		  return true;
	  }

	  // Special Cases 
	  // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	  if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	  // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	  if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	  // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	  if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	  // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	  if (o4 == 0 && onSegment(p2, q1, q2)) return true;
	  
	  return false; // Doesn't fall in any of the above cases 
  }

	  bool onSegment(vec3 p, vec3 q, vec3 r)
	  {
		  if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
			  q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
			  return true;

		  return false;
	  }

	  float max(float x, float y)
	  {
		  if (x > y)
		  {
			  return x;
		  }
		  else
		  {
			  return y;
		  }
	  }

	  float min(float x, float y)
	  {
		  if (x > y)
		  {
			  return y;
		  }
		  else
		  {
			  return x;
		  }
	  }



  // To find orientation of ordered triplet (p, q, r). 
  // The function returns following values 
  // 0 --> p, q and r are colinear 
  // 1 --> Clockwise 
  // 2 --> Counterclockwise 
  int getOrientation(vec3 p, vec3 q, vec3 r)
  {
	  int val = (q.y - p.y) * (r.x - q.x) -
		  (q.x - p.x) * (r.y - q.y);

	  if (val == 0) return 0;  // colinear 

	  return (val > 0) ? 1 : 2; // clock or counterclock wise 
  }
};


