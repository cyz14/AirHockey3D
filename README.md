# 3D Hockey Game 

Yazheng Chen 2014011423
Tong Gao 2014011357

## Game Play

Use mouse to control your mallet.

Use keyborad w,a,s,d keys to control camera view position.

Press G or g key will lose.

When win or lose, press Enter key to continue.

## 开发环境

OS: Windows 10  
IDE: VS 2017  
Package management Tools: NUGET  
Version Control Tools: Git  
Language: C++ (OpenGL)  

## 代码逻辑

> main.cpp  
> * Draw Table and Floor
> * Control Camera View
> * Game Logic
>   - Get Mouse position
>   - Keyboard control. Press Key 'G'/'g' will lose.
>   - AI control mallet
>   - Show game info (Win of Lose, press enter to try again)

> Mallet.cpp/h  
> Class for mallet

> Puck.cpp/h  
> Class for puck

> ai.cpp  
> simple ai logic to control ai mallet, according to puck info

> Wall.cpp/h  
> Class for wall, help puck do wall collision detection.  
> Use as goal, simplify game status judge.

> BMP.cpp/h  
> BMP texture class

> Vector2d.cpp/h  
> Simple Vector2d class to do vector operations, such as dot, norm, +, -, *, /, etc.

> packages.config  
> VS 2017 NUGET package configuratio file.
> We install FreeGLUT by NUGET.
