# Microsoft Developer Studio Project File - Name="eDataServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=eDataServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DataServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DataServer.mak" CFG="eDataServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eDataServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "eDataServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "eDataServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "EF" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Release/DataServer.exe"

!ELSEIF  "$(CFG)" == "eDataServer - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "eDataServer - Win32 Release"
# Name "eDataServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CrashDump.cpp
# End Source File
# Begin Source File

SOURCE=.\CSprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\DSprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\DataServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DataServer.rc
# End Source File
# Begin Source File

SOURCE=.\EDSprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Eprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Fprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\giocp.cpp
# End Source File
# Begin Source File

SOURCE=.\LogProc.cpp
# End Source File
# Begin Source File

SOURCE=.\MapServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5.cpp
# End Source File
# Begin Source File

SOURCE=.\Query.cpp
# End Source File
# Begin Source File

SOURCE=.\RSprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TServerInfoDisplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\winutil.cpp
# End Source File
# Begin Source File

SOURCE=.\WzUdp.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\DataServer.ico
# End Source File
# Begin Source File

SOURCE=.\DataServer.ico
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CrashDump.h
# End Source File
# Begin Source File

SOURCE=.\CSprotocol.h
# End Source File
# Begin Source File

SOURCE=.\DSprotocol.h
# End Source File
# Begin Source File

SOURCE=.\DataServer.h
# End Source File
# Begin Source File

SOURCE=.\EDSprotocol.h
# End Source File
# Begin Source File

SOURCE=.\Eprotocol.h
# End Source File
# Begin Source File

SOURCE=.\Fprotocol.h
# End Source File
# Begin Source File

SOURCE=.\giocp.h
# End Source File
# Begin Source File

SOURCE=.\logproc.h
# End Source File
# Begin Source File

SOURCE=.\MapServerManager.h
# End Source File
# Begin Source File

SOURCE=.\MD5.h
# End Source File
# Begin Source File

SOURCE=.\MD5_KEYVAL.H
# End Source File
# Begin Source File

SOURCE=.\prodef.h
# End Source File
# Begin Source File

SOURCE=.\Query.h
# End Source File
# Begin Source File

SOURCE=.\ReadScript.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RSprotocol.h
# End Source File
# Begin Source File

SOURCE=.\ServerEngine.h
# End Source File
# Begin Source File

SOURCE=.\Sprotocol.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TServerInfoDisplayer.h
# End Source File
# Begin Source File

SOURCE=.\winutil.h
# End Source File
# Begin Source File

SOURCE=.\WzUdp.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
