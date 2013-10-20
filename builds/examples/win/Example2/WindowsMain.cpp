

#include <windows.h>
#include <ShlObj.h>

#include "MyRootViewController.h"
#include "MyGUIApplication.h"

#include <recore.h>

HWND _hWnd = NULL;
HDC _hDC = NULL;
HGLRC _hRC = NULL;
HINSTANCE _hInstance = NULL;
int _framesPerSecond = -1;
int _windowWidth = -1;
int _windowHeight = -1;
int _msaaSamples = -1;
int _maxAnisotrophy = -1;
bool _isFullScreen = false;
bool _hasFocus = false;
bool _enableVerticalSync = false;
MyGUIApplication * _myGUIApplication = NULL;
MyRootViewController * _myRootViewController = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef _UNICODE
const wchar_t *
#else
const char *
#endif	
GetApplicationName()
{
#ifdef _UNICODE
	return L"Example"; 
#else
	return "Example";
#endif	
}


void CreateGUIApplication()
{
	if (_myGUIApplication) 
	{ 
		return; 
	}
	else
	{
		MyRootViewController * rootViewController = new MyRootViewController();
		if (rootViewController) 
		{
			MyGUIApplication * guiApp = new MyGUIApplication(rootViewController, _hWnd);
			if (guiApp) 
			{
				_myRootViewController = rootViewController;
				_myGUIApplication = guiApp;
				return;
			}
			delete rootViewController;
		}
	}
}

void ToggleFullScreen()
{
	static DWORD savedExStyle;
	static DWORD savedStyle;
	static RECT rcSaved;

	_isFullScreen = !_isFullScreen;

	if (_isFullScreen)
	{
		// Moving to full screen mode.
		savedExStyle = GetWindowLong(_hWnd, GWL_EXSTYLE);
		savedStyle = GetWindowLong(_hWnd, GWL_STYLE);
		GetWindowRect(_hWnd, &rcSaved);

		SetWindowLong(_hWnd, GWL_EXSTYLE, 0);
		SetWindowLong(_hWnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		_windowWidth = GetSystemMetrics(SM_CXSCREEN);
		_windowHeight = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0,
			_windowWidth, _windowHeight, SWP_SHOWWINDOW);
	}
	else
	{
		// Moving back to windowed mode.
		SetWindowLong(_hWnd, GWL_EXSTYLE, savedExStyle);
		SetWindowLong(_hWnd, GWL_STYLE, savedStyle);
		SetWindowPos(_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		_windowWidth = rcSaved.right - rcSaved.left;
		_windowHeight = rcSaved.bottom - rcSaved.top;

		SetWindowPos(_hWnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top,
			_windowWidth, _windowHeight, SWP_SHOWWINDOW);
	}

	RERenderDevice::GetDefaultDevice()->SetScreenSize((REFloat32)_windowWidth, (REFloat32)_windowHeight);
}


void SetProcessorAffinity()
{
	// Assign the current thread to one processor. This ensures that timing
	// code runs on only one processor, and will not suffer any ill effects
	// from power management.
	//
	// Based on DXUTSetProcessorAffinity() function from the DXUT framework.

	unsigned long dwProcessAffinityMask = 0;
	unsigned long dwSystemAffinityMask = 0;
	HANDLE hCurrentProcess = GetCurrentProcess();

	if (!GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask))
	{
		return;
	}

	if (dwProcessAffinityMask)
	{
		// Find the lowest processor that our process is allowed to run against.

		unsigned long dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

		// Set this as the processor that our thread must always run against.
		// This must be a subset of the process affinity mask.

		HANDLE hCurrentThread = GetCurrentThread();

		if (hCurrentThread != INVALID_HANDLE_VALUE)
		{
			SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
			CloseHandle(hCurrentThread);
		}
	}

	CloseHandle(hCurrentProcess);
}


void Cleanup()
{
	if (_hDC)
	{
		ReleaseDC(_hWnd, _hDC);
		_hDC = 0;
		_hRC = 0;
	}
}

HWND CreateAppWindow(const WNDCLASSEX & wcl)
{
	// Create a window that is centered on the desktop. It's exactly 1/4 the
	// size of the desktop. Don't allow it to be resized.
	DWORD wndExStyle = WS_EX_OVERLAPPEDWINDOW;
	DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
		WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	HWND hWnd = CreateWindowEx(wndExStyle, wcl.lpszClassName, GetApplicationName(),
		wndStyle, 0, 0, 0, 0, 0, 0, wcl.hInstance, 0);

	if (hWnd)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int halfScreenWidth = screenWidth / 2;
		int halfScreenHeight = screenHeight / 2;

		halfScreenWidth = 1024;
		halfScreenHeight = 768;

		int left = (screenWidth - halfScreenWidth) / 2;
		int top = (screenHeight - halfScreenHeight) / 2;
		RECT rc = {0};

		SetRect(&rc, left, top, left + halfScreenWidth, top + halfScreenHeight);
		AdjustWindowRectEx(&rc, wndStyle, FALSE, wndExStyle);
		MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

		GetClientRect(hWnd, &rc);
		_windowWidth = rc.right - rc.left;
		_windowHeight = rc.bottom - rc.top;

		RERenderDevice::GetDefaultDevice()->SetScreenSize((REFloat32)_windowWidth, (REFloat32)_windowHeight);
	}

	return hWnd;
}

