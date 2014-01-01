!define File "..\release\Axis2.exe"
 
OutFile "GetVersion.exe"
 
Function .onInit
 ## Get file version
 GetDllVersion "${File}" $R0 $R1
  IntOp $R2 $R0 / 0x00010000
  IntOp $R3 $R0 & 0x0000FFFF
  IntOp $R4 $R1 / 0x00010000
  IntOp $R5 $R1 & 0x0000FFFF
  IntOp $R6 $R5 + 0x060
  IntFmt $R7 %c $R6
  StrCpy $R1 "$R2.$R3.$R4$R7"
  StrCpy $R8 "$R2.$R3.$R4.$R5"
 
 ## Write it to a !define for use in main script
 FileOpen $R0 "$EXEDIR\Version.txt" w
  FileWrite $R0 '!define Version "$R1"'
  FileWriteByte $R0 "13"
  FileWriteByte $R0 "10"
  FileWrite $R0 '!define XVersionX "$R8"'
 FileClose $R0

 ## Write it to a version file
 ##FileOpen $R0 "$EXEDIR\AxisII.ver" w
 ## FileWrite $R0 '$R8'
 ##FileClose $R0
 
 Abort
FunctionEnd
 
Section
SectionEnd