/*
 * A C++ framework for OpenGL programming in TNM046 for MT1 2014.
 *
 * This is a small, limited framework, designed to be easy to use
 * for students in an introductory computer graphics course in
 * the first year of a M Sc curriculum. It uses custom code
 * for some things that are better solved by external libraries
 * like GLEW and GLM, but the emphasis is on simplicity and
 * readability, not generality.
 * For the window management, GLFW 3.0 is used for convenience.
 * The framework should work in Windows, MacOS X and Linux.
 * Some Windows-specific stuff for extension loading is still
 * here. GLEW could have been used instead, but for clarity
 * and minimal dependence on other code, we rolled our own extension
 * loading for the things we need. That code is short-circuited on
 * platforms other than Windows. This code is dependent only on
 * the GLFW and OpenGL libraries. OpenGL 3.3 or higher is required.
 *
 * Author: Stefan Gustavson (stegu@itn.liu.se) 2013-2014
 * This code is in the public domain.
 */

// File and console I/O for logging and error reporting
#include <iostream>
#include <sstream>
#include <string>

// In MacOS X, tell GLFW to include the modern OpenGL headers.
// Windows does not want this, so we make this Mac-only.
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>

//More includes
#include "Utilities.hpp"
#include "Shader.hpp"
#include "TriangleSoup.hpp"
#include "Rotator.hpp"
#include "Texture.hpp"

//Declarations
void updateAndRender(float x, float y, float x_rot, float y_rot);
glm::vec2 *getBallPos(glm::vec2 ballPositions[]);
glm::vec2 *startVelocities(glm::vec2 ballVelocities[]);
void ballToBallCollision(glm::vec2 ballPos[], glm::vec2 ballVel[]);
void ballToBorderCollision(glm::vec2 ballPos[], glm::vec2 ballVel[]);
void updateTransFriction(glm::vec2 ballVelocities[], float dt);

GLfloat T[16]; //Object Matrix
GLfloat Trot[16]; //Object Rotation
GLfloat Trot_x[16];
GLfloat Trot_z[16];
GLfloat Trot_z_i[16];
float r = 0.0286;
//GLfloat Trot1[16];
GLint location_T; //Object translations
TriangleSoup myShape;
TriangleSoup poolTable;
Texture tex[16];

/*
 * main(argc, argv) - the standard C++ entry point for the program
 */
