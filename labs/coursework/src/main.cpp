//Author Name: Dimitrios Tsolis
//Matriculation Number: 40204497
//Date Last Modified: 03/04/2017
//The purpose of this coursework is to create a Solar Systm using OpenGL and C++



#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace std::chrono;
using namespace graphics_framework;
using namespace glm;


map<string, mesh> meshes;
map<string, mesh> normal_meshes;
mesh skybox;
//geometry geom;
effect eff;
effect sky_eff;
effect sun_eff;
effect shadow_eff;
free_camera cam;
chase_camera chcam;
bool cambool = true;
double cursor_x;
double cursor_y;
map<string, texture> tex;
map<string, texture> tex_normal_maps; 
point_light light;
cubemap cube_map;
vector<spot_light> spots(9);
shadow_map shadow;
float velocity;
float moon_velocity;
bool planetmovement = true;

// We could just use the Camera's projection, 
// but that has a narrower FoV than the cone of the spot light, so we would get clipping.
// so we have yo create a new Proj Mat with a field of view of 90.
mat4 LightProjectionMat;
//bool plane = false;


effect tex_eff;
effect motion_blur;
frame_buffer frames[2];
frame_buffer temp_frame;
unsigned int current_frame = 0;
geometry screen_quad;
bool motionblurbool = false;

//Bloom
effect bloom_eff;
bool bloombool = false;


//Explosion 
float explode_factor = 0.0f;
bool explodebool = false;
effect explode_eff;


//Lens flare
effect lens_eff;
frame_buffer lens_buff;
vec2 screen_res;
bool lensbool=false;



//Particles
const unsigned int MAX_PARTICLES = 30000;

vec4 positions[MAX_PARTICLES];
vec4 velocitys[MAX_PARTICLES];


GLuint G_Position_buffer, G_Velocity_buffer;


effect particle_eff;
GLuint vao;


effect particle_render;



bool initialise() {
	// *********************************
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	// *********************************
	return true;
}




void setPostProcessVariables(){
	// *********************************
	// Create 2 frame buffers - use screen width and height
	frames[0] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	frames[1] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create a temp framebuffer
	temp_frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());


	// Create screen quad
	vector<vec3> positions{ vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f) };
	vector<vec2> tex_coords{ vec2(0.0, 0.0), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f) };
	screen_quad.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screen_quad.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screen_quad.set_type(GL_TRIANGLE_STRIP);
	// *********************************
}

