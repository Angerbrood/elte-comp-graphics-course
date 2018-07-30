#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
	m_textureID = 0;
	m_textureID_2 = 0;

	m_angleY = 0;
}


CMyApp::~CMyApp(void)
{
}


GLuint CMyApp::GenTexture()
{
    unsigned char tex[256][256][3];
 
    for (int i=0; i<256; ++i)
        for (int j=0; j<256; ++j)
        {
			tex[i][j][0] = rand()%256;
			tex[i][j][1] = rand()%256;
			tex[i][j][2] = rand()%256;
        }
 
	GLuint tmpID;

	// generate a texture resource name
    glGenTextures(1, &tmpID);
	// activate it
    glBindTexture(GL_TEXTURE_2D, tmpID);
	// load data into GPU memory
	glTexImage2D(GL_TEXTURE_2D,		// melyik binding point-on van a textúra erõforrás, amihez tárolást rendelünk
		0,					// melyik részletességi szint adatait határozzuk meg
		GL_RGB,				// textúra belsõ tárolási formátuma (GPU-n)
		256, 256,			// szélesség, magasság
		0,					// nulla kell, hogy legyen ( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml )
		GL_RGB,				// forrás (=CPU-n) formátuma
		GL_UNSIGNED_BYTE,	// forrás egy pixelének egy csatornáját hogyan tároljuk
		tex);				// forráshoz pointer
	glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// bilinear filter on min
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// bilinear filter on max
	glBindTexture(GL_TEXTURE_2D, 0); // deactivate the texture

	return tmpID;
}

glm::vec3 CMyApp::ToDescartes(float u, float v)
{
	return glm::vec3(cosf(u) * sinf(v), cosf(v), sinf(u) * sinf(v));
}

bool CMyApp::Init()
{
	// clear color set to blue-ish
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);		// turn on back-face culling
	glEnable(GL_DEPTH_TEST);	// enable depth-test

	//
	// define the geometry
	//

	Vertex vert[] =
	{ 
		//           x,  y,  z             nx,ny,nz			 s, t
		{glm::vec3(-10,  0, -10), glm::vec3(0, 1, 0), glm::vec2(0, 0)},
		{glm::vec3( 10,  0, -10), glm::vec3(0, 1, 0), glm::vec2(1, 0)},
		{glm::vec3(-10,  0,  10), glm::vec3(0, 1, 0), glm::vec2(0, 1)},
		{glm::vec3( 10,  0,  10), glm::vec3(0, 1, 0), glm::vec2(1, 1)},
	};

	// create index buffer
    GLushort indices[]=
    {
        1,0,2,
        1,2,3,
    };

	// create a VAO
	glGenVertexArrays(1, &m_vaoID);
	// activate the new VAO m_vaoID
	glBindVertexArray(m_vaoID);
	
	// create a VBO
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // activate the VBO m_vboID
	// load the data stored in array vert into the VBO (essentially: upload the data to the GPU)
	glBufferData( GL_ARRAY_BUFFER,	// allocate memory for the active VBO and set its data
				  sizeof(vert),		// size of the VBO allocation, in bytes
				  vert,				// load data into the VBO from this location of the system memory
				  GL_STATIC_DRAW);	// we only want to store data into the VBO once (STATIC), and we want to use the VBO as a source for drawing our scene at each frame (DRAW)
									// for other usage flags see http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
									// {STREAM, STATIC, DYNAMIC} and {DRAW, READ, COPY}
	

	// activate the first general attribute 'channel' in the VAO
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(
		0,				// set the attributes of VAO channel 0
		3,				// this channel has 3 componenets
		GL_FLOAT,		// each of those componenets are floats
		GL_FALSE,		// do not normalize
		sizeof(Vertex),	// stride
		0				// channel 0`s data begins at the beginning of the VBO, no offset
	); 

	// activate 'channel' idx 1
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	// texture coordinates
	glEnableVertexAttribArray(2); 
	glVertexAttribPointer(
		2,
		2, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2*sizeof(glm::vec3)) );

	// create index buffer
	glGenBuffers(1, &m_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

	//
	// shader initialization
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// create the shader container (program)
	m_programID = glCreateProgram();

	// attach the vertex and fragment (pixel) shaders to the program
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// make correspondances between the VAO channels and the shader 'in' variables
	// IMPORTANT: do this prior to linking the programs!
	glBindAttribLocation(	m_programID,	// ID of the shader program from which we want to map a variable to a channel
							0,				// the VAO channel number we want to bind the variable to
							"vs_in_pos");	// the name of the variable in the shader
	glBindAttribLocation( m_programID, 1, "vs_in_normal");
	glBindAttribLocation( m_programID, 2, "vs_in_tex0");

	// link the shaders
	glLinkProgram(m_programID);

	// check the linking
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

	// we can dispose of the vertex and fragment shaders
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// other initializations
	//

	// set the projection matrix
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// query the IDs of the shader uniform variables
	m_loc_world = glGetUniformLocation( m_programID, "world");
	m_loc_worldIT = glGetUniformLocation( m_programID, "worldIT");
	m_loc_mvp  = glGetUniformLocation( m_programID, "MVP" );
	m_loc_texture = glGetUniformLocation(m_programID, "texture");
	m_loc_texture_2 = glGetUniformLocation(m_programID, "texture_2");
	m_loc_eyePos = glGetUniformLocation( m_programID, "eyePos" );
	m_loc_t = glGetUniformLocation(m_programID, "t");

	// generate the texture
	m_textureID = TextureFromFile("texture.png");
	m_textureID_2 = TextureFromFile("texture_2.jpg");


	m_suzanne = ObjParser::parse("suzanne.obj");
	m_suzanne->initBuffers();

	return true;
}

