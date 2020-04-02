#include <windows.h>
#include <SDL.h> //check your include folder, it may be just SDL.h
#include <GL/gl.h>
#include <GL/glu.h>
#include <glfw3.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <SDL_opengl.h>


struct Coordinate
{
	float x;
	float y;
	float z;

	Coordinate(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

struct Face
{
	int faceNum;
	bool four;
	int faces[4];

	Face(int faceN, int f1, int f2, int f3) : faceNum(faceN)
	{

		faces[0] = f1;
		faces[1] = f2;
		faces[3] = f3;
		four = false;
	}
	Face(int faceN, int f1, int f2, int f3, int f4) : faceNum(faceN)
	{

		faces[0] = f1;
		faces[1] = f2;
		faces[3] = f3;
		faces[4] = f4;
		four = true;
	}
};

float angle = 0.0;
int loadOBJ(const char* fileName)
{
	std::vector<std::string*> coords;
	std::vector<Coordinate*> vertices;
	std::vector<Coordinate*> normals;
	std::vector<Face*> faces;
	std::ifstream in(fileName);
	if (!in.is_open())
		return -1;
	char buffer[256];
	while (!in.eof())
	{
		in.getline(buffer, 256);
		coords.push_back(new std::string(buffer));
	}
	for (size_t i = 0; i < coords.size(); i++)
	{
		if (coords[i]->c_str()[0] == '#')
			continue;
		else if (coords[i]->c_str()[0] == 'v' && coords[i]->c_str()[1] == ' ')
		{
			float tmpX, tmpY, tmpZ;
			sscanf_s(coords[i]->c_str(), "v %f %f %f", &tmpX, &tmpY, &tmpZ);
			vertices.push_back(new Coordinate(tmpX, tmpY, tmpZ));
		}
		else if (coords[i]->c_str()[0] == 'v' && coords[i]->c_str()[1] == 'n')
		{
			float tmpX, tmpY, tmpZ;
			sscanf_s(coords[i]->c_str(), "vn %f %f %f", &tmpX, &tmpY, &tmpZ);
			normals.push_back(new Coordinate(tmpX, tmpY, tmpZ));
		}
		else if (coords[i]->c_str()[0] == 'f')
		{
			int a, b, c, d, e;
			if (count(coords.begin(), coords.end(), ' ') == 3)
			{
				sscanf_s(coords[i]->c_str(), "f, %d//%d, %d//%d, %d//%d", &a, &b, &c, &b, &d, &b);
				faces.push_back(new Face(b, a, c, d));
			}
			else
			{
				sscanf_s(coords[i]->c_str(), "f, %d//%d, %d//%d, %d//%d, %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
				faces.push_back(new Face(b, a, c, d, e));
			}
		}
	}
	
	int	numId;
	numId = glGenLists(1);
	glNewList(numId, GL_COMPILE);  
	for (size_t i = 0; i < faces.size(); i++)
	{
		if (faces[i]->four)      //if it's a quad draw a quad
		{
			glBegin(GL_QUADS);
			glNormal3f(normals[faces[i]->faceNum - 1]->x, normals[faces[i]->faceNum - 1]->y, normals[faces[i]->faceNum - 1]->z);
			glVertex3f(vertices[faces[i]->faces[0] - 1]->x, vertices[faces[i]->faces[0] - 1]->y, vertices[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertices[faces[i]->faces[1] - 1]->x, vertices[faces[i]->faces[1] - 1]->y, vertices[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertices[faces[i]->faces[2] - 1]->x, vertices[faces[i]->faces[2] - 1]->y, vertices[faces[i]->faces[2] - 1]->z);
			glVertex3f(vertices[faces[i]->faces[3] - 1]->x, vertices[faces[i]->faces[3] - 1]->y, vertices[faces[i]->faces[3] - 1]->z);
			glEnd();
		}
		else {
			glBegin(GL_TRIANGLES);
			glNormal3f(normals[faces[i]->faceNum - 1]->x, normals[faces[i]->faceNum - 1]->y, normals[faces[i]->faceNum - 1]->z);
			glVertex3f(vertices[faces[i]->faces[0] - 1]->x, vertices[faces[i]->faces[0] - 1]->y, vertices[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertices[faces[i]->faces[1] - 1]->x, vertices[faces[i]->faces[1] - 1]->y, vertices[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertices[faces[i]->faces[2] - 1]->x, vertices[faces[i]->faces[2] - 1]->y, vertices[faces[i]->faces[2] - 1]->z);
			glEnd();
		}
	}
	glEndList();
	//delete everything to avoid memory leaks
	for (size_t i = 0; i < coords.size(); i++)
		delete coords[i];
	for (size_t i = 0; i < faces.size(); i++)
		delete faces[i];
	for (size_t i = 0; i < normals.size(); i++)
		delete normals[i];
	for (size_t i = 0; i < vertices.size(); i++)
		delete vertices[i];
	return numId;     
}

int obj;
void init()
{
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 640.0 / 480.0, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	obj = loadOBJ("D:/obj files/teapot.obj");
	glEnable(GL_LIGHTING);  
	glEnable(GL_LIGHT0);
	float col[] = { 1.0,1.0,1.0,1.0 };  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	float pos[] = { -1.0,1.0,-2.0,1.0 };        //set the position
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(angle, 1.0, 1.0, 1.0);
	//bool flag = glIsList(obj);
	glCallList(obj);       //draw the 3D mesh
}


int main(int argc, char** argv)
{
	GLFWwindow *window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(1440, 720, "window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	} 
	glfwMakeContextCurrent(window);
	bool running = true;
	Uint32 start;
	SDL_Event event;
	init();
	while (running)
	{
		//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
		display();
		glfwSwapBuffers(window);
		//glfwPollEvents();
		//SDL_RenderClear(renderer);
		angle += 0.5;
		if (angle > 360)
			angle -= 360;
		if (1000 / 30 > (SDL_GetTicks() - start))
			SDL_Delay(1000 / 30 - (SDL_GetTicks() - start));
	}
	glfwTerminate();
	//SDL_DestroyWindow(screen);
	//SDL_Quit();
	return 0;
}