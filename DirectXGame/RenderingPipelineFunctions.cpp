#include "RenderingPipelineFunctions.h"
#include "Matrix4x4.h"
#include "math.h"


RenderingPipelineFunctions::RenderingPipelineFunctions(){

}

RenderingPipelineFunctions::~RenderingPipelineFunctions(){

}

Matrix4x4 RenderingPipelineFunctions::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip){
	Matrix4x4 tmp;
	tmp.m[0][0] = 2.0f / (right - left);		 tmp.m[0][1] = 0;										  tmp.m[0][2] = 0;										 tmp.m[0][3] = 0;
	tmp.m[1][0] = 0;								 tmp.m[1][1] = 2.0f / (top - bottom);			  tmp.m[1][2] = 0;										 tmp.m[1][3] = 0;
	tmp.m[2][0] = 0;								 tmp.m[2][1] = 0;										  tmp.m[2][2] = 1.0f / (farClip - nearClip);	 tmp.m[2][3] = 0;
	tmp.m[3][0] =(left+right)/(left-right); tmp.m[3][1] = (top+bottom)/(bottom-top); tmp.m[3][2] = nearClip/(nearClip-farClip); tmp.m[3][3] = 1;
	return tmp;
}

Matrix4x4 RenderingPipelineFunctions::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip){
	Matrix4x4 tmp;
	tmp.m[0][0] = (1.0f / aspectRatio) * (cosf(fovY / 2.0f)/sinf(fovY / 2.0f));  tmp.m[0][1] = 0;								tmp.m[0][2] = 0;													  tmp.m[0][3] = 0;
	tmp.m[1][0] = 0;																					tmp.m[1][1] = (1.0f/tanf(fovY/2.0f)); tmp.m[1][2] = 0;													  tmp.m[1][3] = 0;
	tmp.m[2][0] = 0;																					tmp.m[2][1] = 0;								tmp.m[2][2] = farClip / (farClip - nearClip);				  tmp.m[2][3] = 1;
	tmp.m[3][0] = 0;																					tmp.m[3][1] = 0;								tmp.m[3][2] = (-nearClip*farClip)/(farClip-nearClip); tmp.m[3][3] = 0;
	return tmp;
}

Matrix4x4 RenderingPipelineFunctions::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth){
	Matrix4x4 tmp;
	tmp.m[0][0] = width/2.0f;														tmp.m[0][1] = 0;								tmp.m[0][2] = 0;													  tmp.m[0][3] = 0;
	tmp.m[1][0] = 0;																	tmp.m[1][1] = -(height/2.0f);				tmp.m[1][2] = 0;													  tmp.m[1][3] = 0;
	tmp.m[2][0] = 0;																	tmp.m[2][1] = 0;								tmp.m[2][2] = maxDepth - minDepth;					  tmp.m[2][3] = 0;
	tmp.m[3][0] = left+width/2.0f;												tmp.m[3][1] = top+height/2.0f;			tmp.m[3][2] = minDepth;										  tmp.m[3][3] = 1;
	return tmp;
}

