// Pass 1 vertex shader
/*
  [2 marks] Edit shaders/pass1.vert to correctly compute the colour,
  normal, and depth.  These are passed to the fragment shader, which
  will store these values in the corresponding textures.  You do not
  have to edit the fragment shader in shaders/pass1.frag.
*/
// Stores colour, normal, depth

#version 300 es

uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP;

//precision mediump float;

//uniform mat4 OCS_to_lightCCS;

layout (location = 0) in mediump vec3 vertPosition;
layout (location = 1) in mediump vec3 vertNormal;
layout (location = 2) in mediump vec3 vertTexCoord;

// Your shader should compute the colour, normal (in the VCS), and
// depth (in the range [0,1] with 0=near and 1=far) and store these
// values in the corresponding variables.

out mediump vec3 colour;
out mediump vec3 normal;
out mediump float depth;

//layout (location=0) in vec3 vertPosition;

//out float ccsLightDepth;

void main()

{
  // calc vertex position in CCS (always required)


  gl_Position = MVP * vec4( vertPosition, 1.0 );
  
  //vec4 ccsLightPos  = OCS_to_lightCCS * vec4(vertPosition, 1.0f);
  //gl_Position = ccsLightPos;

  //ccsLightDepth = 0.5 * ((ccsLightPos.z / ccsLightPos.w) + 1.0);

  // Provide a colour 

  //colour = vec3(1.0,0.0,0.0);         // YOUR CODE HERE
  colour = vec3(1,0,0);

  // calculate normal in VCS

  //normal = vec3(0.0,1.0,0.0);         // YOUR CODE HERE
  normal = (MV * vec4( vertNormal, 0.0)).xyz;

  // Calculate the depth in [0,1]

  //depth = 0.5;                  // YOUR CODE HERE
  depth = (gl_Position.z/gl_Position.w+1.0)/2.0;  
}
