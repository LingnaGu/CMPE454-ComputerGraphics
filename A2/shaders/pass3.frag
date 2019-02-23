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

  mediump float depth     = texture2D(depthSampler, texCoords).r;
  mediump float laplacian = texture2D(laplacianSampler, texCoords).r;

  // [1 mark] Discard the fragment if it is a background pixel not
  // near the silhouette of the object.

  // YOUR CODE HERE
  if (depth == 1.0 && laplacian > -0.1)
  {
      // Where 1.0 and -0.1 are depth and laplacian thresholds
      discard;
  }

  // [0 marks] Look up value for the colour and normal.  Use the RGB
  // components of the texture as texture2D( ... ).rgb or texture2D( ... ).xyz.

  // YOUR CODE HERE

  mediump vec3 colour = texture2D(colourSampler, texCoords).rgb;
  mediump vec3 normal = texture2D(normalSampler, texCoords).xyz;

  // [2 marks] Compute Cel shading, in which the diffusely shaded
  // colour is quantized into four possible values.  Do not allow the
  // diffuse component, N dot L, to be below 0.2.  That will provide
  // some ambient shading.  Your code should use the 'numQuata' below
  // to have that many divisions of quanta of colour.  Your code
  // should be very efficient.

  // YOUR CODE HERE

  mediump float NdotL = dot( normalize(normal), lightDir );

  const int numQuanta = 3;
  mediump float i = 0;

  for ( i = float(0); i < float(1); i += float( float(1) / float(numQuanta) ) )
  {
    if (NdotL > i) 
    {
      outputColour = i * vec4(colour, 1.0);
    }
  }

  if (NdotL < 0.2) 
  {
    outputColour = 0.2 * vec4(colour, 1.0);
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
  int count = 0;
  
  for (int y = 0-kernelRadius; y <= kernelRadius+1; y++)
  {
    for (int x = 0-kernelRadius; x <= kernelRadius+1; x++)
    {
      mediump vec2 fragCoords = vec2(texCoords.x + x, texCoords.y + y);

      if (fragCoords.x >= 0 && fragCoords.y >= 0 && fragCoords.x < 50 && fragCoords.y < 50)
      {
        if ( texture2D(laplacianSampler, fragCoords).r < -0.1 )
        {
          count++;
          break;
        }
      }
    }
  }

  // YOUR CODE HERE

  // [0 marks] Output the fragment colour.  If there is an edge
  // fragment in the 3x3 neighbourhood of this fragment, output a
  // black colour.  Otherwise, output the cel-shaded colour.
  //
  // Since we're making this black is there is any edge in the 3x3
  // neighbourhood of the fragment, the silhouette will be wider
  // than if we test only the fragment.

  // YOUR CODE HERE
  if (count == 0)
  {
    outputColour = vec4(0.0,0.0,0.0,1.0);
  }
  else
  {
    outputColour = vec4(colour, 1);
  }
}
