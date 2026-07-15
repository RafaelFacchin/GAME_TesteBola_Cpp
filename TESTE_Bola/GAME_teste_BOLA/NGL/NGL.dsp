# Microsoft Developer Studio Project File - Name="NGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=NGL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NGL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NGL.mak" CFG="NGL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NGL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NGL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NGL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "NGL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "NGL - Win32 Release"
# Name "NGL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\NGLAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLBrick.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLImage.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLInputManager.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLMain.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLObject.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLScene.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLSound.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLSoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLTimeAccumulator.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLTimeHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLVector.cpp
# End Source File
# Begin Source File

SOURCE=.\NGLVideoManager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\NGLAnimation.h
# End Source File
# Begin Source File

SOURCE=.\NGLBrick.h
# End Source File
# Begin Source File

SOURCE=.\NGLFrame.h
# End Source File
# Begin Source File

SOURCE=.\NGLImage.h
# End Source File
# Begin Source File

SOURCE=.\NGLInputManager.h
# End Source File
# Begin Source File

SOURCE=.\NGLLayer.h
# End Source File
# Begin Source File

SOURCE=.\NGLMain.h
# End Source File
# Begin Source File

SOURCE=.\NGLObject.h
# End Source File
# Begin Source File

SOURCE=.\NGLScene.h
# End Source File
# Begin Source File

SOURCE=.\NGLSound.h
# End Source File
# Begin Source File

SOURCE=.\NGLSoundManager.h
# End Source File
# Begin Source File

SOURCE=.\NGLSprite.h
# End Source File
# Begin Source File

SOURCE=.\NGLTimeAccumulator.h
# End Source File
# Begin Source File

SOURCE=.\NGLTimeHandler.h
# End Source File
# Begin Source File

SOURCE=.\NGLVector.h
# End Source File
# Begin Source File

SOURCE=.\NGLVideoManager.h
# End Source File
# Begin Source File

SOURCE=.\StlFiles.h
# End Source File
# End Group
# End Target
# End Project
