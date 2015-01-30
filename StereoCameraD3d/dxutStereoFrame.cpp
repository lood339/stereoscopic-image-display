#include "DxutStereoFrame.h"
#include <iostream>
#include "cvxImage.h"
#include <conio.h>
#include "nvmath.h"
#include "deviceParameters.h"
#include <fstream>
#include <Commdlg.h>

using namespace std;
using nv::vec3f;

// initialize the static parameters
CDXUTDialogResourceManager  DxutStereoFrame::s_dialogResourceManager;
CDXUTDialog                 DxutStereoFrame::s_controlPanel;                  
CDXUTDialog                 DxutStereoFrame::s_sampleUI;

ID3DXFont*                  DxutStereoFrame::s_pFont9 = NULL;
CDXUTTextHelper*            DxutStereoFrame::s_pTxtHelper = NULL;
ID3DXSprite*                DxutStereoFrame::s_pSprite9 = NULL;
CvxStereoSurface *          DxutStereoFrame::s_pStereoSurface = NULL;
D3dStereoCamera*		    DxutStereoFrame::s_pCamera = NULL;
StdGauge       *            DxutStereoFrame::s_pstdGauge = NULL;
StdGauge       *            DxutStereoFrame::s_groundTruthGauge = NULL;
UsertInputUI   *            DxutStereoFrame::s_userInput = NULL;
bool                        DxutStereoFrame::s_isFullScreen = false;
ImageModel  *               DxutStereoFrame::s_imageModel = NULL;
SampleItem  *               DxutStereoFrame::s_pSampleItem = NULL;
FileIO *                    DxutStereoFrame::s_fileIO = NULL;
bool						DxutStereoFrame::s_isFinishTesting = false;
int							DxutStereoFrame::s_lastMousePosX = 0;
int							DxutStereoFrame::s_lastMousePosY = 0;
CvxReferenceFrame *         DxutStereoFrame::s_pEstimatedItem = NULL;
bool                        DxutStereoFrame::s_isShowGroudTruth = false;

#define IDC_TOGGLEFULLSCREEN    1
#define IDC_PEAK                2
#define IDC_VALLEY              3
#define IDC_DEBUGUI_DEPTH       4
#define IDC_DEBUGUI_NX          5
#define IDC_DEBUGUI_NY          6
#define IDC_DEBUGUI_NZ          7
#define IDC_SLANT_ANGLE         8
#define IDC_ROTATE_ANGLE        9
#define IDC_MODEL_SELECTION     10
#define IDC_SHOW_REFERENCE      11
#define IDC_OPEN_NEW_FILE       12
#define IDC_SAVE_RESULT_FILE    13
#define IDC_OPEN_MULTIPLE_NEW_FILE 14

//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------

void DxutStereoFrame::InitApp()
{

	s_userInput = new UsertInputUI;
	assert(s_userInput);

	s_controlPanel.Init( &DxutStereoFrame::s_dialogResourceManager );
	s_sampleUI.Init( &DxutStereoFrame::s_dialogResourceManager );

	s_controlPanel.SetCallback( OnGUIEvent ); int iY = 10;
	s_controlPanel.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen (F8)", 35, iY, 125, 22,  VK_F8);
	s_controlPanel.AddButton( IDC_OPEN_NEW_FILE, L"Open ...", 35, iY += 30, 125, 22, L'O', false, &s_userInput->_openNewFile );
	s_controlPanel.AddButton( IDC_OPEN_MULTIPLE_NEW_FILE, L"Open Mult...", 35, iY += 30, 125, 22, L'M', false, &s_userInput->_openMultipleNewFile );
	s_controlPanel.AddComboBox(IDC_MODEL_SELECTION, 35, iY += 30, 125, 22, L'O', false, &s_userInput->_modelCB);
	s_controlPanel.AddCheckBox(IDC_SHOW_REFERENCE, L"Reference (R).", 35, iY += 30, 125, 22, false, L'R', false,  &s_userInput->_showReference);
	

	if(s_userInput->_modelCB)
	{
		s_userInput->_modelCB->SetDropHeight( 50 );
		s_userInput->_modelCB->AddItem( L"Ground Truth",  &(s_userInput->models[0]));
		s_userInput->_modelCB->AddItem( L"Only Slant", &(s_userInput->models[2]));
		s_userInput->_modelCB->AddItem( L"Only Rotation", &(s_userInput->models[3]));
		s_userInput->_modelCB->AddItem( L"3D Rotation", &(s_userInput->models[4]));
	
	}
	s_sampleUI.SetCallback( OnGUIEvent ); iY = 10;

	//load a default configure file
//	string configFile("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\image_circle\\sphere_config.txt");
//	string configFile("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\monkey\\monkey_config.txt");
//	string configFile("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\monkey_transparent\\monkey_transparent_config.txt");
//	string configFile("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\random_obj_1\\random_obj_1_config.txt");
//	string configFile("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\random_object_2\\random_object_2_config.txt");
//	string configFile("E:\\code\\d3d_stereo_image\\StereoCameraD3d\\StereoCameraD3d\\random_object_2_mask\\random_object_mask_2_config.txt");
//	string configFile("F:\\drop_box\\Dropbox\\file_exchange\\gauge_experiment\\z_images\\practice_sphere\\config_bino.txt");
//	string configFile("F:\\drop_box\\Dropbox\\file_exchange\\gauge_experiment_blender\\images\\practice_sphere\\config_bino.txt");
	string configFile("F:\\drop_box\\Dropbox\\file_exchange\\gauge_experiment_hat\\images\\practice_sphere\\config_bino.txt");	

	// load default configure file
	bool isOk =  LoadConfigFile(configFile);

	if(!isOk)
	{
		_cprintf("can't open file %s, please open a new file.\n", configFile.c_str());
		isOk = OpenNewFile();
		return;
	}	
}