int main(int argc, char *argv[]) {

    using namespace std;

    //Declarations
	int width, height;
    Shader myShader;
    GLfloat Tt[16]; GLfloat Tx[16]; //Temporary matrices
    GLfloat T_table[16];
    GLfloat Ty[16]; //Temporary matrices
    GLfloat T2[16]; //Part of MV mat
    GLfloat P[16]; //Perspective
    GLfloat MV[16]; //Modelview matrix
    GLfloat Prot[16];

    float dt;

    //Start positions and start velocities
    glm::vec2 ballPositions[16];
    glm::vec2 ballVelocities[16];
    glm::vec2 ballRotations[16];
    getBallPos(ballPositions);
    startVelocities(ballVelocities);

    for(int i = 0; i < 16; ++i){
        ballRotations[i].x = ballRotations[i].y = 0.0f;
    }

    //Animation matrices
    GLint location_P;
    GLint location_MV;
    float time;
    GLint location_time;


    Texture tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7, tex8, tex9,
    tex10, tex11, tex12, tex13, tex14, tex15, texBord;

    GLint location_tex;

    MouseRotator myMouseRotator;
    KeyTranslator myKeyTranslator;

    //Constant Matrices (Not animated)
    Utilities::mat4perspective(P, M_PI/4, 1, 0.1, 100.0);
    Utilities::mat4translate(T2, -1.465f, -0.5, -3.0);

    const GLFWvidmode *vidmode;  // GLFW struct to hold information about the display
	GLFWwindow *window;    // GLFW struct to hold information about the window

    // Initialise GLFW
    glfwInit();

    // Determine the desktop size
    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// Make sure we are getting a GL context of at least version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //3 default
	// Exclude old legacy cruft from the context. We don't need it, and we don't want it.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a square window (aspect 1:1) to fill half the screen height
    window = glfwCreateWindow(vidmode->height, vidmode->height, "GLprimer", NULL, NULL);
    if (!window)
    {
        cout << "Unable to open window. Terminating." << endl;
        glfwTerminate(); // No window was opened, so we can't continue in any useful way
        return -1;
    }

    // Make the newly created window the "current context" for OpenGL
    // (This step is strictly required, or things will simply not work)
    glfwMakeContextCurrent(window);

    //Initiate interactions
    myMouseRotator.init(window);
    myKeyTranslator.init(window);

    Utilities::loadExtensions();

    //Create objects here
    myShape.createSphere(0.0286f, 32);
    poolTable.readOBJ("PoolTable/PoolTable2.obj");


    myShader.createShader("vertex.glsl", "fragment.glsl");

    //Create textures
    location_tex = glGetUniformLocation(myShader.programID, "tex");

    tex0.createTexture("textures/BallCue.tga");     tex[0] = tex0;
    tex1.createTexture("textures/Ball1.tga");       tex[1] = tex1;
    tex2.createTexture("textures/Ball2.tga");       tex[2] = tex2;
    tex3.createTexture("textures/Ball3.tga");       tex[3] = tex3;
    tex4.createTexture("textures/Ball4.tga");       tex[4] = tex4;
    tex5.createTexture("textures/Ball8.tga");       tex[5] = tex5;
    tex6.createTexture("textures/Ball6.tga");       tex[6] = tex6;
    tex7.createTexture("textures/Ball7.tga");       tex[7] = tex7;
    tex8.createTexture("textures/Ball5.tga");       tex[8] = tex8;
    tex9.createTexture("textures/Ball9.tga");       tex[9] = tex9;
    tex10.createTexture("textures/Ball10.tga");     tex[10] = tex10;
    tex11.createTexture("textures/Ball11.tga");     tex[11] = tex11;
    tex12.createTexture("textures/Ball12.tga");     tex[12] = tex12;
    tex13.createTexture("textures/Ball13.tga");     tex[13] = tex13;
    tex14.createTexture("textures/Ball14.tga");     tex[14] = tex14;
    tex15.createTexture("textures/Ball15.tga");     tex[15] = tex15;

    texBord.createTexture("textures/Bord.tga");

    //Skicka variabler till shaders
	location_time = glGetUniformLocation(myShader.programID, "time");
	if(location_time == -1) {
        cout << "Unable to locate variable 'time' in shader!" << endl;
	}
	location_P = glGetUniformLocation(myShader.programID, "P");
	location_MV = glGetUniformLocation(myShader.programID, "MV");
	location_T = glGetUniformLocation(myShader.programID, "T");


    // Show some useful information on the GL context
    cout << "GL vendor:       " << glGetString(GL_VENDOR) << endl;
    cout << "GL renderer:     " << glGetString(GL_RENDERER) << endl;
    cout << "GL version:      " << glGetString(GL_VERSION) << endl;
    cout << "Desktop size:    " << vidmode->width << "x" << vidmode->height << " pixels" << endl;

    glfwSwapInterval(0); // Do not wait for screen refresh between frames

     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     glEnable(GL_CULL_FACE);
     glEnable(GL_DEPTH_TEST);

    myShape.printInfo();
    poolTable.printInfo();

    float prev_time = 0.0f;

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Get window size. It may start out different from the requested
        // size, and will change if the user resizes the window.
        glfwGetWindowSize(window, &width, &height);
        // Set viewport. This is the pixel rectangle we want to draw into.
        glViewport(0, 0, width, height ); // The entire window

        //Display FPS
        Utilities::displayFPS(window);

		// Set the clear color and depth, and clear the buffers for drawing
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //Interaction, Camera
        myMouseRotator.poll(window);
        myKeyTranslator.poll(window);
        Utilities::mat4rotx(Tx, myMouseRotator.theta);
        Utilities::mat4roty(Ty, myMouseRotator.phi);
        Utilities::mat4translate(Tt, myKeyTranslator.tran_x, myKeyTranslator.tran_y, myKeyTranslator.tran_z);


        //MV Controls the camera, FPS-style
        //I ordning: Translatera från origo, rotera, translatera med knappar
        Utilities::mat4mult(Tx,Ty,MV);
        Utilities::mat4mult(MV,T2,MV);
        Utilities::mat4mult(MV,Tt,MV);


        /* ---- Rendering code should go here ---- */
        time = (float)glfwGetTime();    //Number of seconds since program started
        glUseProgram(myShader.programID);   //Activate the shader to set its variable
        glUniform1f(location_time, time); //Copy value to shader
        glUniform1i(location_tex, 0);

        //Send projection and modelview matrices
        glUniformMatrix4fv(location_P, 1, GL_FALSE, P);
        glUniformMatrix4fv(location_MV, 1, GL_FALSE, MV);


        ballToBorderCollision(ballPositions, ballVelocities);
        ballToBallCollision(ballPositions, ballVelocities);

        dt = time - prev_time; //Time passed since last iteration

        // Friction
        //updateTransFriction(ballVelocities, dt);

        //Render 16 objects
        for(unsigned int i = 0; i < 16; ++i){
            glBindTexture(GL_TEXTURE_2D, tex[i].textureID);
            ballPositions[i] += ballVelocities[i]*dt; //dt orsakar kollisionsfel, antagligen pga olika steglängd
            ballRotations[i] += ballVelocities[i]*dt/r;
            updateAndRender(ballPositions[i].x, ballPositions[i].y, ballRotations[i].x, ballRotations[i].y);
        }

        prev_time = time; //Set previous time to current time

        // Render Table
        Utilities::mat4rotx(T_table, M_PI/2);
        Utilities::mat4scale(Trot, 0.0195f);
        Utilities::mat4mult(T_table, Trot, Trot);
        Utilities::mat4translate(T, 1.065f, 0.5325f+0.05f, -0.530f);
        Utilities::mat4mult(T, Trot, T);
        glUniformMatrix4fv(location_T, 1, GL_FALSE, T);
        glBindTexture(GL_TEXTURE_2D, texBord.textureID);
        poolTable.render();

        // Textures for object 1
        glBindTexture(GL_TEXTURE_2D, 0);

        glUseProgram(0);

		// Swap buffers, i.e. display the image and prepare for next frame.
        glfwSwapBuffers(window);

		// Poll events (read keyboard and mouse input)
		glfwPollEvents();

        // Exit if the ESC key is pressed (and also if the window is closed).
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
          glfwSetWindowShouldClose(window, GL_TRUE);
        }

    }

    // Close the OpenGL window and terminate GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void updateAndRender(float x, float y, float x_rot, float y_rot){

    /*Utilities::mat4rotx(Trot_x, -y_rot);
    Utilities::mat4roty(Trot_z, x_rot);
    Utilities::mat4mult(Trot_z, Trot_x, Trot);*/

    float angle = x/sqrt(pow(x,2)+pow(y,2));
    float rot = sqrt(pow(x,2)+pow(y,2))/r;

    Utilities::mat4rotz(Trot_z, angle);
    Utilities::mat4rotx(Trot_x, -rot);
    Utilities::mat4rotz(Trot_z_i, -angle);

    Utilities::mat4mult(Trot_x, Trot_z, Trot);
    Utilities::mat4mult(Trot_z_i, Trot, Trot);


    Utilities::mat4translate(T, x, y, 0.0f);
    Utilities::mat4mult(T, Trot, T);
    glUniformMatrix4fv(location_T, 1, GL_FALSE, T);
    myShape.render();
}

