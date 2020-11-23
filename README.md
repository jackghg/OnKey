# OnKey
Lightweight on-screen keyboard in c and gtk2, optimized for portable devices.
## Features
- lightweight
- usable on small screens
- unicode*
- relatively highly customizable
- small source code
- for Xorg
## Limitations
- doesn't support auto show / hide
- you can't change the buttons size atm
## Install
To build and install use:  
`sudo make build clean install`  
You can `sudo make uninstall`  
To open and close the app you could use a small script like 'tkey', see the file.
## Options
-e   Expand the window width to match the screen width  
-k   Temporary set the system keyboard layout to 'us' while the app is open  
-w   The window width in pixels  
-h   The window height in pixels  
-o   Vertical offset. Adjust the window position, on the bottom of the screen  
-t   Loads your gtk 2 style file in your home, named '.ok.rc'  
-f   Shows flat buttons  
-p   Try to make the app visible on top of fullscreen applications  
**Examples**:  
...
## About the keyboard layout
The keyboard layout should be 'us' because it target devices without a keyboard, where 'us' could be a 'standard'.
To have the application works correctly you can:  
\- set the system keyboard layout to 'us'
\- use the -k option
\- write a keys table for your layout
\- use unicode codes for all or part of the keys*
## Style
You can customize colors and font using a gtk 2 style file in your home folder, see the -t option. Or the app will follow the system theme.
## \*Warning
This app will remap the typically unused keycode 249 to emulate a unicode character key. To make sure...
## Make your keys table
...
