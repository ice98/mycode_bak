# Microsoft Developer Studio Project File - Name="SocketLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SocketLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SocketLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SocketLib.mak" CFG="SocketLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SocketLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SocketLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SocketLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SocketLib___Win32_Release"
# PROP BASE Intermediate_Dir "SocketLib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SocketLib___Win32_Release"
# PROP Intermediate_Dir "SocketLib___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SOCKETLIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SOCKETLIB_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "SocketLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SocketLib___Win32_Debug"
# PROP BASE Intermediate_Dir "SocketLib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SocketLib___Win32_Debug"
# PROP Intermediate_Dir "SocketLib___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SOCKETLIB_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SOCKETLIB_EXPORTS" /YX /FD /GZ  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SocketLib - Win32 Release"
# Name "SocketLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AppBase.cpp
# End Source File
# Begin Source File

SOURCE=.\AppLog.cpp
# End Source File
# Begin Source File

SOURCE=.\NTService.cpp
# End Source File
# Begin Source File

SOURCE=.\SLBlockSession.cpp
# End Source File
# Begin Source File

SOURCE=.\SLBlockSessionMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SLMessageQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\SLTask.cpp
# End Source File
# Begin Source File

SOURCE=.\SLTcpSession.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketLib.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketLib.rc
# End Source File
# Begin Source File

SOURCE=.\SocketRunner.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketSource.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpClient.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpRunner.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpServer.cpp
# End Source File
# Begin Source File

SOURCE=.\UdpRunner.cpp
# End Source File
# Begin Source File

SOURCE=.\UdpSource.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AppBase.h
# End Source File
# Begin Source File

SOURCE=.\AppLog.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\NTService.h
# End Source File
# Begin Source File

SOURCE=.\NTServiceEventLogMsg.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SLBaseType.h
# End Source File
# Begin Source File

SOURCE=.\SLBlockSession.h
# End Source File
# Begin Source File

SOURCE=.\SLBlockSessionMgr.h
# End Source File
# Begin Source File

SOURCE=.\SLCommonDef.h
# End Source File
# Begin Source File

SOURCE=.\SLMessageQueue.h
# End Source File
# Begin Source File

SOURCE=.\SLTask.h
# End Source File
# Begin Source File

SOURCE=.\SLTaskEx.h
# End Source File
# Begin Source File

SOURCE=.\SLTcpSession.h
# End Source File
# Begin Source File

SOURCE=.\SLTcpSessionEvent.h
# End Source File
# Begin Source File

SOURCE=.\SocketAPI.h
# End Source File
# Begin Source File

SOURCE=.\SocketEvent.h
# End Source File
# Begin Source File

SOURCE=.\SocketLib.h
# End Source File
# Begin Source File

SOURCE=.\SocketRunner.h
# End Source File
# Begin Source File

SOURCE=.\SocketSource.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\TcpClient.h
# End Source File
# Begin Source File

SOURCE=.\TcpRunner.h
# End Source File
# Begin Source File

SOURCE=.\TcpServer.h
# End Source File
# Begin Source File

SOURCE=.\UdpRunner.h
# End Source File
# Begin Source File

SOURCE=.\UdpSource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "syn"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sync\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\sync\Event.h
# End Source File
# Begin Source File

SOURCE=.\sync\Guard.h
# End Source File
# Begin Source File

SOURCE=.\sync\Mutex.h
# End Source File
# Begin Source File

SOURCE=.\sync\MutexGuard.h
# End Source File
# Begin Source File

SOURCE=.\sync\NullMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\sync\NullMutex.h
# End Source File
# Begin Source File

SOURCE=.\sync\ProcessMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\sync\ProcessMutex.h
# End Source File
# Begin Source File

SOURCE=.\sync\Semaphore.cpp
# End Source File
# Begin Source File

SOURCE=.\sync\Semaphore.h
# End Source File
# Begin Source File

SOURCE=.\sync\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\sync\Thread.h
# End Source File
# Begin Source File

SOURCE=.\sync\ThreadGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\sync\ThreadGroup.h
# End Source File
# Begin Source File

SOURCE=.\sync\ThreadManager.h
# End Source File
# Begin Source File

SOURCE=.\sync\ThreadMutex.h
# End Source File
# Begin Source File

SOURCE=.\sync\TimerQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\sync\TimerQueue.h
# End Source File
# End Group
# End Target
# End Project
