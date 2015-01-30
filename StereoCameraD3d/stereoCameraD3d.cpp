//--------------------------------------------------------------------------------------
//	Stereo Image show
//--------------------------------------------------------------------------------------

#include "cvxDxut.h"
#include "cvxD3d9.h"
#include "nv_stereo_utils.h"
#include "d3dCamera.h"
#include "dxutStereoFrame.h"
#include <conio.h>


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
#define MAIN_HERE 1


#if MAIN_HERE

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device (either D3D9 or D3D10) 
    // that is available on the system depending on which D3D callbacks are set below

    // Set DXUT callbacks
    DXUTSetCallbackMsgProc( DxutStereoFrame::MsgProc );
    DXUTSetCallbackKeyboard( DxutStereoFrame::OnKeyboard );
    DXUTSetCallbackFrameMove( DxutStereoFrame::OnFrameMove );
	DXUTSetCallbackMouse(DxutStereoFrame::OnMouse);

    DXUTSetCallbackD3D9DeviceAcceptable( DxutStereoFrame::IsD3D9DeviceAcceptable );
	DXUTSetCallbackD3D9DeviceCreated( DxutStereoFrame::OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( DxutStereoFrame::OnD3D9ResetDevice );
    DXUTSetCallbackD3D9DeviceLost( DxutStereoFrame::OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( DxutStereoFrame::OnD3D9DestroyDevice );
    DXUTSetCallbackD3D9FrameRender( DxutStereoFrame::OnD3D9FrameRender );

	//add an console window
	if (!AllocConsole())
	{
		MessageBox(NULL,
			L"Create console window failed!",
			L"Console",
			MB_ICONEXCLAMATION | MB_OK);

	}
	_cprintf("Open a console!\n");
	InitCommonControls();
	_cprintf("init common controls!\n");

    DxutStereoFrame::InitApp();
    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"Stereo Image" );

	DXUTCreateDevice( true, DxutStereoFrame::halfWidth(), DxutStereoFrame::halfHeight());
    DXUTMainLoop(); // Enter into the DXUT render loop

	// delete console
	if (!FreeConsole())
	{
		MessageBox(NULL,
			L"Free console window failed!",
			L"Console",
			MB_ICONEXCLAMATION | MB_OK);
	}

    return DXUTGetExitCode();
}

#endif

