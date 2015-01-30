#include "SampleItem.h"
#include "cvxDxut.h"
#include "nvmath.h"
#include <conio.h>
#include <assert.h>

SampleItem::SampleItem()
{
	m_x = 480;
	m_y = 270;
	
	m_gdNormal = vec3f(0, 0, -1.0);
	m_gdPosition = vec3f(0, 0, 8.0f);

	m_estimatedSlantAngle  = 0;
	m_estimatedRotationAngle = 0;

	m_gdSlantAngle = -1;
	m_gdRotationAngle = -1;
}

void SampleItem::setSampleCenter(int x, int y, bool fullScreen)
{
	if(!fullScreen)
	{
		x *= 2;
		y *= 2;
	}
	m_x = x;
	m_y = y;
	m_estimatedSlantAngle = (float)(rand()%90);
	m_estimatedRotationAngle = (float)(rand()%360);

}

void SampleItem::setGroundtruthNormal(const vec3f &n)
{
	m_gdNormal = n;
	m_gdSlantAngle      = (NV_PI - acos(n.z)) * 180 /NV_PI;
	m_gdRotationAngle   = (atan2(n.y, n.x) + NV_PI )* 180 /NV_PI;
}
void SampleItem::setGroundtruthPosition(const vec3f &p)
{
	m_gdPosition = p;
}
void SampleItem::addSlant()
{
	m_estimatedSlantAngle += 2;
	if(m_estimatedSlantAngle > 80)
	{
		m_estimatedSlantAngle = 80;
	}
}
void SampleItem::decreaseSlant()
{
	m_estimatedSlantAngle -= 2;
	if(m_estimatedSlantAngle < -80)
	{
		m_estimatedSlantAngle = -80;
	}	
}
void SampleItem::flipSlant()
{
	if(m_estimatedSlantAngle < 0)
	{
		m_estimatedSlantAngle = -m_estimatedSlantAngle;

	}
}

void SampleItem::adjustSlantByMouseMove(int mx, int my)
{
	if(m_gdRotationAngle >= 0 && m_gdRotationAngle <=90 ||
       m_gdRotationAngle >= 270 && m_gdRotationAngle <=360)
	{	
		mx = -mx;
	}

	m_estimatedSlantAngle += 1.0*mx/2.0;
	if(m_estimatedSlantAngle < -90)
	{
		m_estimatedSlantAngle = -90;
	}
	if(m_estimatedSlantAngle > 90)
	{
		m_estimatedSlantAngle = 90;
	}
//	_cprintf(" rotation angle is %f\n", m_gdRotationAngle);
}

void SampleItem::setRotationAngle(int x, int y, bool fullScreen)
{
	if(!fullScreen)
	{
		x *= 2;
		y *= 2;
	}
	if(abs( x - m_x ) <= 2 && abs( y - m_y ) <= 2)
	{
		_cprintf("mouse point too close to the gauge!!!\n");		
		return ;
	}	
	float dx = x - m_x;
	float dy = y - m_y;
	dy = -dy;            //coordinate of y is upside down

	float phi = atan2(dy, dx) + NV_PI;
	m_estimatedRotationAngle = (int)(phi * 180/NV_PI);
}
void SampleItem::setNormalByMouseMove(int deltaX, int deltaY)
{
//	_cprintf("delta x y is %d %d\n", deltaX, deltaY);
	//rotation matrix from mouse movement
	float angleX = 1.0 * deltaX/180 * NV_PI;
	float angleY = 1.0 * deltaY/180 * NV_PI;

	D3DXMATRIXA16 matRx;
	D3DXMATRIXA16 matRy;
	D3DXMATRIXA16 matRotation;

	D3DXMatrixRotationX( &matRx, angleY);
	D3DXMatrixRotationY( &matRy, angleX);	
	matRotation = matRx * matRy;

	nv::matrix3f r;
	r._11 = matRotation._11;
	r._12 = matRotation._12;
	r._13 = matRotation._13;

	r._21 = matRotation._21;
	r._22 = matRotation._22;
	r._23 = matRotation._23;

	r._31 = matRotation._31;
	r._32 = matRotation._32;
	r._33 = matRotation._33;

	nv::vec3f currentNormal = normal();
	currentNormal =  r * currentNormal;
	currentNormal = nv::normalize(currentNormal);

	//currentNormal to estimated slant and rotation angle
	m_estimatedSlantAngle      = (NV_PI - acos(currentNormal.z)) * 180 /NV_PI;
	m_estimatedRotationAngle   = (atan2(currentNormal.y, currentNormal.x) + NV_PI )* 180 /NV_PI;

	if(m_estimatedSlantAngle > 90) m_estimatedSlantAngle = 90;
	if(m_estimatedSlantAngle < -90) m_estimatedSlantAngle = -90;
	if(m_estimatedRotationAngle < 0) m_estimatedRotationAngle = 0;
	if(m_estimatedRotationAngle > 360) m_estimatedRotationAngle = 360;
}

