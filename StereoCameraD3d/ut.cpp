#include "ut.h"
#include "cvxImage.h"
#include "ImageModel.h"
#include <conio.h>
#include <fstream>
#include "nvmath.h"
#include "FileIO.h"
#include "cvxReferenceFrame.h"

using cvx::Argb8UImage;
using cvx::Rgb8UImage;
using nv::matrix3f;
using std::tr1::shared_ptr;
using namespace std;

int g_x = 0;
int g_y = 0;
ImageModel iModel;
void onMouse(int event, int x, int y, int flags, void* param )
{
	if (event == EVENT_LBUTTONDOWN)
	{
		g_x = x;
		g_y = y;

		vec3f n;
		n = iModel.calcNormalFromImage(x, y, true);
	//	iModel.getScaleNormal(x, y, nx, ny, nz);
		_cprintf("%f %f %f\n", n.x, n.y, n.z);
	}

}

void UT_STEREO_CAMERA_D3D::test_RecoverNormal(void)
{
//	ImageModel iModel;

	string imageName = string("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\mid.png");
	string depthMapName = string("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\depth.png");
	//string normalMapName = string("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\normal_front_face.png");
	//string normalMapName = string("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\normal_cube.png");
	//string normalMapName = string("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\head_normal.jpg");
	//string normalMapName = string("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\normal_2.png");
	string normalMapName = string("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\normal.png");
	string alphaMapName = string("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\alpha.png");

	float nx = 0.0f, ny = 0.0f, nz = 0.0f;
	int x = 480;
	int y = 270;

//	iModel.load(imageName, depthMapName, normalMapName, alphaMapName);
	iModel.calcNormalFromImage(x, y, true);
//	iModel.getPosition(x, y, 0.0, 6.0, px, py, pz);

	Mat nMap = cv::imread(normalMapName, 1);
//	cv::resize(nMap, nMap, cv::Size(960, 540));

	cv::imshow("normal", nMap);
	cv::setMouseCallback("normal", onMouse);

	while(true)
	{
		int key = cvWaitKey(0);

		
		if (key == 27)
		{
			break;
		}
	}


	_cprintf("%f %f %f\n", nx, ny, nz);	
}

void UT_STEREO_CAMERA_D3D::test_SceneInput(void)
{
	SceneConfig aScene;

	ifstream iFile("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\sphere_config.txt");

	iFile>>aScene;

	ofstream oFile("out.txt");
	oFile<<aScene;
}

void UT_STEREO_CAMERA_D3D::test_matrix3x(void)
{
	matrix3f a;
	matrix3f b;
	for(int i = 0; i<9; ++i)
	{
		a._array[i] = i+1.0f;
		b._array[i] = i+1.0f;
	}

	matrix3f c = a + b;
	matrix3f d = a - b;
	matrix3f e = 3.0 * a;	
	matrix3f f = e.transpose(e);

	cout<<a<<endl;
	getchar();
}
void UT_STEREO_CAMERA_D3D::test_rotateByAxis(void)
{

}

void UT_STEREO_CAMERA_D3D::test_experimentItemIO(void)
{
	string fileName = "E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\random_object_2_mask\\random_object_mask_2_config.txt";

	SceneConfig scene;
	ifstream ifile(fileName.c_str(), ios_base::in);
	ifile>>scene;  //read file

//	cout<<scene<<endl;
}

void UT_STEREO_CAMERA_D3D::test_FileIO()
{
	//jianhui_config.txt
	/*
	FileIO();
	~FileIO();

	bool loadConfigFile( const string &fileName);
	bool hasNextFile();
	string currentFileName();
	ofstream * getOfStream();
	void closeOutFileStream();
	*/

	FileIO fi;
	bool isOk = false;
	
	isOk = fi.loadConfigFile("jianhui_config.txt");
	ofstream *os = fi.getOfStream();
	assert(isOk);

	string currentFileName = string("");
	do
	{
		currentFileName = fi.currentFileName();
		(*os)<<currentFileName<<endl;
		_cprintf("file name is %s\n", currentFileName.c_str());
	}while(fi.hasNextFile());
	fi.closeOutFileStream();

	getchar();
}

void UT_STEREO_CAMERA_D3D::test_RotateNormal()
{
	int deltaX = 0;
	int deltaY = 0;
//	SampleItem::setNormalByMouseMove(deltaX, deltaY);

}

void UT_STEREO_CAMERA_D3D::test_ReferenceFrame()
{
	cv::Mat leftImage  = cv::imread("F:\\drop_box\\Dropbox\\file_exchange\\gauge_experiment_blender\\images\\practice_sphere\\left\\Image0001.png", 1);
	cv::Mat rightImage = cv::imread("F:\\drop_box\\Dropbox\\file_exchange\\gauge_experiment_blender\\images\\practice_sphere\\right\\Image0001.png", 1);

	assert(!leftImage.empty());
	assert(!rightImage.empty());

	shared_ptr<CvxReferenceFrame> pFrame (new CvxReferenceFrame());
	pFrame->loadImages(leftImage, rightImage);

}




