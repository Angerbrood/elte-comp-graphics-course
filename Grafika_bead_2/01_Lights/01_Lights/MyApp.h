#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Mesh_OGL3.h"
#include "ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	// private functions
	GLuint GenTexture();
	glm::vec3 ToDescartes(float u, float v);

	// shader related variables
	GLuint m_programID; // shaderek program ID

	// transformation matrices
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// IDs of shader variables
	GLuint	m_loc_world;
	GLuint	m_loc_worldIT;
	GLuint	m_loc_mvp;
	GLuint	m_loc_texture;
	GLuint	m_loc_texture_2;
	GLuint	m_loc_t;
	GLuint  m_loc_eyePos;

	float	m_angleY;

	glm::vec3 m_eye = glm::vec3(5, 5, 5);
	float u = 0;
	float v = 3;
	glm::vec3 fw = ToDescartes(u, v);
	glm::vec3 at = m_eye + fw;
	glm::vec3 up = glm::vec3(0, 1, 0);
	bool is_left_pressed = false;

	// OpenGL resource identifiers
	GLuint m_vaoID; // vertex array object resource name
	GLuint m_vboID; // vertex buffer object resource name
	GLuint m_ibID;  // index buffer object resource name
	GLuint m_textureID; // textúra resource name
	GLuint m_textureID_2;

	// data to be stored at each vertex
	struct Vertex
	{
		glm::vec3 p;	// vertex position
		glm::vec3 n;	// vertex normal
		glm::vec2 t;	// texture coordinates
	};

	Mesh* m_suzanne;
};

