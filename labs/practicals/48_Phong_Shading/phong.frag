#version 440

// A directional light structure
struct directional_light {
  vec4 ambient_intensity;
  vec4 light_colour;
  vec3 light_dir;
};

// A material structure
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Directional light for the scene
uniform directional_light light;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;

// Incoming position
layout(location = 0) in vec3 vertex_position;
// Incoming normal
layout(location = 1) in vec3 transformed_normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord_in;

// Outgoing colour
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 darkcolour;

void main() {

  // *********************************
  // Calculate ambient component
  vec4 ambient = mat.diffuse_reflection*light.ambient_intensity;
  // Calculate k
  float ka = max(dot(transformed_normal, light.light_dir), 0.0);
  // Calculate diffuse component
  vec4 diffuse = ka*(mat.diffuse_reflection*light.light_colour);
  // Calculate view direction
  vec3 view_dir = vec3(eye_pos-vertex_position);
  // Calculate half vector
   vec3 half_vector = normalize(light.light_dir+view_dir);
  // Calculate specular component
  // Calculate k
  float kb = pow(max(dot(transformed_normal, half_vector), 0.0), mat.shininess);
  vec4 specular = kb * (mat.specular_reflection*light.light_colour);
  // Sample texture
  vec4 sample_texture = texture(tex,tex_coord_in);
  // Calculate primary colour component
  vec4 primary = mat.emissive + ambient + diffuse;

  float brightness = dot(sample_texture.xyz, vec3(0.2126, 0.7152, 0.0722))
	
  // Calculate final colour - remember alpha
  primary.a = 1.0f;
  if(brightness > 1.0)
	 colour = primary * sample_texture+specular;
  // *********************************
}