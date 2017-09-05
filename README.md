# Graphics-Library
Download required libraries from here


## It is a tradition to use Turbo C for graphic in C/CPP. But it's also a pain in the neck. Here we are using Code::Blocks IDE, which will ease out our work.


Steps to run graphic code in CodeBlocks
## 1. Install Code::Blocks
Make sure you have installed Code::Blocks IDE on your machine. If you don't have this IDE or have any issue with compiler download and install it from here.  http://sourceforge.net/projects/codeblocks/files/Binaries/16.01/Windows/codeblocks-16.01mingw-setup.exe

## 2. Download the required header files
We need few files to be included in the lib folder of Code::Blocks.
Download the files from here https://github.com/SagarGaniga/Graphics-Library

## 3. Include graphics.h and winbgim.h
Copy and Paste the graphics.h and winbgim.h files into include folder of Code::Blocks directory.

Path: C:\Program Files (x86)\CodeBlocks\MinGW\include

## 4. Include libbgi.a
Copy and paste libbgi.a file in the lib folder of Code:Blocks

Path: C:\Program Files (x86)\CodeBlocks\MinGW\lib

## 5. Add Link Libraries in Linker Setting
   * In the Code::Blocks application go to, Settings > Compiler

   * In the Global Compiler setting, click on the Linker Settings

   * In Link Libraries, Add and browse to C:\Program Files (x86)\CodeBlocks\MinGW\lib\ and select libbgi.a.

   * Paste this in the Other Linker Option tab of Linker Settings (i.e. on the right-hand side)

     -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32

   * Save the setting and restart the application

### To test the setting copy paste any computer graphics code from 
https://github.com/SagarGaniga/computer-graphics