void CreateAndScaleMeshes() {
	// Create Sphere
	meshes["plane"] = mesh(geometry_builder::create_plane());
	meshes["box"] = mesh(geometry_builder::create_box());
	meshes["sun"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["mercury"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["venus"] = mesh(geometry_builder::create_sphere(20, 20));
	normal_meshes["earth"] = mesh(geometry_builder::create_sphere(20, 20));                                     //mesh(geometry("models/earth.obj"));
	meshes["moon"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["mars"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["jupiter"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["saturn"] = mesh(geometry("models/saturn.obj"));
	meshes["uranus"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["neptune"] = mesh(geometry_builder::create_sphere(20, 20));

	meshes["spaceinvader"] = mesh(geometry("models/Space_Invader.obj"));

	meshes["falcon"] = mesh(geometry("models/starwars-millennium-falcon.obj"));
	meshes["god"] = mesh(geometry("models/hand.OBJ"));
	meshes["death"] = mesh(geometry("models/Death_Star.obj"));

	//Transform Objects
	//meshes["plane"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	meshes["plane"].get_transform().translate(vec3(0.0f, -60.0f, 0.0f));
	meshes["plane"].get_transform().scale = vec3(0.01f, 0.01f, 0.01f);
	meshes["box"].get_transform().translate(vec3(0.0f, -59.0f, 0.0f));
	meshes["box"].get_transform().scale = vec3(2.1f, 2.1f, 2.1f);
	//meshes["box"].get_transform().scale = vec3(27.1f, 10.1f, 63.0f);
	meshes["sun"].get_transform().scale = vec3(30.0f, 30.0f, 30.0f);
	meshes["sun"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));
	meshes["sun"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	meshes["mercury"].get_transform().scale = vec3(0.23f, 0.23f, 0.23f);
	meshes["mercury"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	meshes["venus"].get_transform().scale = vec3(1.5f, 1.5f, 1.5f);
	meshes["venus"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	normal_meshes["earth"].get_transform().scale = vec3(1.0f, 1.0f, 1.0f);
	normal_meshes["earth"].get_transform().translate(vec3(meshes["sun"].get_transform().position.x + 30.0f, meshes["sun"].get_transform().position.y + 30.0f, 0.0f));
	normal_meshes["earth"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));
	meshes["moon"].get_transform().scale = vec3(0.16f, 0.16f, 0.16f);
	meshes["moon"].get_transform().translate(vec3(normal_meshes["earth"].get_transform().position.x + 10.0f, normal_meshes["earth"].get_transform().position.y + 10.0f, 0.0f));
	meshes["mars"].get_transform().scale = vec3(0.48f, 0.48f, 0.48f);
	meshes["mars"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	meshes["mars"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));
	meshes["jupiter"].get_transform().scale = vec3(11.3f, 11.3f, 11.3f);
	meshes["jupiter"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	meshes["jupiter"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));
	meshes["saturn"].get_transform().scale = vec3(11.0f, 11.0f, 11.0f);
	meshes["saturn"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	meshes["uranus"].get_transform().scale = vec3(4.1f, 4.1f, 4.1f);
	meshes["uranus"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	meshes["uranus"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));
	meshes["neptune"].get_transform().scale = vec3(3.8f, 3.8f, 3.8f);
	meshes["neptune"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	meshes["neptune"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));

	meshes["falcon"].get_transform().scale = vec3(0.00001f, 0.00001f, 0.00001f);
	meshes["falcon"].get_transform().translate(vec3(0.0f, 50.0f, 0.0f));
	meshes["god"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	meshes["god"].get_transform().scale = vec3(0.01f, 0.01f, 0.01f);
	meshes["spaceinvader"].get_transform().scale = vec3(0.001f, 0.001f, 0.001f);

	//meshes["death"].get_transform().scale = vec3(0.00001f, 0.00001f, 0.00001f);
	meshes["death"].get_transform().translate(vec3(150.0f, 0.0f, 0.0f));

}

void CreateSkybox() {
	// Create box geometry for skybox 
	skybox = mesh(geometry_builder::create_box());
	// Scale box by 100
	skybox.get_transform().scale *= 100.0f;
	// Load the cubemap
	array<string, 6> filenames = { "textures/Stars/purplenebula_ft.png", "textures/Stars/purplenebula_bk.png", "textures/Stars/purplenebula_up.png",
		"textures/Stars/purplenebula_dn.png", "textures/Stars/purplenebula_rt.png", "textures/Stars/purplenebula_lf.png" };
	// Create cube_map
	cube_map = cubemap(filenames);
}

void SetMaterials() {
	material mat;
	// *********************************
	// Set materials
	// - all emissive is black
	// - all specular is white
	// - all shininess is 25
	mat.set_emissive(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	mat.set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_shininess(25.0f);

	//Sun to yellow
	mat.set_emissive(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	mat.set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	mat.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["sun"].set_material(mat);

	mat.set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Earth to azure
	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	normal_meshes["earth"].set_material(mat);

	//Moon to white
	mat.set_emissive(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["moon"].set_material(mat);



	//Brighter planets
	//mat.set_emissive(vec4(0.01f, 0.01f, 0.01f, 1.0f));
	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["plane"].set_material(mat);
	meshes["mercury"].set_material(mat);
	meshes["venus"].set_material(mat);
	normal_meshes["earth"].set_material(mat);
	meshes["mars"].set_material(mat);
	meshes["jupiter"].set_material(mat);
	meshes["saturn"].set_material(mat);
	meshes["uranus"].set_material(mat);
	meshes["neptune"].set_material(mat);

	meshes["spaceinvader"].set_material(mat);


	meshes["box"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["box"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	meshes["box"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["box"].get_material().set_shininess(25.0f);


	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	meshes["falcon"].set_material(mat);

	//mat.set_emissive(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["god"].set_material(mat);



	mat.set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	mat.set_emissive(vec4(0.01f, 0.01f, 0.01f, 1.0f));
	meshes["death"].set_material(mat);

}

void loadTextures() {
	// Load texture
	tex["plane"] = texture("textures/check_1.png");
	tex["box"] = texture("textures/water.jpg");
	tex["sun"] = texture("textures/sun.jpg");
	tex["mercury"] = texture("textures/mercury.jpg");
	tex["venus"] = texture("textures/venus.jpg");
	tex["earth"] = texture("textures/4096_earth.jpg");
	tex["moon"] = texture("textures/moon.jpg");
	tex["mars"] = texture("textures/mars.jpg");
	tex["jupiter"] = texture("textures/jupiter.jpg");
	tex["saturn"] = texture("textures/saturn.jpg");
	tex["uranus"] = texture("textures/uranus.jpg");
	tex["neptune"] = texture("textures/neptune.jpg");
	tex["spaceinvader"] = texture("textures/spaceinvader.jpg");


	tex["lens"] = texture("textures/lens.png");


	tex["falcon"] = texture("textures/falcon.jpg");
	tex["god"] = texture("textures/water.jpg");
	tex["death"] = texture("textures/deathstar.jpg");


	// Load brick_normalmap.jpg texture
	tex_normal_maps["earth"] = texture("textures/4096_normal.jpg");
}

void setLightProperties() {
	//Spotlight ranges are all set to 0 because they are going to be used for part 2


	// Spot 0, Position (x of planet, y of planet plus 30, z of planet)
	// Green, Direction (x of planet, y of planet, z of planet) normalized
	// 20 range,0.5 power
	spots[0].set_position(vec3(meshes["mercury"].get_transform().position.x, meshes["mercury"].get_transform().position.y + 30.0f, meshes["mercury"].get_transform().position.z));
	spots[0].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[0].set_direction(normalize(vec3(meshes["mercury"].get_transform().position.x, meshes["mercury"].get_transform().position.y, meshes["mercury"].get_transform().position.z)));
	spots[0].set_range(0.0f);
	spots[0].set_power(0.5f);

	// Spot 1, Position (x of planet, y of planet plus 30, z of planet)
	// Green, Direction (x of planet, y of planet, z of planet) normalized
	// 20 range,0.5 power
	spots[1].set_position(vec3(meshes["venus"].get_transform().position.x, meshes["venus"].get_transform().position.y + 30.0f, meshes["venus"].get_transform().position.z));
	spots[1].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[1].set_direction(normalize(vec3(meshes["venus"].get_transform().position.x, meshes["venus"].get_transform().position.y, meshes["venus"].get_transform().position.z)));
	spots[1].set_range(0.0f);
	spots[1].set_power(0.5f);

	// Spot 2, Position (x of planet, y of planet plus 30, z of planet)
	// Green, Direction (x of planet, y of planet, z of planet) normalized
	// 20 range,0.5 power
	spots[2].set_position(vec3(meshes["neptune"].get_transform().position.x, meshes["neptune"].get_transform().position.y + 30.0f, meshes["neptune"].get_transform().position.z));
	spots[2].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[2].set_direction(normalize(vec3(meshes["neptune"].get_transform().position.x, meshes["neptune"].get_transform().position.y, meshes["neptune"].get_transform().position.z)));
	spots[2].set_range(0.0f);
	spots[2].set_power(0.5f);



	// Spot 1, Position (x of planet, y of planet plus 30, z of planet)
	// Green, Direction (x of planet, y of planet, z of planet) normalized
	// 20 range,0.5 power
	spots[3].set_position(vec3(meshes["mars"].get_transform().position.x, meshes["mars"].get_transform().position.y + 30.0f, meshes["mars"].get_transform().position.z));
	spots[3].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[3].set_direction(normalize(vec3(meshes["mars"].get_transform().position.x, meshes["mars"].get_transform().position.y, meshes["mars"].get_transform().position.z)));
	spots[3].set_range(0.0f);
	spots[3].set_power(0.5f);

	// Spot 1, Position (x of planet, y of planet plus 30, z of planet)
	// Green, Direction (x of planet, y of planet, z of planet) normalized
	// 20 range,0.5 power
	spots[4].set_position(vec3(meshes["jupiter"].get_transform().position.x, meshes["jupiter"].get_transform().position.y + 100.0f, meshes["jupiter"].get_transform().position.z));
	spots[4].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[4].set_direction(normalize(vec3(meshes["jupiter"].get_transform().position.x, meshes["jupiter"].get_transform().position.y, meshes["jupiter"].get_transform().position.z)));
	spots[4].set_range(0.0f);
	spots[4].set_power(0.9f);

	// Spot 1, Position (x of planet, y of planet plus 30, z of planet)
	// Green, Direction (x of planet, y of planet, z of planet) normalized
	// 20 range,0.5 power
	spots[5].set_position(vec3(meshes["uranus"].get_transform().position.x, meshes["uranus"].get_transform().position.y + 30.0f, meshes["uranus"].get_transform().position.z));
	spots[5].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[5].set_direction(normalize(vec3(meshes["uranus"].get_transform().position.x, meshes["uranus"].get_transform().position.y, meshes["uranus"].get_transform().position.z)));
	spots[5].set_range(0.0f);
	spots[5].set_power(0.5f);


	// Spot 1, Position (x of planet, y of planet plus 30, z of planet)
	// Green, Direction (x of planet, y of planet, z of planet) normalized
	// 20 range,0.5 power
	spots[6].set_position(vec3(meshes["saturn"].get_transform().position.x, meshes["saturn"].get_transform().position.y + 30.0f, meshes["saturn"].get_transform().position.z));
	spots[6].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[6].set_direction(normalize(vec3(meshes["saturn"].get_transform().position.x, meshes["saturn"].get_transform().position.y, meshes["saturn"].get_transform().position.z)));
	spots[6].set_range(0.0f);
	spots[6].set_power(0.5f);



	// Spot 1, Position (x of planet, y of planet plus 30, z of planet)
	// Green, Direction (x of planet, y of planet, z of planet) normalized
	// 20 range,0.5 power
	spots[7].set_position(vec3(normal_meshes["earth"].get_transform().position.x, normal_meshes["earth"].get_transform().position.y + 30.0f, normal_meshes["earth"].get_transform().position.z));
	spots[7].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spots[7].set_direction(normalize(vec3(normal_meshes["earth"].get_transform().position.x, normal_meshes["earth"].get_transform().position.y, normal_meshes["earth"].get_transform().position.z)));
	spots[7].set_range(0.0f);
	spots[7].set_power(0.5f);

	// Spot 8, Position (x of box plus 20, y of box plus 23, z of box)
	// Green, Direction (x of box, y of box, z of box) normalized    
	// 20 range,0.5 power
	spots[8].set_position(vec3(20.0f, -36.0f, 0.0f));

	spots[8].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//spots[8].set_direction(normalize(vec3(0, -59.0f, 0.0f) - vec3(20.0f, -36.0f, 0.0f)));
	spots[8].set_direction(normalize(vec3(meshes["box"].get_transform().position.x, meshes["box"].get_transform().position.y, meshes["box"].get_transform().position.z)));
	spots[8].set_range(500.0f);
	spots[8].set_power(10.0f);


	//Point light - Set lighting values, Position (-25, 10, -10)
	light.set_position(vec3(0.0f, 0.0f, 0.0f));
	//Point light - Light colour white
	light.set_light_colour(vec4(1.0f, 1.0f, 0.5f, 1.0f));
	//Point light - Set range to 1000
	light.set_range(1000.0f);

}

void loadShaders() {
	// Load in shaders 

	/*
	std::vector<std::string> vf = std::vector<std::string>{ "shaders/simple_shader.frag",
	"shaders/part_normal_map.frag",
	"shaders/part_spot.frag",
	"shaders/part_shadow.frag" };
	eff.add_shader(vf, GL_FRAGMENT_SHADER);
	*/

	eff.add_shader("shaders/simple_shader.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/simple_shader.frag", GL_FRAGMENT_SHADER);
	//eff.add_shader("shaders/explode.geom", GL_GEOMETRY_SHADER);
	eff.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	eff.add_shader("shaders/part_shadow.frag", GL_FRAGMENT_SHADER);


	//vector<string> frag_shaders{ "shaders/simple_shader.frag", "shaders/part_normal_map.frag",
	//   "shaders/part_spot.frag" };
	//eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);  
	// Build effect
	eff.build();

	explode_eff.add_shader("shaders/explode.vert", GL_VERTEX_SHADER);
	explode_eff.add_shader("shaders/explode.geom", GL_GEOMETRY_SHADER);
	explode_eff.add_shader("shaders/explode.frag", GL_FRAGMENT_SHADER);
	explode_eff.build();

	//Lens flare shaders
	lens_eff.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);   
	lens_eff.add_shader("shaders/lensflare.frag", GL_FRAGMENT_SHADER);
	lens_eff.build();


	 
	//Load in motion blur shaders
	motion_blur.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);
	motion_blur.add_shader("shaders/motion_blur.frag", GL_FRAGMENT_SHADER);
	//Build effect
	motion_blur.build();

	//Load in Particles effect
	particle_eff.add_shader("shaders/particle.comp", GL_COMPUTE_SHADER);
	particle_eff.build();

	//Load in Render Particle effect
	particle_render.add_shader("shaders/basic_colour.vert", GL_VERTEX_SHADER);
	particle_render.add_shader("shaders/basic_colour.frag", GL_FRAGMENT_SHADER);
	particle_render.build();

	//Load in tex_effect shaders
	tex_eff.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);
	tex_eff.add_shader("shaders/simple_texture.frag", GL_FRAGMENT_SHADER);
	//Build effect
	tex_eff.build();

	//Load in tex_effect shaders
	bloom_eff.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);
	bloom_eff.add_shader("shaders/bloom.frag", GL_FRAGMENT_SHADER);
	//Build effect
	bloom_eff.build();

	//Load in Skybox shaders
	sky_eff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
	sky_eff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);


	// Build effect
	sky_eff.build();
	//Load in Sun shaders
	sun_eff.add_shader("shaders/sun_shader.vert", GL_VERTEX_SHADER);
	sun_eff.add_shader("shaders/sun_shader.frag", GL_FRAGMENT_SHADER);
	sun_eff.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);
	// Build effect
	sun_eff.build();
	//Load in Shadow Shaders
	shadow_eff.add_shader("shaders/simple_shader.vert", GL_VERTEX_SHADER);
	shadow_eff.add_shader("shaders/simple_shader.frag", GL_FRAGMENT_SHADER);
	shadow_eff.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);
	shadow_eff.add_shader("shaders/part_spot.frag", GL_FRAGMENT_SHADER);
	shadow_eff.add_shader("shaders/part_shadow.frag", GL_FRAGMENT_SHADER);
	// Build effect
	shadow_eff.build();

	if (CHECK_GL_ERROR) {
		std::cout << 1;
	}
}

void setCamProperties() {
	// Set free camera properties
	cam.set_position(vec3(0.0f, 10.0f, 0.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
	// Set chase camera properties
	chcam.set_pos_offset(vec3(0.0f, 0.5f, 2.0f));
	chcam.set_springiness(0.5f);
	chcam.move(meshes["falcon"].get_transform().position, eulerAngles(meshes["falcon"].get_transform().orientation));
	chcam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
}

//Function that creates a float between two values
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void setParticles() {
		default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
		uniform_real_distribution<float> dist(-1.0, 1.0);

		// Initilise particles
		for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
			float randD = dist(rand);

			//positions[i] = vec4(randD*14.0f + meshes["death"].get_transform().position.x, 30.0f , cos(randD)*15.0f*randD + meshes["death"].get_transform().position.z, 0.0f); 
			vec4 velocities = vec4(vec4(7 + (10.0f * dist(rand)), 7 + (10.0f * dist(rand)), 0.1f + (7 * dist(rand)), 0.0f));
			velocities = 4.0f * velocities;
			positions[i] = vec4(meshes["death"].get_transform().position.x, meshes["death"].get_transform().position.y +3, meshes["death"].get_transform().position.z, 0.0f);
			positions[i].w = positions[i].x; 

			velocitys[i] = RandomFloat(10.0f,20.0f)*normalize(vec4( (10.0f * dist(rand)),  (10.0f * dist(rand)),  (10.0f * dist(rand)), 0.0f));
			//velocitys[i] = vec4(0.5f + dist(rand), 0.5f + dist(rand), 0.5f + dist(rand), 0.0f);
		}

		// a useless vao, but we need it bound or we get errors.
		glGenVertexArrays(1, &vao);
		
		glBindVertexArray(vao);
		// *********************************
		//Generate Position Data buffer
		glGenBuffers(1, &G_Position_buffer);
		// Bind as GL_SHADER_STORAGE_BUFFER
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, G_Position_buffer);
		// Send Data to GPU, use GL_DYNAMIC_DRAW
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * MAX_PARTICLES, positions, GL_DYNAMIC_DRAW);

		// Generate Velocity Data buffer
		glGenBuffers(1, &G_Velocity_buffer);
		// Bind as GL_SHADER_STORAGE_BUFFER
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, G_Velocity_buffer);
		// Send Data to GPU, use GL_DYNAMIC_DRAW
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * MAX_PARTICLES, velocitys, GL_DYNAMIC_DRAW);
		// *********************************
		//Unbind
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		renderer::setClearColour(0, 0, 0);

		renderer::bind(particle_eff);

		vec3 deathstarposition = meshes["death"].get_transform().position;
		glUniform3fv(particle_eff.get_uniform_location("DSP"), 1, value_ptr(deathstarposition));

	}




bool load_content() {

	//Light Projection Matrix
	LightProjectionMat = perspective<float>(half_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.f);

	// Create shadow map- use screen size
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());

	screen_res = vec2(renderer::get_screen_width(), renderer::get_screen_height());


	setPostProcessVariables();

	CreateSkybox();

	CreateAndScaleMeshes();

	SetMaterials();

	loadTextures();

	setLightProperties();

	loadShaders();

	setParticles();

	setCamProperties();

	if (CHECK_GL_ERROR) {
		std::cout << 1;
	}
	

	return true;
}




bool update(float delta_time) {

	if (CHECK_GL_ERROR) {
		std::cout << 1;
	}
	current_frame = (current_frame + 1) % 2;

	//cout << 1 / delta_time << endl;  //Framerate

	static float range = 100.0f;
	// The target object
	static mesh &target_mesh = meshes["falcon"];

	if (glfwGetKey(renderer::get_window(), 'J')) {
		explodebool = true; 
	}

	if (explodebool) {
		if (explode_factor<100) {
			explode_factor += 0.1f;
		}
		else {
			explode_factor = 0.0f;
			explodebool = false;
		}
	}

	if (glfwGetKey(renderer::get_window(), 'F')) {
		lensbool = false;
	}
	if (glfwGetKey(renderer::get_window(), 'C') ) {
		lensbool = true;
	}





	//Set the camera boolean
	if (glfwGetKey(renderer::get_window(), 'I')) {
		cambool = true;
	}
	if (glfwGetKey(renderer::get_window(), 'U')) {
		cambool = false;
	}

	//Set the motion blur boolean
	if (glfwGetKey(renderer::get_window(), 'B')) {
		motionblurbool = true;
	}
	if (glfwGetKey(renderer::get_window(), 'V')) {
		motionblurbool = false;
	}

	if (glfwGetKey(renderer::get_window(), 'M')) {
		bloombool = true;
	}
	if (glfwGetKey(renderer::get_window(), 'N')) {
		bloombool = false;
	}






	//Set position of spotlights above the moving Planets
	spots[0].set_position(vec3(meshes["mercury"].get_transform().position.x, meshes["mercury"].get_transform().position.y + 30.0f, meshes["mercury"].get_transform().position.z));
	spots[0].set_direction(normalize(vec3(meshes["mercury"].get_transform().position.x, meshes["mercury"].get_transform().position.y, meshes["mercury"].get_transform().position.z)));
	spots[1].set_position(vec3(meshes["venus"].get_transform().position.x, meshes["venus"].get_transform().position.y + 30.0f, meshes["venus"].get_transform().position.z));
	spots[1].set_direction(normalize(vec3(meshes["venus"].get_transform().position.x, meshes["venus"].get_transform().position.y, meshes["venus"].get_transform().position.z)));
	spots[2].set_position(vec3(meshes["neptune"].get_transform().position.x, meshes["neptune"].get_transform().position.y + 30.0f, meshes["neptune"].get_transform().position.z));
	spots[2].set_direction(normalize(vec3(meshes["neptune"].get_transform().position.x, meshes["neptune"].get_transform().position.y, meshes["neptune"].get_transform().position.z)));
	spots[3].set_position(vec3(meshes["mars"].get_transform().position.x, meshes["mars"].get_transform().position.y + 30.0f, meshes["mars"].get_transform().position.z));
	spots[3].set_direction(normalize(vec3(meshes["mars"].get_transform().position.x, meshes["mars"].get_transform().position.y, meshes["mars"].get_transform().position.z)));
	spots[4].set_position(vec3(meshes["jupiter"].get_transform().position.x, meshes["jupiter"].get_transform().position.y + 30.0f, meshes["jupiter"].get_transform().position.z));
	spots[4].set_direction(normalize(vec3(meshes["jupiter"].get_transform().position.x, meshes["jupiter"].get_transform().position.y, meshes["jupiter"].get_transform().position.z)));
	spots[5].set_position(vec3(meshes["uranus"].get_transform().position.x, meshes["uranus"].get_transform().position.y + 30.0f, meshes["uranus"].get_transform().position.z));
	spots[5].set_direction(normalize(vec3(meshes["uranus"].get_transform().position.x, meshes["uranus"].get_transform().position.y, meshes["uranus"].get_transform().position.z)));
	spots[6].set_position(vec3(meshes["saturn"].get_transform().position.x, meshes["saturn"].get_transform().position.y + 30.0f, meshes["saturn"].get_transform().position.z));
	spots[6].set_direction(normalize(vec3(meshes["saturn"].get_transform().position.x, meshes["saturn"].get_transform().position.y, meshes["saturn"].get_transform().position.z)));
	spots[7].set_position(vec3(normal_meshes["earth"].get_transform().position.x, normal_meshes["earth"].get_transform().position.y + 30.0f, normal_meshes["earth"].get_transform().position.z));
	spots[7].set_direction(normalize(vec3(normal_meshes["earth"].get_transform().position.x, normal_meshes["earth"].get_transform().position.y, normal_meshes["earth"].get_transform().position.z)));

	//Turn on Spotlights
	if (glfwGetKey(renderer::get_window(), 'X')) {
		//spots[0].set_range(40.0f);
		//spots[1].set_range(40.0f);
		//spots[2].set_range(40.0f);
		//spots[3].set_range(40.0f);
		//spots[4].set_range(40.0f);
		//spots[5].set_range(40.0f);
		//spots[6].set_range(40.0f);
		//spots[7].set_range(40.0f);
		spots[8].set_range(1000.0f);
		spots[8].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	//Turn off Spotlights
	if (glfwGetKey(renderer::get_window(), 'Z')) {
		spots[0].set_range(0.0f);
		spots[1].set_range(0.0f);
		spots[2].set_range(0.0f);
		spots[3].set_range(0.0f);
		spots[4].set_range(0.0f);
		spots[5].set_range(0.0f);
		spots[6].set_range(0.0f);
		spots[7].set_range(0.0f);
		spots[8].set_range(0.0f);
		spots[8].set_light_colour(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}


	// Press s to save



	// O and P to change point light range
	if (glfwGetKey(renderer::get_window(), 'P')) {
		range = range*(1.0f + 2.0f*delta_time);
	}
	if (glfwGetKey(renderer::get_window(), 'O')) {
		range = range*(1.0f - 2.0f*delta_time);
	}

	//Set the boolean to true/false to stop the planets
	if (glfwGetKey(renderer::get_window(), 'L')) {
		planetmovement = true;
	}
	if (glfwGetKey(renderer::get_window(), 'K')) {
		planetmovement = false;
	}


	if (glfwGetKey(renderer::get_window(), 'G')) {
		meshes["plane"].get_transform().scale = vec3(10.0f, 10.0f, 10.0f);

	}
	if (glfwGetKey(renderer::get_window(), 'H')) {
		meshes["plane"].get_transform().scale = vec3(0.01f, 0.01f, 0.01f);
	}
	//Move Around the Sun depending on the planetmovement boolean
	if (planetmovement) {
		meshes["mercury"].get_transform().position = (vec3(cos(velocity*3.0f)*35.0f, 0.0f, sin(velocity*3.0f)*35.0f) + meshes["sun"].get_transform().position); 
		meshes["venus"].get_transform().position = (vec3(cos(velocity*2.0f)*45.0f, 0.0f, sin(velocity*2.0f)*45.0f) + meshes["sun"].get_transform().position);
		normal_meshes["earth"].get_transform().position = (vec3(cos(velocity)*55.0f, 0.0f, sin(velocity)*55.0f) + meshes["sun"].get_transform().position);
		meshes["moon"].get_transform().position = (vec3(cos(moon_velocity*2.0f)*3.0f, 0.0f, sin(moon_velocity*3.0f)*2.0f) + normal_meshes["earth"].get_transform().position);
		meshes["mars"].get_transform().position = (vec3(cos(velocity*0.8f)*65.0f, 0.0f, sin(velocity*0.8f)*65.0f) + meshes["sun"].get_transform().position);
		meshes["jupiter"].get_transform().position = (vec3(cos(velocity*0.7f)*80.0f, 0.0f, sin(velocity*0.7f)*80.0f) + meshes["sun"].get_transform().position);
		meshes["saturn"].get_transform().position = (vec3(cos(velocity*0.6f)*95.0f, 0.0f, sin(velocity*0.6f)*95.0f) + meshes["sun"].get_transform().position);
		meshes["uranus"].get_transform().position = (vec3(cos(velocity*0.5f)*110.0f, 0.0f, sin(velocity*0.5f)*110.0f) + meshes["sun"].get_transform().position);
		meshes["neptune"].get_transform().position = (vec3(cos(velocity*0.4f)*125.0f, 0.0f, sin(velocity*0.4f)*125.0f) + meshes["sun"].get_transform().position);
	}
	else {
		meshes["mercury"].get_transform().position = vec3(meshes["mercury"].get_transform().position.x, meshes["mercury"].get_transform().position.y, meshes["mercury"].get_transform().position.z);
		meshes["venus"].get_transform().position = vec3(meshes["venus"].get_transform().position.x, meshes["venus"].get_transform().position.y, meshes["venus"].get_transform().position.z);
		normal_meshes["earth"].get_transform().position = vec3(normal_meshes["earth"].get_transform().position.x, normal_meshes["earth"].get_transform().position.y, normal_meshes["earth"].get_transform().position.z);
		meshes["moon"].get_transform().position = vec3(meshes["moon"].get_transform().position.x, meshes["moon"].get_transform().position.y, meshes["moon"].get_transform().position.z);
		meshes["mars"].get_transform().position = vec3(meshes["mars"].get_transform().position.x, meshes["mars"].get_transform().position.y, meshes["mars"].get_transform().position.z);
		meshes["jupiter"].get_transform().position = vec3(meshes["jupiter"].get_transform().position.x, meshes["jupiter"].get_transform().position.y, meshes["jupiter"].get_transform().position.z);
		meshes["saturn"].get_transform().position = vec3(meshes["saturn"].get_transform().position.x, meshes["saturn"].get_transform().position.y, meshes["saturn"].get_transform().position.z);
		meshes["uranus"].get_transform().position = vec3(meshes["uranus"].get_transform().position.x, meshes["uranus"].get_transform().position.y, meshes["uranus"].get_transform().position.z);
		meshes["neptune"].get_transform().position = vec3(meshes["neptune"].get_transform().position.x, meshes["neptune"].get_transform().position.y, meshes["neptune"].get_transform().position.z);
	}

	//Make hand stay above the earth
	meshes["god"].get_transform().position = vec3(normal_meshes["earth"].get_transform().position.x, normal_meshes["earth"].get_transform().position.y + 5.0f, normal_meshes["earth"].get_transform().position.z);
	//Hand of god
	if (glfwGetKey(renderer::get_window(), 'T')) {
		meshes["god"].get_transform().scale = vec3(10.0f, 10.0f, 10.0f); 
	}

	if (glfwGetKey(renderer::get_window(), 'Y')) {
		meshes["god"].get_transform().scale = vec3(0.01f, 0.01f, 0.01f);
	}

	//Rotate skybox
	skybox.get_transform().rotate(vec3(0.0f,-delta_time/18.0f,0.0f));

	//Set Range
	light.set_range(range);

	//Rotate Planets with aproximate values based on the earth's rotation

	meshes["sun"].get_transform().rotate(vec3(0.0f, 0.0f, -pi<float>()) * delta_time);
	meshes["venus"].get_transform().rotate(vec3(0.0f, 0.0f, -half_pi<float>() / 243.0f) * delta_time);
	meshes["mercury"].get_transform().rotate(vec3(0.0f, 0.0f, -half_pi<float>() / 58.6f) * delta_time);
	normal_meshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, -half_pi<float>()) * delta_time);
	meshes["mars"].get_transform().rotate(vec3(0.0f, 0.0f, -half_pi<float>() / 1.041f) * delta_time);
	meshes["jupiter"].get_transform().rotate(vec3(0.0f, 0.0f, -half_pi<float>() / 0.416f) * delta_time);
	meshes["saturn"].get_transform().rotate(vec3(0.0f, -half_pi<float>() / 0.458, 0.0f) * delta_time);
	meshes["uranus"].get_transform().rotate(vec3(0.0f, 0.0f, -half_pi<float>() / 0.708f) * delta_time);
	meshes["neptune"].get_transform().rotate(vec3(0.0f, 0.0f, -half_pi<float>() / 0.666f) * delta_time);
	//Rotate Box on y axis to show of shadows
	meshes["box"].get_transform().rotate(vec3(0.0f, -pi<float>(), 0.0f) * delta_time);

	// The ratio of pixels to rotation - remember the fov
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() *
		(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
		static_cast<float>(renderer::get_screen_height());

	double current_x;
	double current_y;
	// *********************************
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;
	// Multiply deltas by ratios - gets actual change in orientation
	delta_x *= ratio_width;
	delta_y *= ratio_height;
	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	if (cambool) {
		cam.rotate(delta_x, -delta_y);
		vec3 translation(0.0f, 0.0f, 0.0f);
		if (glfwGetKey(renderer::get_window(), 'W')) {
			translation.z += 3.0f;
		}
		if (glfwGetKey(renderer::get_window(), 'S')) {
			translation.z -= 3.0f;
		}
		if (glfwGetKey(renderer::get_window(), 'A')) {
			translation.x -= 3.0f;
		}
		if (glfwGetKey(renderer::get_window(), 'D')) {
			translation.x += 3.0f;
		}
		// Move camera
		cam.move(translation);

		// Update the camera
		cam.update(delta_time);
		cursor_x = current_x;
		cursor_y = current_y;
	}
	else {
		chcam.rotate(vec3(delta_y, delta_x, 0.0f));
		// Use keyboard to rotate target_mesh - QE rotate on y-axis
		if (glfwGetKey(renderer::get_window(), 'Q')) {
			//meshes["falcon"].get_transform().rotate(vec3(half_pi<float>(), 0 , 0)*delta_time);
			meshes["falcon"].get_transform().position.y += 25.0f * delta_time;
		}
		if (glfwGetKey(renderer::get_window(), 'E')) {
			//meshes["falcon"].get_transform().rotate(vec3(-half_pi<float>(), 0 , 0)*delta_time);
			meshes["falcon"].get_transform().position.y -= 25.0f * delta_time;
		}

		// Use keyboard to move the target_mesh - WSAD
		if (glfwGetKey(renderer::get_window(), 'W')) {
			meshes["falcon"].get_transform().position.z += 25.0f * delta_time;
		} 
		if (glfwGetKey(renderer::get_window(), 'S')) {
			meshes["falcon"].get_transform().position.z -= 25.0f * delta_time;
		}
		if (glfwGetKey(renderer::get_window(), 'A')) {
			meshes["falcon"].get_transform().position.x -= 25.0f * delta_time;
		}
		if (glfwGetKey(renderer::get_window(), 'D')) {
			meshes["falcon"].get_transform().position.x += 25.0f * delta_time;
		}

		chcam.move(meshes["falcon"].get_transform().position, eulerAngles(meshes["falcon"].get_transform().orientation));
		// Update the camera
		chcam.update(delta_time);
		// Update cursor pos
		cursor_x = current_x;
		cursor_y = current_y;

	}
	// Set skybox position to camera position (active camera in centre of skybox)
	if (cambool) {
		skybox.get_transform().position = cam.get_position();
	}
	else {
		skybox.get_transform().position = chcam.get_position();
	}

	velocity -= (delta_time*(1.0f - exp(1.0)) / (1.0f + exp(1.0)));  //Equation for elliptical orbit
	moon_velocity -= delta_time; //Circular Orbit


	// *********************************
	// Update the shadow map light_position from the spot light
	shadow.light_position = spots[8].get_position();
	// do the same for light_dir property
	shadow.light_dir = spots[8].get_direction();

	// *********************************

	//Set the particle dimensions and movement
	if (delta_time > 10.0f) {
		delta_time = 10.0f;
	}
	

	renderer::bind(particle_eff);
	glUniform1f(particle_eff.get_uniform_location("delta_time"), delta_time);
	glUniform3fv(particle_eff.get_uniform_location("max_dims"), 1, value_ptr(vec3(50.0f, 50.0f, 50.0f)));

	
	

	//glUniform3fv(particle_eff.get_uniform_location("max_dims"), 1, value_ptr(vec3(7.0f, 8.0f, 5.0f)));


	return true;


}




void renderSkybox() {
	// Disable depth test,depth mask,face culling
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	// Bind skybox effect
	renderer::bind(sky_eff);
	// Calculate MVP for the skybox
	mat4 MVP;
	auto M = skybox.get_transform().get_transform_matrix();
	if (cambool) {
		auto V = cam.get_view();
		auto P = cam.get_projection();
		MVP = P * V * M;
	}
	else {
		auto V = chcam.get_view();
		auto P = chcam.get_projection();
		MVP = P * V * M;
	}
	// Set MVP matrix uniform
	glUniformMatrix4fv(sky_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	// Set cubemap uniform
	renderer::bind(cube_map, 0);
	glUniform1i(sky_eff.get_uniform_location("cubemap"), 0);
	// Render skybox
	renderer::render(skybox);
	// Enable depth test,depth mask,face culling
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);


	// *********************************
}


void renderMeshes() {

	// Render meshes
	for (auto &e : meshes) {
		if (e.first == "sun") {

		}
		else if (e.first == "saturn") {
			auto m = e.second;
			// Disable Cull face so Ring is visible from both sides
			glDisable(GL_CULL_FACE);
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			mat4 MVP;
			auto M = m.get_transform().get_transform_matrix();
			if (cambool) {
				auto V = cam.get_view();
				auto P = cam.get_projection();
				MVP = P * V * M;
			}
			else {
				auto V = chcam.get_view();
				auto P = chcam.get_projection();
				MVP = P * V * M;
			}
			// Set lightMVP uniform, using:
			//Model matrix from m
			auto LM = m.get_transform().get_transform_matrix();
			// viewmatrix from the shadow map
			auto LV = shadow.get_view();
			//auto LV = glm::lookAt(shadow.light_position, meshes["sun"].get_transform().position, glm::vec3(0.0f, 0.0f, 1.0f)); //Attempt for planet shadows
			// Multiply together with LightProjectionMat
			auto lightMVP = LightProjectionMat*LV*LM;
			// Set uniform
			glUniformMatrix4fv(eff.get_uniform_location("lightMVP"), 1, GL_FALSE, value_ptr(lightMVP));
			// Set MVP matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform 
				1,                               // Number of values - 1 mat4
				GL_FALSE,                        // Transpose the matrix?
				value_ptr(MVP));                 // Pointer to matrix data

												 // *********************************
			// Set M matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			// Set N matrix uniform - remember - 3x3 matrix
			glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			// Bind material
			renderer::bind(m.get_material(), "mat");
			// Bind light
			renderer::bind(light, "point");
			// Bind spot lights
			renderer::bind(spots, "spots");
			// Bind texture
			//cout <<"++++++++HERE: "<< e.first << endl;
			renderer::bind(tex[e.first], 0);
			// Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			// Set eye position - Get this from active camera
			if (cambool) {
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
			}
			else {
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(chcam.get_position()));
			}
			// Bind shadow map texture - use texture unit 1
			renderer::bind(shadow.buffer->get_depth(), 1);
			// Set the shadow_map uniform
			glUniform1i(eff.get_uniform_location("shadow_map"), 1);
			// Render mesh
			renderer::render(m);
			glEnable(GL_CULL_FACE);
		}
		else if (e.first == "death") {
			if (explodebool == false) {
				auto m = e.second;
				// Bind effect
				renderer::bind(eff);
				// Create MVP matrix
				mat4 MVP;
				auto M = m.get_transform().get_transform_matrix();
				if (cambool) {
					auto V = cam.get_view();
					auto P = cam.get_projection();
					MVP = P * V * M;
				}
				else {
					auto V = chcam.get_view();
					auto P = chcam.get_projection();
					MVP = P * V * M;
				}
				// Set lightMVP uniform, using:
				//Model matrix from m
				auto LM = m.get_transform().get_transform_matrix();
				// viewmatrix from the shadow map
				auto LV = shadow.get_view();
				//auto LV = glm::lookAt(shadow.light_position, meshes["sun"].get_transform().position, glm::vec3(0.0f, 0.0f, 1.0f)); //Attempt for planet shadows
				// Multiply together with LightProjectionMat
				auto lightMVP = LightProjectionMat*LV*LM;
				// Set uniform
				glUniformMatrix4fv(eff.get_uniform_location("lightMVP"), 1, GL_FALSE, value_ptr(lightMVP));
				// Set MVP matrix uniform
				glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
					1,                               // Number of values - 1 mat4
					GL_FALSE,                        // Transpose the matrix?
					value_ptr(MVP));                 // Pointer to matrix data

													 // *********************************
													 // Set M matrix uniform
				glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
				// Set N matrix uniform - remember - 3x3 matrix
				glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
				// Bind material
				renderer::bind(m.get_material(), "mat");
				// Bind light
				renderer::bind(light, "point");
				// Bind spot lights
				renderer::bind(spots, "spots");
				// Bind texture
				//renderer::bind(tex[e.first], 0);
				//cout << "++++++++HERE: " << e.first << endl;
				renderer::bind(tex[e.first], 0);
				// Set tex uniform
				glUniform1i(eff.get_uniform_location("tex"), 0);
				// Set eye position - Get this from active camera
				if (cambool) {
					glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
				}
				else {
					glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(chcam.get_position()));
				}
				// Bind shadow map texture - use texture unit 1
				renderer::bind(shadow.buffer->get_depth(), 1);
				// Set the shadow_map uniform
				glUniform1i(eff.get_uniform_location("shadow_map"), 1);
				// Set explode factor uniform
				//glUniform1f(eff.get_uniform_location("explode_factor"), explode_factor);
				// Render mesh
				renderer::render(m);
				// *********************************
			}
		}
		else {
			auto m = e.second;
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			mat4 MVP;
			auto M = m.get_transform().get_transform_matrix();
			if (cambool) {
				auto V = cam.get_view();
				auto P = cam.get_projection();
				MVP = P * V * M;
			}
			else {
				auto V = chcam.get_view();
				auto P = chcam.get_projection();
				MVP = P * V * M;
			}
			// Set lightMVP uniform, using:
			//Model matrix from m
			auto LM = m.get_transform().get_transform_matrix();
			// viewmatrix from the shadow map
			auto LV = shadow.get_view();
			//auto LV = glm::lookAt(shadow.light_position, meshes["sun"].get_transform().position, glm::vec3(0.0f, 0.0f, 1.0f)); //Attempt for planet shadows
			// Multiply together with LightProjectionMat
			auto lightMVP = LightProjectionMat*LV*LM;
			// Set uniform
			glUniformMatrix4fv(eff.get_uniform_location("lightMVP"), 1, GL_FALSE, value_ptr(lightMVP));
			// Set MVP matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
				1,                               // Number of values - 1 mat4
				GL_FALSE,                        // Transpose the matrix?
				value_ptr(MVP));                 // Pointer to matrix data

												 // *********************************
												 // Set M matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			// Set N matrix uniform - remember - 3x3 matrix
			glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			// Bind material
			renderer::bind(m.get_material(), "mat");
			// Bind light
			renderer::bind(light, "point");
			// Bind spot lights
			renderer::bind(spots, "spots");
			// Bind texture
			//renderer::bind(tex[e.first], 0);
			//cout << "++++++++HERE: " << e.first << endl;
			renderer::bind(tex[e.first], 0);
			// Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			// Set eye position - Get this from active camera
			if (cambool) {
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
			}
			else {
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(chcam.get_position()));
			}
			// Bind shadow map texture - use texture unit 1
			renderer::bind(shadow.buffer->get_depth(), 1);
			// Set the shadow_map uniform
			glUniform1i(eff.get_uniform_location("shadow_map"), 1);
			// Render mesh
			renderer::render(m);
			// *********************************
		}
	}


}
//glUniform3fv(particle_eff.get_uniform_location("max_dims"), 1, &(vec3(50.0f, 5.0f, 5.0f))[0]);

