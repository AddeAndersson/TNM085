# Project: GLprimer
# Makefile created by Stefan Gustavson 2015-04-10

# Makefile for Windows mingw32, Linux and MacOSX (gcc environments)

CC   = gpp
OBJ  = GLprimer.o Rotator.o Texture.o TriangleSoup.o Shader.o Utilities.o 
INC  = -I.
OPT = -Wall -O3 -ffast-math -g3
APPNAME = GLprimer

Usage:
	@echo "Usage: make Win32 | Linux | MacOSX | clean | distclean"

GLprimer.o: GLprimer.cpp
	$(CC) $(OPT) $(INC) -c GLprimer.cpp -o GLprimer.o

Rotator.o: pollRotator.cpp
	$(CC) $(OPT) $(INC) -c Rotator.cpp -o Rotator.o

Texture.o: Texture.cpp
	$(CC) $(OPT) $(INC) -c Texture.cpp -o Texture.o

TriangleSoup.o: TriangleSoup.cpp
	$(CC) $(OPT) $(INC) -c  TriangleSoup.cpp -o TriangleSoup.o

Shader.o: Shader.cpp
	$(CC) $(OPT) $(INC) -c  Shader.cpp -o Shader.o

Utilities.o: Utilties.cpp
	$(CC) $(OPT) $(INC) -c  Utilities.cpp -o Utilitites.o

Win32: $(OBJ)
	$(CC) $(OBJ) -o $(APPNAME).exe -L./GLFW -mwindows -lglfw3 -lopengl32 -mconsole -g3

Linux: $(OBJ)
	$(CC) $(OBJ) -o $(APPNAME) -L./GLFW -lglfw3_linux -lGL -lm -lX11 -lXxf86vm -lXrandr -lXi

MacOSX: $(OBJ)
	$(CC) $(OBJ) -o $(APPNAME) -L./GLFW -lglfw3_macosx -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

clean:
	rm -f $(OBJ)

distclean:
	rm -rf $(OBJ) $(APPNAME) $(APPNAME).exe
