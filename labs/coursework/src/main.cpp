#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
map<string, mesh> normal_meshes;
mesh skybox;
geometry geom;
effect eff;
effect sky_eff;
effect sun_eff;
free_camera cam;
double cursor_x; 
double cursor_y;
map<string, texture> tex;
map<string, texture> tex_normal_maps;
point_light light;	
cubemap cube_map;
float velocity;
bool button = true;


bool initialise() {
	// *********************************
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	// *********************************
	return true;
}

bool load_content() {
	// Create box geometry for skybox 
	skybox = mesh(geometry_builder::create_box());  
	// Scale box by 100
	skybox.get_transform().scale *= 100.0f;
	// Load the cubemap
	array<string, 6> filenames = { "textures/Stars/purplenebula_ft.png", "textures/Stars/purplenebula_bk.png", "textures/Stars/purplenebula_up.png",
		"textures/Stars/purplenebula_dn.png", "textures/Stars/purplenebula_rt.png", "textures/Stars/purplenebula_lf.png" };
	// Create cube_map
	cube_map = cubemap(filenames);
  // Create Sphere
	meshes["plane"] = mesh(geometry_builder::create_plane());
	normal_meshes["earth"] = mesh(geometry_builder::create_sphere(20, 20));                                     //mesh(geometry("models/earth.obj"));
	meshes["mercury"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["sun"] = mesh(geometry_builder::create_sphere(20,20));
	meshes["moon"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["falcon"] = mesh(geometry("models/starwars-millennium-falcon.obj"));
	meshes["god"] = mesh(geometry("models/hand.OBJ"));

  //Transform Objects
	meshes["plane"].get_transform().scale = vec3(10.0f, 10.0f, 10.0f);
	meshes["sun"].get_transform().scale = vec3(20.0f, 20.0f, 20.0f);
	meshes["sun"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));
	meshes["sun"].get_transform().translate(vec3(0.0f, 0.0f,0.0f));
	meshes["mercury"].get_transform().scale = vec3(0.23f, 0.23f, 0.23f);
	meshes["mercury"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	normal_meshes["earth"].get_transform().scale = vec3(2.0f, 2.0f, 2.0f);
	normal_meshes["earth"].get_transform().translate(vec3(meshes["sun"].get_transform().position.x + 30.0f, meshes["sun"].get_transform().position.y + 30.0f, 0.0f));
	normal_meshes["earth"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));
	meshes["moon"].get_transform().scale = vec3(0.5f, 0.5f, 0.5f);
	meshes["moon"].get_transform().translate(vec3(normal_meshes["earth"].get_transform().position.x + 10.0f, normal_meshes["earth"].get_transform().position.y + 10.0f, 0.0f));
	meshes["falcon"].get_transform().scale = vec3(0.00001f, 0.00001f, 0.00001f);
	meshes["falcon"].get_transform().translate(vec3(0.0f,50.0f,0.0f));
	meshes["god"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));

	material mat;
	// *********************************
	// Set materials
	// - all emissive is black
	// - all specular is white
	// - all shininess is 25
	mat.set_emissive(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	mat.set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_shininess(100.0f);

	//Sun to yellow
	mat.set_emissive(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	mat.set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	meshes["sun"].set_material(mat);
	//Earth to azure
	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	normal_meshes["earth"].set_material(mat); 

	//Moon to white
	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f)); 
	mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["moon"].set_material(mat); 

	mat.set_emissive(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	meshes["falcon"].set_material(mat);

	//mat.set_emissive(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["god"].set_material(mat);

	// Load texture
	tex["earth"] = texture("textures/4096_earth.jpg"); 
	tex["sun"] = texture("textures/sun.jpg");
	tex["moon"] = texture("textures/moon.jpg");
	tex["plane"] = texture("textures/check_1.png");
	tex["mercury"] = texture("textures/mercury.jpg");
	tex["falcon"] = texture("textures/falcon.jpg");
	tex["god"] = texture("textures/water.jpg");

	// Load brick_normalmap.jpg texture
	tex_normal_maps["earth"] = texture("textures/4096_normal.jpg");

	// Set lighting values, Position (-25, 10, -10)
	light.set_position(vec3(0.0f, 0.0f, 0.0f));
	// Light colour white
	light.set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	// Set range to 20
	light.set_range(1000.0f);

  // Load in shaders 
  eff.add_shader("shaders/simple_shader.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/simple_shader.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();
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

  // Set camera properties
  cam.set_position(vec3(0.0f, 10.0f, 0.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;

}


bool update(float delta_time) {

	//cout << 1 / delta_time << endl;

	static float range = 20.0f;

  //Move Camera
	if (glfwGetKey(renderer::get_window(), '1')) {
		cam.set_position(vec3(meshes["god"].get_transform().position.x, meshes["god"].get_transform().position.y, meshes["god"].get_transform().position.z));
	} 
	if (glfwGetKey(renderer::get_window(), '2')) {
		cam.set_position(vec3(-50, 10, 50));
	}
	if (glfwGetKey(renderer::get_window(), '3')) {
		cam.set_position(vec3(-50, 10, -50));
	}
	if (glfwGetKey(renderer::get_window(), '4')) {
		cam.set_position(vec3(50, 10, -50));
	}

	// O and P to change range
	if (glfwGetKey(renderer::get_window(), 'O')) {
		range = range*(1.0f + 2.0f*delta_time); 
	}
	if (glfwGetKey(renderer::get_window(), 'P')) {
		range = range*(1.0f - 2.0f*delta_time);
	}


	meshes["plane"].get_transform().position = vec3(0.0f,-50.0f,0.0f);

	//Move Around the Sun


	if (glfwGetKey(renderer::get_window(), 'L')) {
		button = true;
	}
	if (glfwGetKey(renderer::get_window(), 'K')) {
		button = false;
	}


	if (button) {
		normal_meshes["earth"].get_transform().position = (vec3(cos(velocity)*50.0f, 0.0f, sin(velocity)*50.0f) + meshes["sun"].get_transform().position);
		meshes["mercury"].get_transform().position = (vec3(cos(velocity*3.0f)*30.0f, 0.0f, sin(velocity*3.0f)*30.0f) + meshes["sun"].get_transform().position);
		meshes["moon"].get_transform().position = (vec3(cos(velocity*3.0f)*3.0f, 0.0f, sin(velocity*3.0f)*3.0f) + normal_meshes["earth"].get_transform().position);
		meshes["god"].get_transform().scale = vec3(0.01f, 0.01f, 0.01f);
	}
	else {
		normal_meshes["earth"].get_transform().position = vec3(normal_meshes["earth"].get_transform().position.x, normal_meshes["earth"].get_transform().position.y, normal_meshes["earth"].get_transform().position.z);
		meshes["mercury"].get_transform().position = vec3(meshes["mercury"].get_transform().position.x, meshes["mercury"].get_transform().position.y, meshes["mercury"].get_transform().position.z);
		meshes["moon"].get_transform().position = vec3(meshes["moon"].get_transform().position.x, meshes["moon"].get_transform().position.y, meshes["moon"].get_transform().position.z);
		meshes["god"].get_transform().scale = vec3(10.0f, 10.0f, 10.0f);
		meshes["god"].get_transform().position = vec3(normal_meshes["earth"].get_transform().position.x, normal_meshes["earth"].get_transform().position.y + 5.0f, normal_meshes["earth"].get_transform().position.z);

	}

	

	//Set Range
	light.set_range(range);

	//Rotate Planets

	meshes["sun"].get_transform().rotate(vec3(0.0f, 0.0f, -pi<float>()) * delta_time);
	normal_meshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, -half_pi<float>()) * delta_time);
	meshes["mercury"].get_transform().rotate(vec3(0.0f, 0.0f,-half_pi<float>()) * delta_time);

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
	cam.rotate(delta_x, -delta_y);
	vec3 translation(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(renderer::get_window(), 'W')) {
		translation.z += 3.0f ;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		translation.z -= 3.0f ;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		translation.x -= 3.0f ;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		translation.x += 3.0f ;
	}
	// Move camera
	cam.move(translation);

  // Update the camera
  cam.update(delta_time);
  cursor_x = current_x;
  cursor_y = current_y;

  // Set skybox position to camera position (camera in centre of skybox)
  skybox.get_transform().position = cam.get_position();

  velocity -= delta_time*(1.0f- exp(1.0))/(1.0f + exp(1.0));
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
	auto M = skybox.get_transform().get_transform_matrix();
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MVP = P * V * M;
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
		else {
			auto m = e.second;
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto V = cam.get_view();
			auto P = cam.get_projection();
			auto MVP = P * V * M;
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
			// Bind texture
			renderer::bind(tex[e.first], 0);
			// Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			// Set eye position - Get this from active camera
			glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
			// Render mesh
			renderer::render(m);
			// *********************************
		}
	}

}


void renderNormalMeshes() {

	for (auto &e : normal_meshes) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
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
		// Bind texture
		renderer::bind(tex[e.first], 0);
		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);
		// Bind normal_map
		renderer::bind(tex_normal_maps[e.first], 1);
		// Set normal_map uniform
		glUniform1i(eff.get_uniform_location("normal_map"), 1);
		// Set eye position - Get this from active camera
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
		// Render mesh
		renderer::render(m);
		// *********************************
	}

}

void renderSun() {
	auto m = meshes["sun"];
	// Bind effect
	renderer::bind(sun_eff);
	// Create MVP matrix
	auto M = m.get_transform().get_transform_matrix();
	auto V = cam.get_view();
	auto P = cam.get_projection();
	auto MVP = P * V * M;
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
	glUniform3fv(sun_eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
	// Render mesh
	renderer::render(m);
	// *********************************
}

bool render() {

	renderSkybox();

	renderMeshes();

	renderNormalMeshes();

	renderSun();


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