void renderNormalMeshes() {


	for (auto &e : normal_meshes) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		mat4 MVP;
		auto M = m.get_transform().get_transform_matrix();
		if (cambool) {
			auto V = cam.get_view();
			auto P = cam.get_projection();
			MVP = P * V * M;
		}
		else {
			auto V = chcam.get_view();
			auto P = chcam.get_projection();
			MVP = P * V * M;
		}
		// Set lightMVP uniform, using:
		//Model matrix from m
		auto LM = m.get_transform().get_transform_matrix();
		// viewmatrix from the shadow map
		auto LV = shadow.get_view();
		// Multiply together with LightProjectionMat
		auto lightMVP = LightProjectionMat*LV*LM;
		// Set uniform
		glUniformMatrix4fv(eff.get_uniform_location("lightMVP"), 1, GL_FALSE, value_ptr(lightMVP));
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
			1,                               // Number of values - 1 mat4
			GL_FALSE,                        // Transpose the matrix?
			value_ptr(MVP));                 // Pointer to matrix data

											 // *********************************
											 // Set M matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		// Set N matrix uniform - remember - 3x3 matrix
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
		// Bind material
		renderer::bind(m.get_material(), "mat");
		// Bind light
		renderer::bind(light, "point");
		// Bind spot lights
		renderer::bind(spots, "spots");
		// Bind texture
		renderer::bind(tex[e.first], 0);
		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);
		// Bind normal_map
		renderer::bind(tex_normal_maps[e.first], 1);
		// Set normal_map uniform
		glUniform1i(eff.get_uniform_location("normal_map"), 1);
		// Set eye position - Get this from active camera
		if (cambool) {
			glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
		}
		else {
			glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(chcam.get_position()));
		}
		// Bind shadow map texture - use texture unit 1
		renderer::bind(shadow.buffer->get_depth(), 1);
		// Set the shadow_map uniform
		glUniform1i(eff.get_uniform_location("shadow_map"), 1);
		// Render mesh
		renderer::render(m);
		// *********************************
	}


}

