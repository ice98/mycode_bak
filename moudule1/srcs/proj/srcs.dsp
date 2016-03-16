# Microsoft Developer Studio Project File - Name="srcs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=srcs - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "srcs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "srcs.mak" CFG="srcs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "srcs - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "srcs - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "srcs - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "srcs - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "_MFC" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "srcs - Win32 Release"
# Name "srcs - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Ice.h
# End Source File
# Begin Source File

SOURCE=..\IceBaseType.h
# End Source File
# Begin Source File

SOURCE=..\IceColor.cpp
# End Source File
# Begin Source File

SOURCE=..\IceColor.h
# End Source File
# Begin Source File

SOURCE=..\IceDll.cpp
# End Source File
# Begin Source File

SOURCE=..\IceDll.h
# End Source File
# Begin Source File

SOURCE=..\IceErr.cpp
# End Source File
# Begin Source File

SOURCE=..\IceErr.h
# End Source File
# Begin Source File

SOURCE=..\IceFile.cpp
# End Source File
# Begin Source File

SOURCE=..\IceFile.h
# End Source File
# Begin Source File

SOURCE=..\IceGlobal.cpp
# End Source File
# Begin Source File

SOURCE=..\IceGlobal.h
# End Source File
# Begin Source File

SOURCE=..\IceNode.cpp
# End Source File
# Begin Source File

SOURCE=..\IceNode.h
# End Source File
# Begin Source File

SOURCE=..\IcePE.h
# End Source File
# Begin Source File

SOURCE=..\IceProc.cpp
# End Source File
# Begin Source File

SOURCE=..\IceProc.h
# End Source File
# Begin Source File

SOURCE=..\IceProcess.H
# End Source File
# Begin Source File

SOURCE=..\IcePub.cpp
# End Source File
# Begin Source File

SOURCE=..\IcePub.h
# End Source File
# Begin Source File

SOURCE=..\IceSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\IceSocket.h
# End Source File
# Begin Source File

SOURCE=..\IceWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\IceWnd.h
# End Source File
# Begin Source File

SOURCE=..\Icy.h
# End Source File
# Begin Source File

SOURCE=..\IcyPub.h
# End Source File
# Begin Source File

SOURCE=..\srcmain.cpp
# End Source File
# Begin Source File

SOURCE=..\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Toolhelp.h
# End Source File
# End Group
# End Target
# End Project
