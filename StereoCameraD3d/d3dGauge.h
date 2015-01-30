#ifndef D3D_GAUGE_H
#define D3D_GAUGE_H

#include "cvxD3d9.h"
#include "nvmath.h"

/*
replaced by stdGauge.
*/
using nv::vec3f;


const int D3DGAUGE_VERTEX_NUM = 72;
class D3DGauge
{
	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;      // The untransformed, 3D position for the vertex
		FLOAT nx, ny, nz;
//		D3DCOLOR diffuse;        // The vertex color

		CUSTOMVERTEX()
		{
			x = 0;
			y = 0;
			z = 0;
			nx = 0;
			ny = 0.0;
			nz = -1.0;
	//		diffuse = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);
		}
	};
	

private:
	CUSTOMVERTEX m_vertex[D3DGAUGE_VERTEX_NUM+2];  //2D gauge 
	CUSTOMVERTEX m_line[D3DGAUGE_VERTEX_NUM+2];    //2D gauge probe
	DWORD m_vertexFVF;

	CUSTOMVERTEX m_orientationGauge[D3DGAUGE_VERTEX_NUM+2];  //3D gauge

	CUSTOMVERTEX m_torusGauge[(D3DGAUGE_VERTEX_NUM+2)*2];      //2D totus
	CUSTOMVERTEX m_thickLineTube[(D3DGAUGE_VERTEX_NUM+2)*2];   //3D tube

	D3DCOLOR m_diffuse;




public:
	D3DGauge();
	void init_light(IDirect3DDevice9 * dev);  //set a point light for gauge
	void setColorIdx(int idx );
	void Draw3DGauge(const vec3f &normal, IDirect3DDevice9 * dev, const vec3f &pos = vec3f(0, 0, 0), float radius = 0.1);  //draw a gauge in the pos with a specified normal 

private:
	void CreateCircle();   //create a 2d circle
	void Draw2DCircle(IDirect3DDevice9 * dev); //draw a 2d circle
	void Draw3DCircle(const vec3f &u, const vec3f &v, const vec3f &w, const vec3f &pos, float radius, IDirect3DDevice9 * dev);  //draw a 3d circle from given local coordiante
	void Draw3DTorus(const vec3f &u, const vec3f &v, const vec3f &w, const vec3f &pos, float rd1, float rd2, IDirect3DDevice9 * dev);
	void DrawThickLine(const vec3f &u, const vec3f &v, const vec3f &w, const vec3f &startPos, const vec3f &endPos, float radius, IDirect3DDevice9 * dev);

	void Draw3DCircleLine(const vec3f &u, const vec3f &v, const vec3f &w, const vec3f &pos, float radius, IDirect3DDevice9 * dev);
};
#endif

/*
set alpha blender
		{
			// Enable alpha blending.
			pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			// Set the source blend state.
			pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);

			// Set the destination blend state.
			pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		}
*/