glm::vec2 *getBallPos(glm::vec2 ballPositions[]) {

    float k = 0.014;

    ballPositions[0].x = (float)0.5325;       ballPositions[0].y = (float)0.5325;
    ballPositions[1].x = (float)1.5975-2*k;   ballPositions[1].y = (float)0.5325;
    ballPositions[2].x = (float)1.6470-k;     ballPositions[2].y = (float)0.5039-k;
    ballPositions[3].x = (float)1.6470-k;     ballPositions[3].y = (float)0.5611+k;
    ballPositions[4].x = (float)1.6965;       ballPositions[4].y = (float)0.4753-k;
    ballPositions[5].x = (float)1.6965;       ballPositions[5].y = (float)0.5325;
    ballPositions[6].x = (float)1.6965;       ballPositions[6].y = (float)0.5897+k;
    ballPositions[7].x = (float)1.7460+k;     ballPositions[7].y = (float)0.4467-2*k;
    ballPositions[8].x = (float)1.7460+k;     ballPositions[8].y = (float)0.5039-k;
    ballPositions[9].x = (float)1.7460+k;     ballPositions[9].y = (float)0.5611+k;
    ballPositions[10].x = (float)1.7460+k;    ballPositions[10].y = (float)0.6183+2*k;
    ballPositions[11].x = (float)1.7955+2*k;  ballPositions[11].y = (float)0.4181-2*k;
    ballPositions[12].x = (float)1.7955+2*k;  ballPositions[12].y = (float)0.4753-k;
    ballPositions[13].x = (float)1.7955+2*k;  ballPositions[13].y = (float)0.5325;
    ballPositions[14].x = (float)1.7955+2*k;  ballPositions[14].y = (float)0.5897+2*k;
    ballPositions[15].x = (float)1.7955+2*k;  ballPositions[15].y = (float)0.6469+2*k;

    return ballPositions;
}

glm::vec2 *startVelocities(glm::vec2 ballVelocities[]){
    //glm::vec2 ballPositions[17]; //16 Balls
    for(int i = 1; i < 16; ++i){
        ballVelocities[i].x = 0.0f;
        ballVelocities[i].y = 0.0f;
    }

    ballVelocities[0].x = 3.0f;
    ballVelocities[0].y = 0.0f;

    return ballVelocities;
}

