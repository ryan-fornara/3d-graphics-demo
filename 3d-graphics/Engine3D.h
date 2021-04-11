#pragma once
#include <SDL/SDL.h>
#include "Mat4x4.h"
#include "Mesh.h"

class Engine3D {
public: 
	Engine3D();

	void draw(double delta);
	bool init();

private:
	bool _initSDLWindow(int w, int h);
	void _drawTriangle(SDL_Renderer* _renderer, Triangle t);
	Vec3 multiplyMatrixVector(Vec3& i, Mat4x4& m)
	{
		Vec3 res;

		res.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		res.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		res.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			res.x /= w; 
			res.y /= w; 
			res.z /= w;
		}

		return res;
	}

	SDL_Renderer* _renderer;
	SDL_Window* _window;
	int _width;
	int _height;

	Mat4x4 _projectionMatrix;

	Mesh _cube;
	float _fTheta;
};