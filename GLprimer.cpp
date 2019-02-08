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

//Declarations



/*
 * main(argc, argv) - the standard C++ entry point for the program
 */
int main(int argc, char *argv[]) {

    using namespace std;

    //Declarations
	int width, height;
    Shader myShader;
    GLfloat Tt[16]; GLfloat Tx[16]; //Temporary matrices
    GLfloat Ty[16]; //Temporary matrices
    GLfloat T[16]; GLfloat T2[16]; //Object mat
    GLfloat P[16]; //Perspective
    GLfloat MV[16]; //Modelview matrix
    GLint location_T;
    GLint location_P;
    GLint location_MV;
    float time;
    GLint location_time;
    TriangleSoup myShape;

    MouseRotator myMouseRotator;
    KeyTranslator myKeyTranslator;

    Ball myBall = Ball(0.0, 0.0, 1.0, 6.0, 0.0, 0.0);


    //Constant Matrices (Not animated)
    Utilities::mat4perspective(P, M_PI/4, 1, 0.1, 100.0);
    Utilities::mat4translate(T2, 0.0, 0.0, -3.0);
    //Utilities::mat4translate(MV, 0.0, 0.0, 0.0);

    //Utilities::mat4mult(T, T2, T);*/
    //Utilities::mat4print(T);

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
    window = glfwCreateWindow(vidmode->height/2, vidmode->height/2, "GLprimer", NULL, NULL);
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
    myShape.createSphere(0.5f, 6);


    myShader.createShader("vertex.glsl", "fragment.glsl");

    //Skicka variabler till shaders
	location_time = glGetUniformLocation(myShader.programID, "time");
	if(location_time == -1) {
        cout << "Unable to locate variable 'time' in shader!" << endl;
	}
	location_T = glGetUniformLocation(myShader.programID, "T");
	location_P = glGetUniformLocation(myShader.programID, "P");
	location_MV = glGetUniformLocation(myShader.programID, "MV");


    // Show some useful information on the GL context
    cout << "GL vendor:       " << glGetString(GL_VENDOR) << endl;
    cout << "GL renderer:     " << glGetString(GL_RENDERER) << endl;
    cout << "GL version:      " << glGetString(GL_VERSION) << endl;
    cout << "Desktop size:    " << vidmode->width << "x" << vidmode->height << " pixels" << endl;

    glfwSwapInterval(0); // Do not wait for screen refresh between frames


    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Get window size. It may start out different from the requested
        // size, and will change if the user resizes the window.
        glfwGetWindowSize( window, &width, &height );
        // Set viewport. This is the pixel rectangle we want to draw into.
        glViewport( 0, 0, width, height ); // The entire window

        //Display FPS
        Utilities::displayFPS(window);

		// Set the clear color and depth, and clear the buffers for drawing
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        //Interaction
        myMouseRotator.poll(window);
        myKeyTranslator.poll(window);
        Utilities::mat4rotx(Tx, myMouseRotator.theta);
        Utilities::mat4roty(Ty, myMouseRotator.phi);
        Utilities::mat4translate(Tt, myKeyTranslator.tran_x, myKeyTranslator.tran_y, myKeyTranslator.tran_z-3.0);

        //MV Controls the camera, FPS-style
        //I ordning: Translatera från origo, rotera, translatera med knappar
        Utilities::mat4mult(Tx,Ty,MV);
        Utilities::mat4mult(MV,T2,MV);
        Utilities::mat4mult(Tt,MV,MV);


        /* ---- Rendering code should go here ---- */
        time = (float)glfwGetTime();    //Number of seconds since program started
        glUseProgram(myShader.programID);   //Activate the shader to set its variable
        glUniform1f(location_time, time); //Copy value to shader

        //Send projection and modelview matrices
        glUniformMatrix4fv(location_P, 1, GL_FALSE, P);
        glUniformMatrix4fv(location_MV, 1, GL_FALSE, MV);

        //Send to shaders and render for object 1
        Utilities::mat4translate(T, 0.0, 0.0, 0.0);
        glUniformMatrix4fv(location_T, 1, GL_FALSE, T);
        myShape.render();

        //Update object matrix and send to shaders and render for object 2
        Utilities::mat4translate(T, 1.0, 0.0, 0.0);
        glUniformMatrix4fv(location_T, 1, GL_FALSE, T);
        myShape.render();

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

