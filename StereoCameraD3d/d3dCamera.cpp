#include "d3dCamera.h"

D3dStereoCamera::D3dStereoCamera()
{

}

D3dStereoCamera::~D3dStereoCamera()
{

}

void D3dStereoCamera::setCamera(IDirect3DDevice9 *pDevice)
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = D3DX_PI  / 3.0f;
	D3DXMatrixRotationY( &matWorld, fAngle );
	pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -10.f);
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	pDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 960/540.0f, 1.0f, 100.0f );
	pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

void D3dStereoCamera::setCamera(float x, float y, float z, float nx, float ny, float nz, IDirect3DDevice9* pDevice)
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matT;
	D3DXMATRIXA16 matRx;
	D3DXMATRIXA16 matRy;
	D3DXMATRIXA16 matRz;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	nx *= D3DX_PI / 360.0f;
	ny *= D3DX_PI / 360.0f;
	nz *= D3DX_PI / 360.0f;
	D3DXMatrixRotationX( &matRx, nx);
	D3DXMatrixRotationY( &matRy, ny);
	D3DXMatrixRotationZ( &matRz, nz);
	D3DXMatrixTranslation(&matT, x, y, z);
	matWorld = matRx * matRy * matRz * matT;
	pDevice->SetTransform( D3DTS_WORLD, &matWorld );


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -10.f);
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	pDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 960/540.0f, 1.0f, 100.0f );
	pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

void D3dStereoCamera::normalToRotateMatrix(D3DXVECTOR3 vNormal, IDirect3DDevice9* pDevice)
{
	//vNormal is an input, and matR as output
	D3DXVECTOR3 vAxis;   
	D3DXVec3Cross( &vAxis, &vNormal, &D3DXVECTOR3( 0,1,0 ) );
	FLOAT fLen = D3DXVec3Length( &vAxis );
	D3DXMATRIXA16 matR;
	if( fLen == 0 ){
		// no rotation, return an identity matrix        
		D3DXMatrixIdentity( &matR );}
	else{        
		FLOAT fAngle = asinf( fLen );
		
		// return axis/angle rotation matrix using 'axis' and 'angle'        
		D3DXMatrixRotationAxis( &matR, &vAxis, fAngle );
	}
	pDevice->SetTransform( D3DTS_WORLD, &matR );	

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -10.f);
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	pDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 960/540.0f, 1.0f, 100.0f );
	pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

void D3dStereoCamera::identifyMatrix(IDirect3DDevice9* pDevice)
{
	//vNormal is an input, and matR as output
	
	D3DXMATRIXA16 matR;
//	D3DXMATRIXA16 matScale;
//	D3DXMATRIXA16 matObject;


	D3DXMatrixIdentity(&matR);
//	D3DXMatrixScaling(&matScale, 0.1, 0.1, 0.1);
//	matObject = matR * matScale;
	
	pDevice->SetTransform( D3DTS_WORLD, &matR );	

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f, 0.f);
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 10.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	pDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, (float)atan(9.0/35.0)*2, 960/540.0f, 1.0f, 100.0f );
	pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

void D3dStereoCamera::setRotationTranslationMatrix(IDirect3DDevice9* pDevice, const vec3f &normal, const vec3f &pos)
{
	//get rotation matrix
	float nx = normal.x;
	float ny = normal.y;
	float nz = normal.z;
	float theta = NV_PI - acos(nz);
	float phi   = atan2(ny, nx);
	
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matT;
	D3DXMATRIXA16 matRy;
	D3DXMATRIXA16 matRz;

	D3DXMatrixRotationY( &matRy, theta);
	D3DXMatrixRotationZ( &matRz, phi);	
	
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);
	matWorld = matRy * matRz * matT;

	pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f, 0.f);
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 10.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	pDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, (float)atan(9.0/35.0)*2, 960/540.0f, 1.0f, 100.0f );
	pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

void D3dStereoCamera::setRotationTranslationMatrixWithEyeDisplacement(IDirect3DDevice9* pDevice, const vec3f &normal, const vec3f &pos, const float dx, const float dy, const float scale)
{
	//get rotation matrix
	float nx = normal.x;
	float ny = normal.y;
	float nz = normal.z;
	float theta = NV_PI - acos(nz);
	float phi   = atan2(ny, nx);
	
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matT;
	D3DXMATRIXA16 matRy;
	D3DXMATRIXA16 matRz;
	D3DXMATRIXA16 matS;

	D3DXMatrixRotationY( &matRy, theta);
	D3DXMatrixRotationZ( &matRz, phi);	
	D3DXMatrixScaling(&matS, scale, scale, scale);
	
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);
	matWorld = matRy * matRz * matS * matT;

	pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( dx, dy, 0.f);
	D3DXVECTOR3 vLookatPt( dx, dy, 10.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	pDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, (float)atan(9.0/35.0)*2, 960/540.0f, 1.0f, 100.0f );
	pDevice->SetTransform( D3DTS_PROJECTION, &matProj );

}

D3DXMATRIX* D3dStereoCamera::MatrixRotationY( D3DXMATRIX *pOut, FLOAT Angle )
{
	//left handed otherwise exchange _13, _31
	pOut->_11 = cos(Angle);
	pOut->_13 = -sin(Angle);
	pOut->_31 = sin(Angle);
	pOut->_33 = cos(Angle);
	return pOut;
}

D3DXMATRIX* D3dStereoCamera::MatrixRotationZ( D3DXMATRIX *pOut, FLOAT Angle )
{
	//left haned
	pOut->_11 = cos(Angle);
	pOut->_12 = sin(Angle);
	pOut->_21 = -sin(Angle);
	pOut->_22 = cos(Angle);
	return pOut;
}

matrix3f D3dStereoCamera::getRotationMatrixFromAxisAngle(const vec3f &axis, float angle)
{
	//result is wrong

	float ax = axis.x;
	float ay = axis.y;
	float az = axis.z;

	matrix3f A_bar;
	matrix3f A_star;
	A_bar._11 = ax*ax;
	A_bar._12 = ax*ay;
	A_bar._13 = ax*az;

	A_bar._21 = ay*ax;
	A_bar._22 = ay*ay;
	A_bar._23 = ay*az;

	A_bar._31 = az*ax;
	A_bar._32 = az*ay;
	A_bar._33 = az*az;

	A_star._11 = 0;
	A_star._12 = -az;
	A_star._13 = ay;

	A_star._21 = az;
	A_star._22 = 0;
	A_star._23 = -ax;

	A_star._31 = -ay;
	A_star._32 = ax;
	A_star._33 = 0;

	matrix3f I_mat;
	matrix3f M = A_bar + cos(angle) * (I_mat - A_bar) + sin(angle)*A_star;
	M = M.transpose(M);

	return M;
}

