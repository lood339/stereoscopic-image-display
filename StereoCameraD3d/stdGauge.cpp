#include "stdGauge.h"

StdGauge::StdGauge()
{
	m_vertexFVF = (D3DFVF_XYZ | D3DFVF_NORMAL);  //(D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_DIFFUSE);
	m_diffuse = D3DCOLOR_ARGB(0x00, 0xFF, 0, 0);
}

void StdGauge::initMaterial(IDirect3DDevice9 * dev)  //set a point light for gauge
{	
	// Set up a material. The material here just has the diffuse and ambient
    // colors set to yellow. Note that only one material can be used at a time.
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    dev->SetMaterial( &mtrl );

    // Set up a white, directional light, with an oscillating direction.
    // Note that many Lights may be active at a time (but each one slows down
    // the rendering of our scene). However, here we are just using one. Also,
    // we need to set the D3DRS_LIGHTING renderstate to enable lighting
	//no light but red color
	/*
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
	*/

    // Finally, turn on some ambient light.
	
    dev->SetRenderState( D3DRS_AMBIENT, m_diffuse);
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}
void StdGauge::setColorIdx(int idx )
{
	switch(idx)
	{
	case 0:
		m_diffuse = D3DCOLOR_ARGB(0x00, 0xFF, 0, 0);
		break;
	case 1:
		m_diffuse = D3DCOLOR_ARGB(0x00, 0x0, 0xFF, 0x0);
		break;
	case 2:
		m_diffuse = D3DCOLOR_ARGB(0x00, 0x0, 0x0, 0xFF);
		break;
	default:
		m_diffuse = D3DCOLOR_ARGB(0x00, 0xFF, 0, 0);
		break;		
	}
}

void StdGauge::setGauge(float radius, float small_radius_ratio, float stick_length )
{
	float edgeAngle = (float)((2*NV_PI) / STDGAUGE_VERTEX_NUM);

	//totus
	float rd1 = radius;
	float rd2 = radius * 0.8f;
	vec3f u = vec3f(1.0, 0.0, 0.0);
	vec3f v = vec3f(0.0, 1.0, 0.0);
	for(int i=0; i<STDGAUGE_VERTEX_NUM+2; i++)
	{		
		//X(t) = C + (r*cos(t))*U + (r*sin(t))*V
		float theta = (i%STDGAUGE_VERTEX_NUM) * edgeAngle;
		nv::vec3f t1 = rd1 * (cos(theta) * u + sin(theta) * v);
		nv::vec3f t2 = rd2 * (cos(theta) * u + sin(theta) * v);

		//Compute X and Y locations		
		m_torusGauge[2*i].x = t2.x;
		m_torusGauge[2*i].y = t2.y;
		m_torusGauge[2*i].z = t2.z;

		int j = 2*i+1;
		m_torusGauge[j].x = t1.x;
		m_torusGauge[j].y = t1.y;
		m_torusGauge[j].z = t1.z;
	}

	//line
	vec3f pos1 = vec3f(0.0, 0.0, 0.0);
	vec3f pos2 = vec3f(0.0, 0.0, 1.0) * radius * stick_length + pos1;
	float r_small = radius * small_radius_ratio;
	for(int i=0; i<STDGAUGE_VERTEX_NUM+2; i++)
	{		
		//X(t) = C + (r*cos(t))*U + (r*sin(t))*V
		float theta = (i%STDGAUGE_VERTEX_NUM) * edgeAngle;
		nv::vec3f t1 = pos1 + r_small * (cos(theta) * u + sin(theta) * v);
		nv::vec3f t2 = pos2 + r_small * (cos(theta) * u + sin(theta) * v);

		//Compute X and Y locations		
		m_thickLineTube[2*i].x = t2.x;
		m_thickLineTube[2*i].y = t2.y;
		m_thickLineTube[2*i].z = t2.z;

		int j = 2*i+1;
		m_thickLineTube[j].x = t1.x;
		m_thickLineTube[j].y = t1.y;
		m_thickLineTube[j].z = t1.z;
	}

}

void StdGauge::DrawGauge(IDirect3DDevice9 * dev, bool isDrawDisk)
{
	if(isDrawDisk)
	{
		dev->SetFVF(m_vertexFVF);	
		dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (STDGAUGE_VERTEX_NUM+2)*2 - 2, &m_torusGauge[0], sizeof(m_torusGauge[0]));
	}
	
	dev->SetFVF(m_vertexFVF);	
	dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (STDGAUGE_VERTEX_NUM+2)*2 - 2, &m_thickLineTube[0], sizeof(m_thickLineTube[0]));		
}
	
