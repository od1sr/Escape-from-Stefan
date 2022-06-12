#include "BoundedPlane.h"
#include "config.h"
#include "logging.h"
#include "Model.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"

using sgl::BoundedPlane;

//indices of VBOs in VAO
static const int VERTICES_DATA_I = 0; 
static const int DIFFUSE_COLOR_I = 1;
static const int SPECULAR_COLOR_I = 2;
static const int PLANE_BASE_VERTICES_LENGTH = (
	COORDINATE_DIMENSIONS + TEXTURE_COORDINATE_DIMENSIONS + NORMAL_VECTOR_COORIDINATES_DIMENSIONS) * 4;
static const int length_of_color_array = sizeof(glm::vec4) * 4;

static const float square_vertices[] = {
	//coords			texture		normals		  
	-1.f, -1.f,  0.f,	0.f, 1.f,	0.f, 1.f, 0.f,
	 1.f, -1.f,  0.f,	1.f, 1.f,   0.f, 1.f, 0.f,
	 1.f,  1.f,  0.f,	1.f, 0.f,	0.f, 1.f, 0.f,
	-1.f,  1.f,  0.f,	0.f, 0.f,	0.f, 1.f, 0.f
};

static const uint square_indices[] = {
	0, 1, 2,
	2, 3, 0
};

sgl::BoundedPlaneSettings::BoundedPlaneSettings()
	: x(0.f), y(0.f), z(0.f), pitch(0.f), yaw(0.f), roll(0.f), mass(0.f),
	diffuse_texture{ 0, TextureType::UNINITIALIZED }, specular_texture{ 0, TextureType::UNINITIALIZED },
	diffuse_color(0.f), specular_color(0.f), shininess(0), width(2.f), length(2.f)
{
}

sgl::PlainObjectSettings& sgl::BoundedPlaneSettings::convertToPlainSettings() const
{
	static PlainObjectSettings st;
	st.x = x; st.y = y; st.z = z;
	st.pitch = pitch; st.yaw = yaw; st.roll = roll;
	st.mass = mass;
	st.collision_shape = new btBox2dShape(btVector3(width/2.f, length/2.f, 0.f));
	st.diffuse_texture = diffuse_texture;
	st.specular_texture = specular_texture; 
	st.diffuse_color = diffuse_color; st.specular_color = specular_color;
	st.shininess = shininess;
	return st;
}

sgl::BoundedPlane::BoundedPlane(const BoundedPlaneSettings& settings)
	:	PlainObject(settings.convertToPlainSettings())
{
	glm::vec2 scale{ settings.width / 2.f, settings.length / 2.f }; //2 is default side of sqare in square_vertices
	float *current_plain_vertices = new float[PLANE_BASE_VERTICES_LENGTH];
	memcpy(current_plain_vertices, square_vertices, PLANE_BASE_VERTICES_LENGTH * sizeof(float));
	for (int i = 0; i < PLANE_BASE_VERTICES_LENGTH; i += PLANE_BASE_VERTICES_LENGTH / 4)
	{
		current_plain_vertices[i] *= scale.x;
		current_plain_vertices[i+1] *= scale.y; 
	}
	vao.loadVertexBuffer((void*)current_plain_vertices, PLANE_BASE_VERTICES_LENGTH * sizeof(float), GL_DYNAMIC_DRAW);
	delete[] current_plain_vertices;
	int length_of_noncolor_vertex_data = COORDINATE_DIMENSIONS + NORMAL_VECTOR_COORIDINATES_DIMENSIONS + 
		TEXTURE_COORDINATE_DIMENSIONS;
	vao.setVertexAttribPointer(
		COORDINATES,
		COORDINATE_DIMENSIONS, 
		GL_FLOAT, 
		GL_FALSE,
		length_of_noncolor_vertex_data * sizeof(float), 
		NULL
	);
	vao.setVertexAttribPointer(
		TEX_COORD, 
		TEXTURE_COORDINATE_DIMENSIONS, 
		GL_FLOAT, 
		GL_FALSE,
		length_of_noncolor_vertex_data * sizeof(float), 
		(void*)(COORDINATE_DIMENSIONS * sizeof(float))
	);
	vao.setVertexAttribPointer(
		SURFACE_NORMAL, 
		NORMAL_VECTOR_COORIDINATES_DIMENSIONS, 
		GL_FLOAT, 
		GL_FALSE,
		length_of_noncolor_vertex_data * sizeof(float),
		(void*)((COORDINATE_DIMENSIONS + TEXTURE_COORDINATE_DIMENSIONS) * sizeof(float))
	);
	vao.loadElementsBuffer((void*)square_indices, sizeof(square_indices), GL_DYNAMIC_DRAW);
}

void sgl::BoundedPlane::setDiffuseColor(float r, float g, float b, float alpha)
{
	PlainObject::setDiffuseColor(r, g, b, alpha);
	float *new_diffuse_color_data = new float[length_of_color_array];
	int length_of_color_buffer = DIFFUSE_COLOR_DIMENSIONS * sizeof(float);
	for (int i = 0; i < 4; ++i)
		memcpy((glm::vec4*)new_diffuse_color_data + i, diffuse_color, sizeof(glm::vec4));
	vao.loadVertexBuffer(new_diffuse_color_data, length_of_color_buffer, GL_DYNAMIC_DRAW, DIFFUSE_COLOR_I);
	delete[] new_diffuse_color_data;
}

void sgl::BoundedPlane::setSpecularColor(float r, float g, float b, float alpha)
{
	PlainObject::setSpecularColor(r, g, b, alpha);
	float *new_specular_color_data = new float[length_of_color_array];
	int length_of_color_buffer = SPECULAR_COLOR_DIMENSIONS * sizeof(float);
	for (int i = 0; i <= 3; ++i)
		memcpy((glm::vec4*)new_specular_color_data + i, specular_color, sizeof(glm::vec4));
	vao.loadVertexBuffer(new_specular_color_data, length_of_color_buffer, GL_DYNAMIC_DRAW, SPECULAR_COLOR_I);
	delete[] new_specular_color_data;
}

sgl::BoundedPlane::~BoundedPlane()
{
}

void sgl::BoundedPlane::draw(Shader &shader) const
{
	PlainObject::draw(shader);
	GL_CALL(glDrawElements(GL_TRIANGLES, triangle_corners_per_square, GL_UNSIGNED_INT, 0));
}

float sgl::BoundedPlane::getWidth() const
{
	return ((btBox2dShape*)rigid_body->getCollisionShape())->getHalfExtentsWithoutMargin().x();
}

float sgl::BoundedPlane::getLength() const
{
	return ((btBox2dShape*)rigid_body->getCollisionShape())->getHalfExtentsWithoutMargin().y();
}