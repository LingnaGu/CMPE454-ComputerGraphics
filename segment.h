// segment.h


class Segment {
 public:

  vec3 tail, head;

  Segment( vec3 t, vec3 h )
    { tail = t; head = h; }

  bool intersects( Segment const& s2 ) 
  {
    // YOUR CODE HERE
	bool doesIntersect = false;

	doesIntersect = ccw(head, s2.head, s2.tail) != ccw(tail, s2.head, s2.tail) && ccw(head, tail, s2.head) != ccw(head, tail, s2.tail);

	return doesIntersect;
  }
  
  bool ccw(vec3 A, vec3 B, vec3 C)
  {
	  bool val = false;

	  val = (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);

	  return val;
  }
};