bool DxutStereoFrame::OpenNewFile()
{
	char szFile[1024] = {0};       // buffer for file name

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileNameA(&ofn))
	{
		string configFile(szFile);
		return LoadConfigFile(configFile);		
	}
	else
	{
		_cprintf("GetOpenFileName not return new file\n");
		return false;
	}
}

bool DxutStereoFrame::LoadConfigFile(const string &fileName)
{
	SceneConfig config;
	ifstream ifile(fileName.c_str(), ios_base::in);
	if(!ifile)
	{
		_cprintf("can't open file %s\n", fileName.c_str());
		return false;
	}
	ifile>>config;

	SAFE_DELETE(s_pStereoSurface);
	s_pStereoSurface = new CvxStereoSurface(config.m_leftName, config.m_rightName);

	//add stereo camera
	SAFE_DELETE(s_pCamera);
	s_pCamera = new D3dStereoCamera;
	assert(s_pCamera);
	
	//s_pGauge = new D3DGauge;
	SAFE_DELETE(s_pstdGauge);
	s_pstdGauge = new StdGauge;
	assert(s_pstdGauge);
	s_pstdGauge->setColorIdx(0);
	s_pstdGauge->setGauge(g_cur_gauge_size);

	SAFE_DELETE(s_groundTruthGauge);
	s_groundTruthGauge = new StdGauge;
	s_groundTruthGauge->setColorIdx(1);
	s_groundTruthGauge->setGauge(g_ground_truth_gauge_size);

	// image model
	SAFE_DELETE(s_imageModel);
	s_imageModel = new ImageModel;
	s_imageModel->loadConfig(fileName);

	SAFE_DELETE(s_pSampleItem);
	s_pSampleItem = new SampleItem;
	s_imageModel->getNextSampleItem(*s_pSampleItem);

	//clear stored estimated items
	SAFE_DELETE(s_pEstimatedItem);
	s_pEstimatedItem = new CvxReferenceFrame;
	assert(s_pEstimatedItem);
		
	_cprintf("load configure file %s\n", fileName.c_str());	
	s_isFinishTesting = false;
	return true;
}

bool DxutStereoFrame::OpenNewFileWithMultipleTestCases()
{
	char szFile[1024] = {0};       // buffer for file name

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileNameA(&ofn))
	{
		string fileName(szFile);
		if(s_fileIO)
		{
			s_fileIO->closeOutFileStream();
			SAFE_DELETE(s_fileIO);
		}
		s_fileIO = new FileIO;
		assert(s_fileIO);
		bool isOk = s_fileIO->loadConfigFile(fileName);
		assert(isOk);
		string configFile = s_fileIO->currentFileName();
		assert(configFile != string(""));

		s_isFinishTesting = false;
		return LoadConfigFile(configFile);
	}
	else
	{
		_cprintf("OpenNewFileWithMultipleTestCases not return new file\n");
		return false;
	}
}

