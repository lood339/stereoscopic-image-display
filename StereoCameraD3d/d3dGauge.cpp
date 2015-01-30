#include "d3dGauge.h"
#include "nvmath.h"

using nv::vec3f;

D3DGauge::D3DGauge()
{
	m_vertexFVF = (D3DFVF_XYZ | D3DFVF_NORMAL);  //(D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_DIFFUSE);
	m_diffuse = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);
}
void D3DGauge::CreateCircle()
{
	const float radius = 1.0f;
	const float PI = 3.14159f;
	int i;	
	float X;	
	float Y;	
	float Theta;	
	float WedgeAngle;	//Size of angle between two points on the circle (single wedge)	
	//Pre-compute WedgeAngle	
	WedgeAngle = (float)((2*PI) / D3DGAUGE_VERTEX_NUM);

	m_vertex[0].x = 0;
	m_vertex[0].y = 0;
	m_vertex[0].z = 0.0f;
//	m_vertex[0].diffuse = D3DCOLOR_ARGB(0xFF, 0, 0, 0xFF);
	//Set up vertices for a circle	
	//Used <= in the for statement to ensure last point meets first point (closed circle)	
	for(i=0; i<D3DGAUGE_VERTEX_NUM; i++)
	{		
		//Calculate theta for this vertex		
		Theta = (i) * WedgeAngle;				
		//Compute X and Y locations		
		X = (float)(radius * cos(Theta));		
		Y = (float)(radius * sin(Theta));		
		m_vertex[i+1].x = X;
		m_vertex[i+1].y = Y;
		m_vertex[i+1].z = 0.0f;
//		m_vertex[i+1].diffuse = D3DCOLOR_ARGB(0xFF, 0, 0, 0xFF);
	}
	// repeat start point
	m_vertex[D3DGAUGE_VERTEX_NUM+1].x = radius;
	m_vertex[D3DGAUGE_VERTEX_NUM+1].y = 0;
	m_vertex[D3DGAUGE_VERTEX_NUM+1].z = 0.0f;
//	m_vertex[D3DGAUGE_VERTEX_NUM+1].diffuse = D3DCOLOR_ARGB(0xFF, 0, 0, 0xFF);

	//draw small needle
	m_line[0].x = 0;
	m_line[0].y = 0;
	m_line[0].z = -radius;
//	m_line[0].diffuse = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);
	//Set up vertices for a circle	
	//Used <= in the for statement to ensure last point meets first point (closed circle)	
	for(i=0; i<D3DGAUGE_VERTEX_NUM; i++)
	{		
		//Calculate theta for this vertex		
		Theta = (i) * WedgeAngle;				
		//Compute X and Y locations		
		X = (float)(0.1 * radius * cos(Theta));		
		Y = (float)(0.1 * radius * sin(Theta));		
		m_line[i+1].x = X;
		m_line[i+1].y = Y;
		m_line[i+1].z = 0.0f;
//		m_line[i+1].diffuse = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);
	}
	// repeat start point
	m_line[D3DGAUGE_VERTEX_NUM+1].x = 0.1f*radius;
	m_line[D3DGAUGE_VERTEX_NUM+1].y = 0;
	m_line[D3DGAUGE_VERTEX_NUM+1].z = 0.0f;
//	m_line[D3DGAUGE_VERTEX_NUM+1].diffuse = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);

}

