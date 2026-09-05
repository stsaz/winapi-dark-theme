# Dark Theme for Win32API GUI Apps

A single-file C library for adding dark theme support to Win32 GUI applications.
Requires Windows 10 or later.

![Dark Theme Example](winapi-dark-theme-example.png)

## Quick Start

```c
#include <windows.h>
#include "dark-theme.h"
#include "dark-theme.c"

struct dark_theme dts = {}, *dt = NULL;

if (!dark_theme_init(&dts, 0)
    && DARK_THEME_DARK == dark_theme_query()) {

    // Set custom colors (RGB)
    dark_theme_colors(&dts, 0x222222, 0xeeeeee);

    // Apply dark theme for the application
    dark_theme_ctl(&dts, DARK_THEME_APP, 0);

    // Activate dark theme for the next `dark_theme_ctl()` calls
    dt = &dts;
}

// ... create your window and controls ...

// Apply for the window
dark_theme_ctl(dt, DARK_THEME_WINDOW_TITLE, hWnd);
dark_theme_ctl(dt, DARK_THEME_WINDOW_MAIN_MENU, hWnd);
dark_theme_ctl(dt, DARK_THEME_WINDOW, hWnd);

// Apply dark theme to individual controls
dark_theme_ctl(dt, DARK_THEME_BUTTON, hButton);
dark_theme_ctl(dt, DARK_THEME_CHECKBOX, hCheckbox);
dark_theme_ctl(dt, DARK_THEME_RADIOBUTTON, hRadioButton);
dark_theme_ctl(dt, DARK_THEME_EDIT, hEdit);
dark_theme_ctl(dt, DARK_THEME_TAB, hTab);
dark_theme_ctl(dt, DARK_THEME_TRACKBAR, hTrackBar);
dark_theme_ctl(dt, DARK_THEME_PROGRESSBAR, hProgressBar);
dark_theme_ctl(dt, DARK_THEME_COMBOBOX, hComboBox);
dark_theme_ctl(dt, DARK_THEME_LISTVIEW, hListView);
dark_theme_ctl(dt, DARK_THEME_TREEVIEW, hTreeView);
dark_theme_ctl(dt, DARK_THEME_STATUSBAR, hStatusBar);
```

## Building the Example App

```bash
cd example && make
```

## API Reference

### dark_theme_init

`int dark_theme_init(struct dark_theme *theme, unsigned flags)`

Initialize the dark theme context.
Loads the required functions from `uxtheme.dll` and `dwmapi.dll`.

* **theme**: Zero-initialized `struct dark_theme` pointer
* **flags**: Reserved for future use (pass 0)
* **Returns**: 0 on success, non-zero on error

### dark_theme_query

`enum DARK_THEME dark_theme_query()`

Query the system's current color theme.

* **Returns**: `DARK_THEME_LIGHT` or `DARK_THEME_DARK`

### dark_theme_ctl

`int dark_theme_ctl(struct dark_theme *theme, unsigned flags, HWND h)`

Apply theme settings to a window or control.

* **theme**: Initialized theme context
* **flags**: Value from `enum DARK_THEME_FLAGS`
* **h**: HWND of the target window/control (ignored for `DARK_THEME_APP`, use 0)
* **Returns**: 0 on success, -1 on error

### DARK_THEME_FLAGS

| Flag | Description |
|------|-------------|
| `DARK_THEME_APP` | Enable dark mode for the entire application process |
| `DARK_THEME_WINDOW_TITLE` | Dark title bar via DWM |
| `DARK_THEME_WINDOW` | Dark window background; automatically applies dark colors to all child static controls, listboxes, and edit controls |
| `DARK_THEME_WINDOW_MAIN_MENU` | Main Menu with custom highlight colors |
| `DARK_THEME_BUTTON` | Button |
| `DARK_THEME_CHECKBOX` | Check Box |
| `DARK_THEME_RADIOBUTTON` | Radio Button |
| `DARK_THEME_EDIT` | Edit control (scrollbars) |
| `DARK_THEME_TAB` | Tab control with custom highlight colors |
| `DARK_THEME_TRACKBAR` | TrackBar control with custom background/thumb colors |
| `DARK_THEME_PROGRESSBAR` | ProgressBar with custom background/foreground colors |
| `DARK_THEME_COMBOBOX` | ComboBox control |
| `DARK_THEME_LISTVIEW` | ListView with custom header/text colors |
| `DARK_THEME_TREEVIEW` | TreeView control |
| `DARK_THEME_STATUSBAR` | Status Bar |

### dark_theme_colors

`void dark_theme_colors(struct dark_theme *t, unsigned background, unsigned text)`

Set background and text colors.
The function also sets the colors for tab or menu highlighting automatically.

* **background**: Background color in RGB
* **text**: Text color in RGB

## Acknowledgments

This library was largely inspired by the following projects:

* [notepad-plus-plus/notepad-plus-plus](https://github.com/notepad-plus-plus/notepad-plus-plus) — Notepad++ dark theme implementation
* [adzm/win32-custom-menubar-aero-theme](https://github.com/adzm/win32-custom-menubar-aero-theme) — Win32 custom menubar theming
* [ysc3839/win32-darkmode](https://github.com/ysc3839/win32-darkmode) — Win32 dark mode utilities

Many thanks to the authors and contributors of these projects!