void renderSun() {
	
	auto m = meshes["sun"];	// Bind effect

	renderer::bind(sun_eff);
	// Create MVP matrix
	mat4 MVP;
	auto M = m.get_transform().get_transform_matrix();
	if (cambool) {
		auto V = cam.get_view();
		auto P = cam.get_projection();
		MVP = P * V * M;
	}
	else {
		auto V = chcam.get_view();
		auto P = chcam.get_projection();
		MVP = P * V * M;
	}
	// Set MVP matrix uniform
	glUniformMatrix4fv(sun_eff.get_uniform_location("MVP"), // Location of uniform
		1,                               // Number of values - 1 mat4
		GL_FALSE,                        // Transpose the matrix?
		value_ptr(MVP));                 // Pointer to matrix data

										 // *********************************
										 // Set M matrix uniform
	glUniformMatrix4fv(sun_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
	// Set N matrix uniform - remember - 3x3 matrix
	glUniformMatrix3fv(sun_eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
	// Bind material
	renderer::bind(m.get_material(), "mat");
	// Bind light
	renderer::bind(light, "point");

	// Bind texture
	renderer::bind(tex["sun"], 0);
	// Set tex uniform
	glUniform1i(sun_eff.get_uniform_location("tex"), 0);
	// Set eye position - Get this from active camera
	if (cambool) {
		glUniform3fv(sun_eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
	}
	else {
		glUniform3fv(sun_eff.get_uniform_location("eye_pos"), 1, value_ptr(chcam.get_position()));
	}
	// Render mesh
	renderer::render(m);
	// *********************************



}

void renderspaceinvaderTransformation() {    //Transform Hierarchy

	// Bind effect
	renderer::bind(eff);
	// Get PV
	mat4 PV = cam.get_projection() * cam.get_view();
	if (cambool) {
		PV = cam.get_projection() * cam.get_view();
	}
	else {
		PV = chcam.get_projection() * chcam.get_view();
	}
	// Set the texture value for the shader here
	glUniform1i(eff.get_uniform_location("tex"), 0);
	// Find the lcoation for the MVP uniform
	const auto loc = eff.get_uniform_location("MVP");

	auto M = meshes["spaceinvader"].get_transform().get_transform_matrix();

	// Apply the heirarchy chain
	M = meshes["sun"].get_transform().get_transform_matrix() * M;

	// Set MVP matrix uniform
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(PV * M));
	// Bind texture to renderer
	renderer::bind(tex["spaceinvader"], 0);
	// Render mesh
	renderer::render(meshes["spaceinvader"]);

}

void renderShadows() {


	// Set render target to shadow map
	renderer::set_render_target(shadow);
	// Clear depth buffer bit
	glClear(GL_DEPTH_BUFFER_BIT);
	// Set face cull mode to front
	glCullFace(GL_FRONT);
	// *********************************

	// Bind shader
	renderer::bind(shadow_eff);

	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		// Create MVP matrix
		mat4 M = m.get_transform().get_transform_matrix();
		// *********************************
		// View matrix taken from shadow map
		mat4 V = shadow.get_view();
		//V = glm::lookAt(shadow.light_position, meshes["sun"].get_transform().position , glm::vec3(0.0f, 0.0f, 1.0f));


		// *********************************
		mat4 MVP = LightProjectionMat * (V * M);
		// Set MVP matrix uniform
		glUniformMatrix4fv(shadow_eff.get_uniform_location("MVP"), // Location of uniform
			1,                                      // Number of values - 1 mat4
			GL_FALSE,                               // Transpose the matrix?
			value_ptr(MVP));                        // Pointer to matrix data
													// Render mesh
		renderer::render(m);
	}


	// Render meshes
	for (auto &e : normal_meshes) {
		auto m = e.second;
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		// *********************************
		// View matrix taken from shadow map
		auto V = shadow.get_view();
		// *********************************
		auto MVP = LightProjectionMat * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(shadow_eff.get_uniform_location("MVP"), // Location of uniform
			1,                                      // Number of values - 1 mat4
			GL_FALSE,                               // Transpose the matrix?
			value_ptr(MVP));                        // Pointer to matrix data
													// Render mesh
		renderer::render(m);
	}

	// *********************************
	// Set render target back to the screen
	renderer::set_render_target();
	// Set face cull mode to back
	glCullFace(GL_BACK);
	// *********************************

	// Bind shader
	renderer::bind(eff);
}

void renderMotionblur() {
	if (motionblurbool) {

		// !!!!!!!!!!!!!!! SECOND PASS !!!!!!!!!!!!!!!!
		// *********************************
		// Set render target to current frame
		renderer::set_render_target(frames[current_frame]);
		// Clear frame
		renderer::clear();
		// Bind motion blur effect
		renderer::bind(motion_blur);
		// MVP is now the identity matrix
		auto MVP = mat4(1.0f);
		// Set MVP matrix uniform
		glUniformMatrix4fv(motion_blur.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// Bind tempframe to TU 0.
		renderer::bind(temp_frame.get_frame(), 0);
		// Bind frames[(current_frame + 1) % 2] to TU 1.
		renderer::bind(frames[(current_frame + 1) % 2].get_frame(), 1);
		// Set tex uniforms
		glUniform1i(motion_blur.get_uniform_location("tex"), 0);
		glUniform1i(motion_blur.get_uniform_location("previous_frame"), 1);
		// Set blend factor (0.9f)
		glUniform1f(motion_blur.get_uniform_location("blend_factor"), 0.9f);
		// Render screen quad
		renderer::render(screen_quad);

		// !!!!!!!!!!!!!!! SCREEN PASS !!!!!!!!!!!!!!!!

		// Set render target back to the screen
		renderer::set_render_target();
		renderer::bind(tex_eff);
		// Set MVP matrix uniform
		glUniformMatrix4fv(tex_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// Bind texture from frame buffer
		renderer::bind(frames[current_frame].get_frame(), 0);
		// Set the uniform
		glUniform1i(tex_eff.get_uniform_location("tex"), 0);
		// Render the screen quad
		renderer::render(screen_quad);
		// *********************************

	}
}

void renderBloom() {
		if (bloombool) {
			// Set render target back to the screen
			renderer::set_render_target();
			// Bind Tex effect
			renderer::bind(bloom_eff);
			// MVP is now the identity matrix
			auto MVP = mat4(1.0f);
			// Set MVP matrix uniform
			glUniformMatrix4fv(bloom_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			// Bind texture from frame buffer
			renderer::bind(temp_frame.get_frame(), 0);
			// Set the tex uniform
			glUniform1i(bloom_eff.get_uniform_location("tex"), 0);
			// Set inverse width Uniform
			glUniform1f(bloom_eff.get_uniform_location("inverse_width"), 1.0 / renderer::get_screen_width());
			// Set inverse height Uniform
			glUniform1f(bloom_eff.get_uniform_location("inverse_height"), 1.0 / renderer::get_screen_height());
			// Render the screen quad
			renderer::render(screen_quad);
		}
}

void renderParticles1() {
	if (explodebool) {

		renderer::set_render_target();
		// Bind Compute Shader
		renderer::bind(particle_eff);
		// Bind data as SSBO
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, G_Position_buffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, G_Velocity_buffer);
		// Dispatch
		glDispatchCompute(MAX_PARTICLES / 128, 1, 1);
		// Sync, wait for completion
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		mat4 MVP;
		mat4 M(1.0f);
		if (cambool) {
			auto V = cam.get_view();
			auto P = cam.get_projection();
			MVP = P * V * M;
		}
		else {
			auto V = chcam.get_view();
			auto P = chcam.get_projection();
			MVP = P * V * M;
		}

		// Bind render effect
		renderer::bind(particle_render);

		glUniform4fv(particle_render.get_uniform_location("colour"), 1, value_ptr(vec4(0.886f, 0.345f, 0.133f, 1.0f)));
		// Set MVP matrix uniform
		glUniformMatrix4fv(particle_render.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		// Bind position buffer as GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, G_Position_buffer);
		// Setup vertex format
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		// Render
		glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
		// Tidy up
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}
}

void renderExplosion() {
	if (explodebool) {
		renderer::bind(eff);
		// Create MVP matrix
		mat4 MVP;
		auto M = meshes["death"].get_transform().get_transform_matrix();
		if (cambool) {
			auto V = cam.get_view();
			auto P = cam.get_projection();
			MVP = P * V * M;
		}
		else {
			auto V = chcam.get_view();
			auto P = chcam.get_projection();
			MVP = P * V * M;
		}
		renderer::bind(explode_eff);
		// Set MVP matrix uniform
		glUniformMatrix4fv(explode_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// *********************************
		// Set explode factor uniform
		glUniform1f(explode_eff.get_uniform_location("explode_factor"), explode_factor);
		glDisable(GL_CULL_FACE);
		renderer::render(meshes["death"]); 
	}
}

void renderLensflare() { 
	
	if (lensbool) {

		//renderer::set_render_target(temp_frame);
		// Clear frame
		//renderer::clear(); 

		// Set render target back to the screen
		renderer::set_render_target();
		// Bind Tex effect
		renderer::bind(lens_eff);
		// MVP 
		auto MVP = mat4(1.0f);    
	/*	mat4 MVP;
		mat4 M(1.0f);
		if (cambool) {
			auto V = cam.get_view();
			auto P = cam.get_projection();
			MVP = P * V * M;
		}
		else {
			auto V = chcam.get_view();
			auto P = chcam.get_projection();
			MVP = P * V * M;
		}*/
		// Set MVP matrix uniform 
		glUniformMatrix4fv(lens_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// Bind texture 
		//renderer::bind(tex["lens"], 0);  
		renderer::bind(temp_frame.get_frame(), 0);  
		// Set the tex uniform
		glUniform1i(lens_eff.get_uniform_location("tex"), 0);
		//Uniform screen
		glUniform2fv(lens_eff.get_uniform_location("resolution"), 1, value_ptr(screen_res));
		//Render screen quad
		renderer::render(screen_quad);
		//renderer::set_render_target();
	}
	

}

bool render() {

	// Set clear colour to reddish 
	renderer::setClearColour(1.0f, 0.1f, 0.1f);

	//renderer::bind(lens_eff);

	// !!!!!!!!!!!!!!! FIRST PASS !!!!!!!!!!!!!!!!

	if (motionblurbool) {
			// Set render target to temp frame
			renderer::set_render_target(temp_frame);
			// Clear frame
			renderer::clear();
		}

	if (bloombool) {
			// Set render target to temp frame
			renderer::set_render_target(temp_frame);
			// Clear frame
			renderer::clear();
	} 

	if (lensbool) {
			// Set render target to temp frame
			renderer::set_render_target(temp_frame);
			// Clear frame
			renderer::clear();
		}   

	//renderer::set_render_target();
	
	renderspaceinvaderTransformation(); //The Transformation object is inside the sun, so user has to navigate there if he wishes to see it

	renderSkybox();  

	renderMeshes();

	renderNormalMeshes();

	renderParticles1();

	renderSun(); 

	renderShadows();
	
	renderMotionblur();

	renderLensflare();

	renderBloom();

	renderExplosion();
	
//	if (CHECK_GL_ERROR) {
//		std::cout << 1;
//	}

	return true;
}

void main() {
	// Create application
	app application("Graphics Coursework");
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_initialise(initialise);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}