void D3DGauge::Draw3DCircle(const vec3f &u, const vec3f &v, const vec3f &w, const vec3f &pos, float radius, IDirect3DDevice9 * dev)
{
	float WedgeAngle = (float)((2*NV_PI) / D3DGAUGE_VERTEX_NUM);	//Size of angle between two points on the circle (single wedge)	

	D3DCOLOR color = D3DCOLOR_ARGB(0xFF, 0, 0, 0xFF);

	m_orientationGauge[0].x = pos.x;
	m_orientationGauge[0].y = pos.y;
	m_orientationGauge[0].z = pos.z;
//	m_orientationGauge[0].diffuse = color;
	//Set up vertices for a circle	
	//Used <= in the for statement to ensure last point meets first point (closed circle)	
	for(int i=0; i<D3DGAUGE_VERTEX_NUM; i++)
	{		
		//X(t) = C + (r*cos(t))*U + (r*sin(t))*V
		float theta = (i) * WedgeAngle;
		nv::vec3f t = pos + radius * cos(theta) * u + radius * sin(theta) * v;
		//Compute X and Y locations		
		m_orientationGauge[i+1].x = t.x;
		m_orientationGauge[i+1].y = t.y;
		m_orientationGauge[i+1].z = t.z;
//		m_orientationGauge[i+1].diffuse = color;
	}
	// repeat start point
	m_orientationGauge[D3DGAUGE_VERTEX_NUM+1].x = m_orientationGauge[1].x;
	m_orientationGauge[D3DGAUGE_VERTEX_NUM+1].y = m_orientationGauge[1].y;
	m_orientationGauge[D3DGAUGE_VERTEX_NUM+1].z = m_orientationGauge[1].z;
//	m_orientationGauge[D3DGAUGE_VERTEX_NUM+1].diffuse = color;

	//draw the 3D circle
	dev->SetFVF(m_vertexFVF);	
	dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, D3DGAUGE_VERTEX_NUM, &m_orientationGauge[0], sizeof(m_orientationGauge[0]));  //D3DPT_TRIANGLEFAN  D3DPT_LINESTRIP
}

void D3DGauge::Draw3DTorus(const vec3f &u, const vec3f &v, const vec3f &w, const vec3f &pos, float rd1, float rd2, IDirect3DDevice9 * dev)
{
	//data m_torusGauge[D3DGAUGE_VERTEX_NUM*2];
	float WedgeAngle = (float)((2*NV_PI) / D3DGAUGE_VERTEX_NUM);

//	D3DCOLOR color = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);

	for(int i=0; i<D3DGAUGE_VERTEX_NUM+2; i++)
	{		
		//X(t) = C + (r*cos(t))*U + (r*sin(t))*V
		float theta = (i%D3DGAUGE_VERTEX_NUM) * WedgeAngle;
		nv::vec3f t1 = pos + rd1 * cos(theta) * u + rd1 * sin(theta) * v;
		nv::vec3f t2 = pos + rd2 * cos(theta) * u + rd2 * sin(theta) * v;

		//Compute X and Y locations		
		m_torusGauge[2*i].x = t2.x;
		m_torusGauge[2*i].y = t2.y;
		m_torusGauge[2*i].z = t2.z;
//		m_torusGauge[2*i].diffuse = m_diffuse;

		int j = 2*((i+1)%(D3DGAUGE_VERTEX_NUM+2))+1;
		m_torusGauge[j].x = t1.x;
		m_torusGauge[j].y = t1.y;
		m_torusGauge[j].z = t1.z;
//		m_torusGauge[j].diffuse = m_diffuse;
	}

	dev->SetFVF(m_vertexFVF);	
	dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (D3DGAUGE_VERTEX_NUM+2)*2 - 2, &m_torusGauge[0], sizeof(m_torusGauge[0]));
}

