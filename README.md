# OpenGL_Shadings

This project shows how to perform some shadings (Polygon shading, Gouraud shading and Phong shading) with 3D models using OpenGL.

![demo](/images/demo01.gif?raw=true)

## Installation

There are two main steps to run the code of this project:
1. Copy the additional OpenGL libraries (added to the project) in their respective places as listed below.
2. Open the project in Visual Studio. 

### Libraries

You need to include the OpenGL libraries in the following locations:
1. C:\Program Files\Microsoft Visual Studio 10.0\VC\dll:
	1. glu32.dll
	2. glut32.dll
	3. opengl32.dll
2. C:\Program Files\Microsoft Visual Studio 10.0\VC\include\GL:
	1. GL.H
	2. GLAux.h
	3. glew.h
	4. GLU.H
	5. glui.h
	6. glut.
	7. glxew.h
	8. wglew.h
3. C:\Program Files\Microsoft Visual Studio 10.0\VC\lib:
	1. glew32.lib
	2. glew32d.lib
	3. glew32s.lib
	4. glfw3.lib
	5. glfw3dll.lib
	6. GLU32.LIB
	7. glui32.lib
	8. glut32.lib
4. C:\Windows\System32:
	1. glu32.dll
	2. glut32.dll

The list above shows how to create a local installation of OpenGl. If you want to use OpenGL just for this project, include the **glu32.dll** and **glut32.dll** in the folder of the generated executable.

![lib01](/images/lib/opengl_lib01.png?raw=true)

![lib02](/images/lib/opengl_lib02.png?raw=true)

![lib03](/images/lib/opengl_lib03.png?raw=true)

![lib04](/images/lib/opengl_lib04.png?raw=true)

## Keyboard controls

When running the actual control, a set of keyboard controls can be used:
	1. 'm': Change model from the list of models loaded by default.
	2. 's': Change the shading model applied in the selected 3D model.
	3. 'a': Activates the rotation movement of the model.
	4. '0': Camera zoom in.
	5. '9': Camera zoom out.

## Code

There are different parts of the code that are worth to mention.

### Keyboard options

The function below
```c++
void keyboard(unsigned char key,int x,int y)
```
includes all the controls that are available from the keyboard.

### Main

The Main code is composed as
```c++
int main(int argc,char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(60,50);
    glutInitWindowSize(900,700);
    glutCreateWindow("Shading examples");

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    
    myInit();
    glutMainLoop();
        
    return 0;
}
```
where the default OpenGL functions are included prior to the *myInit()* function. If you want to add custom code, please do so in the *myInit()* or after it.

### Visualization

When running the program, the defaul 3D models are loaded:
1. To change the displayed model, press 'm'.
2. To start and stop the rotation movement, press 'a'.

![demo2](/images/demo02.gif?raw=true)