void ProcessUserKeys(UINT msg, WPARAM wParam, LPARAM lParam)
{
	const REInt32 iPosX = (REInt32)LOWORD(lParam);
	const REInt32 iPosY = (REInt32)HIWORD(lParam);

	if (msg == WM_KEYDOWN)
	{
		const wchar_t c = (wchar_t)wParam;
		//mainGameApp->KeyDown(c);

		/*
		if (wParam == 'F')
		{
			ToggleFullScreen();
		}
		*/
	}
	/*else if (msg == WM_KEYUP)
	{
		
	}*/
	else if (msg == WM_LBUTTONDOWN)
	{
		const REFloat32 coordX = (REFloat32)iPosX;
		const REFloat32 coordY = (REFloat32)iPosY;
		//mainGameApp->OnClickOnScreenStart(coordX, coordY);
	}
	else if (msg == WM_LBUTTONUP)
	{
		const REFloat32 coordX = (REFloat32)iPosX;
		const REFloat32 coordY = (REFloat32)iPosY;
		//mainGameApp->OnClickOnScreenEnd(coordX, coordY);
	}
	/*else if (msg == WM_RBUTTONDOWN)
	{
		//int iPosX = LOWORD(lParam);
		//int iPosY = HIWORD(lParam);
	}*/
	else if (msg == WM_MOUSEMOVE)
	{
		const REFloat32 coordX = (REFloat32)iPosX;
		const REFloat32 coordY = (REFloat32)iPosY;
		if (wParam == MK_LBUTTON)
		{
			//mainGameApp->OnClickMoveOnScreen(coordX, coordY);
		}
		//mainGameApp->OnMouseMoveOnScreen(coordX, coordY);
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		switch (wParam)
		{	
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			_hasFocus = true;
			break;

		case WA_INACTIVE:
			if (_isFullScreen)
			{
				ShowWindow(hWnd, SW_MINIMIZE);
			}
			_hasFocus = false;
			break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		_windowWidth = static_cast<int>(LOWORD(lParam));
		_windowHeight = static_cast<int>(HIWORD(lParam));
		RERenderDevice::GetDefaultDevice()->SetScreenSize((REFloat32)_windowWidth, (REFloat32)_windowHeight);
		break;

	default:
		ProcessUserKeys(msg, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	//ShowCursor(FALSE);

	WNDCLASSEX wcl = { 0 };
	wcl.cbSize = sizeof(wcl);
	wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = _hInstance = hInstance;
	wcl.hCursor = LoadCursor(0, IDC_ARROW);
	wcl.hbrBackground = 0;
	wcl.lpszMenuName = 0;
	wcl.hIconSm = 0;
	wcl.lpszClassName = GetApplicationName();
#ifdef _UNICODE
	wcl.hIcon = LoadIcon(0, L"icon_win.ico");
#else
	wcl.hIcon = LoadIcon(0, "icon_win.ico");
#endif

	if (!RegisterClassEx(&wcl))
	{
		return 0;
	}

	_hWnd = CreateAppWindow(wcl);
	MSG msg = { 0 };
	if (_hWnd)
	{
		SetProcessorAffinity();
		int initialized = 0;
		if ( RERenderDevice::GetDefaultDevice()->InitializeWithWindow(_hWnd) )
		{
			REResourcesStorage resourcesStorage;
			resourcesStorage.AddResourcesPath("data0.dat");
			resourcesStorage.AddResourcesPath("media0.dat");

			RERenderDevice::GetDefaultDevice()->SetRenderSize(2.666666f, 4.0f);
			RERenderDevice::GetDefaultDevice()->SetScreenSize((REFloat32)_windowWidth, (REFloat32)_windowHeight);

			initialized = 1;
		}

		if ( initialized )
		{
			ShowWindow(_hWnd, nCmdShow);
			UpdateWindow(_hWnd);

			CreateGUIApplication();

			if ( (_myGUIApplication == NULL) || (_myRootViewController == NULL) )
			{
				return 1;
			}
			_myGUIApplication->Initialize();
			_myGUIApplication->Start();

			//ToggleFullScreen();

			const RETimeInterval frameTime = (1.0 / 60.0);
			RETimeInterval lastTime = 0.0;
			while (true)
			{
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{
						break;
					}
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if (msg.message == WM_QUIT)
				{
					break;
				}

				if (_hasFocus)
				{
					const RETimeInterval currentTime = RETime::GetDefaultTime()->GetSecondsFromAppStart();

					if ((currentTime - lastTime) >= frameTime)
					{
						printf("\n Render %f", (float)currentTime);
						_myGUIApplication->Update();

						_myGUIApplication->Render();

						ValidateRect(_hWnd, NULL);

						lastTime = currentTime;
					}
				}
				else
				{
					WaitMessage();
				}
			}
		}

		Cleanup();
		UnregisterClass(wcl.lpszClassName, hInstance);
	}

	//GameApp::ReleaseDefaultApp();
	//mainGameApp = NULL;
	//RERenderDevice::ReleaseDefaultDevice();
	return static_cast<int>(msg.wParam);
}


