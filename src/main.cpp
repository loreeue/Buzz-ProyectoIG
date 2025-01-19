#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

// Prototipos
void	configScene();
void	initializeLights();
void	renderScene();
void	drawScene(glm::mat4 &P, glm::mat4 &V);
void	setLights(glm::mat4 P, glm::mat4 V);

// Funciones de inicialización
void	initializeLights();
void	initializeMaterials();
void	initializeTextures();

// Funciones de dibujo
void	drawScene(glm::mat4 &P, glm::mat4 &V);
void	drawObjectMat(Model &model, Material &material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void	drawObjectTex(Model &model, Textures &textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void	drawPlane1(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void	drawPlane2(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void	drawFly(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void	drawLamp1(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void	drawLamp2(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void	drawAllLamps1(const glm::mat4 &P, const glm::mat4 &V);
void	drawAllLamps2(const glm::mat4 &P, const glm::mat4 &V);
void	drawHouse(const glm::mat4 &P, const glm::mat4 &V);
void	drawWindows(const glm::mat4 &P, const glm::mat4 &V);
void	drawBackground(const glm::mat4 &P, const glm::mat4 &V);
void	drawBread(const glm::mat4 &P, const glm::mat4 &V);
void	drawTable(const glm::mat4 &P, const glm::mat4 &V);
void	drawSign(glm::mat4 P, glm::mat4 V, glm::mat4 M);

// Callbacks
void	funFramebufferSize(GLFWwindow* window, int width, int height);
void	funKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void	funCursorPos(GLFWwindow* window, double xpos, double ypos);
void	funScroll(GLFWwindow* window, double xoffset, double yoffset);

// Shaders y modelos
Shaders	shaders;
Model	sphere, sphere_tex, plane, cylinder;
Model	structure, roof, windows;
Model	table;
Model	ala1, ala2, ojos;
Model	cabeza, cuerpo_alante, cuerpo_detras, debajo_cuerpo;
Model	patader1, patader1_2, patader2, patader2_2, patader3, patader3_2;
Model	pataizq1, pataizq1_2, pataizq2, pataizq2_2, pataizq3, pataizq3_2;

// Materiales
Material	matPink;
Material	matGreen;
Material	matBlack;

// Texturas
Texture		imgNoEmissive;
Texture		imgGrass, imgGrassS, imgGrassN;
Texture		imgWings;
Texture		imgHouseWall, imgHouseWallS, imgHouseWallN;
Texture		imgHouseRoof, imgHouseRoofS, imgHouseRoofN;
Texture		imgHouseWindows;
Texture		imgBread;
Texture		imgBackground;
Texture		imgWindowPlane;
Texture		imgTable;
Texture		imgSign;

Textures	texGrass;
Textures	texWings;
Textures	texHouseWall;
Textures	texHouseRoof;
Textures	texHouseWindows;
Textures	texBread;
Textures	texBackground;
Textures	texWindowPlane;
Textures	texTable;
Textures	texSign;

// Luces y materiales
#define		NLD 1
#define		NLP 3
#define		NLF 4
Light		lightG;
Light		lightD[NLD];
Light		lightP[NLP];
Light		lightF[NLF];
Material	mluz;

// Viewport
int	w = 800, h = 600;

// Parámetros de la cámara
float		alphax = 0.0f, alphay = 0.0f;
float		cameraDistance = 30.0f;
glm::vec3	up(0.0f, 1.0f, 0.0f);
bool		firstPersonView = false; // Para la cámara de la mosca
bool		viewingSign = false; // Para la cámara del cartel

// Animaciones
float	posX = 0.0;
float	posY = 3.0;
float	posZ = 2.0;
bool	moveLegs = false;
float	flyRotation = 0.0f;

// Interaccion con las luces
float	lampStates[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // Para apagar y encender las farolas
bool	lightPOn = true;
bool	rotateLight = false; // Para la luz rotatoria
float	lightAngle = 0.0f;
float	lightRadius = 20.0f;

int	main()
{
	// Inicialización de GLFW
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Creación de ventana
	GLFWwindow* window = glfwCreateWindow(w, h, "BUZZ IG", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Inicialización de GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		return -1;
	}
	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	// Callbacks
	glfwSetFramebufferSizeCallback(window, funFramebufferSize);
	glfwSetKeyCallback(window, funKey);
	glfwSetCursorPosCallback(window, funCursorPos);
	glfwSetScrollCallback(window, funScroll);

	// Configuración de la escena
	configScene();

	// Bucle principal
	while (!glfwWindowShouldClose(window))
	{
		renderScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Finalización
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void	configScene()
{
	glEnable(GL_DEPTH_TEST);

	// Para las transparencias
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shaders.initShaders("resources/shaders/vshader.glsl", "resources/shaders/fshader.glsl");

	// Cargar modelos
	sphere.initModel("resources/models/sphere.obj");
	plane.initModel("resources/models/plane.obj");
	cylinder.initModel("resources/models/cylinder.obj");
	sphere_tex.initModel("resources/models/sphere_tex.obj");
	structure.initModel("resources/models/estructura.obj");
	roof.initModel("resources/models/techo.obj");
	windows.initModel("resources/models/ventanas.obj");
	table.initModel("resources/models/table.obj");

	ala1.initModel("resources/models/ala1.obj");
	ala2.initModel("resources/models/ala2.obj");
	cabeza.initModel("resources/models/cabeza.obj");
	cuerpo_alante.initModel("resources/models/cuerpo_alante.obj");
	cuerpo_detras.initModel("resources/models/cuerpo_detras.obj");
	debajo_cuerpo.initModel("resources/models/debajo_cuerpo.obj");
	ojos.initModel("resources/models/ojos.obj");
	patader1.initModel("resources/models/patader1.obj");
	patader1_2.initModel("resources/models/patader1_2.obj");
	patader2.initModel("resources/models/patader2.obj");
	patader2_2.initModel("resources/models/patader2_2.obj");
	patader3.initModel("resources/models/patader3.obj");
	patader3_2.initModel("resources/models/patader3_2.obj");
	pataizq1.initModel("resources/models/pataizq1.obj");
	pataizq1_2.initModel("resources/models/pataizq1_2.obj");
	pataizq2.initModel("resources/models/pataizq2.obj");
	pataizq2_2.initModel("resources/models/pataizq2_2.obj");
	pataizq3.initModel("resources/models/pataizq3.obj");
	pataizq3_2.initModel("resources/models/pataizq3_2.obj");

	// Cargar texturas
	imgNoEmissive.initTexture("resources/textures/no_emissive.png");
	imgGrass.initTexture("resources/textures/grass.jpg");
	imgGrassS.initTexture("resources/textures/grass_specular.png");
	imgGrassN.initTexture("resources/textures/grass_normal.png");
	imgWings.initTexture("resources/textures/wings.png");
	imgHouseWall.initTexture("resources/textures/casa_pared.png");
	imgHouseWallS.initTexture("resources/textures/casa_pared_specular.png");
	imgHouseWallN.initTexture("resources/textures/casa_pared_normal.png");
	imgHouseRoof.initTexture("resources/textures/casa_tejas.png");
	imgHouseRoofS.initTexture("resources/textures/casa_tejas_specular.png");
	imgHouseRoofN.initTexture("resources/textures/casa_tejas_normal.png");
	imgHouseWindows.initTexture("resources/textures/casa_ventanas.png");
	imgBread.initTexture("resources/textures/bread.png");
	imgBackground.initTexture("resources/textures/fondo.jpg");
	imgWindowPlane.initTexture("resources/textures/window.png");
	imgTable.initTexture("resources/textures/table.jpg");
	imgSign.initTexture("resources/textures/cartel.png");

	// Luces
	initializeLights();

	// Materiales
	initializeMaterials();

	// Texturas
	initializeTextures();
}

void	initializeLights()
{
	// Luz ambiental global
	lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

	// Luces direccionales
	lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);
	lightD[0].ambient = glm::vec3(0.05, 0.05, 0.05);
	lightD[0].diffuse = glm::vec3(0.3, 0.3, 0.3);
	lightD[0].specular = glm::vec3(0.3, 0.3, 0.3);

	// Luces posicionales
	// Para el pan
	lightP[0].position = glm::vec3(-6.0, 3.0, 0.0);
	lightP[0].ambient = glm::vec3(0.2, 0.2, 0.2);
	lightP[0].diffuse = glm::vec3(0.9, 0.9, 0.9);
	lightP[0].specular = glm::vec3(0.9, 0.9, 0.9);
	lightP[0].c0 = 1.00;
	lightP[0].c1 = 0.22;
	lightP[0].c2 = 0.20;

	// Para el cartel
	lightP[1].position = glm::vec3(10.0f, 8.0f, 12.0f);
	lightP[1].ambient = glm::vec3(0.2, 0.2, 0.2);
	lightP[1].diffuse = glm::vec3(0.9, 0.9, 0.9);
	lightP[1].specular = glm::vec3(0.9, 0.9, 0.9);
	lightP[1].c0 = 1.0f;
	lightP[1].c1 = 0.09f;
	lightP[1].c2 = 0.032f;

	// Luz rotatoria
	lightP[2].position = glm::vec3(lightRadius, 3.0f, 0.0f);
	lightP[2].ambient = glm::vec3(0.05, 0.05, 0.05);
	lightP[2].diffuse = glm::vec3(1.0, 1.0, 1.0);
	lightP[2].specular = glm::vec3(1.0, 1.0, 1.0);
	lightP[2].innerCutOff = glm::cos(glm::radians(10.0f));
	lightP[2].outerCutOff = glm::cos(glm::radians(25.0f));
	lightP[2].c0 = 1.0f;
	lightP[2].c1 = 0.09f;
	lightP[2].c2 = 0.032f;

	// Luces focales
	float planeSize = 14.0f;
	for (int i = 0; i < NLF; i++)
	{
		lightF[i].ambient = glm::vec3(0.2, 0.2, 0.2);
		lightF[i].diffuse = glm::vec3(0.9, 0.9, 0.9);
		lightF[i].specular = glm::vec3(0.9, 0.9, 0.9);
		lightF[i].innerCutOff = 30.0;
		lightF[i].outerCutOff = lightF[i].innerCutOff + 5.0;
		lightF[i].c0 = 1.000;
		lightF[i].c1 = 0.090;
		lightF[i].c2 = 0.032;
	}

	// Posición y dirección de cada luz focal
	lightF[0].position = glm::vec3(-planeSize, 2.0f, -planeSize);
	lightF[0].direction = glm::vec3(1.0f, -1.0f, 1.0f);

	lightF[1].position = glm::vec3(planeSize, 2.0f, -planeSize);
	lightF[1].direction = glm::vec3(-1.0f, -1.0f, 1.0f);

	lightF[2].position = glm::vec3(-planeSize, 2.0f, planeSize);
	lightF[2].direction = glm::vec3(1.0f, -1.0f, -1.0f);

	lightF[3].position = glm::vec3(planeSize, 2.0f, planeSize);
	lightF[3].direction = glm::vec3(-1.0f, -1.0f, -1.0f);
}

void	initializeTextures()
{
	texGrass.diffuse = imgGrass.getTexture();
	texGrass.specular = imgGrassS.getTexture();
	texGrass.emissive = imgNoEmissive.getTexture();
	texGrass.normal = imgGrassN.getTexture();
	texGrass.shininess = 10.0;

	texWings.diffuse = imgWings.getTexture();
	texWings.specular = imgWings.getTexture();
	texWings.emissive = imgWings.getTexture();
	texWings.normal = 0;
	texWings.shininess = 10.0;

	texHouseWall.diffuse = imgHouseWall.getTexture();
	texHouseWall.specular = imgHouseWallS.getTexture();
	texHouseWall.emissive = imgNoEmissive.getTexture();
	texHouseWall.normal = imgHouseWallN.getTexture();
	texHouseWall.shininess = 10.0;

	texHouseRoof.diffuse = imgHouseRoof.getTexture();
	texHouseRoof.specular = imgHouseRoofS.getTexture();
	texHouseRoof.emissive = imgNoEmissive.getTexture();
	texHouseRoof.normal = imgHouseRoofN.getTexture();
	texHouseRoof.shininess = 10.0;

	texHouseWindows.diffuse = imgHouseWindows.getTexture();
	texHouseWindows.specular = imgHouseWindows.getTexture();
	texHouseWindows.emissive = imgNoEmissive.getTexture();
	texHouseWindows.normal = 0;
	texHouseWindows.shininess = 10.0;

	texBread.diffuse = imgBread.getTexture();
	texBread.specular = imgBread.getTexture();
	texBread.emissive = imgNoEmissive.getTexture();
	texBread.normal = 0;
	texBread.shininess = 10.0;

	texBackground.diffuse = imgBackground.getTexture();
	texBackground.specular = imgBackground.getTexture();
	texBackground.emissive = imgNoEmissive.getTexture();
	texBackground.normal = 0;
	texBackground.shininess = 10.0;

	texWindowPlane.diffuse = imgWindowPlane.getTexture();
	texWindowPlane.specular = imgWindowPlane.getTexture();
	texWindowPlane.emissive = imgNoEmissive.getTexture();
	texWindowPlane.normal = 0;
	texWindowPlane.shininess = 10.0;

	texTable.diffuse = imgTable.getTexture();
	texTable.specular = imgTable.getTexture();
	texTable.emissive = imgNoEmissive.getTexture();
	texTable.normal = 0;
	texTable.shininess = 10.0;

	texSign.diffuse = imgSign.getTexture();
	texSign.specular = imgSign.getTexture();
	texSign.emissive = imgNoEmissive.getTexture();
	texSign.normal = 0;
	texSign.shininess = 10.0;
}

void	initializeMaterials()
{
	matPink.ambient = glm::vec4(0.39f, 0.33f, 0.35f, 1.0f);
	matPink.diffuse = glm::vec4(0.39f, 0.33f, 0.35f, 1.0f);
	matPink.specular = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	matPink.emissive = glm::vec4(0.2f, 0.1f, 0.15f, 1.0f);
	matPink.shininess = 64.0f;

	matGreen.ambient = glm::vec4(0.32f, 0.37f, 0.32f, 1.0f);
	matGreen.diffuse = glm::vec4(0.32f, 0.37f, 0.32f, 1.0f);
	matGreen.specular = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	matGreen.emissive = glm::vec4(0.1f, 0.2f, 0.1f, 1.0f);
	matGreen.shininess = 64.0f;

	matBlack.ambient = glm::vec4(0.0, 0.0, 0.0, 1.0);
	matBlack.diffuse = glm::vec4(0.0, 0.0, 0.0, 1.0);
	matBlack.specular = glm::vec4(0.0, 0.0, 0.0, 1.0);
	matBlack.emissive = glm::vec4(0.0, 0.0, 0.0, 1.0);
	matBlack.shininess = 0.0;
}

void	renderScene()
{
	// Limpiar buffers
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaders.useShaders();

	// Configurar proyección y vista
	glm::mat4	P = glm::perspective(glm::radians(60.0f), (float)w / h, 0.1f, 100.0f);
	float		x = cameraDistance * cos(glm::radians(alphay)) * sin(glm::radians(alphax));
	float		y = cameraDistance * sin(glm::radians(alphay)) + 5.0f;
	float		z = cameraDistance * cos(glm::radians(alphay)) * cos(glm::radians(alphax));
	glm::vec3	eye(x, y, z);
	glm::vec3	center(0.0f, 0.0f, 0.0f);
	if (viewingSign)
	{
		eye = glm::vec3(10.0f, 6.0f, 15.0f);
		center = glm::vec3(10.0f, 5.5f, 10.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (firstPersonView)
	{
		float offsetDistance = -3.0f; // Distancia desde el centro de la mosca
		float offsetHeight = 0.5f; // Altura sobre el centro de la mosca

		// Calcula la posición de la cámara relativa a la mosca
		glm::vec3 offset = glm::vec3(
			sin(glm::radians(flyRotation)) * offsetDistance,
			offsetHeight,
			cos(glm::radians(flyRotation)) * offsetDistance
		);

		eye = glm::vec3(posX, posY, posZ) + offset;
		center = glm::vec3(posX, posY, posZ) + glm::vec3(
			sin(glm::radians(flyRotation)),
			0.0f,
			cos(glm::radians(flyRotation))
		);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		float x = cameraDistance * cos(glm::radians(alphay)) * sin(glm::radians(alphax));
		float y = cameraDistance * sin(glm::radians(alphay)) + 5.0f;
		float z = cameraDistance * cos(glm::radians(alphay)) * cos(glm::radians(alphax));
		eye = glm::vec3(x, y, z);
		center = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	glm::mat4 V = glm::lookAt(eye, center, up);

	// Actualizar posición de la luz rotatoria
	if (rotateLight)
	{
		lightAngle += 0.02f; // Incrementa el ángulo
		if (lightAngle >= 360.0f) lightAngle -= 360.0f;

		// Calcula la nueva posición de la luz
		lightP[2].position.x = lightRadius * cos(lightAngle);
		lightP[2].position.z = lightRadius * sin(lightAngle);
	}

	// Fijamos las luces
	setLights(P,V);

	// Dibujar la escena completa
	drawScene(P, V);
}

void	drawScene(glm::mat4 &P, glm::mat4 &V)
{
	// Dibujar el fondo
	drawBackground(P, V);

	// Dibujar el plano
	drawPlane1(P, V, I);
	drawPlane2(P, V, I);

	// Dibujar las farolas (los postes)
	drawAllLamps1(P, V);

	// Dibujar el pan
	drawBread(P, V);

	// Dibujar la mesa
	drawTable(P, V);

	// Dibujar la casa
	drawHouse(P, V);

	// Dibujar el cartel
	drawSign(P, V, I);

	// Dibujar la mosca
	drawFly(P, V, I);

	// Dibujar las farolas (las luces)
	drawAllLamps2(P, V);

	// Dibujar ventanas casa
	drawWindows(P, V);
}

void	setLights(glm::mat4 P, glm::mat4 V)
{
	shaders.setLight("ulightG",lightG);
	for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
	for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);

	// Actualizar las luces focales con sus estados
	for(int i=0; i<NLF; i++) {
		Light tempLight = lightF[i];
		tempLight.ambient  = lampStates[i] * lightF[i].ambient;
		tempLight.diffuse  = lampStates[i] * lightF[i].diffuse;
		tempLight.specular = lampStates[i] * lightF[i].specular;
		shaders.setLight("ulightF["+toString(i)+"]", tempLight);
	}

	for(int i=0; i<NLP; i++) {
		Light tempLight = lightP[i];
		if (!lightPOn) {
			tempLight.ambient = glm::vec3(0.0f);
			tempLight.diffuse = glm::vec3(0.0f);
			tempLight.specular = glm::vec3(0.0f);
		}
		shaders.setLight("ulightP["+toString(i)+"]", tempLight);

		// Dibujar la esfera de la luz posicional
		glm::mat4 M = glm::translate(I, lightP[i].position) * glm::scale(I, glm::vec3(0.1));
		Material tempMluz = mluz;
		tempMluz.emissive = glm::vec4(lightPOn ? 1.0f : 0.2f);
		drawObjectMat(sphere, tempMluz, P, V, M);
	}

	// Dibujar las esferas de luz según su estado
	for(int i=0; i<NLF; i++) {
		Material tempMluz = mluz;
		tempMluz.emissive = glm::vec4(lampStates[i], lampStates[i], lampStates[i], 1.0);
		glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025));
		drawObjectMat(sphere, tempMluz, P, V, M);
	}
}

void	drawObjectMat(Model &model, Material &material, glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
	shaders.setMat4("uM"  ,M);
	shaders.setMat4("uPVM",P*V*M);
	shaders.setBool("uWithMaterials",true);
	shaders.setMaterial("umaterial",material);
	model.renderModel(GL_FILL);
}

void	drawObjectTex(Model &model, Textures &textures, glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
	shaders.setMat4("uM"  ,M);
	shaders.setMat4("uPVM",P*V*M);
	shaders.setBool("uWithMaterials",false);
	shaders.setTextures("utextures",textures);
	model.renderModel(GL_FILL);
}

void	drawPlane1(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	glm::mat4 S = glm::scale(I, glm::vec3(30.0f));
	drawObjectTex(plane, texGrass, P, V, M * S);
}

void	drawPlane2(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	glm::mat4 S = glm::scale(I, glm::vec3(30.0f));
	glm::mat4 T = glm::translate(I, glm::vec3(0.0f, -0.01f, 0.0f));
	drawObjectMat(plane, matBlack, P, V, M * T * S);
}

void	drawFly(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	glm::mat4 S = glm::scale(I, glm::vec3(0.03f));
	glm::mat4 T = glm::translate(I, glm::vec3(posX, posY, posZ));
	glm::mat4 R = glm::rotate(I, glm::radians(flyRotation), glm::vec3(0.0f, 1.0f, 0.0f));

	// Animación de las alas
	float wingAngle = sin(glfwGetTime() * 10.0f) * 15.0f;
	glm::mat4 R_angle_der = glm::rotate(I, glm::radians(wingAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 R_angle_izq = glm::rotate(I, glm::radians(-wingAngle), glm::vec3(0.0f, 0.0f, 1.0f));

	// Animación de las patas
	float legAngle = 0.0f;
	if (posY <= 1.1f && moveLegs)
		legAngle = sin(glfwGetTime() * 15.0f) * 20.0f;
	glm::mat4 R_leg_front = glm::rotate(I, glm::radians(legAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 R_leg_back = glm::rotate(I, glm::radians(-legAngle), glm::vec3(1.0f, 0.0f, 0.0f));

	// Dibujar las partes de la mosca
	drawObjectTex(ala1, texWings, P, V, T * R * S * R_angle_der);
	drawObjectTex(ala2, texWings, P, V, T * R * S * R_angle_izq);
	drawObjectMat(cabeza, matGreen, P, V, T * R * S);
	drawObjectMat(cuerpo_alante, matPink, P, V, T * R * S);
	drawObjectMat(cuerpo_detras, matPink, P, V, T * R * S);
	drawObjectMat(debajo_cuerpo, matPink, P, V, T * R * S);
	drawObjectMat(ojos, matPink, P, V, T * R * S);

	// Dibujar las patas con animación
	drawObjectMat(patader1, matBlack, P, V, T * R * S * R_leg_front);
	drawObjectMat(patader1_2, matBlack, P, V, T * R * S * R_leg_front);
	drawObjectMat(patader2, matBlack, P, V, T * R * S);
	drawObjectMat(patader2_2, matBlack, P, V, T * R * S);
	drawObjectMat(patader3, matBlack, P, V, T * R * S * R_leg_back);
	drawObjectMat(patader3_2, matBlack, P, V, T * R * S * R_leg_back);
	drawObjectMat(pataizq1, matBlack, P, V, T * R * S * R_leg_front);
	drawObjectMat(pataizq1_2, matBlack, P, V, T * R * S * R_leg_front);
	drawObjectMat(pataizq2, matBlack, P, V, T * R * S);
	drawObjectMat(pataizq2_2, matBlack, P, V, T * R * S);
	drawObjectMat(pataizq3, matBlack, P, V, T * R * S * R_leg_back);
	drawObjectMat(pataizq3_2, matBlack, P, V, T * R * S * R_leg_back);
}

void	drawLamp1(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	Material posteMaterial;
	posteMaterial.ambient   = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	posteMaterial.diffuse   = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	posteMaterial.specular  = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
	posteMaterial.emissive  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	posteMaterial.shininess = 32.0f;

	// Poste de la farola (siempre visible)
	glm::mat4 S_pole = glm::scale(I, glm::vec3(0.1f, 2.0f, 0.1f));
	drawObjectMat(cylinder, posteMaterial, P, V, M * S_pole);
}

void	drawLamp2(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	// Luz de la farola
	int lampIndex = 0;
	glm::vec3 pos = glm::vec3(M[3]);
	if (pos.x > 0) lampIndex += 1;
	if (pos.z > 0) lampIndex += 2;

	float intensity = lampStates[lampIndex] * 0.7f + 0.3f;
	glm::vec4 sphereColor;
	if (intensity > 0.5f) // Farola encendida: color blanco
		sphereColor = glm::vec4(intensity, intensity, intensity, 0.7f);
	else // Farola apagada: color rosado
		sphereColor = glm::vec4(0.8f, 0.5f, 0.6f, 0.7f);

	Material sphereMaterial;
	sphereMaterial.ambient = sphereColor * 0.2f;
	sphereMaterial.diffuse = sphereColor;
	sphereMaterial.specular = glm::vec4(1.0f);
	sphereMaterial.emissive = sphereColor * intensity;
	sphereMaterial.shininess = 64.0f;

	glm::mat4 T_light = glm::translate(M, glm::vec3(0.0f, 2.0f, 0.0f));
	glm::mat4 S_light = glm::scale(I, glm::vec3(0.6f));

	// Dibujar la esfera transparente
	glDepthMask(GL_TRUE);
		drawObjectMat(sphere, sphereMaterial, P, V, T_light * S_light);
	glDepthMask(GL_FALSE);
}

void	drawAllLamps1(const glm::mat4 &P, const glm::mat4 &V)
{
	float planeSize = 14.0f;
	glm::mat4 lampPos1 = glm::translate(I, glm::vec3(-planeSize, 2.0f, -planeSize));
	glm::mat4 lampPos2 = glm::translate(I, glm::vec3(planeSize, 2.0f, -planeSize));
	glm::mat4 lampPos3 = glm::translate(I, glm::vec3(-planeSize, 2.0f, planeSize));
	glm::mat4 lampPos4 = glm::translate(I, glm::vec3(planeSize, 2.0f, planeSize));

	drawLamp1(P, V, lampPos1);
	drawLamp1(P, V, lampPos2);
	drawLamp1(P, V, lampPos3);
	drawLamp1(P, V, lampPos4);
}

void	drawAllLamps2(const glm::mat4 &P, const glm::mat4 &V)
{
	float planeSize = 14.0f;
	glm::mat4 lampPos1 = glm::translate(I, glm::vec3(-planeSize, 2.0f, -planeSize));
	glm::mat4 lampPos2 = glm::translate(I, glm::vec3(planeSize, 2.0f, -planeSize));
	glm::mat4 lampPos3 = glm::translate(I, glm::vec3(-planeSize, 2.0f, planeSize));
	glm::mat4 lampPos4 = glm::translate(I, glm::vec3(planeSize, 2.0f, planeSize));

	drawLamp2(P, V, lampPos1);
	drawLamp2(P, V, lampPos2);
	drawLamp2(P, V, lampPos3);
	drawLamp2(P, V, lampPos4);
}

void	drawHouse(const glm::mat4 &P, const glm::mat4 &V)
{
	glm::mat4 R = glm::rotate(I, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 T = glm::translate(I, glm::vec3(0.0f, -0.3f, -5.0f));

	drawObjectTex(structure, texHouseWall, P, V, T * R);
	drawObjectTex(roof, texHouseRoof, P, V, T * R);
	drawObjectTex(windows, texHouseWindows, P, V, T * R);
}

void	drawWindows(const glm::mat4 &P, const glm::mat4 &V)
{
	// Dibujar planos de las ventanas (hay 8 ventanas)
	glDepthMask(GL_FALSE);

	glm::mat4 Rw = glm::rotate(I, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Sw = glm::scale(I, glm::vec3(0.6f, 0.6f, 1.0f));

	glm::mat4 Tw1 = glm::translate(I, glm::vec3(2.0f, 1.5f, -1.7f));
	drawObjectTex(plane, texWindowPlane, P, V, Tw1 * Sw * Rw);

	glm::mat4 Rw2 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Tw2 = glm::translate(I, glm::vec3(2.8f, 1.5f, -2.9f));
	drawObjectTex(plane, texWindowPlane, P, V, Tw2 * Rw2 * Sw * Rw);

	glm::mat4 Rw3 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Tw3 = glm::translate(I, glm::vec3(2.8f, 1.5f, -5.2f));
	drawObjectTex(plane, texWindowPlane, P, V, Tw3 * Rw3 * Sw * Rw);

	glm::mat4 Rw4 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Tw4 = glm::translate(I, glm::vec3(3.5f, 1.5f, -8.2f));
	drawObjectTex(plane, texWindowPlane, P, V, Tw4 * Rw4 * Sw * Rw);

	glm::mat4 Rw5 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Tw5 = glm::translate(I, glm::vec3(-3.5f, 1.5f, -5.2f));
	drawObjectTex(plane, texWindowPlane, P, V, Tw5 * Rw5 * Sw * Rw);

	glm::mat4 Tw6 = glm::translate(I, glm::vec3(2.2f, 1.5f, -9.8f));
	drawObjectTex(plane, texWindowPlane, P, V, Tw6 * Sw * Rw);

	glm::mat4 Tw7 = glm::translate(I, glm::vec3(-0.2f, 1.5f, -9.8f));
	drawObjectTex(plane, texWindowPlane, P, V, Tw7 * Sw * Rw);

	glm::mat4 Tw8 = glm::translate(I, glm::vec3(-2.3f, 1.5f, -9.8f));
	drawObjectTex(plane, texWindowPlane, P, V, Tw8 * Sw * Rw);

	glDepthMask(GL_TRUE);
}

void	drawBackground(const glm::mat4 &P, const glm::mat4 &V)
{
	glm::mat4 T = glm::translate(I, glm::vec3(0.0f, 4.0f, 0.0f));
	glm::mat4 S = glm::scale(I, glm::vec3(20.0f));
	glm::mat4 R = glm::rotate(I, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glDepthMask(GL_FALSE);
		drawObjectTex(sphere_tex, texBackground, P, V, R * T * S);
	glDepthMask(GL_TRUE);
}

void	drawBread(const glm::mat4 &P, const glm::mat4 &V)
{
	glm::mat4 T = glm::translate(I, glm::vec3(-6.0f, 1.3f, 0.0f));
	glm::mat4 S = glm::scale(I, glm::vec3(0.2f, 0.1f, 0.1f));
	drawObjectTex(sphere_tex, texBread, P, V, T * S);
}

void	drawTable(const glm::mat4 &P, const glm::mat4 &V)
{
	glm::mat4 T = glm::translate(I, glm::vec3(-6.0f, 0.0f, 0.0f));
	glm::mat4 S = glm::scale(I, glm::vec3(2.0f));
	drawObjectTex(table, texTable, P, V, T * S);
}

void	drawSign(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
	// Posición del cartel
	glm::mat4 T = glm::translate(I, glm::vec3(10.0f, 0.0f, 10.0f));

	// Dibujar los postes
	glm::mat4 S_pole = glm::scale(I, glm::vec3(0.1f, 2.0f, 0.1f));
	glm::mat4 T_pole1 = glm::translate(I, glm::vec3(-0.75f, 2.0f, -0.1f));
	glm::mat4 T_pole2 = glm::translate(I, glm::vec3(0.75f, 2.0f, -0.1f));

	drawObjectMat(cylinder, matBlack, P, V, M * T * T_pole1 * S_pole);
	drawObjectMat(cylinder, matBlack, P, V, M * T * T_pole2 * S_pole);

	// Dibujar el cartel
	glm::mat4 S_sign = glm::scale(I, glm::vec3(2.5f, 2.5f, 2.0f));
	glm::mat4 T_sign1 = glm::translate(I, glm::vec3(0.0f, 5.5f, 0.0f));
	glm::mat4 T_sign2 = glm::translate(I, glm::vec3(0.0f, 5.5f, -0.05f));
	glm::mat4 R_sign = glm::rotate(I, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	drawObjectTex(plane, texSign, P, V, M * T * T_sign1 * R_sign * S_sign);
	drawObjectMat(plane, matBlack, P, V, M * T * T_sign2 * R_sign * S_sign);
}

bool	isFlyFacingRight()
{
	return (flyRotation < 90.0f || flyRotation >= 270.0f);
}

void	funFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	w = width;
	h = height;
}

void	funKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
		case GLFW_KEY_UP:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				if (posY < 15.0f)
					posY += 0.05;
			}
			break;
		case GLFW_KEY_DOWN:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				if (posY > 1.1f)
					posY -= 0.05;
			}
			break;
		case GLFW_KEY_LEFT:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				float moveAngle = glm::radians(flyRotation);
				bool canMove = true;

				// Comprobación para el extremo derecho
				if (posX >= 15.0f || posZ >= 15.0f)
				{
					if (isFlyFacingRight()) canMove = false;
					else canMove = true;
				}
				// Comprobación para el extremo izquierdo
				else if (posX <= -15.0f || posZ <= -15.0f)
				{
					if (isFlyFacingRight()) canMove = true;
					else canMove = false;
				}
				if (canMove)
				{
					posZ -= 0.05f * cos(moveAngle);
					posX -= 0.05f * sin(moveAngle);
					moveLegs = true;
				}
			}
			else if (action == GLFW_RELEASE)
				moveLegs = false;
			break;

		case GLFW_KEY_RIGHT:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				float moveAngle = glm::radians(flyRotation);
				bool canMove = true;

				// Comprobación para el extremo derecho
				if (posX >= 15.0f || posZ >= 15.0f)
				{
					if (isFlyFacingRight()) canMove = true;
					else canMove = false;
				}
				// Comprobación para el extremo izquierdo
				else if (posX <= -15.0f || posZ <= -15.0f)
				{
					if (isFlyFacingRight()) canMove = false;
					else canMove = true;
				}
				if (canMove)
				{
					posZ += 0.05f * cos(moveAngle);
					posX += 0.05f * sin(moveAngle);
					moveLegs = true;
				}
			}
			else if (action == GLFW_RELEASE)
				moveLegs = false;
			break;
		case GLFW_KEY_1:
		case GLFW_KEY_2:
		case GLFW_KEY_3:
		case GLFW_KEY_4:
			if (action == GLFW_PRESS)
				lampStates[key - GLFW_KEY_1] = lampStates[key - GLFW_KEY_1] == 0.0f ? 1.0f : 0.0f;
			break;
		case GLFW_KEY_M:
			if (action == GLFW_PRESS)
				firstPersonView = !firstPersonView;
			break;
		case GLFW_KEY_D:
			flyRotation += 45.0f;
			if (flyRotation >= 360.0f) flyRotation -= 360.0f;
			break;
		case GLFW_KEY_I:
			flyRotation -= 45.0f;
			if (flyRotation < 0.0f) flyRotation += 360.0f;
			break;
		case GLFW_KEY_P:
			if (action == GLFW_PRESS)
				lightPOn = !lightPOn;
			break;
		case GLFW_KEY_L:
			if (action == GLFW_PRESS)
				rotateLight = !rotateLight;
			break;
		case GLFW_KEY_C:
			if (action == GLFW_PRESS)
				viewingSign = !viewingSign;
			break;
		default:
			break;
	}
}

void	funCursorPos(GLFWwindow* window, double xpos, double ypos)
{
	if (viewingSign || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) return;

	alphax = 90.0f * (2.0f * xpos / (float)w - 1.0f);
	alphay = 90.0f * (1.0f - 2.0f * ypos / (float)h);

	if (alphay > 89.9f) alphay = 89.9f;
	if (alphay < -89.9f) alphay = -89.9f;
}

void	funScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraDistance += yoffset;
	float minDistance = 5.0f;
	float maxDistance = 35.0f;

	if (cameraDistance < minDistance)
		cameraDistance = minDistance;
	if (cameraDistance > maxDistance)
		cameraDistance = maxDistance;
}
