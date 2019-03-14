



How to run the program under Windows using OpenGL and GLFW. Please follow the instructions below exactly. All the steps are required. If you just want to see the simulation, use the GLprimer.exe.

1. Start Code::Blocks.

2. In the menu, select File→New→Project... click Empty project, click Next, name the project, specify the folder where you want your project files, click Next and Finish. The project file is created and saved.

3. Place all the .cpp, .hpp and .glsl files from the material and the folders GL, GLFW, meshes and textures (along with their contents) in the newly created project folder.

4. Right click on the project name in the left hand panel. Select Add Files..., select all files with extensions .hpp, .cpp and .glsl in the folder, and click Open

5. Select Project→Build Options..., select the tab Linker Settings. Under Link libraries, click Add, type glfw3 and click OK. Click Add again, type opengl32 and click OK. Under Other linker options, type -mwindows -mconsole. This will make both a graphics window and a text console appear when you run the program. The console is useful for displaying debugging output. In the text field in the tab Search Directories→Compiler, type a dot (.), and in the field under the tab Search Directories→Linker, type .\GLFW (dot, backslash, capital ”GLFW”). This will make the compiler look for header files and library files also in the local project directory and not just in the compiler’s own directories.

6. Open the file named ”GLPrimer”, build and run the program. 
7. Press 'A' to start.

8. To start the simulation, press “A” to make the cue ball move.