bool DxutStereoFrame::OpenSaveFile()
{
	char szFile[1024] = {0};       // buffer for file name

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetSaveFileNameA(&ofn))
	{
		string oFileName(szFile);
		if(s_imageModel)
		{
			s_imageModel->saveResult(oFileName);
			_cprintf("save to file %s\n", szFile);
			return true;
		}	
	}
	_cprintf("unfinish save file.\n");
	return false;
}



void DxutStereoFrame::RenderImageCount(int num, int totalNum)
{
	WCHAR buf[256];
	swprintf_s(buf, 256, L"%d / %d", num, totalNum);
	s_pTxtHelper->Begin();
	s_pTxtHelper->SetInsertionPos( 5, 10 );
	s_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	s_pTxtHelper->DrawTextLine(buf);	
	s_pTxtHelper->End();
}


void DxutStereoFrame::RenderThanks()
{
	s_pTxtHelper->Begin();
	s_pTxtHelper->SetInsertionPos( 5, 5 );
	s_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	s_pTxtHelper->DrawTextLine(L"Thanks for participating the experiment!");
	s_pTxtHelper->End();
}

void DxutStereoFrame::RenderAngleDifferenceReminder()
{
	s_pTxtHelper->Begin();
	s_pTxtHelper->SetInsertionPos( 5, 5 );
	s_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	s_pTxtHelper->DrawTextLine(L"Error is large than 15 degree. Please re-adjust the disk again to the groud truth!");
	s_pTxtHelper->End();
}

void DxutStereoFrame::RenderSeparation()
{
	s_pTxtHelper->Begin();
	s_pTxtHelper->SetInsertionPos( 5, 5 );
	s_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	s_pTxtHelper->DrawTextLine(L"Ctrl F3 or Ctrl F4 to adjust seperations. After that, use page down to next scene!");
	s_pTxtHelper->End();
}

int DxutStereoFrame::halfWidth()
{
	return FULLSCREEN_WIDTH/2;
}

int DxutStereoFrame::halfHeight()
{
	return FULLSCREEN_HEIGHT/2;
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK DxutStereoFrame::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
									    void* pUserContext )
{
	// Pass messages to dialog resource manager calls so GUI state is updated correctly
	*pbNoFurtherProcessing = s_dialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;	


 	switch(uMsg)
 	{
	case WM_MOUSEMOVE :		
		 if(s_imageModel && s_userInput && s_userInput->isExperiment() && s_userInput->isMouseMoving())
		 {
			 int iMouseX = ( short )LOWORD( lParam );
			 int iMouseY = ( short )HIWORD( lParam );

			 if(s_userInput->is3DRotation())
			 {
				 int deltaX = iMouseX - s_lastMousePosX;
				 int deltaY = iMouseY - s_lastMousePosY;

				 s_pSampleItem->setNormalByMouseMove(deltaX, deltaY);
				 s_lastMousePosX = iMouseX;
				 s_lastMousePosY = iMouseY;
			 }
			 else if(s_userInput->isRotationChange())
			 {
				 s_pSampleItem->setRotationAngle(iMouseX, iMouseY, s_isFullScreen);
			 }
			 else if(s_userInput->isSlantChange())
			 {
				 int deltaX = iMouseX - s_lastMousePosX;
				 int deltaY = iMouseY - s_lastMousePosY;

				 s_pSampleItem->adjustSlantByMouseMove(deltaX, deltaY);
				 s_lastMousePosX = iMouseX;
				 s_lastMousePosY = iMouseY;
			 }
			 
			 s_pSampleItem->printAngles(s_userInput->isSlantChange(), s_userInput->isRotationChange());
		 }		 		 
		break;

	case WM_LBUTTONDOWN:
		if(s_imageModel && s_userInput && s_userInput->isExperiment())
		{
			int iMouseX = ( short )LOWORD( lParam );
			int iMouseY = ( short )HIWORD( lParam );	

			if(s_userInput->is3DRotation())
			{
				s_lastMousePosX = iMouseX;
				s_lastMousePosY = iMouseY;
				s_userInput->setMouseMoving();
			}
			else if(s_userInput->isRotationChange())
			{
				s_pSampleItem->flipSlant();			
				s_pSampleItem->setRotationAngle(iMouseX, iMouseY, s_isFullScreen);
				s_userInput->setMouseMoving();
			}
			else if(s_userInput->isSlantChange())
			{
				s_lastMousePosX = iMouseX;
				s_lastMousePosY = iMouseY;
				s_userInput->setMouseMoving();
			}
			
			s_pSampleItem->printAngles(s_userInput->isSlantChange(), s_userInput->isRotationChange());
		}
		break;

	case WM_LBUTTONUP:
		if(s_imageModel && s_userInput && s_userInput->isExperiment())
		{
			s_userInput->resetMouseMoving();
			
		}
		break;
 	}
	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = s_controlPanel.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;
	*pbNoFurtherProcessing = s_sampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	return 0;
}

