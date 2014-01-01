SetCompressor /SOLID /FINAL lzma
!define AXIS_PATH "..\release"
!define MSVS_PATH "C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86"

  !system "GetVersion.exe"
  !include "Version.txt"
  !include "MUI2.nsh"

  !define MULTIUSER_EXECUTIONLEVEL Highest
  !define MULTIUSER_MUI

;--------------------------------
;General

  ;Name and file
  Name "Axis2 - ${Version}"
  OutFile "Axis2_Setup_${Version}.exe"

  ;Icons
  !define MUI_ICON "Icons/Axis2.ico"
  !define MUI_UNICON "Icons/UnAxis2.ico"

  ;Settings
  BrandingText "Axis2 Installation System"
  CRCCheck on
  ShowInstDetails show
  ShowUninstDetails show

  ;Version Info
  VIProductVersion "${XVersionX}"
  VIAddVersionKey /LANG=1033-English "ProductVersion" "${Version}"
  VIAddVersionKey /LANG=1033-English "ProductName" "Axis2 GM Tool"
  VIAddVersionKey /LANG=1033-English "FileVersion" "${Version}"
  VIAddVersionKey /LANG=1033-English "FileDescription" "GameMaster Help Tool"
  VIAddVersionKey /LANG=1033-English "LegalCopyright" "Benoit Croussette, 2006 - 2008"
  VIAddVersionKey /LANG=1033-English "Comments" "This tool was designed to simplify a GameMaster's job"

;--------------------------------
;Interface Configuration

  ;Default installation folder
  !define MULTIUSER_INSTALLMODE_INSTDIR "Axis2"
  
  ;Get installation folder from registry if available
  ;!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "SOFTWARE\Sphere\GM Tools"
  ;!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME "RootPath"

  ;Install Mode Page Settings
  !define MULTIUSER_INSTALLMODE_DEFAULT_CURRENTUSER
  !define MULTIUSER_INSTALLMODEPAGE_TEXT_ALLUSERS "Install for All Users               (Administrator privileges are required)"
  !define MULTIUSER_INSTALLMODEPAGE_TEXT_CURRENTUSER "Install for this User only       (Recommended)"

  !include "MultiUser.nsh"

  ;Start Menu Page Settings
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT HKCU
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "SOFTWARE\Axis2" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu"
  !define MUI_STARTMENUPAGE_TEXT_CHECKBOX "Do not add Start Menu Folder"

  !define MUI_LICENSEPAGE_TEXT_TOP "You must agree to this license before installing."

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "Icons\Axis2.bmp"
  !define MUI_HEADERIMAGE_UNBITMAP "Icons\Axis2.bmp"
  !define MUI_ABORTWARNING

;--------------------------------
;Variables

  Var StartMenuFolder
  Var TempInstalldir

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "${AXIS_PATH}\LICENSE"
  !insertmacro MULTIUSER_PAGE_INSTALLMODE
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;StartUp

Function .onInit
  !insertmacro MULTIUSER_INIT
FunctionEnd

Function un.onInit
  StrCpy $TempInstalldir $INSTDIR
  !insertmacro MULTIUSER_UNINIT
  StrCpy $INSTDIR $TempInstalldir
FunctionEnd

;--------------------------------
;Languages
 
 !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Default Section" SecDefault

  SetOutPath "$INSTDIR"  
  File "${AXIS_PATH}\Axis2.exe"
  File "${AXIS_PATH}\Axis2.chm"
  File "${AXIS_PATH}\Axis2.ini"
  File "${AXIS_PATH}\DoorWiz.ini"
  File "${AXIS_PATH}\LightWiz.ini"
  File "${AXIS_PATH}\changelog.txt"
  File "${AXIS_PATH}\hoglocs.scp"
  SetOutPath "$INSTDIR\Language"
  File "${AXIS_PATH}\Language\*.*"
  IfFileExists "$INSTDIR\Microsoft.VC80.CRT\*.*" +3 0
  SetOutPath "$INSTDIR\Microsoft.VC80.CRT"
  File "${MSVS_PATH}\Microsoft.VC80.CRT\*.*"
  IfFileExists "$INSTDIR\Microsoft.VC80.MFC\*.*" +3 0
  SetOutPath "$INSTDIR\Microsoft.VC80.MFC"
  File "${MSVS_PATH}\Microsoft.VC80.MFC\*.*"
  

  ${if} $MultiUser.InstallMode == "AllUsers"
    ;Store installation folder
    WriteRegStr HKLM "Software\Sphere\GM Tools" "RootPath" "$INSTDIR"
    WriteIniStr "$INSTDIR\Axis2.ini" "SETTINGS Default" "RegInstallation" "Machine"

    ;Store Uninstallation Info
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axis2" "DisplayName" "Axis2 (remove only)"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axis2" "UninstallString" '"$INSTDIR\Axis2_uninst.exe"'
  ${else}
    ;Store installation folder
    WriteRegStr HKCU "Software\Sphere\GM Tools" "RootPath" "$INSTDIR"
    WriteIniStr "$INSTDIR\Axis2.ini" "SETTINGS Default" "RegInstallation" "User"

    ;Store Uninstallation Info
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axis2" "DisplayName" "Axis2 (remove only)"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axis2" "UninstallString" '"$INSTDIR\Axis2_uninst.exe"'
  ${endif}
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Axis2_uninst.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Axis2.lnk" "$INSTDIR\Axis2.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END

  CreateShortCut "$DESKTOP\Axis2.lnk" "$INSTDIR\Axis2.exe"

  DeleteRegKey HKCU "SOFTWARE\Axis2"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Axis2_uninst.exe"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDefault ${LANG_ENGLISH} "Default section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDefault} $(DESC_SecDefault)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

ReadIniStr $0 "$TempInstalldir\Axis2.ini" "SETTINGS Default" "RegInstallation"
  ${if} $0 == "Machine"
    Call un.MultiUser.InstallMode.AllUsers
  ${else}
    Call un.MultiUser.InstallMode.CurrentUser
  ${endif}

Delete "$TempInstalldir\Axis2.exe"
Delete "$TempInstalldir\Axis2.chm"
Delete "$TempInstalldir\Axis2.ini"
Delete "$TempInstalldir\DoorWiz.ini"
Delete "$TempInstalldir\LightWiz.ini"
Delete "$TempInstalldir\changelog.txt"
Delete "$TempInstalldir\hoglocs.scp"
Delete "$DESKTOP\Axis2.lnk"
RMDir /r "$TempInstalldir\Language"
RMDir /r "$TempInstalldir\Microsoft.VC80.CRT"
RMDir /r "$TempInstalldir\Microsoft.VC80.MFC"
Delete "$TempInstalldir\Axis2_uninst.exe"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  StrCmp $StartMenuFolder "" NO_SHORTCUTS  
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Axis2.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  NO_SHORTCUTS:

MessageBox MB_YESNO "Do you want to remove your Axis2 settings?" IDNO Skip
  ${if} $0 == "Machine"
    DeleteRegKey HKLM "SOFTWARE\Sphere\GM Tools"
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Axis2"
  ${else}
    DeleteRegKey HKCU "SOFTWARE\Sphere\GM Tools"
    DeleteRegKey HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Axis2"
  ${endif}
skip:

SectionEnd