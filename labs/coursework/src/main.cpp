#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
geometry geom;
effect eff;
free_camera cam;
double cursor_x;
double cursor_y;
map<string, texture> tex;
point_light light;	
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
  // Create Sphere
	meshes["plane"] = mesh(geometry_builder::create_plane());
	meshes["earth"] = mesh(geometry_builder::create_sphere(20, 20));                                     //mesh(geometry("models/earth.obj"));
	meshes["mercury"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["sun"] = mesh(geometry_builder::create_sphere(20,20));
	meshes["moon"] = mesh(geometry_builder::create_sphere(20, 20));

  //Transform Objects
	meshes["plane"].get_transform().scale = vec3(10.0f, 10.0f, 10.0f);
	meshes["sun"].get_transform().scale = vec3(10.0f, 10.0f, 10.0f);
	meshes["sun"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	meshes["sun"].get_transform().translate(vec3(0.0f, 0.0f,0.0f));
	meshes["mercury"].get_transform().scale = vec3(0.23f, 0.23f, 0.23f);
	meshes["mercury"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
    meshes["earth"].get_transform().scale = vec3(0.5f, 0.5f, 0.5f);
    meshes["earth"].get_transform().translate(vec3(meshes["sun"].get_transform().position.x + 30.0f, meshes["sun"].get_transform().position.y + 30.0f, 0.0f));
	meshes["moon"].get_transform().scale = vec3(0.1, 0.1, 0.1);
	meshes["moon"].get_transform().translate(vec3(meshes["earth"].get_transform().position.x + 10.0f, meshes["earth"].get_transform().position.y + 10.0f, 0.0f));

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
	mat.set_emissive(vec4(0.0f, 0.5f, 1.0f, 1.0f));
	mat.set_diffuse(vec4(0.0f, 0.5f, 1.0f, 1.0f));
	meshes["earth"].set_material(mat);

	//Moon to white
	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["earth"].set_material(mat);

	// Load texture
	tex["earth"] = texture("textures/4096_earth.jpg"); 

	// Set lighting values, Position (-25, 10, -10)
	light.set_position(vec3(0.0f, 100.0f,0.0f));
	// Light colour white
	light.set_light_colour(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	// Set range to 20
	light.set_range(100.0f);

  // Load in shaders
  eff.add_shader("shaders/simple_shader.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/simple_shader.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 10.0f, 0.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;

}


bool update(float delta_time) {

	static float range = 20.0f;

  //Move Camera
	if (glfwGetKey(renderer::get_window(), '1')) {
		cam.set_position(vec3(50, 10, 50));
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
		range += 100.f*delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'P')) {
		range -= 100.f*delta_time;
	}


	meshes["plane"].get_transform().position = vec3(0.0f,-20.0f,0.0f);

	//Move Around the Sun


	if (glfwGetKey(renderer::get_window(), 'L')) {
		button = true;
	}
	if (glfwGetKey(renderer::get_window(), 'K')) {
		button = false;
	}


	if (button) {
		meshes["earth"].get_transform().position = (vec3(cos(velocity)*50.0f, 0.0f, sin(velocity)*50.0f) + meshes["sun"].get_transform().position);
		meshes["mercury"].get_transform().position = (vec3(cos(velocity*3.0f)*15.0f, 0.0f, sin(velocity*3.0f)*15.0f) + meshes["sun"].get_transform().position);
	}
	else {
		meshes["earth"].get_transform().position = vec3(meshes["earth"].get_transform().position.x, meshes["earth"].get_transform().position.y, meshes["earth"].get_transform().position.z);
		meshes["mercury"].get_transform().position = vec3(meshes["mercury"].get_transform().position.x, meshes["mercury"].get_transform().position.y, meshes["mercury"].get_transform().position.z);
	}


	//Move moon around the earth
	meshes["moon"].get_transform().position = (vec3(cos(velocity*2.0f)*2.0f, 0.0f, sin(velocity*2.0f)*2.0f) + meshes["earth"].get_transform().position);

	//Set Range
	light.set_range(range);

	//Rotate Planets

	meshes["sun"].get_transform().rotate(vec3(0.0f, 0.0f, pi<float>()) * delta_time);
	meshes["earth"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()) * delta_time);
	meshes["mercury"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()/*58.7f*/) * delta_time);

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

  velocity -= delta_time;
  return true;


}

bool render() {
	// Render meshes
	for (auto &e : meshes) {
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
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(mat3(m.get_transform().get_normal_matrix())));
		// Bind material
		renderer::bind(m.get_material(), "mat");
		// Bind light
		renderer::bind(light, "point");
		// Bind texture
		renderer::bind(tex["earth"], 0);
		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);
		// Set eye position- Get this from active camera
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
		// Render mesh
		renderer::render(m);
		// *********************************
	}

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