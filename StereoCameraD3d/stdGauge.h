#ifndef STD_GAUGE_H
#define STD_GAUGE_H 1

// geometry of standard gauge

#include "cvxD3d9.h"
#include "nvmath.h"

using nv::vec3f;

const int STDGAUGE_VERTEX_NUM = 72;

//draw a gauge with direction to (0, 0, 1);
class StdGauge
{	
	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;      // The untransformed, 3D position for the vertex
		FLOAT nx, ny, nz;

		CUSTOMVERTEX()
		{
			x = 0;
			y = 0;
			z = 0;
			nx = 0;
			ny = 0.0;
			nz = -1.0;
		}
	};

	// vertex positions
	CUSTOMVERTEX m_torusGauge[(STDGAUGE_VERTEX_NUM+2)*2];      //2D totus, the base of the gauge 
	CUSTOMVERTEX m_thickLineTube[(STDGAUGE_VERTEX_NUM+2)*2];   //3D tube,  the stick of the gauge

	D3DCOLOR m_diffuse;   //color
	DWORD    m_vertexFVF;	

public:
	StdGauge();
	void setColorIdx(int idx );
	void setGauge(float radius = 0.1, float small_radius = 0.08, float stick_length = 1.5);
	void initMaterial(IDirect3DDevice9 * dev);  //set a point light for gauge	
	void DrawGauge(IDirect3DDevice9 * dev, bool isDrawDisk);  //draw a gauge 
};

#endif