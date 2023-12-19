#include "Matrix.h"

float & Matrix4x4::operator[](int index)
{
	switch (index)
	{
	case 0:
		return m00;
	case 1:
		return m10;
	case 2:
		return m20;
	case 3:
		return m30;
	case 4:
		return m01;
	case 5:
		return m11;
	case 6:
		return m21;
	case 7:
		return m31;
	case 8:
		return m02;
	case 9:
		return m12;
	case 10:
		return m22;
	case 11:
		return m32;
	case 12:
		return m03;
	case 13:
		return m13;
	case 14:
		return m23;
	case 15:
		return m33;
	default:
		throw;
	}
}

void Matrix4x4::OpenGlArray(GLfloat (&array)[16])
{
	array[0] =  m00;
	array[1] =  m01;
	array[2] =  m02;
	array[3] =  m03;
	
	array[4] =  m10;
	array[5] =  m11;
	array[6] =  m12;
	array[7] =  m13;
	
	array[8] =  m20;
	array[9] =  m21;
	array[10] = m22;
	array[11] = m23;
	
	array[12] = m30;
	array[13] = m31;
	array[14] = m32;
	array[15] = m33;
}

Matrix4x4 Matrix4x4::IdentityMatrix()
{
	return Matrix4x4{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1,
	};
}

Matrix4x4 Matrix4x4::TranslationMatrix(Vector2d position)
{
	Matrix4x4 result{IdentityMatrix()};
	result.m30 = position.x;
	result.m31 = position.y;

	return result;
}

Matrix4x4 Matrix4x4::RotationMatrix(float rotation)
{
	Matrix4x4 rotationMatrix{IdentityMatrix()};
	rotationMatrix.m00 = cos(rotation);
	rotationMatrix.m10 = -sin(rotation);
	rotationMatrix.m01 = sin(rotation);
	rotationMatrix.m11 = cos(rotation);
	
	return rotationMatrix;
}

void Matrix4x4::PrintMatrix(const Matrix4x4 &matrix)
{
	std::cout << matrix.m00 << " " << matrix.m01 << " " << matrix.m02 << " " << matrix.m03 << std::endl;
	std::cout << matrix.m10 << " " << matrix.m11 << " " << matrix.m12 << " " << matrix.m13 << std::endl;
	std::cout << matrix.m20 << " " << matrix.m21 << " " << matrix.m22 << " " << matrix.m23 << std::endl;
	std::cout << matrix.m30 << " " << matrix.m31 << " " << matrix.m32 << " " << matrix.m33 << std::endl;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const
{
	Matrix4x4 result;
	
	result.m00 = this->m00 * other.m00 + this->m01 * other.m10 + this->m02 * other.m20 + this->m03 * other.m30;
	result.m01 = this->m00 * other.m01 + this->m01 * other.m11 + this->m02 * other.m21 + this->m03 * other.m31;
	result.m02 = this->m00 * other.m02 + this->m01 * other.m12 + this->m02 * other.m22 + this->m03 * other.m32;
	result.m03 = this->m00 * other.m03 + this->m01 * other.m13 + this->m02 * other.m23 + this->m03 * other.m33;

	result.m10 = this->m10 * other.m00 + this->m11 * other.m10 + this->m12 * other.m20 + this->m13 * other.m30;
	result.m11 = this->m10 * other.m01 + this->m11 * other.m11 + this->m12 * other.m21 + this->m13 * other.m31;
	result.m12 = this->m10 * other.m02 + this->m11 * other.m12 + this->m12 * other.m22 + this->m13 * other.m32;
	result.m13 = this->m10 * other.m03 + this->m11 * other.m13 + this->m12 * other.m23 + this->m13 * other.m33;

	result.m20 = this->m20 * other.m00 + this->m21 * other.m10 + this->m22 * other.m20 + this->m23 * other.m30;
	result.m21 = this->m20 * other.m01 + this->m21 * other.m11 + this->m22 * other.m21 + this->m23 * other.m31;
	result.m22 = this->m20 * other.m02 + this->m21 * other.m12 + this->m22 * other.m22 + this->m23 * other.m32;
	result.m23 = this->m20 * other.m03 + this->m21 * other.m13 + this->m22 * other.m23 + this->m23 * other.m33;

	result.m30 = this->m30 * other.m00 + this->m31 * other.m10 + this->m32 * other.m20 + this->m33 * other.m30;
	result.m31 = this->m30 * other.m01 + this->m31 * other.m11 + this->m32 * other.m21 + this->m33 * other.m31;
	result.m32 = this->m30 * other.m02 + this->m31 * other.m12 + this->m32 * other.m22 + this->m33 * other.m32;
	result.m33 = this->m30 * other.m03 + this->m31 * other.m13 + this->m32 * other.m23 + this->m33 * other.m33;

	return result;
}




