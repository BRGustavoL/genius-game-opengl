// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>


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
vec3 gOrientation1;
 
vec3 cameraOrientation(0.0f, 5.0f, 10.0f); //Posição da camera
vec3 lookOrientation(-2.0f, 0.17f, 0.0f);
vec3 upOrientation(0.0f, 1.0f, 0.0f);

vec3 LightPosition(0.0f, 12.0f, 3.19f);
vec3 LightPositionVerde(-2.0f, 2.60f, -0.60f);

float anguloProjection = 45.0f;
float widthProjection = 4.0f;
float heightProjection = 3.0f;
float farProjection = 0.1f;
float nearProjection = 100.0f;
 
vec3 gPosition2( 1.5f, 0.0f, 0.0f); //Obejeto2 Posi��o
quat gOrientation2;
 
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
float LightPowerGeral = 100.00;
float LightPowerBotoes = 0.30;

void addTweakBar() {
    // Initialize the GUI
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(1366, 680);
    TwBar * EulerGUI = TwNewBar("Euler settings");
   // TwBar * QuaternionGUI = TwNewBar("Quaternion settings");
    TwBar * ViewGUI = TwNewBar("View Settings");
    TwBar * ProjectionGUI = TwNewBar("Projection Settings");
    TwSetParam(EulerGUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    //TwSetParam(QuaternionGUI, NULL, "position", TW_PARAM_CSTRING, 1, "808 16");
    TwSetParam(ViewGUI, NULL, "position", TW_PARAM_CSTRING, 1, "16 408");
    TwSetParam(ViewGUI, NULL, "position", TW_PARAM_CSTRING, 1, "808 408");
 
    TwAddVarRW(EulerGUI, "Euler X", TW_TYPE_FLOAT, &gOrientation1.x, "step=0.01");
    TwAddVarRW(EulerGUI, "Euler Y", TW_TYPE_FLOAT, &gOrientation1.y, "step=0.01");
    TwAddVarRW(EulerGUI, "Euler Z", TW_TYPE_FLOAT, &gOrientation1.z, "step=0.01");
    TwAddVarRW(EulerGUI, "Pos X"  , TW_TYPE_FLOAT, &gPosition1.x, "step=0.1");
    TwAddVarRW(EulerGUI, "Pos Y"  , TW_TYPE_FLOAT, &gPosition1.y, "step=0.1");
    TwAddVarRW(EulerGUI, "Pos Z"  , TW_TYPE_FLOAT, &gPosition1.z, "step=0.1");
 
    //TwAddVarRW(QuaternionGUI, "Quaternion", TW_TYPE_QUAT4F, &gOrientation2, "showval=true open=true ");
    //TwAddVarRW(QuaternionGUI, "Use LookAt", TW_TYPE_BOOL8 , &gLookAtOther, "help='Look at the other monkey ?'");
 
    //View
    TwAddVarRW(ViewGUI, "C�mera X", TW_TYPE_FLOAT, &cameraOrientation.x, "step=0.01");
    TwAddVarRW(ViewGUI, "C�mera Y", TW_TYPE_FLOAT, &cameraOrientation.y, "step=0.01");
    TwAddVarRW(ViewGUI, "C�mera Z", TW_TYPE_FLOAT, &cameraOrientation.z, "step=0.01");
    TwAddVarRW(ViewGUI, "Look X", TW_TYPE_FLOAT, &lookOrientation.x, "step=0.01");
    TwAddVarRW(ViewGUI, "Look Y", TW_TYPE_FLOAT, &lookOrientation.y, "step=0.01");
    TwAddVarRW(ViewGUI, "Look Z", TW_TYPE_FLOAT, &lookOrientation.z, "step=0.01");
    TwAddVarRW(ViewGUI, "UP X", TW_TYPE_FLOAT, &upOrientation.x, "step=0.01");
    TwAddVarRW(ViewGUI, "UP Y", TW_TYPE_FLOAT, &upOrientation.y, "step=0.01");
    TwAddVarRW(ViewGUI, "UP Z", TW_TYPE_FLOAT, &upOrientation.z, "step=0.01");


    //Light
    TwAddVarRW(ViewGUI, "Luz Geral", TW_TYPE_FLOAT, &LightPowerGeral, "step=0.01");
    TwAddVarRW(ViewGUI, "Luz Botoes", TW_TYPE_FLOAT, &LightPowerBotoes, "step=0.01");

    TwAddVarRW(ViewGUI, "Luz Geral X", TW_TYPE_FLOAT, &LightPosition.x, "step=0.01");
    TwAddVarRW(ViewGUI, "Luz Geral Y", TW_TYPE_FLOAT, &LightPosition.y, "step=0.01");
    TwAddVarRW(ViewGUI, "Luz Geral Z", TW_TYPE_FLOAT, &LightPosition.z, "step=0.01");

    TwAddVarRW(ViewGUI, "Luz Verde X", TW_TYPE_FLOAT, &LightPositionVerde.x, "step=0.01");
    TwAddVarRW(ViewGUI, "Luz Verde Y", TW_TYPE_FLOAT, &LightPositionVerde.y, "step=0.01");
    TwAddVarRW(ViewGUI, "Luz Verde Z", TW_TYPE_FLOAT, &LightPositionVerde.z, "step=0.01");
 
 
    //Projection
    TwAddVarRW(ProjectionGUI, "�gulo de Proje��o", TW_TYPE_FLOAT, &anguloProjection, "step=0.01");
    TwAddVarRW(ProjectionGUI, "Largura Proje��o", TW_TYPE_FLOAT, &widthProjection, "step=0.01");
    TwAddVarRW(ProjectionGUI, "Altura Proje��o", TW_TYPE_FLOAT, &heightProjection, "step=0.01");
    TwAddVarRW(ProjectionGUI, "Near Proje��o", TW_TYPE_FLOAT, &nearProjection, "step=0.01");
    TwAddVarRW(ProjectionGUI, "Far Proje��o", TW_TYPE_FLOAT, &farProjection, "step=0.01");    
}

void KeyBoardCameraPosition(float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) { // F1- FRENTE
		cameraOrientation.x = 0.0;
		cameraOrientation.y = 5.0;
		cameraOrientation.z = 10.0;
		lookOrientation.x = -2.0;
		lookOrientation.y = 0.17;
		lookOrientation.z = 0.0;
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) { // F2- CIMA
		cameraOrientation.x = -1.20;
		cameraOrientation.y = 8.90;
		cameraOrientation.z = 1.0;
		lookOrientation.x = -2.0;
		lookOrientation.y = 0.17;
		lookOrientation.z = 0.0;
	}
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) { // F3- TRAZ
		cameraOrientation.x = -1.54;
		cameraOrientation.y = -9.45;
		cameraOrientation.z = 0.00;
		lookOrientation.x = -2.0;
		lookOrientation.y = 0.17;
		lookOrientation.z = 0.0;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) { // TESTES
		glm::lookAt(
			glm::vec3(cameraOrientation.x = 0.0f, cameraOrientation.y = 15.0f, cameraOrientation.z = 7.0f), // Camera is here
			glm::vec3(lookOrientation.x = 0.0f, lookOrientation.y = 16.66f, lookOrientation.z = 5.0f ), // and looks here
			glm::vec3(upOrientation.x = 0.0f, upOrientation.y = 1.0f, upOrientation.z = 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { // TESTES
		if (anguloProjection == 60.0f) {
			anguloProjection = 45.0f;
		}else{
			anguloProjection = 60.0f;
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

 
int main( void )
{
 
    // Initialise GLFW
    if( !glfwInit() )
    {
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
 
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
 
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

    loadOBJ("obj/mesa.obj", vertices, uvs, normals);
   	GLuint sizeMesa = vertices.size();

    loadOBJ("obj/genius_base.obj", vertices, uvs, normals);
    GLuint sizeBaseMaior = vertices.size();
    
		loadOBJ("obj/genius_base2.obj", vertices, uvs, normals);
    GLuint sizeBaseLiga = vertices.size();
    
		loadOBJ("obj/genius_verde.obj", vertices, uvs, normals);
    GLuint sizeVerde = vertices.size();
    
		loadOBJ("obj/genius_vermelho.obj", vertices, uvs, normals);
    GLuint sizeVermelho = vertices.size();
    
		loadOBJ("obj/genius_amarelo.obj", vertices, uvs, normals);
    GLuint sizeAmarelo = vertices.size();
    
		loadOBJ("obj/genius_azul.obj", vertices, uvs, normals);
    GLuint sizeAzul = vertices.size();
    
		loadOBJ("obj/genius_centro.obj", vertices, uvs, normals);
    GLuint sizeCentroLigaMaior = vertices.size();
    
		loadOBJ("obj/genius_centro1.obj", vertices, uvs, normals);
    GLuint sizeCentroLigaMenor = vertices.size();

    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

    // printf("TAMANHO MESA %d\n", sizeBaseMaior-sizeMesa);
    // printf("TAMANHO BASE MAIOR %d\n", sizeBaseMaior);
    // printf("TAMANHO BASE LIGA %d\n", sizeBaseLiga);
    // printf("TAMANHO BOT�O VERDE %d\n", sizeVerde);
    // printf("TAMANHO BOT�O VERMELHO %d\n", sizeVermelho);
    // printf("TAMANHO BOT�O AMARELO %d\n", sizeAmarelo);
    // printf("TAMANHO BOT�O AZUL %d\n", sizeAzul);
    // printf("TAMANHO CENTRO LIGA MAIOR %d\n", sizeCentroLigaMaior);
    // printf("TAMANHO CENTRO LIGA MENOR %d\n", sizeCentroLigaMenor);

    addBuffer(indexed_vertices,indexed_uvs,indexed_normals,indices);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightIDPosition = glGetUniformLocation(programID, "LightPosition_worldspace");
    GLuint LightIDPositionVerde = glGetUniformLocation(programID, "LightPosition_worldspaceVerde");
		GLuint LightIDPowerGeral = glGetUniformLocation(programID, "LightPower_worldspaceGeral");
    GLuint LightIDPowerBotoes = glGetUniformLocation(programID, "LightPower_worldspaceBotoes");
 
    // For speed computation
    double lastTime = glfwGetTime();
    double lastFrameTime = lastTime;
    int nbFrames = 0;
    glm::mat4 ModelMatrix2;
    
    do{
			// Measure speed
			double currentTime = glfwGetTime();
			float deltaTime = (float)(currentTime - lastFrameTime);
			lastFrameTime = currentTime;
			nbFrames++;
			if (currentTime - lastTime >= 1.0){
				printf("%f ms/frame\n", 1000.0/double(nbFrames));
				nbFrames = 0;
				lastTime += 1.0;
			}
			
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) animationCamera(deltaTime, currentTime);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
			
			// Use our shader
			glUseProgram(programID);
			KeyBoardCameraPosition(deltaTime);

			configAttributes();

			// Index buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

			glm::vec3 lightPos = glm::vec3(2,8,2);
      glUniform3f(LightIDPosition, LightPosition.x, LightPosition.y, LightPosition.z);
      glUniform3f(LightIDPositionVerde, LightPositionVerde.x, LightPositionVerde.y, LightPositionVerde.z);
      glUniform1f(LightIDPowerGeral, LightPowerGeral);
			glUniform1f(LightIDPowerBotoes, LightPowerBotoes);

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
				glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;
				ConfigAndRender(MatrixID, Base2, TextureID, sizeBaseLiga-sizeBaseMaior, sizeBaseLiga, ModelMatrix2);
			}

			{ // BOTAO VERDE
				glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
				glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
				glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
				glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;
				ConfigAndRender(MatrixID, BotaoVerde, TextureID, sizeVerde-sizeBaseLiga, sizeVerde, ModelMatrix2);
			}

			{ // BOTAO VERMELHO
				glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
				glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
				glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
				glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;
				ConfigAndRender(MatrixID, BotaoVermelho, TextureID, sizeVermelho-sizeVerde, sizeVermelho, ModelMatrix2);
			}

			{ // BOTAO AMARELO
				glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
				glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
				glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
				glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;
				ConfigAndRender(MatrixID, BotaoAmarelo, TextureID, sizeAmarelo-sizeVermelho, sizeAmarelo, ModelMatrix2);
			}

			{ // BOTAO AZUL
				glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
				glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
				glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
				glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;
				ConfigAndRender(MatrixID, BotaoAzul, TextureID, sizeAzul-sizeAmarelo, sizeAzul, ModelMatrix2);

			}

			{ // CENTRO LIGA MAIOR
				glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
				glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
				glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
				glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;
				ConfigAndRender(MatrixID, Centro, TextureID, sizeCentroLigaMaior-sizeAzul, sizeCentroLigaMaior, ModelMatrix2);
			}

			{ // CENTRO LIGA MENOR
				glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
				glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
				glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
				glm::mat4 ModelMatrix2 = TranslationMatrix * RotationMatrix * ScalingMatrix;
				ConfigAndRender(MatrixID, Centro1, TextureID, sizeCentroLigaMenor-sizeCentroLigaMaior, sizeCentroLigaMenor, ModelMatrix2);
			}



			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2); 

			// Draw GUI
			TwDraw();

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
 
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
