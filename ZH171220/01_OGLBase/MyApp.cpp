#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}


CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}
glm::vec3 CMyApp::GetUV(float u, float v)
{
	// origó középpontú, egységsugarú gömb parametrikus alakja: http://hu.wikipedia.org/wiki/G%C3%B6mb#Egyenletek 
	// figyeljünk:	matematikában sokszor a Z tengely mutat felfelé, de nálunk az Y, tehát a legtöbb képlethez képest nálunk
	//				az Y és Z koordináták felcserélve szerepelnek
	u *= 2 * 3.1415f;
	v *= 3.1415f;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 2;

	return glm::vec3(r*cu*sv, r*cv, r*su*sv);
}
void CMyApp::createSphere() {
	glm::vec3 vert[(N + 1)*(M + 1)];
	glm::vec3 norm[(N + 1)*(M + 1)];
	glm::vec2 text[(N + 1)*(M + 1)];

	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= M; ++j) {
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j*(N + 1)] = GetUV(u, v);
			norm[i + j*(N + 1)] = GetUV(u, v);
			text[i + j*(N + 1)] = glm::vec2(u, v);
		}
	}

	GLushort indices[3 * 2 * (N)*(M)];
	for (int i = 0; i<N; ++i)
		for (int j = 0; j<M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j)*	(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j)*	(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1)*(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j)*	(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1)*(N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1)*(N + 1);
		}

	m_spherePos = vert;
	m_sphereNorm = norm;
	m_sphereText = text;

	m_sphereInd = indices;

	m_sphereVao.Init(
	{
		{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_spherePos },
		{ CreateAttribute<1, glm::vec3, 0, sizeof(glm::vec3)>, m_sphereNorm },
		{ CreateAttribute<2, glm::vec2, 0, sizeof(glm::vec2)>, m_sphereText },
	},
	m_sphereInd
	);


}
void CMyApp::drawSphere(float x, float y, float z, float scale) {
	m_sphereVao.Bind();

	glm::mat4 world = glm::translate(glm::vec3(x, y, z)) * glm::scale(glm::vec3(1, 1, 1) * scale);

	m_programLight.SetUniform("MVP", m_camera.GetViewProj() * world);
	m_programLight.SetUniform("world", world);
	m_programLight.SetUniform("worldIT", glm::inverse(glm::transpose(world)));
	m_programLight.SetTexture("texImage", 0, m_SphereTexture);
	glDrawElements(GL_TRIANGLES, 3 * 2 * (N)*(M), GL_UNSIGNED_SHORT, 0);


}
void CMyApp::drawSpider(float x, float y, float z) {
	m_sphereVao.Bind();

	glm::mat4 world = glm::translate(glm::vec3(x, y, z)) * glm::scale(glm::vec3(2.2, -0.5, 1));

	m_programLight.SetUniform("MVP", m_camera.GetViewProj() * world);
	m_programLight.SetUniform("world", world);
	m_programLight.SetUniform("worldIT", glm::inverse(glm::transpose(world)));
	m_programLight.SetTexture("texImage", 0, m_SphereTexture);
	m_programLight.SetTexture("texImage", 0, m_SmileTexture);
	glDrawElements(GL_TRIANGLES, 3 * 2 * (N)*(M), GL_UNSIGNED_SHORT, 0);
}
void CMyApp::createPlane() {
	glm::vec3 vert[4];
	glm::vec3 norm[4];
	glm::vec2 text[4];

	vert[0] = glm::vec3(-10, 0, -10);
	vert[1] = glm::vec3(10, 0, -10);
	vert[2] = glm::vec3(-10, 0, 10);
	vert[3] = glm::vec3(10, 0, 10);

	norm[0] = glm::vec3(0, 1, 0);
	norm[1] = glm::vec3(0, 1, 0);
	norm[2] = glm::vec3(0, 1, 0);
	norm[3] = glm::vec3(0, 1, 0);

	text[0] = glm::vec2(0, 0);
	text[1] = glm::vec2(1, 0);
	text[2] = glm::vec2(0, 1);
	text[3] = glm::vec2(1, 1);

	GLushort indices[] =
	{
		1,0,2,
		1,2,3,
	};

	m_planePos = vert;	
	m_planeNorm = norm;
	m_planeText = text;

	m_planeInd = indices;

	m_planeVao.Init(
	{
		{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_planePos },
		{ CreateAttribute<1, glm::vec3, 0, sizeof(glm::vec3)>, m_planeNorm },
		{ CreateAttribute<2, glm::vec2, 0, sizeof(glm::vec2)>, m_planeText },
	},
	m_planeInd
	);


}
void CMyApp::drawPlane() {
	m_planeVao.Bind();

	glm::mat4 world = glm::translate(glm::vec3(1, 0, 1));

	m_programLight.SetUniform("MVP", m_camera.GetViewProj() * world);
	m_programLight.SetUniform("world", world);
	m_programLight.SetUniform("worldIT", glm::inverse(glm::transpose(world)));
	m_programLight.SetTexture("texImage", 0, m_PlaneTexture);

	glDrawElements(GL_TRIANGLES, 6,	GL_UNSIGNED_SHORT,0);					

}
void CMyApp::createCylinder() {
	glm::vec3 vert[2 * side_num];
	glm::vec3 norm[2 * side_num];
	glm::vec2 text[2 * side_num];
	GLushort indices[2 * side_num];

	for (int i = 0; i < side_num; ++i) {
		float x = cosf(i * 2 * f_PI / side_num);
		float z = sinf(i * 2 * f_PI / side_num);
		vert[i] = glm::vec3(x, -1, z);
		norm[i] = glm::vec3(x, 0, z);
		text[i] = glm::vec2((float)i / side_num, 0);
		indices[i] = 2 * i;
	}
	for (int i = side_num; i < side_num * 2; ++i) {
		float x = cosf(i * 2 * f_PI / side_num *2 );
		float z = sinf(i * 2 * f_PI / side_num * 2);
		vert[i] = glm::vec3(x, 1, z);
		norm[i] = glm::vec3(x, 0, z);
		text[i] = glm::vec2((float)i / side_num, 1);
		indices[i] = 2 * i + 1;
	}


	m_cylinderPos = vert;
	m_cylinderNorm = norm;
	m_cylinderText = text;

	m_cylinderInd = indices;

	m_cylinderVao.Init(
	{
		{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_cylinderPos },
		{ CreateAttribute<1, glm::vec3, 0, sizeof(glm::vec3)>, m_cylinderNorm },
		{ CreateAttribute<2, glm::vec2, 0, sizeof(glm::vec2)>, m_cylinderText },
	},
	m_cylinderInd
	);

}
void CMyApp::drawCylinder() {
	m_cylinderVao.Bind();

	glm::mat4 world = glm::translate(glm::vec3(1, 8, 4));

	m_programLight.SetUniform("MVP", m_camera.GetViewProj() * world);
	m_programLight.SetUniform("world", world);
	m_programLight.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

	glDrawElements(GL_TRIANGLE_STRIP, 2 * side_num , GL_UNSIGNED_SHORT, 0);
}
void CMyApp::drawCylinderBottom() {
	m_cylinderBottomVao.Bind();

	glm::mat4 world = glm::translate(glm::vec3(1, 7, 4));

	m_programLight.SetUniform("MVP", m_camera.GetViewProj() * world);
	m_programLight.SetUniform("world", world);
	m_programLight.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

	glDrawElements(GL_TRIANGLE_FAN, side_num + 2, GL_UNSIGNED_SHORT, 0);
}
void CMyApp::drawCylinderTop() {
	m_cylinderTopVao.Bind();

	glm::mat4 world = glm::translate(glm::vec3(1, 10, 4));

	m_programLight.SetUniform("MVP", m_camera.GetViewProj() * world);
	m_programLight.SetUniform("world", world);
	m_programLight.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

	glDrawElements(GL_TRIANGLE_FAN, side_num + 2, GL_UNSIGNED_SHORT, 0);
}
void CMyApp::createCylinderBottom() {
	glm::vec3 vert[side_num];
	glm::vec3 norm[side_num];
	glm::vec2 text[side_num];


	for (int i = 0; i < side_num; ++i) {
		float x = cosf(i * 2 * f_PI / side_num);
		float z = sinf(i * 2 * f_PI / side_num);

		vert[i] = glm::vec3(x, -0.5, z);
		norm[i] = glm::vec3(0, -1, 0);
		text[i] = glm::vec2(0.5*x + 0.5, 0.5*z + 0.5);
	}
	GLushort indices[side_num];
	for (int i = 0; i < side_num; ++i) {
		indices[i] = i + 1;
	}

	m_cylinderBottomPos = vert;
	m_cylinderBottomNorm = norm;
	m_cylinderBottomText = text;

	m_cylinderBottomInd = indices;

	m_cylinderBottomVao.Init(
	{
		{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_cylinderBottomPos },
		{ CreateAttribute<1, glm::vec3, 0, sizeof(glm::vec3)>, m_cylinderBottomNorm },
		{ CreateAttribute<2, glm::vec2, 0, sizeof(glm::vec2)>, m_cylinderBottomText },
	},
	m_cylinderBottomInd
	);
}
void CMyApp::createCylinderTop() {
	glm::vec3 vert[side_num];
	glm::vec3 norm[side_num];
	glm::vec2 text[side_num];

	for (int i = 0; i < side_num; ++i) {
		float x = cosf(i * 2 * f_PI / side_num);
		float z = sinf(i * 2 * f_PI / side_num);

		vert[i] = glm::vec3(x, -0.5, z);
		norm[i] = glm::vec3(0, -1, 0);
		text[i] = glm::vec2(0.5*x + 0.5, 0.5*z + 0.5);
	}
	GLushort indices[side_num];
	for (int i = 0; i < side_num; ++i) {
		indices[i] = i + 1;
	}

	m_cylinderTopPos = vert;
	m_cylinderTopNorm = norm;
	m_cylinderTopText = text;

	m_cylinderTopInd = indices;

	m_cylinderTopVao.Init(
	{
		{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_cylinderTopPos },
		{ CreateAttribute<1, glm::vec3, 0, sizeof(glm::vec3)>, m_cylinderTopNorm },
		{ CreateAttribute<2, glm::vec2, 0, sizeof(glm::vec2)>, m_cylinderTopText },
	},
	m_cylinderTopInd
	);
}
bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	//
	// shaderek betöltése
	//

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_col" }				// VAO 1-es csatorna menjen a vs_in_col-ba
	});

	m_program.LinkProgram();

	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	m_programSkybox.Init(
	{
		{ GL_VERTEX_SHADER, "skybox.vert" },
		{ GL_FRAGMENT_SHADER, "skybox.frag" }
	},
	{
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
	}
	);
	m_programAxis.Init(
	{
		{ GL_VERTEX_SHADER, "axes.vert" },
		{ GL_FRAGMENT_SHADER, "axes.frag" }
	}
	);
	m_programLight.Init(
	{
		{ GL_VERTEX_SHADER, "light.vert" },
		{ GL_FRAGMENT_SHADER, "light.frag" }
	},
	{
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_normal" },
		{ 2, "vs_in_tex0" },
	}
	);


	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe (m_buffer*) való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_gpuBufferPos konstruktora már létrehozott egy GPU puffer azonosítót és a most következõ BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_gpuBufferPos típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/
	m_gpuBufferPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1),
			// elülsõ lap
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(-1, 1, 1),

	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_gpuBufferIndices.BufferData(
		std::vector<int>{
		// hátsó lap
		0, 1, 2,
			2, 3, 0,
			// elülsõ lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felsõ
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_vao.Init(
	{
		// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_gpuBufferPos GPU pufferben vannak
		{ CreateAttribute<		0,						// csatorna: 0
								glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
								0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
								sizeof(glm::vec3)		// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
							>, m_gpuBufferPos },		
	},
	m_gpuBufferIndices
	);

	// skybox
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	m_SphereTexture.AttachFromFile("pottyos.png");
	m_PlaneTexture.AttachFromFile("plane.png");
	m_SmileTexture.AttachFromFile("export.png");

	createSphere();
	createPlane();
	createCylinder();
	createCylinderBottom();
	createCylinderTop();
	return true;
}

void CMyApp::TextureFromFileAttach(const char* filename, GLuint role) const
{
	SDL_Surface* loaded_img = IMG_Load(filename);

	int img_mode = 0;

	if (loaded_img == 0)
	{
		std::cout << "[TextureFromFile] Hiba a kép betöltése közben: " << filename << std::endl;
		return;
	}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	if (loaded_img->format->BytesPerPixel == 4)
		img_mode = GL_BGRA;
	else
		img_mode = GL_BGR;
#else
	if (loaded_img->format->BytesPerPixel == 4)
		img_mode = GL_RGBA;
	else
		img_mode = GL_RGB;
#endif

	glTexImage2D(role, 0, GL_RGBA, loaded_img->w, loaded_img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, loaded_img->pixels);

	SDL_FreeSurface(loaded_img);
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_programAxis.Use();
	m_programAxis.SetUniform("mvp", m_camera.GetViewProj());
	glDrawArrays(GL_LINES, 0, 6);


	m_programLight.Use();
	m_programLight.SetUniform("eye_pos", m_camera.GetEye());

	drawSphere(-1, 1, 0, 1);
	drawSphere(1, 1, 0, 0.5);
	drawSphere(0, 3, 0, 0.25);
	drawSpider(6, 1, 0);

	drawPlane();
	drawCylinderBottom();
	drawCylinderTop();
	drawCylinder();

	m_vao.Bind();

	m_program.Use();


	// skybox

	// mentsük el az elõzõ Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_vao.Bind();

	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", m_camera.GetViewProj() * glm::translate( m_camera.GetEye()) );
	
	// cube map textúra beállítása 0-ás mintavételezõre és annak a shaderre beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);

	// az elõzõ három sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);

	//ImGui Testwindow
	ImGui::ShowTestWindow();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}