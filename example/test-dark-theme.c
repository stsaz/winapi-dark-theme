/** Example program for testing the dark theme */

#define UNICODE
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <commctrl.h>
#include "dark-theme.h"

#define IDM_FILE_EXIT 200
#define IDM_HELP_ABOUT 201

static void addMenuItem(HMENU hMenu, UINT wID, const wchar_t *pszText, HMENU hSubMenu)
{
	MENUITEMINFOW mi = {
		.cbSize = sizeof(mi),
		.fMask = MIIM_ID | MIIM_STRING | MIIM_SUBMENU,
		.wID = wID,
		.fType = MFT_STRING,
		.dwTypeData = (wchar_t *)pszText,
		.hSubMenu = hSubMenu,
	};
	InsertMenuItemW(hMenu, GetMenuItemCount(hMenu), TRUE, &mi);
}

static HMENU createMainMenu(HINSTANCE hInstance)
{
	HMENU hMenu = CreateMenu();
	HMENU hFile = CreatePopupMenu();
	addMenuItem(hMenu, 0, L"File", hFile);
	addMenuItem(hFile, IDM_FILE_EXIT, L"Exit", NULL);

	HMENU hHelp = CreatePopupMenu();
	addMenuItem(hMenu, 0, L"Help", hHelp);
	addMenuItem(hHelp, IDM_HELP_ABOUT, L"About", NULL);

	return hMenu;
}

#define Y(y, dy) ({ unsigned oy = y; y += dy + 10; (oy); })

static HWND createCtl(DWORD exStyle, const wchar_t *clsName, const wchar_t *text, DWORD style, int x, int y, int w, int h, HWND parent)
{
	return CreateWindowExW(exStyle, clsName, text, WS_CHILD | WS_VISIBLE | WS_TABSTOP | style, x, y, w, h, parent, 0, NULL, NULL);
}

static HWND createCtlWithFont(DWORD exStyle, const wchar_t *clsName, const wchar_t *text, DWORD style, int x, int y, int w, int h, HWND parent, HFONT font)
{
	HWND hwnd = createCtl(exStyle, clsName, text, style, x, y, w, h, parent);
	SendMessageW(hwnd, WM_SETFONT, (WPARAM)font, 0);
	return hwnd;
}

static LRESULT WINAPI wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nShow)
{
	struct dark_theme dts = {}, *dt = NULL;
	if (!dark_theme_init(&dts, 0)) {
		if (DARK_THEME_DARK == dark_theme_ctl(&dts, DARK_THEME_QUERY, 0)) {
			dark_theme_colors(&dts, 0x222222, 0xeeeeee);
			dark_theme_ctl(&dts, DARK_THEME_APP, 0);
			dt = &dts;
		}
	}

	HMENU hMenu = createMainMenu(hInstance);

	WNDCLASSW wc = {
		.lpfnWndProc = wndProc,
		.hInstance = hInstance,
		.lpszClassName = L"DarkThemeClass",
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
	};
	RegisterClassW(&wc);
	HWND hWnd = CreateWindowExW(0, L"DarkThemeClass", L"Dark Theme Test App",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 450, 400, NULL, hMenu, hInstance, NULL);

	LOGFONTW lf = {
		.lfCharSet = OEM_CHARSET,
		.lfQuality = PROOF_QUALITY,
		.lfFaceName = L"Arial",
		.lfHeight = -(11 * 96 / 72),
	};
	HFONT font = CreateFontIndirectW(&lf);

	unsigned y = 10;
	HWND hTab = createCtlWithFont(0, L"SysTabControl32", NULL, TCS_TABS, 10, Y(y, 25), 400, 25, hWnd, font);
	TCITEMW tci = {
		.mask = TCIF_TEXT,
		.pszText = L"Tab 1",
	};
	TabCtrl_InsertItem(hTab, 0, &tci);
	tci.pszText = L"Tab 2";
	TabCtrl_InsertItem(hTab, 1, &tci);

	HWND hLbl = createCtlWithFont(0, L"STATIC", L"Label", SS_LEFT, 10, Y(y, 20), 400, 20, hWnd, font);
	HWND hBtn = createCtlWithFont(0, L"BUTTON", L"Push Button", BS_PUSHBUTTON, 10, Y(y, 25), 400, 25, hWnd, font);
	HWND hChk = createCtlWithFont(0, L"BUTTON", L"Check Box", BS_AUTOCHECKBOX, 10, Y(y, 25), 400, 25, hWnd, font);
	HWND hRad = createCtlWithFont(0, L"BUTTON", L"Radio Button", BS_AUTORADIOBUTTON, 10, Y(y, 25), 400, 25, hWnd, font);
	HWND hEdit = createCtlWithFont(WS_EX_CLIENTEDGE, L"EDIT", L"Text here", ES_AUTOHSCROLL, 10, Y(y, 25), 400, 25, hWnd, font);

	HWND hLv = createCtl(0, L"SysListView32", NULL, LVS_REPORT | LVS_NOSORTHEADER, 10, y, 200, 100, hWnd);
	LVCOLUMNW col = {
		.mask = LVCF_TEXT | LVCF_WIDTH,
		.pszText = L"Name",
		.cx = 100,
	};
	ListView_InsertColumn(hLv, 0, &col);
	col.pszText = L"Value";
	ListView_InsertColumn(hLv, 1, &col);
	LVITEMW lvi = {
		.mask = LVIF_TEXT,
		.iSubItem = 0,
		.pszText = L"Item 1",
	};
	ListView_InsertItem(hLv, &lvi);
	ListView_SetItemText(hLv, 0, 1, L"Text here");

	HWND hLb = createCtlWithFont(0, L"LISTBOX", NULL, LBS_NOTIFY | LBS_STANDARD, 210, Y(y, 100), 200, 100, hWnd, font);
	SendMessageW(hLb, LB_ADDSTRING, 0, (LPARAM)L"Row 1");
	SendMessageW(hLb, LB_ADDSTRING, 0, (LPARAM)L"Row 2");

	HWND hStBar = createCtl(0, L"msctls_statusbar32", NULL, 0, 0, 0, 0, 0, hWnd);
	int parts[] = { 100 };
	SendMessageW(hStBar, SB_SETPARTS, 1, (LPARAM)&parts);
	SendMessageW(hStBar, SB_SETTEXT, 0, (LPARAM)L"Ready");

	dark_theme_ctl(dt, DARK_THEME_WINDOW_TITLE, hWnd);
	dark_theme_ctl(dt, DARK_THEME_WINDOW_MAIN_MENU, hWnd);
	dark_theme_ctl(dt, DARK_THEME_WINDOW, hWnd);
	dark_theme_ctl(dt, DARK_THEME_TAB, hTab);
	dark_theme_ctl(dt, DARK_THEME_BUTTON, hBtn);
	dark_theme_ctl(dt, DARK_THEME_CHECKBOX, hChk);
	dark_theme_ctl(dt, DARK_THEME_CHECKBOX, hRad);
	dark_theme_ctl(dt, DARK_THEME_EDIT, hEdit);
	dark_theme_ctl(dt, DARK_THEME_LISTVIEW, hLv);
	dark_theme_ctl(dt, DARK_THEME_STATUSBAR, hStBar);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0)) {

		if (IsDialogMessage(hWnd, &msg))
			continue;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}