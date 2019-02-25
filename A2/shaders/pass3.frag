// Pass 3 fragment shader
//
// Output fragment colour based using
//    (a) Cel shaded diffuse surface
//    (b) wide silhouette in black

#version 300 es

uniform mediump vec3 lightDir;     // direction toward the light in the VCS
uniform mediump vec2 texCoordInc;  // texture coord difference between adjacent texels

in mediump vec2 texCoords;              // texture coordinates at this fragment

// The following four textures are now available and can be sampled
// using 'texCoords'

uniform sampler2D colourSampler;
uniform sampler2D normalSampler;
uniform sampler2D depthSampler;
uniform sampler2D laplacianSampler;

out mediump vec4 outputColour;          // the output fragment colour as RGBA with A=1


void main()

{
  // [0 marks] Look up values for the depth and Laplacian.  Use only
  // the R component of the texture as texture2D( ... ).r

  // YOUR CODE HERE
  mediump float depth = texture(depthSampler, texCoords).r;
  mediump float laplacian = texture(laplacianSampler, texCoords).r;

  // [1 mark] Discard the fragment if it is a background pixel not
  // near the silhouette of the object.

  // YOUR CODE HERE
  if (depth == 1.0)
  {
    // Discard fragment if background pixel
    discard;
  }

  // [0 marks] Look up value for the colour and normal.  Use the RGB
  // components of the texture as texture2D( ... ).rgb or texture2D( ... ).xyz.

  // YOUR CODE HERE
  mediump vec3 colour = texture(colourSampler, texCoords).rgb;
  mediump vec3 normal = texture(normalSampler, texCoords).xyz;  

  // [2 marks] Compute Cel shading, in which the diffusely shaded
  // colour is quantized into four possible values.  Do not allow the
  // diffuse component, N dot L, to be below 0.2.  That will provide
  // some ambient shading.  Your code should use the 'numQuata' below
  // to have that many divisions of quanta of colour.  Your code
  // should be very efficient.

  // YOUR CODE HERE

  const int numQuanta = 3;
  highp float increment = 1.0 / float(numQuanta);
  highp float NL = dot( normalize(normal), lightDir );  // Compute N dot L

  // Iterate through quantized shading levels to find correct level based on NL
  for ( highp float i = 0.0; i < 1.0; i += increment ) 
  {
    if (NL < 0.2) 
    {
      // Don't allow diffuse component to be below 0.2
      outputColour = 0.2 * vec4(colour, 1.0);
      break;
    }
    if (NL > i) 
    {
      outputColour = i * vec4(colour, 1.0);
    }
  }

  // [2 marks] Count number of fragments in the 3x3 neighbourhood of
  // this fragment with a Laplacian that is less than -0.1.  These are
  // the edge fragments.  Your code should use the 'kernelRadius'
  // below and check all fragments in the range
  //
  //    [-kernelRadius,+kernelRadius] x [-kernelRadius,+kernelRadius]
  //
  // around this fragment.

  const int kernelRadius = 1;
  bool isEdge = false;
  int count = 0;
  
  // Iterate through neighbourhood to detect if the fragment is part of an edge, or has an edge in it's neighbourhood
  for (int y = 0-kernelRadius; y <= kernelRadius; y++)
  {
    for (int x = 0-kernelRadius; x <= kernelRadius; x++)
    {
      // Calculate fragment coords for lookup
      mediump vec2 fragCoords = vec2( texCoords.x + float(x), texCoords.y + float(y) );

      if ( texture2D(laplacianSampler, fragCoords).r < -0.1 )
      {
        // We have identified an edge
        if (x == 0 && y == 0)
        {
          // If this fragment itself is an edge
          isEdge = true;
        }

        count++;
      }
    }
  }

  // [0 marks] Output the fragment colour.  If there is an edge
  // fragment in the 3x3 neighbourhood of this fragment, output a
  // black colour.  Otherwise, output the cel-shaded colour.
  //
  // Since we're making this black is there is any edge in the 3x3
  // neighbourhood of the fragment, the silhouette will be wider
  // than if we test only the fragment.

  // YOUR CODE HERE

  if (count > 0)
  {
    // Detected edge in 3x3 neighbourhood
    mediump vec4 edgeColour = vec4(0.0, 0.0, 0.0, 1.0); // Black
    
    if (isEdge)
    {
      outputColour = edgeColour;
    }
    else
    {
      // Output black blended with phong computed colour if fragment isn't an edge but has edges in it's 3x3 neighbourhood
      mediump vec3 Iin = vec3( 1.0, 1.0, 1.0 ); // incoming light
      mediump vec3 Ia  = vec3( 0.0, 0.0, 0.0 ); // ambient light
      mediump vec3 kd  = vec3(outputColour.x, outputColour.y, outputColour.z); // kd = surface colour from calling program
      mediump vec3 ks  = vec3( 0.4, 0.4, 0.4 ); // ks = specular coefficients
      mediump float shininess = 400.0;	        // shininess = specular exponent

      mediump vec3 Iout = Ia;

      if(NL > 0.0)
      {
        Iout += NL * vec3( kd.r * Iin.r, kd.g * Iin.g, kd.b * Iin.b ); 
        mediump vec3 R = (2.0 * NL) * normal - lightDir;
        mediump vec3 V = vec3(0.0,0.0,1.0);	// direction toward eye in the VCS

        mediump float RdotV = dot( R, V );

        if (RdotV > 0.0)
        {
	      Iout += pow( RdotV, shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b );
        }
      }
    
      Iout = 0.5 * Iout + 0.5 * vec3(edgeColour.x, edgeColour.y, edgeColour.z);
      outputColour = vec4(Iout, 1.0);
    }
  }
  else
  {
    // Did not detect edge in 3x3 neighbourhood, outputColour is already defined during quantization
  }
}
