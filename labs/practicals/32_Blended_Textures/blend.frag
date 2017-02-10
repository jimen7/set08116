#version 440 core

// Main textures
uniform sampler2D texs[2];
// Blend map
uniform sampler2D blend;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing fragment colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Sample the two main textures
  vec4 sample1 = texture(texs[1], tex_coord);
  vec4 sample2 = texture(texs[0], tex_coord);
  // Sample the blend texture
  vec4 blendSample = texture(blend, tex_coord);
  // Mix the main samples using r component from blend value
  colour = mix(sample1, sample2, blendSample.r);
 //colour =  mix(vec4(1,0,0,1), sample1, blendSample.r);
 //colour = sample1;
  // *********************************
}