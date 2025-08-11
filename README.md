
 **MM2Hook : Project NuHook**  
 ![enter image description here](https://i.imgur.com/F9JrZvz.png)  
A cleaned up fork of mm2hook, with a larger focus on the Lua modding API and capabilities.
***
[![Build status](https://ci.appveyor.com/api/projects/status/96k957iuc4l9susg?svg=true)](https://ci.appveyor.com/project/Dummiesman/mm2hook)

A simple DLL-injection module for hooking into Midtown Madness 2, allowing for further control of the game.

Also included are complimentary bug-fixes for random crashes, as well as a fix for the infamous **heap overrun** error.

## Features
<img align="right" src="https://i.imgur.com/A5ZMdzB.png" alt="Discord Rich Presence support for MM2Hook" />

 * Console output window
 * Lua scripting
 
 * ### [Discord Rich Presence](https://discordapp.com/rich-presence) Integration:
   * Dynamically updates based on the state of your game (playing a race, cruising around, in the main menu, etc.)
   * Shows a thumbnail picture for default vehicles/cities
     - Thumbnail text shows the full name (mods included!)
 
## Installation
1) Grab the latest version [here](https://github.com/Dummiesman/mm2hook/releases/latest).
2) **Optional:** Download the latest version of Discord Rich Presence [here](https://github.com/discordapp/discord-rpc/releases/latest), and grab `discord-rpc.dll` from the `win32-dynamic\bin` folder.
3) Place all files in your Midtown Madness 2 installation directory and enjoy!

**Note for Wine users:** In order to use this on Wine, you must open winecfg, go to the Libraries tab, and add an override for dinput. Set the override to either "native" or "native then builtin"

## Building
Using a recent version of the Visual Studio IDE (including Visual Studio 2022 14.39) *should* more or less work as expected. If you'd prefer to use something else, a [simple build script](build.bat) is also available.

Note that this still requires installing parts of the [Build Tools for Visual Studio](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) to work:

- `C++/CLI support for v143 build tools` (latest should be fine)
- `MSVC v143 - VS 2022 C++ x64/x86 build tools` (latest should be fine)
- `C++ ATL for latest v143 build tools (x86 & x64)`
- `C++ MFC for latest v143 build tools (x86 & x64)`
- `Windows 10 SDK (10.0.18362.0)`
- `Windows Universal C Runtime`
- `C++ Build Tools core features`
- `C++ core features`

## Contributing
If you'd like to contribute to the project, please [fork](https://help.github.com/articles/about-forks/) the project and submit a [pull request](https://help.github.com/articles/about-pull-requests/).

All contributions are welcomed, including ones that fix something as simple as a typo.

#### Coding Style
Please use spaces instead of tabs, and try to keep your code consistent with the rest of the project.

## Arguments

mm2hook supports some custom parameters to enable or disable functions and patches. These are the most commonly used:

| Argument | Parameters | Description |
| :------- | :--------: | :---------- |
| -nopause | *N/A*      | Allows the game to keep running even when unfocused. |
| -width | `size_in_pixels` | Sets the window width in pixels. |
| -height | `size_in_pixels` | Sets the window height in pixels. |
| -sw | *N/A* | Forces the game to run in software mode. |
| -borderless | *N/A* | Sets the window to borderless mode. |
| -novsync | *N/A* | Disables vertical sync. |
| -nohwtnl | *N/A* | Disables hardware T&L. |
| -windowed | *N/A* | Forces the game to windowed mode. |
| -fullscreen | *N/A* | Forces the game to fullscreen mode. |
| -bitdepth | `bit_depth` | Sets the bit depth in bits. |
| -heapsize | `size_in_mb` | Sets the heap size (in megabytes). |

## New geometry objects

mm2hook supports more geometries which can be added in PKG files as new realistic features in game.

Pop-up headlights objects.

| Object | Description |
| :------- | :---------|
| PLIGHTON | Appears when car lights are on. |
| PLIGHTOFF | Appears when car lights are off. |

Spinning wheel objects appear when wheel rotation rate is high.

| Object | Description |
| :------- | :---------|
| SWHL0 | Front left blurry wheel. |
| SWHL1 | Front right blurry wheel. |
| SWHL2 | Back left blurry wheel. |
| SWHL3 | Back right blurry wheel. |
| SWHL4 | Back back left blurry wheel. |
| SWHL5| Back back right blurry wheel. |

**Note:**
In order to export these objects in the right order, you must use Blender PKG Add-On [here](https://github.com/Dummiesman/PKGImportExport), otherwise it won't work properly.
