#version 450 core

// Point light information
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

// Spot light data
#ifndef SPOT_LIGHT
#define SPOT_LIGHT
struct spot_light {
  vec4 light_colour;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float power;
};
#endif

// Material information
struct material { 
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

vec3 calc_normal(in vec3 normal, in vec3 tangent, in vec3 binormal, in sampler2D normal_map, in vec2 tex_coord_out);
//vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
         //           in vec4 tex_colour);

// Point light for the scene
uniform point_light point;
// Spot lights being used in the scene
uniform spot_light spots[8];
// Material for the object
uniform material mat;
// Eye position
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;
// Normal map to sample from
uniform sampler2D normal_map;

// Incoming position
layout(location = 0) in vec3 vertex_position;
// Incoming normal
layout(location = 1) in vec3 transformed_normal;
// Incoming texture coordinate
layout(location = 2) in vec3 tex_coord_out1;
// Incoming tangent
layout(location = 3) in vec3 tangent_out;
// Incoming binormal
layout(location = 4) in vec3 binormal_out;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Get distance between point light and vertex
  float d = distance(point.position, vertex_position);
  // Calculate attenuation factor
  float a = (point.constant + (point.linear*d) + (point.quadratic*d*d));
  // Calculate light colour
	vec4 light_colour = point.light_colour/a;
	light_colour.a = 1.0f;
  // Calculate light dir
  vec3 light_dir = normalize(point.position - vertex_position); 
  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient

  // Calculate k
  float ka = max(dot(transformed_normal, light_dir), 0.0f);
  // Calculate diffuse
  vec4 diffuse = ka * (mat.diffuse_reflection * light_colour);
  // Calculate view direction
  vec3 view_dir = normalize(eye_pos - vertex_position);
  // Calculate half vector
  vec3 half_vector  = normalize(light_dir + view_dir);
  // Calculate specular component
  // Calculate k
  float kb = pow(max(dot(transformed_normal, half_vector), 0.0f), mat.shininess);
  // Calculate specular
  vec4 specular = kb * (mat.specular_reflection * light_colour);
  // Sample texture
  vec4 sample_texture = texture(tex, tex_coord_out1.xy);
    // Calculate normal from normal map
  vec3 calculated_normal = calc_normal(transformed_normal, tangent_out, binormal_out, normal_map, tex_coord_out1.xy);
  // Calculate primary colour component
  vec4 primary = (mat.emissive + diffuse);
  // Calculate final colour - remember alpha
  primary.a = 1.0f;
  colour = (primary * sample_texture) + specular;
  colour.a = 1.0f;

    // Sum spot lights
//  for (int i = 0; i < 8; ++i) {
	//colour += calculate_spot(spots[i], mat, vertex_position, transformed_normal, view_dir, sample_texture);
 // }

  // *********************************
}