// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include <chrono>
#include <thread>
#include <unistd.h>

// Include GLEW
#include <GL/glew.h>
 
// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;
 
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;
 
// Include AntTweakBar
#include <AntTweakBar.h>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/quaternion_utils.hpp> // See quaternion_utils.cpp for RotationBetweenVectors, LookAt and RotateTowards
 
vec3 gPosition1(-1.5f, 0.0f, 0.0f); //Obejeto Posicao
vec3 gPositionTelaInicial(0.0f, 0.0f, 0.0f);
vec3 gOrientation1(0.0f, 0.0f, 0.0f);
 
vec3 cameraOrientation(-3.15f, 2.0f, 25.49f); //Posição da camera
vec3 lookOrientation(0.77f, 0.74f, -2.17f);
vec3 upOrientation(0.0f, 1.0f, 0.0f);

vec3 LightPosition(0.0f, 12.0f, 3.19f);
vec3 LightPositionVerde(-2.0f, 2.60f, -0.60f);
vec3 LightPositionVermelho(-0.85f, 2.60f, -0.60f);
vec3 LightPositionAzul(-1.0f, 2.60f, 0.60f);
vec3 LightPositionAmarelo(-2.0f, 2.60f, 0.70f);
vec3 LightPositionTelaInicio(0.70f, -0.79f, 10.0f);

float anguloProjection = 45.0f;
float widthProjection = 4.0f;
float heightProjection = 3.0f;
float farProjection = 0.1f;
float nearProjection = 100.0f;
vec3 gPosition2( 1.5f, 0.0f, 0.0f); //Obejeto2 Posi��o
quat gOrientation2;
int sequencia[5][6] = {
  {2, 6},
  {4, 6, 4},
  {6, 2, 4, 6},
  {8, 6, 4, 6, 2},
  {8, 2, 4, 8, 2, 8}
};
int sequenciaJogador[] = {};
int arrayVazio[] = {};
int pontuacao = 0;
int indexLinha = 0;
int indexColuna = 0;
int indexJogador = 0;
int indexAzul = 0;
int indexAmarelo = 0;
int indexVermelho = 0;
int indexVerde = 0;
int contadormaroto = 0;
int indexAvancado = 0;
bool gLookAtOther = true;
bool apertou = false;
float angleRad = 0.0;

GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint programID;
GLuint ModelMatrixID;
GLuint ViewMatrixID;
GLuint vertexPosition_modelspaceID;
GLuint vertexUVID;
GLuint vertexNormal_modelspaceID;
GLuint elementbuffer;
float LightPowerGeral = 60.0;
float LightPowerVerde = 0.0;
float LightPowerVermelho = 0.0;
float LightPowerAzul = 0.0;
float LightPowerAmarelo = 0.0;
float LightPowerTelaInicio = 49.0;

int jogoPausado = 1;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

void resetGame() {
  indexColuna = 0;
  printf("JOGO RESETADO\n");
}

void callSleep(int timer) {
  sleep(timer);
}

