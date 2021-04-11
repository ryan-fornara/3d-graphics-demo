#include "Engine3D.h"
#include <iostream>


Engine3D::Engine3D()
{
	_window = nullptr;
	_width = 500;
	_height = 500;
}

bool Engine3D::init()
{
	bool status = _initSDLWindow(_width, _height);

	//Build cube mesh
	_cube.tris = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
	};


	//Build Projection Matrix
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = _height / _width;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * (float)M_PI);

	_projectionMatrix.m[0][0] = fAspectRatio * fFovRad;
	_projectionMatrix.m[1][1] = fFovRad;
	_projectionMatrix.m[2][2] = fFar / (fFar - fNear);
	_projectionMatrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	_projectionMatrix.m[2][3] = 1.0f;
	_projectionMatrix.m[3][3] = 0.0f;


	return status;
}

bool Engine3D::_initSDLWindow(int w, int h)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//Create a new window
	SDL_Window* window = SDL_CreateWindow("3d Graphics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);

	//Kill if window fails to initialize
	if (!window) return 1;

	_renderer = SDL_CreateRenderer(window, -1, 0);

	return 0;
}

void Engine3D::draw(double delta)
{

	//Set the draw color...
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//Clear the screen.
	SDL_RenderClear(_renderer);

	// Set up rotation matrices
	Mat4x4 matRotZ, matRotX;
	_fTheta += 0.001f * (float)delta;

	// Rotation Z
	matRotZ.m[0][0] = cosf(_fTheta);
	matRotZ.m[0][1] = sinf(_fTheta);
	matRotZ.m[1][0] = -sinf(_fTheta);
	matRotZ.m[1][1] = cosf(_fTheta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(_fTheta * 0.5f);
	matRotX.m[1][2] = sinf(_fTheta * 0.5f);
	matRotX.m[2][1] = -sinf(_fTheta * 0.5f);
	matRotX.m[2][2] = cosf(_fTheta * 0.5f);
	matRotX.m[3][3] = 1;


	// Draw Triangles
	for (auto tri : _cube.tris)
	{
		Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

		// Rotate in Z-Axis
		triRotatedZ.p[0] = multiplyMatrixVector(tri.p[0], matRotZ);
		triRotatedZ.p[1] = multiplyMatrixVector(tri.p[1], matRotZ);
		triRotatedZ.p[2] = multiplyMatrixVector(tri.p[2], matRotZ);

		// Rotate in X-Axis
		triRotatedZX.p[0] = multiplyMatrixVector(triRotatedZ.p[0], matRotX);
		triRotatedZX.p[1] = multiplyMatrixVector(triRotatedZ.p[1], matRotX);
		triRotatedZX.p[2] = multiplyMatrixVector(triRotatedZ.p[2], matRotX);

		// Offset into the screen
		triTranslated = triRotatedZX;
		triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
		triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
		triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

		// Project triangles from 3D --> 2D
		triProjected.p[0] = multiplyMatrixVector(triTranslated.p[0], _projectionMatrix);
		triProjected.p[1] = multiplyMatrixVector(triTranslated.p[1], _projectionMatrix);
		triProjected.p[2] = multiplyMatrixVector(triTranslated.p[2], _projectionMatrix);

		// Scale into view
		triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
		triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
		triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
		triProjected.p[0].x *= 0.5f * (float)_width;
		triProjected.p[0].y *= 0.5f * (float)_height;
		triProjected.p[1].x *= 0.5f * (float)_width;
		triProjected.p[1].y *= 0.5f * (float)_height;
		triProjected.p[2].x *= 0.5f * (float)_width;
		triProjected.p[2].y *= 0.5f * (float)_height;

		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		// Rasterize triangle
		_drawTriangle(_renderer, triProjected);
	}

	SDL_RenderPresent(_renderer);
}


void Engine3D::_drawTriangle(SDL_Renderer* renderer, Triangle t)
{
	SDL_RenderDrawLine(renderer, (int)t.p[0].x, (int)t.p[0].y, (int)t.p[1].x, (int)t.p[1].y);
	SDL_RenderDrawLine(renderer, (int)t.p[1].x, (int)t.p[1].y, (int)t.p[2].x, (int)t.p[2].y);
	SDL_RenderDrawLine(renderer, (int)t.p[2].x, (int)t.p[2].y, (int)t.p[0].x, (int)t.p[0].y);
}