# OpenGL_Shadings

This project shows how to perform the Polygon shading, Gouraud shading and Phong shading of some 3D models using OpenGL.

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
2. C:\Program Files\Microsoft Visual Studio 10.0\VC\include\GL
	1. GL.H
	2. GLAux.h
	3. glew.h
	4. GLU.H
	5. glui.h
	6. glut.
	7. glxew.h
	8. wglew.h
3. C:\Windows\System32
	1. glu32.dll
	2. glut32.dll

The list above shows how to create a local installation of OpenGl. If you want to use OpenGL just for this project, include the **glu32.dll** and **glut32.dll** in the folder of the generated executable.

## Code