void CALLBACK DxutStereoFrame::OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if (bKeyDown)
	{
		switch(nChar)
		{
				
			case VK_SPACE:
				{
					//user can't input data when adjust seperation
					if(s_userInput->isAdjustSeperation())
					{
						break;  
					}
					if(s_imageModel && s_userInput->isExperiment() && !s_userInput->_showReference->GetChecked())
					{
						s_imageModel->saveItem(*s_pSampleItem);
						s_imageModel->printCurrentResult(s_userInput->isSlantOnly(), s_userInput->isRotationOnly());

						// set n p to reference
						vec3f n = s_pSampleItem->normal();
						if(s_userInput->isSlantOnly())
						{
							n = s_pSampleItem->normalWithRotate();
						}
						else if(s_userInput->isRotationOnly())
						{
							n = s_pSampleItem->normalWithSlant();
						}	
						vec3f pos = s_pSampleItem->getGroundtruthPosition();
					
						if(s_pEstimatedItem)
						{
							s_pEstimatedItem->pushBackNormalPosition(n, pos);
							n = s_pSampleItem->getGroundtruthNormal();      //store groudtruth normal
							s_pEstimatedItem->pushBackGroundTruthNormal(n);
						}
						bool next = s_imageModel->getNextSampleItem(*s_pSampleItem);
						if(!next)
						{
							if(s_fileIO && s_imageModel && s_userInput->isTesting())
							{
								_cprintf("page down is pressed, save result and load new configure.\n");
								// save current result
								ofstream *os = s_fileIO->getOfStream();
								s_imageModel->saveResult(*os);

								// get next configure file
								bool hasNext = s_fileIO->hasNextFile();
								if(hasNext)
								{
									string fileName = s_fileIO->currentFileName();
									LoadConfigFile(fileName);
									s_userInput->nextSeperationState();
								}
								else
								{
									s_fileIO->closeOutFileStream();
									s_isFinishTesting = true;
								}						
							}
							else
							{
								s_isFinishTesting = true;
								s_imageModel->saveResult("most_current_result.txt");
							}
						}						
					}
					else if (s_imageModel && s_userInput->isExperiment() && s_userInput->_showReference->GetChecked()) // practice model
					{
					//	s_userInput->nextReferenceModel();
						if(s_userInput->is3DRotation())
						{
							float angle = s_pSampleItem->getAngularDifferenceIn3D();
							if(angle > 15.0)
							{
								_cprintf("angle difference is %f degree.\n", angle);
								s_userInput->setReference2ShowGroundtruthAndReminder();							
								break;	
							}
							else
							{
								s_userInput->setReference2ShowGroundTruth();
							}
						}
						bool isShowGroundTruth = s_userInput->isReferenceGroundTruth();
						
						//set reference as false
						if(isShowGroundTruth)
						{
							s_imageModel->saveItem(*s_pSampleItem);
							s_imageModel->printCurrentResult(s_userInput->isSlantOnly(), s_userInput->isRotationOnly());
							// set n p to reference	
							vec3f n = s_pSampleItem->normal();
							if(s_userInput->isSlantOnly())
							{
								n = s_pSampleItem->normalWithRotate();
							}
							else if(s_userInput->isRotationOnly())
							{
								n = s_pSampleItem->normalWithSlant();
							}	
							vec3f pos = s_pSampleItem->getGroundtruthPosition();
						
							if(s_pEstimatedItem)
							{
								s_pEstimatedItem->pushBackNormalPosition(n, pos);
								n = s_pSampleItem->getGroundtruthNormal();      //store groudtruth normal
								s_pEstimatedItem->pushBackGroundTruthNormal(n);
							}

							bool next = s_imageModel->getNextSampleItem(*s_pSampleItem);
							s_userInput->setReference2Practice();
							if(!next)
							{
								s_imageModel->saveResult("result.txt");
							}							
						}
					}
				}
				break;
			case VK_NEXT:
				{
					// get next configure from the s_fileIO					
					if(s_fileIO && s_imageModel && s_userInput->isAdjustSeperation())
					{
						s_userInput->nextSeperationState();
					}
				}
				break;
			case 0x41:  //A key
				{			
					if(s_pEstimatedItem)
					{
						s_pEstimatedItem->clear();
					}
				}
				break;
			default:
				break;
		}
	}
}