void D3DGauge::DrawThickLine(const vec3f &u, const vec3f &v, const vec3f &w, const vec3f &pos1, const vec3f &pos2, float radius, IDirect3DDevice9 * dev)
{
	// CUSTOMVERTEX m_thickLineTube[(D3DGAUGE_VERTEX_NUM+2)*2];
	float WedgeAngle = (float)((2*NV_PI) / D3DGAUGE_VERTEX_NUM);

//	D3DCOLOR color = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);

	for(int i=0; i<D3DGAUGE_VERTEX_NUM+2; i++)
	{		
		//X(t) = C + (r*cos(t))*U + (r*sin(t))*V
		float theta = (i%D3DGAUGE_VERTEX_NUM) * WedgeAngle;
		nv::vec3f t1 = pos1 + radius * cos(theta) * u + radius * sin(theta) * v;
		nv::vec3f t2 = pos2 + radius * cos(theta) * u + radius * sin(theta) * v;

		//Compute X and Y locations		
		m_thickLineTube[2*i].x = t2.x;
		m_thickLineTube[2*i].y = t2.y;
		m_thickLineTube[2*i].z = t2.z;
//		m_thickLineTube[2*i].diffuse = m_diffuse;

		int j = 2*((i+1)%(D3DGAUGE_VERTEX_NUM+2))+1;
		m_thickLineTube[j].x = t1.x;
		m_thickLineTube[j].y = t1.y;
		m_thickLineTube[j].z = t1.z;
//		m_thickLineTube[j].diffuse = m_diffuse;
	}

	dev->SetFVF(m_vertexFVF);	
	dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (D3DGAUGE_VERTEX_NUM+2)*2 - 2, &m_thickLineTube[0], sizeof(m_thickLineTube[0]));

}

void D3DGauge::Draw3DCircleLine(const vec3f &u, const vec3f &v, const vec3f &w, const vec3f &pos, float radius, IDirect3DDevice9 * dev)
{
	float WedgeAngle = (float)((2*NV_PI) / D3DGAUGE_VERTEX_NUM);;	//Size of angle between two points on the circle (single wedge)	

	//Used <= in the for statement to ensure last point meets first point (closed circle)	
	for(int i=0; i<D3DGAUGE_VERTEX_NUM; i++)
	{		
		//X(t) = C + (r*cos(t))*U + (r*sin(t))*V
		float theta = i * WedgeAngle;
		nv::vec3f t = pos + radius * cos(theta) * u + radius * sin(theta) * v;
		//Compute X and Y locations		
		m_orientationGauge[i].x = t.x;
		m_orientationGauge[i].y = t.y;
		m_orientationGauge[i].z = t.z;
//		m_orientationGauge[i].diffuse = D3DCOLOR_ARGB(0xFF, 0, 0, 0xFF);
	}
	// repeat start point
	m_orientationGauge[D3DGAUGE_VERTEX_NUM].x = m_orientationGauge[0].x;
	m_orientationGauge[D3DGAUGE_VERTEX_NUM].y = m_orientationGauge[0].y;
	m_orientationGauge[D3DGAUGE_VERTEX_NUM].z = m_orientationGauge[0].z;
//	m_orientationGauge[D3DGAUGE_VERTEX_NUM].diffuse = D3DCOLOR_ARGB(0xFF, 0, 0, 0xFF);

	//draw the 3D circle
	dev->SetFVF(m_vertexFVF);	
	dev->DrawPrimitiveUP(D3DPT_LINESTRIP, D3DGAUGE_VERTEX_NUM, &m_orientationGauge[0], sizeof(m_orientationGauge[0]));  //D3DPT_TRIANGLEFAN  D3DPT_LINESTRIP

}


void D3DGauge::Draw3DGauge(const vec3f &normal, IDirect3DDevice9 * dev, const vec3f &pos, float radius)
{
	//draw local orth coordinate
	vec3f n = normal;	
	n = nv::normalize(n);
	//choose a axis with least dot product (the angle is the largest)
	float nx = fabs(n.x);
	float ny = fabs(n.y);
	float nz = fabs(n.z);

	vec3f ax;  //axis that has the largest angle with normal
	if(nx < ny && nx < nz)
	{
		ax = vec3f(1.0, 0, 0);
	}
	else if(ny < nx && ny < nz)
	{
		ax = vec3f(0, 1.0, 0);
	}
	else
	{
		ax = vec3f(0, 0.0, 1.0);
	}
	
	vec3f W, U, V;
	W = n;
	U = nv::cross(W, ax);
	U = nv::normalize(U);
	V = nv::cross(U, W);
	V = nv::normalize(V);

	//U V W as X Y Z coordinate
	//draw circle in the local area
//	Draw3DCircleLine(U, V, W, pos, radius, dev);
//	Draw3DCircle(U, V, W, pos, radius, dev);
	Draw3DTorus(U, V, W, pos, radius, radius * 0.85f, dev);

	vec3f startPos = pos;
	vec3f endPos = normal * radius * 1.4f + pos;
	DrawThickLine(U, V, W, startPos, endPos, radius * 0.08f, dev);
	
}


