#ifndef CVX_SURFACE_H
#define CVX_SURFACE_H 1

//bind stereo images to IDirect3DSurface9 in D3D 9

#include "cvxImage.h"


struct IDirect3DDevice9;
struct IDirect3DSurface9;

//stereo image
class CvxStereoSurface
{
	cv::Mat    m_leftImage;
	cv::Mat    m_rightImage;
	cv::Mat    m_monoImage;   //half size of left image
	
public:
	RECT               m_rect;
	IDirect3DSurface9 *m_surface;

public:
	CvxStereoSurface(const string &lFile, const string &rFile);	
	~CvxStereoSurface();

	void updateImage(const string &lFile, const string &rFile);
	void updateImage(const cv::Mat &lImage, const cv::Mat &rImage);

	void loadSurface(IDirect3DDevice9 *device, bool isFullScreen);	
	void releaseSurface();

private:
	void loadStereoSurface(IDirect3DDevice9 *device);  //load left and right image to a IDirect3DSurface9 to show stereo image
	void loadMonoSurface(IDirect3DDevice9 *device);    //load mid and mid image to a IDirect3DSurface9 to show monocular image
};




#endif