#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\projection.hpp>
#include <iostream>

using namespace std;
using namespace glm;

int main() {

	////Defining Vectors - 12.1.1
	vec2 a1(1.0f, 0.0f);
	vec2 a2(0.0f, 1.0f);
	vec3 b1(1.0f, 0.0f, 1.0f);
	vec3 b2(0.0f, 1.0f, 1.0f);
	vec4 c1(2.0f, 0.0f, 3.0f, 0.0f);
	vec4 c2(1.0f, 2.0f, 0.0f, 1.0f);

	//vec3 a13(vec2(1.0f, 0.0f), 0.0f);
	//vec4 a23(vec2(0.0f, 1.0f), 0.0f, 1.0f);

	//vec2 b12(vec3(1.0f, 0.0f, 1.0f));
	//vec4 b24(vec3(0.0f, 1.0f, 1.0f), 1.0f);

	//vec3 c13(vec4(2.0f, 0.0f, 3.0f, 0.0f));
	//vec2 c22(vec4(1.0f, 2.0f, 0.0f, 1.0f));


	////Vector Scaling - 12.1.3

	vec2 a3 = a1 * 2.0f;
	vec2 a4 = a2 / 2.0f;
	vec3 b3 = b1 * 2.0f;
	vec3 b4 = b2 / 2.0f;
	vec4 c3 = c1 * 2.0f;
	vec4 c4 = c2 / 2.0f;

	////Length - 12.1.4

	float l1 = length(a1);
	float l2 = length(b1);
	float l3 = length(c1);

	////Normalization - 12.1.5

	vec2 n1= normalize(a1);
	vec3 n2 = normalize(b1);
	vec3 n3= normalize(c1);

	////Dot Product - 12.1.6

	float ad = dot(a1, a2);
	float bd = dot(b1, b2);
	float cd = dot(c1, c2);

	//Projection:
	vec3 p = proj(b1, b2);

	////Cross Product - 12.1.7

	vec3 cr = cross(b1, b2);


	////MATRICES - 12.2.1

	mat4 a(1.0f);




}