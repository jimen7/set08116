#version 450 core

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming binormal
layout(location = 3) in vec3 binormal;
// Incoming tangent
layout(location = 4) in vec3 tangent;
// Incoming texture coordinate
layout(location = 10) in vec2 tex_coord_in;

// Outgoing vertex position
layout(location = 0) out vec3 vertex_position;
// Outgoing transformed normal
layout(location = 1) out vec3 transformed_normal;
// Outgoing texture coordinate
layout(location = 2) out vec3 tex_coord_out1;
// Outgoing tangent
layout(location = 3) out vec3 tangent_out;
// Outgoing binormal
layout(location = 4) out vec3 binormal_out;

void main() {
  // Set position
  gl_Position = MVP * vec4(position, 1);
  // *********************************
  // Output other values to fragment shader
  vertex_position = vec3(M * vec4(position, 1.0f));
  transformed_normal = -1.0f * N * normal;
  tex_coord_out1 = vec3(tex_coord_in,0.0f);
  // *********************************
    // Transform tangent
  tangent_out = N * tangent;
  // Transform binormal
  binormal_out = N * binormal;
  // *********************************
}