void CALLBACK DxutStereoFrame::OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, 
					  bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext )
{
	if(bLeftButtonDown)
	{
		if(s_imageModel && s_userInput && s_userInput->isShowGroundTruth())
		{
			vec3f normal, position;
			
			normal = s_imageModel->calcNormalFromImage(xPos, yPos, s_isFullScreen);			
			position = s_imageModel->calcPositionFromImage(xPos, yPos, s_isFullScreen);

			s_pSampleItem->setSampleCenter(xPos, yPos, s_isFullScreen);
			s_pSampleItem->setGroundtruthNormal(normal);
			s_pSampleItem->setGroundtruthPosition(position);

			_cprintf("normal is %f %f %f\n", normal.x, normal.y, normal.z);
			_cprintf("ground truth position is %f %f %f\n\n", position.x, position.y, position.z);
		}		
	}	
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK DxutStereoFrame::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK DxutStereoFrame::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
									 D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	// Skip backbuffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	// No fallback defined by this app, so reject any device that 
	// doesn't support at least ps2.0
	if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
		return false;

	return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK DxutStereoFrame::OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									void* pUserContext )
{
	HRESULT hr;

	V_RETURN( s_dialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
	//  V_RETURN( g_SettingsDlg.OnD3D9CreateDevice( pd3dDevice ) );

	V_RETURN( D3DXCreateFont( pd3dDevice, 30, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"Arial", &s_pFont9 ) );

	float fObjectRadius = 1.0;
	V_RETURN( CDXUTDirectionWidget::StaticOnD3D9CreateDevice( pd3dDevice ) );

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK DxutStereoFrame::OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice,
								   const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( s_dialogResourceManager.OnD3D9ResetDevice() );

	if( s_pFont9 ) V_RETURN( s_pFont9->OnResetDevice() );

	V_RETURN( D3DXCreateSprite( pd3dDevice, &s_pSprite9 ) );
	s_pTxtHelper = new CDXUTTextHelper( s_pFont9, s_pSprite9, NULL, NULL, 30 );


	s_controlPanel.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
	s_controlPanel.SetSize( 170, 170 );
	s_sampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 350 );
	s_sampleUI.SetSize( 170, 300 );


	_cprintf("this program don't support arbitrary resize window. Only support half screen and full screen with F8\n");
	
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK DxutStereoFrame::OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;

	// Clear the render target and the zbuffer 
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		
		// show pre-rendered images
		s_pStereoSurface->loadSurface(pd3dDevice, s_isFullScreen);				
		IDirect3DSurface9 *backBuf = NULL;
		pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuf);  //add reference number
		pd3dDevice->StretchRect(s_pStereoSurface->m_surface, NULL, backBuf, &s_pStereoSurface->m_rect, D3DTEXF_NONE);
		s_pStereoSurface->releaseSurface();
		backBuf->Release();	
		
		//show the gauges
		//show ground truth gauge
		if((s_pSampleItem && s_userInput && s_userInput->isShowGroundTruth()) || s_userInput->isAdjustSeperation())
		{
			nv::vec3f normal = s_pSampleItem->getGroundtruthNormal();
			nv::vec3f position = s_pSampleItem->getGroundtruthPosition();
			
			s_pCamera->setRotationTranslationMatrix(pd3dDevice, normal, position);
			s_pstdGauge->setColorIdx(0);
			s_pstdGauge->initMaterial(pd3dDevice);
			s_pstdGauge->DrawGauge(pd3dDevice, true);
		}
		//show user input gauge direction
		else if(s_pSampleItem && s_userInput && s_userInput->isExperiment())
		{
			vec3f n = s_pSampleItem->normal();

			if(s_userInput->isSlantOnly())
			{
				n = s_pSampleItem->normalWithRotate();
			}
			else if(s_userInput->isRotationOnly())
			{
				n = s_pSampleItem->normalWithSlant();
			}	

			vec3f pos = s_pSampleItem->getGroundtruthPosition();

			s_pCamera->setRotationTranslationMatrix(pd3dDevice, n, pos);
			s_pstdGauge->setColorIdx(0);
			s_pstdGauge->initMaterial(pd3dDevice);
			s_pstdGauge->DrawGauge(pd3dDevice, true);

			//draw ground truth
			if(s_userInput->_showReference->GetChecked() && s_userInput->isShowReference())
			{
				vec3f n = s_pSampleItem->getGroundtruthNormal();
				
				s_pCamera->setRotationTranslationMatrix(pd3dDevice, n, pos);
				s_groundTruthGauge->setColorIdx(1);
				s_groundTruthGauge->initMaterial(pd3dDevice);
				s_groundTruthGauge->DrawGauge(pd3dDevice, true);
			}	
		}

		// render text in the left corner of the window
		if(s_userInput->isAdjustSeperation())
		{
			RenderSeparation();
		}
		else if(s_isFinishTesting)
		{
			RenderThanks();
		}
		else if(s_userInput->isReferenceRemindder() && s_userInput->_showReference->GetChecked())
		{
			RenderAngleDifferenceReminder();
		}
		else
		{
			int restNum = 0;
			int totalNum = 0;
			s_imageModel->getNumbers(restNum, totalNum);
			RenderImageCount(restNum, totalNum);
		}

		//render control pannel		
		DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); // These events are to help PIX identify what the code is doing		
		V( s_controlPanel.OnRender( fElapsedTime ) );
		V( s_sampleUI.OnRender( fElapsedTime ) );
		DXUT_EndPerfEvent();		

		V( pd3dDevice->EndScene() );		
	}
}

