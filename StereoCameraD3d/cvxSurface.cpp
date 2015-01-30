#include "cvxSurface.h"
#include "cvxD3d9.h"
#include "cvxDxut.h"
#include "nv_stereo_utils.h"
#include <conio.h>
#include "cvxImage.h"
using cvx::Argb8UImage;



/************************************************************************/
/* 
	CvxStereoSurface
*/
/************************************************************************/


CvxStereoSurface::CvxStereoSurface(const string &lFile, const string &rFile)
{
	m_surface = NULL;

	updateImage(lFile, rFile);

	assert(m_leftImage.rows == m_rightImage.rows);
	assert(m_leftImage.cols == m_rightImage.cols);

	assert(m_leftImage.rows == 1080);
	assert(m_leftImage.cols == 1920);
}

CvxStereoSurface::~CvxStereoSurface()
{
	SAFE_RELEASE(m_surface);
}

void CvxStereoSurface::updateImage(const string &lFile, const string &rFile)
{
	m_leftImage = cv::imread(lFile, 1);
	assert(!m_leftImage.empty());

	m_rightImage = cv::imread(rFile, 1);
	assert(!m_rightImage.empty());

	m_monoImage = m_leftImage.clone();
	cv::resize(m_monoImage, m_monoImage, cv::Size(m_monoImage.cols/2, m_monoImage.rows/2));

	assert(m_leftImage.rows == m_rightImage.rows);
	assert(m_leftImage.cols == m_rightImage.cols);
}

void CvxStereoSurface::updateImage(const cv::Mat &lImage, const cv::Mat &rImage)
{
	assert(!lImage.empty());
	assert(!rImage.empty());

	m_leftImage = lImage.clone();
	m_rightImage = rImage.clone();

	m_monoImage = m_leftImage.clone();
	cv::resize(m_monoImage, m_monoImage, cv::Size(m_monoImage.cols/2, m_monoImage.rows/2));

	assert(m_leftImage.rows == m_rightImage.rows);
	assert(m_leftImage.cols == m_rightImage.cols);

	assert(m_leftImage.rows == 1080);
	assert(m_leftImage.cols == 1920);
}


void CvxStereoSurface::loadStereoSurface(IDirect3DDevice9 *device)
{
	HRESULT rt = device->CreateOffscreenPlainSurface(m_leftImage.cols*2, m_leftImage.rows, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_surface, NULL);
	assert(rt == D3D_OK);

	m_rect.left = 0;
	m_rect.top = 0;	
	m_rect.right = m_leftImage.cols;
	m_rect.bottom = m_leftImage.rows;

	rt = D3DXLoadSurfaceFromMemory(m_surface, NULL, &m_rect, m_leftImage.data, D3DFMT_R8G8B8, m_leftImage.step.p[0], NULL, &m_rect, D3DX_FILTER_NONE, D3DTEXF_NONE);
	assert(rt == D3D_OK);

	RECT dstRect;
	dstRect.left = m_leftImage.cols;
	dstRect.top  = 0;
	dstRect.right  = m_leftImage.cols * 2;
	dstRect.bottom = m_leftImage.rows;

	rt = D3DXLoadSurfaceFromMemory(m_surface, NULL, &dstRect, m_rightImage.data, D3DFMT_R8G8B8, m_rightImage.step.p[0], NULL, &m_rect, D3DX_FILTER_NONE, D3DTEXF_NONE);
	assert(rt == D3D_OK);

	//write magic number: key point

	// Lock the stereo image
	D3DLOCKED_RECT lr;
	m_surface->LockRect(&lr,NULL,0);
	// write stereo signature in the last raw of the stereo image
	LPNVSTEREOIMAGEHEADER pSIH =
		(LPNVSTEREOIMAGEHEADER)(((unsigned char *) lr.pBits) + (lr.Pitch * (m_leftImage.rows-1)));

	// Update the signature header values
	pSIH->dwSignature = NVSTEREO_IMAGE_SIGNATURE;
	pSIH->dwBPP = 32;
	pSIH->dwFlags = SIH_SWAP_EYES; // Src image has left on left and right on right, thats why this flag is not needed.  SIH_SCALE_TO_FIT  SIH_SWAP_EYES
	pSIH->dwWidth = m_leftImage.cols*2;
	pSIH->dwHeight = m_leftImage.rows;

	// Unlock surface
	m_surface->UnlockRect();

}
void CvxStereoSurface::loadMonoSurface(IDirect3DDevice9 *device)
{
	device->CreateOffscreenPlainSurface(m_monoImage.cols, m_monoImage.rows, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_surface, NULL);

	m_rect.left = 0;
	m_rect.top = 0;	
	m_rect.right = m_monoImage.cols;
	m_rect.bottom = m_monoImage.rows;

	D3DXLoadSurfaceFromMemory(m_surface, NULL, &m_rect, m_monoImage.data, D3DFMT_R8G8B8, m_monoImage.step.p[0], NULL, &m_rect, D3DX_FILTER_NONE, D3DTEXF_NONE);

}

void CvxStereoSurface::loadSurface(IDirect3DDevice9 *device, bool isFullscreen)
{
	assert(device);	

	if (isFullscreen)
	{
		loadStereoSurface(device);
	}
	else
	{
		loadMonoSurface(device);
	}
}

void CvxStereoSurface::releaseSurface()
{
	SAFE_RELEASE(m_surface);
}