void addTweakBar() {
  // Initialize the GUI
  TwInit(TW_OPENGL_CORE, NULL);
  TwWindowSize(1024, 768);
  TwBar * ViewGUI = TwNewBar("Luz e Camera");
  TwSetParam(ViewGUI, NULL, "position", TW_PARAM_CSTRING, 1, "16 408");
  TwSetParam(ViewGUI, NULL, "position", TW_PARAM_CSTRING, 1, "808 408");

  //View
  // TwAddVarRW(ViewGUI, "Camera X", TW_TYPE_FLOAT, &cameraOrientation.x, "step=0.01");
  // TwAddVarRW(ViewGUI, "Camera Y", TW_TYPE_FLOAT, &cameraOrientation.y, "step=0.01");
  // TwAddVarRW(ViewGUI, "Camera Z", TW_TYPE_FLOAT, &cameraOrientation.z, "step=0.01");
  // TwAddVarRW(ViewGUI, "Look X", TW_TYPE_FLOAT, &lookOrientation.x, "step=0.01");
  // TwAddVarRW(ViewGUI, "Look Y", TW_TYPE_FLOAT, &lookOrientation.y, "step=0.01");
  // TwAddVarRW(ViewGUI, "Look Z", TW_TYPE_FLOAT, &lookOrientation.z, "step=0.01");
  // TwAddVarRW(ViewGUI, "UP X", TW_TYPE_FLOAT, &upOrientation.x, "step=0.01");
  // TwAddVarRW(ViewGUI, "UP Y", TW_TYPE_FLOAT, &upOrientation.y, "step=0.01");
  // TwAddVarRW(ViewGUI, "UP Z", TW_TYPE_FLOAT, &upOrientation.z, "step=0.01");

  //Light
  TwAddVarRW(ViewGUI, "Luz Geral", TW_TYPE_FLOAT, &LightPowerGeral, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Amarelo", TW_TYPE_FLOAT, &LightPowerAmarelo, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Verde", TW_TYPE_FLOAT, &LightPowerVerde, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Vermelha", TW_TYPE_FLOAT, &LightPowerVermelho, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Azul", TW_TYPE_FLOAT, &LightPowerAzul, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Tela Inicio", TW_TYPE_FLOAT, &LightPowerTelaInicio, "step=0.01");

  TwAddVarRW(ViewGUI, "Luz Geral X", TW_TYPE_FLOAT, &LightPosition.x, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Geral Y", TW_TYPE_FLOAT, &LightPosition.y, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Geral Z", TW_TYPE_FLOAT, &LightPosition.z, "step=0.01");

  TwAddVarRW(ViewGUI, "Luz Verde X", TW_TYPE_FLOAT, &LightPositionVerde.x, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Verde Y", TW_TYPE_FLOAT, &LightPositionVerde.y, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Verde Z", TW_TYPE_FLOAT, &LightPositionVerde.z, "step=0.01");

  TwAddVarRW(ViewGUI, "Luz Vermelha X", TW_TYPE_FLOAT, &LightPositionVermelho.x, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Vermelha Y", TW_TYPE_FLOAT, &LightPositionVermelho.y, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Vermelha Z", TW_TYPE_FLOAT, &LightPositionVermelho.z, "step=0.01");

  TwAddVarRW(ViewGUI, "Luz Azul X", TW_TYPE_FLOAT, &LightPositionAzul.x, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Azul Y", TW_TYPE_FLOAT, &LightPositionAzul.y, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Azul Z", TW_TYPE_FLOAT, &LightPositionAzul.z, "step=0.01");

  TwAddVarRW(ViewGUI, "Luz Amarelo X", TW_TYPE_FLOAT, &LightPositionAmarelo.x, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Amarelo Y", TW_TYPE_FLOAT, &LightPositionAmarelo.y, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Amarelo Z", TW_TYPE_FLOAT, &LightPositionAmarelo.z, "step=0.01");

  TwAddVarRW(ViewGUI, "Luz Tela Inicio X", TW_TYPE_FLOAT, &LightPositionTelaInicio.x, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Tela Inicio Y", TW_TYPE_FLOAT, &LightPositionTelaInicio.y, "step=0.01");
  TwAddVarRW(ViewGUI, "Luz Tela Inicio Z", TW_TYPE_FLOAT, &LightPositionTelaInicio.z, "step=0.01");
}

void validaAcerto() {
  for(int i = 0; i <= sizeof(sequenciaJogador); i++) {
    for(int j = 0; j <= sizeof(sequencia[indexLinha][0]); j++) {
      printf("SSASDAASD %d\n", sequencia[indexLinha][j]);
      if (sequenciaJogador[i] == sequencia[indexLinha][j]) {
        pontuacao++;
      }
    }
  }
}

void insereArrayUsuario(int valor) {
  for (int i = 0; i <= sizeof(sequenciaJogador); i++) {
    sequenciaJogador[i] = valor;
  }
}

void avancaJogada(int sequencia) {
  insereArrayUsuario(0);
  resetGame();
  indexLinha = sequencia;
}

void KeyBoardInteraction(float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) { // F1- FRENTE
		cameraOrientation.x = 0.0;
		cameraOrientation.y = 5.0;
		cameraOrientation.z = 10.0;
		lookOrientation.x = -2.0;
		lookOrientation.y = 0.17;
		lookOrientation.z = 0.0;
    gOrientation1.y = 0.0f;
    LightPowerGeral = 60.0;
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) { // F2- CIMA
		cameraOrientation.x = -1.20;
		cameraOrientation.y = 8.90;
		cameraOrientation.z = 1.0;
		lookOrientation.x = -2.0;
		lookOrientation.y = 0.17;
		lookOrientation.z = 0.0;
    gOrientation1.y = 0.0f;
    LightPowerGeral = 10.0;
	}
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) { // F3- TRAZ
		cameraOrientation.x = -1.54;
		cameraOrientation.y = -9.45;
		cameraOrientation.z = 0.00;
		lookOrientation.x = -2.0;
		lookOrientation.y = 0.17;
		lookOrientation.z = 0.0;
    LightPowerGeral = 60.0;
  }
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		glm::lookAt(
			glm::vec3(cameraOrientation.x = 0.0f, cameraOrientation.y = 15.0f, cameraOrientation.z = 7.0f), // Camera is here
			glm::vec3(lookOrientation.x = 0.0f, lookOrientation.y = 16.66f, lookOrientation.z = 5.0f ), // and looks here
			glm::vec3(upOrientation.x = 0.0f, upOrientation.y = 1.0f, upOrientation.z = 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		resetGame();
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (anguloProjection == 60.0f) {
			anguloProjection = 45.0f;
		}else{
			anguloProjection = 60.0f;
		}
	}
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
    jogoPausado = 0;
    cameraOrientation.x = 0.0f;
    cameraOrientation.y = 5.0f;
    cameraOrientation.z = 10.0f;
    lookOrientation.x = -2.0f;
    lookOrientation.y = 0.17f;
    lookOrientation.z = 0.0f;
    LightPowerTelaInicio = 0.0;
  }
  if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
    LightPowerAzul = 0.2;
    indexAzul++;
    insereArrayUsuario(2);
  }else {
    LightPowerAzul = 0.0;
  }
  if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
    LightPowerVermelho = 0.2;
    indexVermelho++;
    insereArrayUsuario(6);
  }else {
    LightPowerVermelho = 0.0;
  }
  if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
    LightPowerAmarelo = 0.2;
    indexAmarelo++;
    insereArrayUsuario(4);
  }else {
    LightPowerAmarelo = 0.0;
  }
  if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
    LightPowerVerde = 0.2;
    indexVerde++;
    insereArrayUsuario(8);
  }else {
    LightPowerVerde = 0.0;
  }

  if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) indexLinha = 0;
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) indexLinha = 1;
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) indexLinha = 2;
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) indexLinha = 3;
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) indexLinha = 4;

  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    if (sequencia[indexLinha][indexColuna] == 2) {
      indexColuna++;
      LightPowerAzul = 0.2;
    }
    else if (sequencia[indexLinha][indexColuna] == 6) {
      indexColuna++;
      LightPowerVermelho = 0.2;
    }
    else if (sequencia[indexLinha][indexColuna] == 8) {
      indexColuna++;
      LightPowerVerde = 0.2;
    }
    else if (sequencia[indexLinha][indexColuna] == 4) {
      indexColuna++;
      LightPowerAmarelo = 0.2;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
    validaAcerto();
    printf("PONTOS: %d\n", pontuacao);
    if (pontuacao == 2) {
      avancaJogada(1);
    }
    if (pontuacao == 5) {
      avancaJogada(2);
    }
    if (pontuacao == 9) {
      avancaJogada(3);
    }
    if (pontuacao == 14) {
      avancaJogada(4);
    }
    if (pontuacao == 20) {
      printf("VOCE ACERTOU TODAS AS SEQUENCIAS \n");
    }
  }
}

