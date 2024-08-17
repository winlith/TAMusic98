# TAMusic98
Run GOG / Steam version of Total Annihilation on Windows 98!

## Instructions

1. Install Total Annihilation from GOG or Steam on a modern PC (the GOG installer doesn't run on W98).
2. Copy the game files onto your Windows 98 PC.
3. Download win32.dll file from [**here**](https://github.com/winlith/TAMusic98/releases/download/1.01/win32.dll) and replace it in the game's folder.
4. Run TotalA.exe and check if it works. If it loads the main menu, you're good to go.
5. If it crashes, rename `tacticsX.hpi` files to `tacticsX.ufo`.
6. Enjoy!

## Issue with GOG / Steam TA on Windows 98

Total Annihilation was originally released on CDs, which contained the game's music. The CD Audio playback was handled by Windows library `winmm.dll`. The GOG and Steam versions of the game's executable are patched to load a different library, `win32.dll` instead, which intercepts the game's requests for CD Audio playback and instead plays MP3 files from the `music` folder. This library was compiled using MSVC 2008, which unfortunately doesn't run on Windows 98.

## Solution

The `win32.dll` library from this repo does essentially the same thing as the original one, but is compiled using Microsoft Visual C++ 6.0, which is a period correct compiler, and so has no problem running on older versions of Windows.

## Building
1. Install Microsoft Visual C++ 6.0.
2. Open `TAMusic98.dsw` workspace file.
3. Set desired build configuration.
4. Build the project. The compiled library will appear in Debug or Release folder, depending on chosen build config.