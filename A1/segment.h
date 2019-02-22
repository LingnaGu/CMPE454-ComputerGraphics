// segment.h


class Segment {
 public:

  vec3 tail, head;

  Segment( vec3 t, vec3 h )
    { tail = t; head = h; }

  bool intersects( Segment const& s2 ) {

    // YOUR CODE HERE
    bool doIntersect = false;
    
    doIntersect = computeOrientation(head, s2.head, s2.tail) != computeOrientation(tail, s2.head, s2.tail) && computeOrientation(head, tail, s2.head) != computeOrientation(head, tail, s2.tail);

    return doIntersect;
  }
  
  /* To find the orientation of the two lines, we calculate the orientation using the slope of each triplet of points.
     The following explanation is taken from: 
     https://www.geeksforgeeks.org/orientation-3-ordered-points/
     
     Slope of line segment (p1, p2): σ = (y2 - y1)/(x2 - x1)
     Slope of line segment (p2, p3): τ = (y3 - y2)/(x3 - x2)

     If  σ < τ, the orientation is counterclockwise (left turn)
     If  σ = τ, the orientation is collinear
     If  σ > τ, the orientation is clockwise (right turn) */
  bool computeOrientation(vec3 A, vec3 B, vec3 C)
  {
	  bool val = false;

	  val = (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);

	  return val;
  }
};