void animationCamera(float deltaTime, float currentTime) {
	const float radius = 4.0f;
	float camX = radius * sin(currentTime);
	float camZ = radius * cos(currentTime);
	cameraOrientation.x = camX + 6.0;
	cameraOrientation.z = camZ + 6.0;
	gOrientation1.y += 3.14159f/radius * deltaTime;
	lookOrientation.x = 0.0;
}


void ConfigAndRender(GLuint MatrixID, GLuint Texture, GLuint TextureID, GLuint inicio, GLuint count, glm::mat4 ModelMatrix){
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(anguloProjection), widthProjection / heightProjection, farProjection, nearProjection);
	glm::mat4 ViewMatrix = glm::lookAt(
		glm::vec3( cameraOrientation.x, cameraOrientation.y, cameraOrientation.z ), // Camera is here
		glm::vec3( lookOrientation.x, lookOrientation.y, lookOrientation.z ), // and looks here
		glm::vec3( upOrientation.x, upOrientation.y, upOrientation.z )  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	// Draw the triangles !
	glDrawRangeElements(
		GL_TRIANGLES,      // mode
		inicio,
		inicio+count,
		count,    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);
}

void addBuffer(std::vector<glm::vec3> indexed_vertices,std::vector<glm::vec2> indexed_uvs,std::vector<glm::vec3> indexed_normals,std::vector<unsigned short> indices){
  // Load it into a VBO
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

  
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

  
  glGenBuffers(1, &normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

  // Generate a buffer for the indices as well
  
  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
}

void configAttributes(){
   // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    vertexPosition_modelspaceID,  // The attribute we want to configure
    3,                            // size
    GL_FLOAT,                     // type
    GL_FALSE,                     // normalized?
    0,                            // stride
    (void*)0                      // array buffer offset
  );

  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glVertexAttribPointer(
    vertexUVID,                   // The attribute we want to configure
    2,                            // size : U+V => 2
    GL_FLOAT,                     // type
    GL_FALSE,                     // normalized?
    0,                            // stride
    (void*)0                      // array buffer offset
  );

  // 3rd attribute buffer : normals
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glVertexAttribPointer(
    vertexNormal_modelspaceID,    // The attribute we want to configure
    3,                            // size
    GL_FLOAT,                     // type
    GL_FALSE,                     // normalized?
    0,                            // stride
    (void*)0                      // array buffer offset
  );
}

 
int main (void) {
  // Initialise GLFW
  if(!glfwInit()) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    getchar();
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "GENIUS - GAME", NULL, NULL);
  if( window == NULL ){
    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
    getchar();
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return -1;
  }

  addTweakBar(); 

  // Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
  glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
  glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
  glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
  glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
  glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  // HABILITAR CURSOR MOUSE
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  
  // Set the mouse at the center of the screen
  glfwPollEvents();
  glfwSetCursorPos(window, 1366/2, 860/2);

  // Dark background
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  // Create and compile our GLSL program from the shaders
  
  programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );
  // programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  ViewMatrixID = glGetUniformLocation(programID, "V");
  ModelMatrixID = glGetUniformLocation(programID, "M");

  // Get a handle for our buffers
  vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
  vertexUVID = glGetAttribLocation(programID, "vertexUV");
  vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

  // Load the texture
  GLuint Texture = loadDDS("texture/mesa.dds");
  //GLuint Genius = loadDDS("texture/uvmap.DDS");
  GLuint TelaInicio = loadDDS("texture/telainicial.dds");
  GLuint Base = loadDDS("texture/preto.dds");
  GLuint Base2 = loadDDS("texture/cinza.dds");
  GLuint BotaoVerde = loadDDS("texture/verde.dds");
  GLuint BotaoVermelho = loadDDS("texture/vermelho.dds");
  GLuint BotaoAmarelo = loadDDS("texture/amarelo.dds");
  GLuint BotaoAzul = loadDDS("texture/azul.dds");
  GLuint Centro = loadDDS("texture/laranja.dds");
  GLuint Centro1 = loadDDS("texture/laranja1.dds");
  
  // Get a handle for our "myTextureSampler" uniform
  GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

  // Read our .obj file
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;

  std::vector<unsigned short> indices;
  std::vector<glm::vec3> indexed_vertices;
  std::vector<glm::vec2> indexed_uvs;
  std::vector<glm::vec3> indexed_normals;

  loadOBJ("objetos/mesa.obj", vertices, uvs, normals);
  GLuint sizeMesa = vertices.size();

  loadOBJ("objetos/base_maior_novo.obj", vertices, uvs, normals);
  GLuint sizeBaseMaior = vertices.size();
  
  loadOBJ("objetos/base_menor_novo.obj", vertices, uvs, normals);
  GLuint sizeBaseLiga = vertices.size();
  
  loadOBJ("objetos/btn_verde.obj", vertices, uvs, normals);
  GLuint sizeVerde = vertices.size();
  
  loadOBJ("objetos/btn_vermelho.obj", vertices, uvs, normals);
  GLuint sizeVermelho = vertices.size();
  
  loadOBJ("objetos/btn_amarelo.obj", vertices, uvs, normals);
  GLuint sizeAmarelo = vertices.size();
  
  loadOBJ("objetos/btn_azul.obj", vertices, uvs, normals);
  GLuint sizeAzul = vertices.size();
  
  loadOBJ("objetos/btn_ligar_centro.obj", vertices, uvs, normals);
  GLuint sizeCentroLigaMaior = vertices.size();
  
  loadOBJ("objetos/btn_ligar_centro2.obj", vertices, uvs, normals);
  GLuint sizeCentroLigaMenor = vertices.size();

  loadOBJ("objetos/tela_inicial.obj", vertices, uvs, normals);
  GLuint sizeTelaInicio = vertices.size();

  indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

  addBuffer(indexed_vertices,indexed_uvs,indexed_normals,indices);

  // Get a handle for our "LightPosition" uniform
  glUseProgram(programID);
  GLuint LightIDPosition = glGetUniformLocation(programID, "LightPosition_worldspace");
  GLuint LightIDPositionVerde = glGetUniformLocation(programID, "LightPosition_worldspaceVerde");
  GLuint LightIDPositionVermelho = glGetUniformLocation(programID, "LightPosition_worldspaceVermelho");
  GLuint LightIDPositionAzul = glGetUniformLocation(programID, "LightPosition_worldspaceAzul");
  GLuint LightIDPositionAmarelo = glGetUniformLocation(programID, "LightPosition_worldspaceAmarelo");
  GLuint LightIDPositionTelaInicio = glGetUniformLocation(programID, "LightPosition_worldspaceTelaInicio");
  
  GLuint LightIDPowerGeral = glGetUniformLocation(programID, "LightPower_worldspaceGeral");
  GLuint LightIDPowerVerde = glGetUniformLocation(programID, "LightPower_worldspaceVerde");
  GLuint LightIDPowerVermelho = glGetUniformLocation(programID, "LightPower_worldspaceVermelho");
  GLuint LightIDPowerAzul = glGetUniformLocation(programID, "LightPower_worldspaceAzul");
  GLuint LightIDPowerAmarelo = glGetUniformLocation(programID, "LightPower_worldspaceAmarelo");
  GLuint LightIDPowerTelaInicio = glGetUniformLocation(programID, "LightPower_worldspaceTelaInicio");

  GLuint LightIDPowerBotoes = glGetUniformLocation(programID, "LightPower_worldspaceBotoes");

  // For speed computation
  double lastTime = glfwGetTime();
  double lastFrameTime = lastTime;
  int nbFrames = 0;
  do{
    // Measure speed
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastFrameTime);
    lastFrameTime = currentTime;
    nbFrames++;
    if (currentTime - lastTime >= 1.0){
      // printf("%f ms/frame\n", 1000.0/double(nbFrames));
      nbFrames = 0;
      lastTime += 1.0;
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) animationCamera(deltaTime, currentTime);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
    
    // Use our shader
    glUseProgram(programID);

    configAttributes();

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    KeyBoardInteraction(deltaTime);

    glm::vec3 lightPos = glm::vec3(2,8,2);
    glUniform3f(LightIDPosition, LightPosition.x, LightPosition.y, LightPosition.z);
    glUniform3f(LightIDPositionVerde, LightPositionVerde.x, LightPositionVerde.y, LightPositionVerde.z);
    glUniform3f(LightIDPositionVermelho, LightPositionVermelho.x, LightPositionVermelho.y, LightPositionVermelho.z);
    glUniform3f(LightIDPositionAzul, LightPositionAzul.x, LightPositionAzul.y, LightPositionAzul.z);
    glUniform3f(LightIDPositionAmarelo, LightPositionAmarelo.x, LightPositionAmarelo.y, LightPositionAmarelo.z);
    glUniform3f(LightIDPositionTelaInicio, LightPositionTelaInicio.x, LightPositionTelaInicio.y, LightPositionTelaInicio.z);
    
  if (jogoPausado == 0) {
    glUniform1f(LightIDPowerGeral, LightPowerGeral);
    glUniform1f(LightIDPowerVerde, LightPowerVerde);
    glUniform1f(LightIDPowerVermelho, LightPowerVermelho);
    glUniform1f(LightIDPowerAzul, LightPowerAzul);
    glUniform1f(LightIDPowerAmarelo, LightPowerAmarelo);

    { // MESA
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, Texture, TextureID, 0, sizeMesa, ModelMatrix);
    }

    { // BASE MAIOR
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, Base, TextureID, sizeBaseMaior-sizeMesa, sizeBaseMaior, ModelMatrix);
    }

    { // BASE LIGA
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, Base2, TextureID, sizeBaseLiga-sizeBaseMaior, sizeBaseLiga, ModelMatrix);
    }


    { // BOTAO VERDE
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, BotaoVerde, TextureID, sizeVerde-sizeBaseLiga, sizeVerde, ModelMatrix);
    }

    { // BOTAO VERMELHO
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, BotaoVermelho, TextureID, sizeVermelho-sizeVerde, sizeVermelho, ModelMatrix);
    }

    { // BOTAO AMARELO
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, BotaoAmarelo, TextureID, sizeAmarelo-sizeVermelho, sizeAmarelo, ModelMatrix);
    }

    { // BOTAO AZUL
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, BotaoAzul, TextureID, sizeAzul-sizeAmarelo, sizeAzul, ModelMatrix);

    }

    { // CENTRO LIGA MAIOR
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, Centro, TextureID, sizeCentroLigaMaior-sizeAzul, sizeCentroLigaMaior, ModelMatrix);
    }

    { // CENTRO LIGA MENOR
      glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
      glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
      glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
      glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
      ConfigAndRender(MatrixID, Centro1, TextureID, sizeCentroLigaMenor-sizeCentroLigaMaior, sizeCentroLigaMenor, ModelMatrix);
    }
  }
  if (jogoPausado == 1) {
    glUniform1f(LightIDPowerTelaInicio, LightPowerTelaInicio);
    glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
    glm::mat4 TranslationMatrix = translate(mat4(), gPositionTelaInicial); // A bit to the left
    glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
    ConfigAndRender(MatrixID, TelaInicio, TextureID, 0, sizeTelaInicio, ModelMatrix);
  }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2); 

    // Draw GUI
    TwDraw();

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    if (sequencia[indexLinha][indexColuna-1] == 2 || 
    sequencia[indexLinha][indexColuna-1] == 6 || 
    sequencia[indexLinha][indexColuna-1] == 8 || 
    sequencia[indexLinha][indexColuna-1] == 4) {
      callSleep(1);
    }
  } // Check if the ESC key was pressed or the window was closed
  while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  // Cleanup VBO and shader
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &uvbuffer);
  glDeleteBuffers(1, &normalbuffer);
  glDeleteBuffers(1, &elementbuffer);
  glDeleteProgram(programID);
  glDeleteTextures(1, &Texture);

  // Close GUI and OpenGL window, and terminate GLFW
  TwTerminate();
  glfwTerminate();

  return 0;
}
