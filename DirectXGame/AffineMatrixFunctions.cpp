#include "AffineMatrixFunctions.h"
#include "Material.h"
#include "Vector3.h"
#include "cmath"

AffineMatrixFunctions::AffineMatrixFunctions() {}

AffineMatrixFunctions::~AffineMatrixFunctions() {}

Vector3 AffineMatrixFunctions::Add(Vector3 v1, Vector3 v2) {
	Vector3 tmp;
	tmp.x = v1.x += v2.x;
	tmp.y = v1.y += v2.y;
	tmp.z = v1.z += v2.z;
	return tmp;
}

Vector3 AffineMatrixFunctions::Subtract(Vector3 v1, Vector3 v2) {
	Vector3 tmp;
	tmp.x = v1.x -= v2.x;
	tmp.y = v1.y -= v2.y;
	tmp.z = v1.z -= v2.z;
	return tmp;
}

Vector3 AffineMatrixFunctions::TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}

Matrix4x4 AffineMatrixFunctions::Multiply(Matrix4x4 m1, Matrix4x4 m2) {
	Matrix4x4 tmp;
	tmp.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	              m1.m[0][3] * m2.m[3][0];
	tmp.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
	              m1.m[0][3] * m2.m[3][1];
	tmp.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
	              m1.m[0][3] * m2.m[3][2];
	tmp.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
	              m1.m[0][3] * m2.m[3][3];

	tmp.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	              m1.m[1][3] * m2.m[3][0];
	tmp.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
	              m1.m[1][3] * m2.m[3][1];
	tmp.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
	              m1.m[1][3] * m2.m[3][2];
	tmp.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
	              m1.m[1][3] * m2.m[3][3];

	tmp.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	              m1.m[2][3] * m2.m[3][0];
	tmp.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
	              m1.m[2][3] * m2.m[3][1];
	tmp.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
	              m1.m[2][3] * m2.m[3][2];
	tmp.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
	              m1.m[2][3] * m2.m[3][3];

	tmp.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	              m1.m[3][3] * m2.m[3][0];
	tmp.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
	              m1.m[3][3] * m2.m[3][1];
	tmp.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
	              m1.m[3][3] * m2.m[3][2];
	tmp.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
	              m1.m[3][3] * m2.m[3][3];
	return tmp;
}

Matrix4x4 AffineMatrixFunctions::MakeScaleMatrix(Vector3 scale) {
	Matrix4x4 tmp{
	    {scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1.0f}
    };
	return tmp;
}

Matrix4x4 AffineMatrixFunctions::MakeRotateMatrix(
    Matrix4x4 rotate1, Matrix4x4 rotate2, Matrix4x4 rotate3) {
	Matrix4x4 tmp;
	tmp = Multiply(rotate1, Multiply(rotate2, rotate3));
	return tmp;
}

Matrix4x4 AffineMatrixFunctions::MakeRotateXMatrix(float theta) {
	Matrix4x4 tmp{
	    {1, 0, 0, 0, 0, cosf(theta), sinf(theta), 0, 0, -sinf(theta), cosf(theta), 0, 0, 0, 0,
	     1.0f}
    };
	return tmp;
}

Matrix4x4 AffineMatrixFunctions::MakeRotateYMatrix(float theta) {
	Matrix4x4 tmp{
	    {cosf(theta), 0, -sinf(theta), 0, 0, 1, 0, 0, sinf(theta), 0, cosf(theta), 0, 0, 0, 0,
	     1.0f}
    };
	return tmp;
}

Matrix4x4 AffineMatrixFunctions::MakeRotateZMatrix(float theta) {
	Matrix4x4 tmp{
	    {cosf(theta), sinf(theta), 0, 0, -sinf(theta), cosf(theta), 0, 0, 0, 0, 1.0f, 0, 0, 0, 0,
	     1.0f}
    };
	return tmp;
}

Matrix4x4 AffineMatrixFunctions::MakeTranslateMatrix(Vector3 translate) {
	Matrix4x4 tmp{
	    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, translate.x, translate.y, translate.z, 1.0f}
    };

	return tmp;
}

Matrix4x4
    AffineMatrixFunctions::MakeAffinMatrix(Matrix4x4 Scale, Matrix4x4 Rotate, Matrix4x4 Translate) {
	Matrix4x4 tmp;
	tmp = Multiply(Scale, Multiply(Rotate, Translate));
	return tmp;
}

void AffineMatrixFunctions::show(
    Vector3 scale, float theta1, float theta2, float theta3, Vector3 translate) {
	Matrix4x4 tmp1;
	Matrix4x4 tmp2;
	Matrix4x4 tmp3;
	Matrix4x4 tmp4;
	Matrix4x4 tmp5;
	Matrix4x4 tmp6;
	Matrix4x4 tmp7;

	tmp1 = MakeScaleMatrix(scale);
	tmp2 = MakeRotateXMatrix(theta1);
	tmp3 = MakeRotateYMatrix(theta2);
	tmp4 = MakeRotateZMatrix(theta3);
	tmp5 = MakeTranslateMatrix(translate);
	tmp6 = MakeRotateMatrix(tmp2, tmp3, tmp4);
	tmp7 = MakeAffinMatrix(tmp1, tmp6, tmp5);
}