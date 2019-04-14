# Microsoft Developer Studio Project File - Name="VmWin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=VmWin - Win32 DebugROM
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VmWin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VmWin.mak" CFG="VmWin - Win32 DebugROM"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VmWin - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "VmWin - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "VmWin - Win32 DebugROM" (based on "Win32 (x86) Console Application")
!MESSAGE "VmWin - Win32 ReleaseROM" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Products/Kauai10/VmWin", NSOCAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VmWin - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /O2 /I "..\..\VmWin\h" /I "..\..\VmCommon\h" /I "..\..\VmExtra\h" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D NOGUI=1 /D USE_KNI=1 /D ROMIZING=0 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib /nologo /subsystem:console /map /machine:I386

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /Gm /Gi /GX /ZI /Od /I "..\..\VmWin\h" /I "..\..\VmCommon\h" /I "..\..\VmExtra\h" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D NOGUI=1 /D INCLUDEDEBUGCODE=1 /D EXCESSIVE_GARBAGE_COLLECTION=1 /D USE_KNI=1 /D ROMIZING=0 /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib wsock32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VmWin___Win32_DebugROM"
# PROP BASE Intermediate_Dir "VmWin___Win32_DebugROM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugROM"
# PROP Intermediate_Dir "DebugROM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /I "..\..\VmWin\h" /I "..\..\VmCommon\h" /I "..\..\VmExtra\h" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D INCLUDEDEBUGCODE=1 /D EXCESSIVE_GARBAGE_COLLECTION=1 /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /Gm /Gi /GX /ZI /Od /I "..\..\VmWin\h" /I "..\..\VmCommon\h" /I "..\..\VmExtra\h" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D NOGUI=1 /D INCLUDEDEBUGCODE=1 /D EXCESSIVE_GARBAGE_COLLECTION=1 /D USE_KNI=1 /D ROMIZING=1 /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib wsock32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VmWin___Win32_ReleaseROM"
# PROP BASE Intermediate_Dir "VmWin___Win32_ReleaseROM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseROM"
# PROP Intermediate_Dir "ReleaseROM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /Os /I "..\..\VmWin\h" /I "..\..\VmCommon\h" /I "..\..\VmExtra\h" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /O2 /I "..\..\VmWin\h" /I "..\..\VmCommon\h" /I "..\..\VmExtra\h" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D NOGUI=1 /D USE_KNI=1 /D ROMIZING=1 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /map /machine:I386
# ADD LINK32 wsock32.lib /nologo /subsystem:console /map /machine:I386

!ENDIF 

# Begin Target

# Name "VmWin - Win32 Release"
# Name "VmWin - Win32 Debug"
# Name "VmWin - Win32 DebugROM"
# Name "VmWin - Win32 ReleaseROM"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\VmExtra\src\async.c
# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\cache.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /I "..\..\..\jam\h" /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /I "..\..\..\jam\h" /D BUILD_VERSION=1.1
# ADD CPP /I "..\..\..\jam\h" /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\class.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\src\collector.c
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\commProtocol.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\commProtocol_md.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\datagramProtocol.c
# End Source File
# Begin Source File

SOURCE=..\src\datagramProtocol_md.c
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\debugger.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\debuggerInputStream.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\debuggerOutputStream.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\debuggerSocketIO.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\events.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\src\execute.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\fields.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\frame.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\garbage.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\global.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\hashtable.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\inflate.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\interpret.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\jar.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\src\kni.c
# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\loader.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMEXTRA\SRC\loaderFile.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\log.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMEXTRA\SRC\main.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\native.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\nativeCore.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\TOOLS\JCC\nativeFunctionTableWin.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\pool.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\profiling.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\property.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMEXTRA\SRC\resource.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\TOOLS\JCC\ROMjavaWin.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SRC\runtime2_md.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SRC\runtime_md.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\src\socketProtocol.c
# End Source File
# Begin Source File

SOURCE=..\src\socketProtocol_md.c
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\src\stackmap.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\StartJVM.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\thread.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\SRC\verifier.c

!IF  "$(CFG)" == "VmWin - Win32 Release"

!ELSEIF  "$(CFG)" == "VmWin - Win32 Debug"

# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 DebugROM"

# ADD BASE CPP /D BUILD_VERSION=1.1
# ADD CPP /D BUILD_VERSION=1.1

!ELSEIF  "$(CFG)" == "VmWin - Win32 ReleaseROM"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\src\verifierUtil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\VMEXTRA\H\async.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\cache.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\class.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\commProtocol.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\datagramProtocol.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\debugger.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\debuggerCommands.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\debuggerStreams.h
# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\H\events.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\execute.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\fields.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\frame.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\garbage.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\global.h
# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\H\hashtable.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\inflate.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\inflateint.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\inflatetables.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\interpret.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\jar.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\kni.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\loader.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\log.h
# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\H\long.h
# End Source File
# Begin Source File

SOURCE=..\h\machine_md.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\main.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\messages.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\native.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\pool.h
# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\H\profiling.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\property.h
# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\H\rom.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\runtime.h
# End Source File
# Begin Source File

SOURCE=..\..\VmExtra\h\socketProtocol.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\thread.h
# End Source File
# Begin Source File

SOURCE=..\..\VMCOMMON\H\verifier.h
# End Source File
# Begin Source File

SOURCE=..\..\VmCommon\h\verifierUtil.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