/*----------------------------------/
/        ballToBallCollision        /
/----------------------------------*/
void ballToBallCollision(glm::vec2 ballPos[], glm::vec2 ballVel[]){
    // Auxiliary variables
    glm::vec2 temp;

    // temp variables to store constants
    float C1,C2,m1,m2,norm;
    float massWhiteBall = 0.170;
    float massRestOfBalls = 0.165;
    float sep = 0;

    // distance + separation
    float l = 2*r + sep;

    for(int i = 0; i < 15; ++i){
        for(int  j = i+1; j < 16; j++){

            if( i == 0){
                m1 = massWhiteBall;
                m2 = massRestOfBalls;
            }
            else{
                m1 = m2 = massRestOfBalls;
            }

            norm = distance(ballPos[i], ballPos[j]);

            // Check for collision
            if(norm  <= l){

                temp = ballPos[i] - ballPos[j];
                float length = sqrt(pow(temp.x,2) + pow(temp.y,2));

                // compute new direction
                C1 = (2*m2/(m1+m2))*dot((ballVel[i]-ballVel[j]) , (ballPos[i] - ballPos[j]))/pow(length,2);
                C2 = (2*m1/(m1+m2))*dot((ballVel[j]-ballVel[i]) , (ballPos[j] - ballPos[i]))/pow(length,2);

                // Update velocities
                ballVel[i] = ballVel[i] - C1*(ballPos[i] - ballPos[j]);
                ballVel[j] = ballVel[j] - C2*(ballPos[j] - ballPos[i]);
            }
        }
    }
}

/*----------------------------------/
/        ballToBorderCollision      /
/----------------------------------*/
void ballToBorderCollision(glm::vec2 ballPos[], glm::vec2 ballVel[]){

    for(int i = 0; i < 16; i++){

        // Table properties
        float xMaxLengthTable = 2.13+0.1;//+(2.13/2);
        float xMinLengthTable = 0+0.5;//+(2.13/2);
        float sep = 1e-3;
        float yMaxLengthTable = 1.065-0.125;//+(1.065/2);
        float yMinLengthTable = 0+0.19;//+(1.065/2);

        // Ball properties, r = radius;
        //float r = 0.0286;

        // Check if particles collided with the walls horizontally (x-direction)
          if ( ballPos[i].x + r >= xMaxLengthTable - sep ){

            // Formula to calculate velocity drop and new direction
            ballVel[i].x = -sqrt(0.75*pow(ballVel[i].x,2));
        }

        if ( ballPos[i].x - r <=  xMinLengthTable + sep ){

            // Formula to calculate velocity drop and new direction
            ballVel[i].x = sqrt(0.75*pow(ballVel[i].x,2));
        }

        if ( ballPos[i].y + r >= yMaxLengthTable - sep ){

            // Formula to calculate velocity drop and new direction
            ballVel[i].y = -sqrt(0.75*pow(ballVel[i].y,2));
        }

        if (  ballPos[i].y  - r <= yMinLengthTable + sep  ){

            // Formula to calculate velocity drop and new direction
            ballVel[i].y = sqrt(0.75*pow(ballVel[i].y,2));
        }
    }
}

/*----------------------------------/
/             Friction              /
/----------------------------------*/
void updateTransFriction(glm::vec2 ballVelocities[], float dt){

    const float EPSILON = 1.0e-5;
    float m = 0.165;
    float massWhiteBall = 0.170;
    glm::vec2 transFriction[16];
    float my = 0.1; // True friction for trans is 0.2

    for(int i = 0; i < 16; i++){

        //Adjust mass for the white ball
        if( i == 0 ) m = massWhiteBall;

        // compute friction
        transFriction[i].x = abs(ballVelocities[i].x*my*dt/m);
        transFriction[i].y = abs(ballVelocities[i].y*my*dt/m);

        // Determine working direction of the friction (opposite to the velocity).
        if( ballVelocities[i].x > EPSILON){
            ballVelocities[i].x = ballVelocities[i].x - transFriction[i].x;
        }

        if( ballVelocities[i].x < -EPSILON){
            ballVelocities[i].x = ballVelocities[i].x + transFriction[i].x;
        }

        if( ballVelocities[i].y > EPSILON){
            ballVelocities[i].y = ballVelocities[i].y - transFriction[i].y;
        }

        if( ballVelocities[i].y < -EPSILON){
            ballVelocities[i].y = ballVelocities[i].y + transFriction[i].y;
        }

    }
}
