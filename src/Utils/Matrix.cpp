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

void Matrix4x4::openGlArray(GLfloat (&array)[16])
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

Matrix4x4 Matrix4x4::DefaultMatrix()
{
	return Matrix4x4{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1,
	};
}




