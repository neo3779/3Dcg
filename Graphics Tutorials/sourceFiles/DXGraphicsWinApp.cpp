/*
* File name:	DXGraphicsWinApp.cpp
* Description:	Principal Windows API file for a simple graphics application running 
*				under Windows and using DirectX Graphics. 
* Author: 		Alex J Davison
*/

#include		<Windows.h>
#include		"D3DApplication.h"

// 3D graphics application object
D3DApplication	threeDApp;	

/*
 * Window procedure: function to which Windows sends messages
 *
 * Parameter list
 * 		hWnd:		handle of window
 *		msg:		message identifier
 *		wParam:		first message parameter
 *		lParam:		second message parameter
 */
LRESULT WINAPI wndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:

            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

/*
 * Application entry point
 *
 * Parameter list
 *		hThisApInst:			handle to current instance of application
 *		hPrevApInst: 			handle to previous instance of application
 *		lpszCmdArguments:		pointer to command line for application
 *		nWindowShowState: 		show-state of window
 */
INT WINAPI WinMain( HINSTANCE hThisApInst, HINSTANCE hPrevApInst, LPSTR lpszCmdArguments, 
				    INT nWindowShowState)
{
    // Register window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, wndProc, 0L, 0L, 
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "3D Computer Graphics", NULL };
    RegisterClassEx( &wc );

    // Create application window
    HWND hWnd = CreateWindow( "3D Computer Graphics", "3D Computer Graphics: Alex J Davison (DV003874)", 
                              WS_OVERLAPPEDWINDOW, 100, 100, 800, 800,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

    // Initialise Direct3D
    if( SUCCEEDED( threeDApp.initialiseD3D( hWnd ) ) )
    { 

        // Initialise world 
        if( SUCCEEDED( threeDApp.initialiseWorld() ) )
        {
			// Initialise camera 
			if( SUCCEEDED( threeDApp.initialiseCamera() ) )
			{
				// Show window
				ShowWindow( hWnd, SW_SHOWDEFAULT );
				UpdateWindow( hWnd );

				// Enter message loop
				MSG msg; 
				ZeroMemory( &msg, sizeof(msg) );
				while( msg.message != WM_QUIT )
				{
					if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
					{
						TranslateMessage( &msg );
						DispatchMessage( &msg );
					}
					else
					{
						//Update scene
						threeDApp.update();
						// Trigger rendering
						threeDApp.render();
					}
				}
			}
        }
    }

	// Release graphics API objects
    threeDApp.relD3DObjects();

    UnregisterClass( "3D Computer Graphics", wc.hInstance );
    
	return 0;
}