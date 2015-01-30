#include "cvxImage.h"
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include "ut.h"
#include <conio.h>

using namespace std;
using namespace cv;
using cvx::Rgb16UImage;
using cvx::Rgb32FImage;
using cvx::Bw64FImage;
using cvx::Argb8UImage;
using cvx::Bw32FImage;
using cvx::Bw8UImage;
using cvx::Rgb8UImage;

#define  MAIN_HERE 0
#if MAIN_HERE
//test local shading model
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	if (!AllocConsole())
	{
		MessageBox(NULL,
			L"Create console window failed!",
			L"Console",
			MB_ICONEXCLAMATION | MB_OK);

	}
	_cprintf("Open a console!\n");

//	UT_STEREO_CAMERA_D3D::test_RecoverNormal();
//	UT_STEREO_CAMERA_D3D::test_SceneInput();
//	UT_STEREO_CAMERA_D3D::test_matrix3x();
//	UT_STEREO_CAMERA_D3D::test_rotateByAxis();
//	UT_STEREO_CAMERA_D3D::test_experimentItemIO();
//	UT_STEREO_CAMERA_D3D::test_FileIO();
//	UT_STEREO_CAMERA_D3D::test_RotateNormal();
	UT_STEREO_CAMERA_D3D::test_ReferenceFrame();

	cvWaitKey(0);

	if (!FreeConsole())
	{
		MessageBox(NULL,
			L"Free console window failed!",
			L"Console",
			MB_ICONEXCLAMATION | MB_OK);
	}	
}
#endif