//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------

void CALLBACK DxutStereoFrame::OnD3D9LostDevice( void* pUserContext )
{
	s_dialogResourceManager.OnD3D9LostDevice();
	if( s_pFont9 ) s_pFont9->OnLostDevice();
	SAFE_RELEASE( s_pSprite9 );
	SAFE_DELETE( s_pTxtHelper );

	//add
	CDXUTDirectionWidget::StaticOnD3D9LostDevice();
	
}

//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK DxutStereoFrame::OnD3D9DestroyDevice( void* pUserContext )
{
	s_dialogResourceManager.OnD3D9DestroyDevice();
	SAFE_RELEASE(s_pFont9 );

	//add
	SAFE_RELEASE(s_pSprite9);
	SAFE_DELETE( s_pTxtHelper);
	CDXUTDirectionWidget::StaticOnD3D9DestroyDevice();
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK DxutStereoFrame::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case IDC_TOGGLEFULLSCREEN:
		DXUTToggleFullScreen();
		s_isFullScreen = !s_isFullScreen;

		if(s_userInput)
		{
			if(s_userInput->_openNewFile) s_userInput->_openNewFile->SetVisible(!s_isFullScreen);
			if(s_userInput->_openMultipleNewFile) s_userInput->_openMultipleNewFile->SetVisible(!s_isFullScreen);
			if(s_userInput->_modelCB) s_userInput->_modelCB->SetVisible(!s_isFullScreen);
			if(s_userInput->_showReference) s_userInput->_showReference->SetVisible(!s_isFullScreen);			
		}		
		break;	

	case IDC_MODEL_SELECTION:  //select one new model
		if (s_userInput)
		{
			s_userInput->getModelValue();
			if(s_userInput->isExperiment() && s_imageModel)
			{				
				s_imageModel->resample();
				s_imageModel->getNextSampleItem( *s_pSampleItem);
				if(s_pEstimatedItem) s_pEstimatedItem->clear();
			}
		}
		break;

	case IDC_OPEN_NEW_FILE:
		OpenNewFile();
		break;

	case IDC_SAVE_RESULT_FILE:
		//not used
		break;

	case IDC_OPEN_MULTIPLE_NEW_FILE:
		OpenNewFileWithMultipleTestCases();
		break;

	}
}