float SampleItem::getAngularDifferenceIn3D()
{
	nv::vec3f currentNormal = normal();

	float angle = acos((double)nv::dot(currentNormal, m_gdNormal));
	angle = angle * 180 / NV_PI;
	return angle;
}



vec3f SampleItem::normal()
{
	assert(m_estimatedSlantAngle <= 90);
	assert(m_estimatedRotationAngle <= 360);		

//	_cprintf("slantA rotate A is %d %d\n", m_estimatedSlantAngle, m_estimatedRotationAngle);

	float theta = NV_PI - m_estimatedSlantAngle/180.0 * NV_PI;
	float phi =  m_estimatedRotationAngle/180.0 * NV_PI;

	vec3f n;
	n.x = sin(theta) * cos(phi);
	n.y = sin(theta) * sin(phi);
	n.z = cos(theta);

	n.x = - n.x;
	n.y = - n.y;
	n = nv::normalize(n);	

	return n;
}

vec3f SampleItem::normalWithSlant()
{
	assert(m_estimatedRotationAngle <= 360);		

	float theta = acos(m_gdNormal.z);  //theta from ground truth 
	float phi =  m_estimatedRotationAngle/180.0 * NV_PI;

	vec3f n;
	n.x = sin(theta) * cos(phi);
	n.y = sin(theta) * sin(phi);
	n.z = cos(theta);

	n.x = - n.x;
	n.y = - n.y;
	n = nv::normalize(n);
	return n;
}

vec3f SampleItem::normalWithRotate()
{
	assert(m_estimatedSlantAngle <= 90);
	assert(m_estimatedRotationAngle <= 360);		

//	_cprintf("slantA rotate A is %d %d\n", m_estimatedSlantAngle, m_estimatedRotationAngle);

	float theta = NV_PI - m_estimatedSlantAngle/180.0 * NV_PI;
	float phi = atan2(m_gdNormal.y, m_gdNormal.x) + NV_PI;

	vec3f n;
	n.x = sin(theta) * cos(phi);
	n.y = sin(theta) * sin(phi);
	n.z = cos(theta);

	n.x = - n.x;
	n.y = - n.y;
	n = nv::normalize(n);	
	return n;
}

void SampleItem::printAngles(bool slant, bool rotation)
{
	if(slant && rotation)
	{
		_cprintf("slant angle: %d %d\nrotation angle %d %d\n\n", (int)m_gdSlantAngle, (int)m_estimatedSlantAngle, (int)m_gdRotationAngle, (int)m_estimatedRotationAngle);
	}
	else if(slant)
	{
		_cprintf("slant angle: %d %d\n\n", (int)m_gdSlantAngle, (int)m_estimatedSlantAngle);
	}
	else if(rotation)
	{
		_cprintf("rotation angle %d %d\n\n", (int)m_gdRotationAngle, (int)m_estimatedRotationAngle);
	}
}