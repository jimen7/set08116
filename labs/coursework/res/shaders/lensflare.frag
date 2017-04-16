#version 440

// Sampler used to get texture colour
uniform sampler2D tex;
uniform vec2 resolution;

uniform int ghosts;
uniform bool weightbool;

float dispertion = 0.3;



// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing colour
layout(location = 0) out vec4 out_colour;



void main() {
  // *********************************
  // Set out colour to sampled texture colour
  //out_colour = tex;
 // vec4 colour = texture()
// out_colour = texture(tex , tex_coord) ;

vec2 texcoord = -tex_coord + vec2(1.0);
      vec2 texelSize = 1.0 / vec2(textureSize(tex, 0));
 
   // ghost vector to image centre:
      vec2 ghostVec = (vec2(0.5) - tex_coord) * dispertion;
   
   // sample ghosts:  
      vec4 result = vec4(0.0);
      for (int i = 0; i < ghosts; ++i) { 
         //vec2 offset = fract(tex_coord + ghostVec * float(i));
		 vec2 offset = fract(tex_coord + ghostVec * float(i));

		 if(weightbool){
		 float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
         weight = pow(1.0 - weight, 10.0);

		 result += texture(tex, offset)*weight;
		 }
		 else{
		 result += texture(tex, offset);
		 }
  
         
      }
 
      out_colour = result;
	  out_colour.a = 1.0f;
   

  // *********************************
}