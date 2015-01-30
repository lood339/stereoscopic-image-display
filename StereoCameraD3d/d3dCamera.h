#ifndef D3D_CAMERA_H
#define D3D_CAMERA_H 1

/************************************************************************/
/* 
	set camera parameters in D3D
	the parameters (normal position )is from the Blender

*/
/************************************************************************/
#include "cvxDxut.h"
#include "nvmath.h"

using nv::vec3f;
using nv::matrix3f;

class D3dStereoCamera
{

public:
	D3dStereoCamera();
	~D3dStereoCamera();

	//set the camera to specified normal direction and position (of gauge)
	void setRotationTranslationMatrix(IDirect3DDevice9* pDevice, const vec3f &normalDirection, const vec3f &position);


	// not used
	void setCamera(IDirect3DDevice9* pDevice);
	// not used
	void setCamera(float x, float y, float z, float nx, float ny, float nz, IDirect3DDevice9* pDevice);  //nx ny nz is normal angle in degree
	// not used
	void normalToRotateMatrix(D3DXVECTOR3 vNormal, IDirect3DDevice9* pDevice);
	// not used
	void identifyMatrix(IDirect3DDevice9* pDevice);
	//not used
	void setRotationTranslationMatrixWithEyeDisplacement(IDirect3DDevice9* pDevice, const vec3f &normal, const vec3f &pos, const float dx, const float dy, const float scale);

private:

	//axis must be normalized, angle is radian
	static matrix3f getRotationMatrixFromAxisAngle(const vec3f &axis, float angle);    //rotate with axis which passes origin
	static D3DXMATRIX* MatrixRotationY( D3DXMATRIX *pOut, FLOAT Angle );
	static D3DXMATRIX* MatrixRotationZ( D3DXMATRIX *pOut, FLOAT Angle );

};

#endif