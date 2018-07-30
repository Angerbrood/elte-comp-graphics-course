#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
}


CMyApp::~CMyApp(void)
{
}
bool spinning = false;
int buttonCounter = 0;
bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok
	glPolygonMode(GL_BACK, GL_LINE);

	//
	// geometria letrehozasa
	//

	Vertex vert[] =
	{ 
		// 1. háromszög
		//          x,  y, z             R, G, B
		{ glm::vec3(-1, 0, -1), glm::vec3(0, 1, 0.5) },
		{ glm::vec3(1, 0, -1), glm::vec3(0, 1, 0.5) },
		{ glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0.5) },

		// 2. háromszög
		{ glm::vec3(-1, 0, 1), glm::vec3(1, 1, 0.2) },
		{ glm::vec3(1, 0, -1), glm::vec3(1, 1, 0.2) },
		{ glm::vec3(1, 0, 1), glm::vec3(1, 1, 0.2) },

		// piros oldallap
		{ glm::vec3(1, 2, 1), glm::vec3(1, 0, 0) },
		{ glm::vec3(-1, 0, 1), glm::vec3(1, 0, 0) },
		{ glm::vec3(1, 0, 1), glm::vec3(1, 0, 0) },

		// bibor oldallap
		{ glm::vec3(-1, 2, 1), glm::vec3(0.5, 0, 0) },
		{ glm::vec3(-1, 0, 1), glm::vec3(0.5, 0, 0) },
		{ glm::vec3(1, 2, 1), glm::vec3(0.5, 0, 0) },

		// zöld oldallap
		{ glm::vec3(1, 2, 1), glm::vec3(0, 1, 0) },
		{ glm::vec3(1, 0, 1), glm::vec3(0, 1, 0) },
		{ glm::vec3(1, 0, -1), glm::vec3(0, 1, 0) },

		// kék oldallap
		{ glm::vec3(-1, 2, -1), glm::vec3(0, 0, 1) },
		{ glm::vec3(1, 0, -1), glm::vec3(0, 0, 1) },
		{ glm::vec3(-1, 0, -1), glm::vec3(0, 0, 1) },

		// barna oldallap
		{ glm::vec3(1, 2, -1), glm::vec3(0.5, 0.3, 1) },
		{ glm::vec3(1, 0, -1), glm::vec3(0.5, 0.3, 1) },
		{ glm::vec3(-1, 2, -1), glm::vec3(0.5, 0.3, 1) },

		// fehér oldallap
		{ glm::vec3(-1, 2, -1), glm::vec3(1, 1, 1) },
		{ glm::vec3(-1, 0, -1), glm::vec3(1, 1, 1) },
		{ glm::vec3(-1, 0, 1), glm::vec3(1, 1, 1) },

		//pink oldallap
		{ glm::vec3(-1, 2, -1), glm::vec3(1, 0, 0.5) },
		{ glm::vec3(-1, 0, 1), glm::vec3(1, 0, 0.5) },
		{ glm::vec3(-1, 2, 1), glm::vec3(1, 0, 0.5) },

		//lila oldallap
		{ glm::vec3(1, 0, -1), glm::vec3(0.5, 0, 1) },
		{ glm::vec3(1, 2, -1), glm::vec3(0.5, 0, 1) },
		{ glm::vec3(1, 2, 1), glm::vec3(0.5, 0, 1) },

		// enyhe kék oldalap
		{ glm::vec3(-1, 2, 1), glm::vec3(0, 0.9, 1) },
		{ glm::vec3(1, 2, 1), glm::vec3(0, 0.9, 1) },
		{ glm::vec3(1, 2, -1), glm::vec3(0, 0.9, 1) },

		//libafos zöld
		{ glm::vec3(-1, 2, 1), glm::vec3(0.2, 0.5, 0) },
		{ glm::vec3(1, 2, -1), glm::vec3(0.2, 0.5, 0) },
		{ glm::vec3(-1, 2, -1), glm::vec3(0.2, 0.5, 0) },


	};

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  sizeof(vert),		// ennyi bájt nagyságban
				  vert,	// errõl a rendszermemóriabeli címrõl olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés elõtt kell ezt megtenni!
	glBindAttribLocation(	m_programID,	// shader azonosítója, amibõl egy változóhoz szeretnénk hozzárendelést csinálni
							0,				// a VAO-beli azonosító index
							"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( GL_FALSE == result )
	{
		std::vector<char> ProgramErrorMessage( infoLogLength );
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		
		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy( aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// egyéb inicializálás
	//

	// vetítési mátrix létrehozása
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_world = glGetUniformLocation( m_programID, "world");
	m_loc_view  = glGetUniformLocation( m_programID, "view" );
	m_loc_proj  = glGetUniformLocation( m_programID, "proj" );

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{

	float t = SDL_GetTicks() / 1000.f;

	// nézeti transzformáció beállítása
	//m_matView = glm::lookAt(glm::vec3(5 * cosf(t), 5, 5 * sinf(t)),		// honnan nézzük a színteret
	/*m_matView = glm::lookAt(glm::vec3(15 * cosf(t), 10, 10 * sinf(t)),		// honnan nézzük a színteret
		glm::vec3(0, 0, 0),		// a színtér melyik pontját nézzük
		glm::vec3(0, 1, 0));		// felfelé mutató irány a világban*/

	
	m_matView = glm::lookAt(glm::vec3(30, -15, 10),		// honnan nézzük a színteret
		glm::vec3(0, 0, 0),		// a színtér melyik pontját nézzük
		glm::vec3(0, 1, 0));		// felfelé mutató irány a világban
		
}
void CMyApp::Render()
{


	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	glUseProgram( m_programID );

	// shader parameterek beállítása
	
	
	glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);
	

	float t = SDL_GetTicks() / 10000.0f * 3.14f * 2.0f;
	float spinningDegree = SDL_GetTicks() / 1000.0f * 3.14f;





	//2. feladat
	/*

	//oszlopok kirajzolásaű
	float x = -2;
	float y = -2;
	float z = calcZCoord(x, y);
	m_matWorld = getShiftingTransformation(x, y, z) * getCircleSpinningTransformation(7, t) *  getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	x = 2;
	y = -2;
	z = calcZCoord(x, y);
	m_matWorld = getShiftingTransformation(x, y, z) * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world,1,GL_FALSE,&(m_matWorld[0][0])); 
	glDrawArrays(GL_TRIANGLES,	0, 36);				
	
	x = 2;
	y = 2;
	z = calcZCoord(x, y);
	m_matWorld = getShiftingTransformation(x, y, z) * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world,1,GL_FALSE,&(m_matWorld[0][0])); 
	glDrawArrays(GL_TRIANGLES,	0,	36);				
	

	//2 szélső kocka
	x = -2;
	y = 2;
	z = calcZCoord(x, y);
	m_matWorld = getShiftingTransformation(x, y, z) * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world,1,GL_FALSE,&(m_matWorld[0][0])); 
	glDrawArrays(GL_TRIANGLES,	0, 36);				

	x = 0;
	y = 0;
	z = calcZCoord(x, y);
	m_matWorld = getShiftingTransformation(x, y, z) * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world,1,GL_FALSE,	&(m_matWorld[0][0])); 
	glDrawArrays(GL_TRIANGLES,0,36);
	*/
	
	//T alak
	/*
	for (int i = 0; i <= 4; i = i + 2) {
		m_matWorld = getShiftingTransformation(0, i, 0) * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
		glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, 36);


	}


	m_matWorld = getShiftingTransformation(2, 4, 0) * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_matWorld = getShiftingTransformation(-2, 4, 0) * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	*/
	
	//teljes feladat
	
	//z(-2,-2)
	float x = -2;
	float y = -2;
	float z = calcZCoord(x, y);
	glm::mat4 temp = glm::translate(glm::vec3(x, y, z));
	for (int i = 0; i <= 4; i = i + 2) {
		m_matWorld = getShiftingTransformation(0, i, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
		glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, 36);


	}

	m_matWorld = getShiftingTransformation(2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_matWorld = getShiftingTransformation(-2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//z(2, -2)
	x = 2;
	y = -2;
	z = calcZCoord(x, y);
	temp = glm::translate(glm::vec3(x, y, z));
	for (int i = 0; i <= 4; i = i + 2) {
		m_matWorld = getShiftingTransformation(0, i, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
		glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, 36);


	}

	m_matWorld = getShiftingTransformation(2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_matWorld = getShiftingTransformation(-2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	//z(2,2)
	x = 2;
	y = 2;
	z = calcZCoord(x, y);
	temp = glm::translate(glm::vec3(x, y, z));
	for (int i = 0; i <= 4; i = i + 2) {
		m_matWorld = getShiftingTransformation(0, i, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
		glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, 36);


	}

	m_matWorld = getShiftingTransformation(2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_matWorld = getShiftingTransformation(-2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	//z(-2,2)
	x = -2;
	y = 2;
	z = calcZCoord(x, y);
	temp = glm::translate(glm::vec3(x, y, z));
	for (int i = 0; i <= 4; i = i + 2) {
		m_matWorld = getShiftingTransformation(0, i, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
		glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, 36);


	}

	m_matWorld = getShiftingTransformation(2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_matWorld = getShiftingTransformation(-2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	//z(0,0)
	x = 0;
	y = 0;
	z = calcZCoord(x, y);
	temp = glm::translate(glm::vec3(x, y, z));
	for (int i = 0; i <= 4; i = i + 2) {
		m_matWorld = getShiftingTransformation(0, i, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
		glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, 36);


	}

	m_matWorld = getShiftingTransformation(2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_matWorld = getShiftingTransformation(-2, 4, 0) * temp * getCircleSpinningTransformation(7, t) * getVerticalSpinningTranformation(spinningDegree);
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	
	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}
void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
}
float CMyApp::calcZCoord(float x, float y) {
	float result = (x * x * x) - (x * y) + (y * y);
	return result;
}
glm::mat4 CMyApp::getShiftingTransformation(float x, float y, float z) {
	return glm::translate(glm::vec3(x, y, z));
}
glm::mat4 CMyApp::getVerticalSpinningTranformation(float degree) {
	glm::mat4 result;
	if (spinning) {
		result = glm::rotate<float>(degree, glm::vec3(0, 1, 0));
	}
	else {
		result = glm::rotate<float>(0.0f, glm::vec3(0, 1, 0));
	}
	return result;
}
glm::mat4 CMyApp::getCircleSpinningTransformation(float radius, float ticks) {
	int N = 5;
	glm::mat4 result = glm::rotate<float>(3.14f * 2.0f  + ticks, glm::vec3(0, 1, 0)) *
		glm::translate(glm::vec3(radius, 0, 0));
	return result;
}
void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	++buttonCounter;
	if ((buttonCounter % 2) == 0) {
		spinning = true;
	} else {
		spinning = false;
	}
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

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
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									1000.0f);		// tavoli vagosik
}