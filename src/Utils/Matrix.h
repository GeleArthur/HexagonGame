#pragma once
#include <iostream>
#include <SDL_opengl.h>

#include "Vector2d.h"


struct Matrix4x4
{
	float m00;
	float m10;
	float m20;
	float m30;
	float m01;
	float m11;
	float m21;
	float m31;
	float m02;
	float m12;
	float m22;
	float m32;
	float m03;
	float m13;
	float m23;
	float m33;

	float& operator[](int index);
	void OpenGlArray(GLfloat (&array)[16]);
	static Matrix4x4 IdentityMatrix();
	static Matrix4x4 TranslationMatrix(Vector2d position);
	static Matrix4x4 RotationMatrix(float rotation);

	void PrintMatrix(const Matrix4x4& matrix);
	Matrix4x4 operator*(const Matrix4x4& other) const;
};