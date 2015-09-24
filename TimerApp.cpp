// TimerApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TimerApp.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "timer.h"

using namespace std;

#define MAX_LOADSTRING 100
#define MAX_TIMER_COUNT 1200 // or 20 minutes
#define TIMER_EVENT_UID 23

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
bool bShowWindowTitleBar = true;				// Show or hide the Window Title Bar
Timer timer;									// Timer object to keep track of progress

HFONT hFont;									// Drawing Font

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Create timer class
	timer.initialise(MAX_TIMER_COUNT);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TIMERAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIMERAPP));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIMERAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TIMERAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 100, 60, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   // Remove the Menu
   SetMenu(hWnd, NULL);

   // Set the border
   if (bShowWindowTitleBar == false) {
	   SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
   }

   // Make the window topmost
   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);

   // Set the font
   hFont = CreateFontA(22, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Reset the timer if the timer is not currently running
void resetTimer(HWND hWnd)
{
	if (timer.reset()) {
		// Reset the window
		InvalidateRect(hWnd, NULL, TRUE);
	}
}

// Update the timer running state
void updateTimerState(HWND hWnd)
{
	// Switch state to stopped, kill timer
	if (!timer.switchState()) {
		// Stop the timer
		KillTimer(hWnd, TIMER_EVENT_UID);
	}
	else {
		// Or, start the timer again
		SetTimer(hWnd, TIMER_EVENT_UID, 1000, NULL);
	}
}

/*void updateWindowTitleBar(HWND hWnd)
{
	// Currently shown, hide it
	if (bShowWindowTitleBar) {
		bShowWindowTitleBar = false;
		SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
	}
	else {
		bShowWindowTitleBar = true;
		SetWindowLong(hWnd, GWL_STYLE, WS_CAPTION);
	}
}*/



void paintWindow(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	LPCWSTR sw;

	std::string outputTimer;
	std::wstring wsOutputTimer;

	hdc = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	
	
	outputTimer = timer.formatedString();
	wsOutputTimer = std::wstring(outputTimer.begin(), outputTimer.end());

	sw = wsOutputTimer.c_str();

	// Background for the whole window
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); 
	FillRect(hdc, &rect, hBrush);

	// Draw timer text block
	SetBkColor(hdc, RGB(0, 0, 0));
	SelectObject(hdc, hFont);
	SetRect(&rect, 0, 0, 0, 0);
	SetTextColor(hdc, RGB(255, 60, 60));

	DeleteObject(hBrush);
	


	DrawTextW(hdc, sw, -1, &rect, DT_NOCLIP);

	EndPaint(hWnd, &ps);

}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// Do stuff on window creation
		break;

	case WM_PAINT:
		paintWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	// Update the timer if the timer is runing
	case WM_TIMER:
		if(timer.decrease()) {
			// Redraw the whole window
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else {
			KillTimer(hWnd, TIMER_EVENT_UID);
		}
		break;

	// Right button resets the timer if it is paused
	case WM_RBUTTONDOWN:
		resetTimer(hWnd);
		break;

	// Left button start/stops the timer.
	case WM_LBUTTONDOWN:
		updateTimerState(hWnd);
		break;

	// Keydown management
	case WM_KEYDOWN:
		// Escape or 'Q' quits the app
		if (wParam == VK_ESCAPE || wParam == 81) {
			DestroyWindow(hWnd);
		}

		// 'R' resets the timer
		else if (wParam == 82) {
			resetTimer(hWnd);
		}

		// 'S' starts or stops the timer
		else if (wParam == 83) {
			updateTimerState(hWnd);
		}

		// 'T' show/hide the window title bar
		/*else if (wParam == 84) {
			updateWindowTitleBar(hWnd);
		}*/
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