void D3DGauge::Draw2DCircle(IDirect3DDevice9 * dev)
{
	dev->SetFVF(m_vertexFVF);	
	dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, D3DGAUGE_VERTEX_NUM, &m_vertex[0], sizeof(m_vertex[0]));  //D3DPT_TRIANGLEFAN  D3DPT_LINESTRIP


	dev->SetFVF(m_vertexFVF);	
	dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, D3DGAUGE_VERTEX_NUM, &m_line[0], sizeof(m_line[0]));
}
void D3DGauge::setColorIdx(int idx)
{
	switch(idx)
	{
	case 0:
		m_diffuse = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);
		break;
	case 1:
		m_diffuse = D3DCOLOR_ARGB(0xFF, 0x0, 0xFF, 0x0);
		break;
	case 2:
		m_diffuse = D3DCOLOR_ARGB(0xFF, 0x0, 0x0, 0xFF);
		break;
	default:
		m_diffuse = D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0);
		break;		
	}

}
/*
void D3DGauge::init_light(IDirect3DDevice9 * dev)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;  // D3DLIGHT_DIRECTIONAL  D3DLIGHT_POINT
	// make the light type 'point light'
	light.Ambient = D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f);
	light.Position = D3DXVECTOR3(0.0f, -0.0f, -5.0f); 
	light.Range = 100.0f;    
	// a range of 100    
	light.Attenuation0 = 0.0f;    
	// no constant inverse attenuation    
	light.Attenuation1 = 0.125f;    // only .125 inverse attenuation    
	light.Attenuation2 = 0.0f;    
//	dev->SetLight(0, &light);    
//	dev->LightEnable(0, TRUE);  
	dev->SetRenderState( D3DRS_LIGHTING, TRUE );

//	DWORD ambientColor = D3DCOLOR_ARGB(0xFF, 0xFF, 0x00, 0x00);
#define RED_COLOR	0x00FF0000

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	dev->SetMaterial(&mtrl);
	dev->SetRenderState(D3DRS_AMBIENT, RED_COLOR);
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	dev->SetRenderState( D3DRS_LIGHTING, TRUE );
}
*/

void D3DGauge::init_light(IDirect3DDevice9 * dev)
{
    // Set up a material. The material here just has the diffuse and ambient
    // colors set to yellow. Note that only one material can be used at a time.
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    dev->SetMaterial( &mtrl );

    // Set up a white, directional light, with an oscillating direction.
    // Note that many Lights may be active at a time (but each one slows down
    // the rendering of our scene). However, here we are just using one. Also,
    // we need to set the D3DRS_LIGHTING renderstate to enable lighting
    D3DXVECTOR3 vecDir;
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    vecDir = D3DXVECTOR3( cosf( timeGetTime() / 350.0f ),
                          1.0f,
                          sinf( timeGetTime() / 350.0f ) );
    D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction, &vecDir );
    light.Range = 1000.0f;
    dev->SetLight( 0, &light );
    dev->LightEnable( 0, TRUE );
    dev->SetRenderState( D3DRS_LIGHTING, TRUE );

    // Finally, turn on some ambient light.
	//0x00202020
    dev->SetRenderState( D3DRS_AMBIENT, 0x00FF2020 );
}