void CMyApp::Clean()
{
	delete m_suzanne;
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteTextures(1, &m_textureID);
	glDeleteTextures(1, &m_textureID_2);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// view transform
	//m_eye = glm::vec3(15 * cosf(SDL_GetTicks() / 1000.0f), 10, 15 * sinf(SDL_GetTicks() / 1000.0f));

	m_matView = glm::lookAt(m_eye,						// camera position
							at,		// look at position
							up);		// vector pointing upwards
}


void CMyApp::Render()
{

	// clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// switch on the shader
	glUseProgram( m_programID );

	glUniform3f(m_loc_eyePos, m_eye.x, m_eye.y, m_eye.z);

	// set shader uniforms

	glm::mat4 matWorld = glm::mat4(1.0f);
	glm::mat4 matWorldIT = glm::transpose( glm::inverse( matWorld ) );

	// send the matrices to the GPU
	glUniformMatrix4fv( m_loc_world, 1, GL_FALSE, &( matWorld[0][0]) ); 
	glUniformMatrix4fv( m_loc_worldIT, 1, GL_FALSE, &( matWorldIT[0][0]) ); 
	glm::mat4 mvp = m_matProj*m_matView*matWorld;
	glUniformMatrix4fv( m_loc_mvp,  1, GL_FALSE, &( mvp[0][0]) );


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glUniform1i(m_loc_texture, 0);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureID_2);
	glUniform1i(m_loc_texture_2, 1);

	// switch on the VAO
	glBindVertexArray(m_vaoID);

	// draw the indexed primitive
	glDrawElements(	GL_TRIANGLES,		// primitive type
					6,					// the number of indices (vertices) used in this draw command
					GL_UNSIGNED_SHORT,	// index type
					0);					// index address

	// VAO off
	glBindVertexArray(0);

	matWorld = glm::translate<float>(glm::vec3(0.0f, 1.0f, 0.0f));
	matWorldIT = glm::transpose(glm::inverse(matWorld));

	// send the matrices to the GPU
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(matWorld[0][0]));
	glUniformMatrix4fv(m_loc_worldIT, 1, GL_FALSE, &(matWorldIT[0][0]));
	mvp = m_matProj*m_matView*matWorld;
	glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &(mvp[0][0]));


	float t = sinf(2 * 3.14152f * SDL_GetTicks() / 2000.0f / 4.0f);
	t = (t + 1) / 2.0f;
	glUniform1f(m_loc_t, t);

	// shader off
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	glm::vec3 sd = glm::normalize(glm::cross(up, fw));
	
	switch (key.keysym.sym)
	{
	case SDLK_w:
		m_eye += fw;
		at += fw;
		break;
	case SDLK_s:
		m_eye -= fw;
		at -= fw;
		break;

	case SDLK_a:
		m_eye += sd;
		at += sd;
		break;
	case SDLK_d:
		m_eye -= sd;
		at -= sd;
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if (is_left_pressed)
	{
		u += mouse.xrel / 100.f;
		v += mouse.yrel / 100.f;
		v = glm::clamp(v, 0.1f, 3.0f);
		fw = ToDescartes(u, v);
		at = m_eye + fw;
	}
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
		is_left_pressed = true;
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
		is_left_pressed = false;
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
									100.0f);		// tavoli vagosik
}
