#ifndef CVX_DXUT_FRAME_H
#define CVX_DXUT_FRAME_H 1

//dxut framework: mouse, keyboard etc

#include "cvxDxut.h"
#include "cvxD3d9.h"
#include "nv_stereo_utils.h"
#include "cvxSurface.h"
#include "d3dCamera.h"
#include "d3dGauge.h"
#include "stdGauge.h"
#include "ImageModel.h"
#include <conio.h>
#include "UserInputUI.h"
#include "FileIO.h"
#include "cvxReferenceFrame.h"


class DxutStereoFrame
{
public:
	static CDXUTDialogResourceManager  s_dialogResourceManager;
	static CDXUTDialog                 s_controlPanel;                  // dialog for control panel
	static CDXUTDialog                 s_sampleUI;
	static CDXUTTextHelper*            s_pTxtHelper;

	static ID3DXFont*                  s_pFont9;	
	static ID3DXSprite*                s_pSprite9;

	static CvxStereoSurface *          s_pStereoSurface;      // map image to the surface
	
	static bool                        s_isFullScreen;       // full screen for stereo image display
	static bool						   s_isFinishTesting;
	static bool                        s_isShowGroudTruth;   // show ground truth for practice
	static UsertInputUI     *          s_userInput;          // user input for control panel
	static FileIO           *          s_fileIO;             // file input and output
	static int                         s_lastMousePosX;      // tracking previouse mouse position to adjust the gauge orientation
	static int                         s_lastMousePosY;

	
	static D3dStereoCamera*            s_pCamera;             // camera to display the gauge.
	static StdGauge       *            s_pstdGauge;           //gauge controled by users
	static StdGauge       *            s_groundTruthGauge;    //reference gauge showing ground truth 
	static ImageModel  *               s_imageModel;          //provides ground truth and save result
	static SampleItem  *               s_pSampleItem;         //position and normal values of current gauge
	static CvxReferenceFrame *         s_pEstimatedItem;      //not used, it is used to analyze the experiment result


public:
	/*
		load default parameter fro configuration
	*/
	static void InitApp();   
	
	// render number of total trials and un-test trials
	static void RenderImageCount(int num, int totalNum);

	static void RenderThanks();

	// render reminder of adjusting separation between to image conditions
	static void RenderSeparation();

	// render angular difference between ground truth and estimated angles, used in practice
	static void RenderAngleDifferenceReminder();

	// half of display resolution
	static int halfWidth();
	static int halfHeight();

	// open a single image condistion configuration file
	static bool OpenNewFile();

	// open a multiple image conditions configuration fiel
	static bool OpenNewFileWithMultipleTestCases();
	
	
	// callback function for mouse move, click
	static LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
									 void* pUserContext );

	// callback function for keyboard
	// space    : confirm estimation
	// page down: go to next image condition
	static void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );

	// mouse callback to show ground truth orientation
	static void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, 
								  bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );	

	// render callback function, it is executed very frame
	static void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );

	// not used
	static void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );

	// GUI event, it will be called when switching between full screen and half screen
	static void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

	// check hardware support
	static  bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
											      D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );	

	// call back functions from D3D device 
	static HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );	
	static HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice,  const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static void CALLBACK OnD3D9DestroyDevice( void* pUserContext);
	static void CALLBACK OnD3D9LostDevice( void* pUserContext );


private:
	// load a single image condition configuration file
	static bool LoadConfigFile(const string &fileName);


private:
	// never used
	static bool OpenSaveFile();




};


#endif