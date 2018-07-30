#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"

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
	// segédeljárások
	glm::vec3 GetUV(float u, float v);
	void TextureFromFileAttach(const char* filename, GLuint role) const;
	void createPlane();
	void drawPlane();

	void createSphere();
	void drawSphere(float x, float y, float z, float scale);
	void drawSpider(float x, float y, float z);

	void createCylinder();
	void drawCylinder();
	void createCylinderBottom();
	void drawCylinderBottom();
	void createCylinderTop();
	void drawCylinderTop();

	// shaderekhez szükséges változók
	ProgramObject		m_program;			// shaderek programja
	ProgramObject		m_programSkybox;	// skybox shaderek
	ProgramObject		m_programAxis;
	ProgramObject		m_programLight;

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// pozíciók tömbje

	VertexArrayObject	m_sphereVao;
	ArrayBuffer			m_spherePos;
	ArrayBuffer			m_sphereNorm;
	ArrayBuffer			m_sphereText;
	IndexBuffer			m_sphereInd;

	VertexArrayObject	m_planeVao;
	ArrayBuffer			m_planePos;
	ArrayBuffer			m_planeNorm;
	ArrayBuffer			m_planeText;
	IndexBuffer			m_planeInd;

	VertexArrayObject	m_cylinderVao;
	ArrayBuffer			m_cylinderPos;
	ArrayBuffer			m_cylinderNorm;
	ArrayBuffer			m_cylinderText;
	IndexBuffer			m_cylinderInd;

	VertexArrayObject	m_cylinderBottomVao;
	ArrayBuffer			m_cylinderBottomPos;
	ArrayBuffer			m_cylinderBottomNorm;
	ArrayBuffer			m_cylinderBottomText;
	IndexBuffer			m_cylinderBottomInd;

	VertexArrayObject	m_cylinderTopVao;
	ArrayBuffer			m_cylinderTopPos;
	ArrayBuffer			m_cylinderTopNorm;
	ArrayBuffer			m_cylinderTopText;
	IndexBuffer			m_cylinderTopInd;



	gCamera				m_camera;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;
	Texture2D			m_SphereTexture;
	Texture2D			m_SmileTexture;
	Texture2D			m_PlaneTexture;


	static const int N = 10, M = 10;
	static const int side_num = 50;
	const float f_PI = 3.14512f;
};

