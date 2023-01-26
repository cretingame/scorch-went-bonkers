[Setup]
AppName=Scorch Went Bonkers
AppVerName=Scorch Went Bonkers 1.03
DefaultDirName={pf}\SWB
DefaultGroupName=Scorch Went Bonkers
OutputDir=archive
Compression=bzip/9
UninstallFilesDir={app}\uninst

[Types]
Name: "full"; Description: "Full instalation"

[Components]
Name: "main"; Description: "Program Files"; Types: full; Flags: fixed

[Tasks]
Name: desktopicon;     Description: "Create a &desktop icon"; GroupDescription: "Additional tasks:"; Components: main

[Files]
Source: "swb.exe";                       DestDir: "{app}";                Components: main;
Source: "docs\readme.htm";               DestDir: "{app}";                Components: main;
Source: "archive\alleg40.dll";           DestDir: "{app}";                Components: main;
Source: "archive\fmod.dll";              DestDir: "{app}";                Components: main;
Source: "gfx\*.*";                       DestDir: "{app}\gfx";            Components: main;
Source: "gfx\back\*.*";                  DestDir: "{app}\gfx\back";       Components: main;
Source: "gfx\bullets\*.*";               DestDir: "{app}\gfx\bullets";    Components: main;
Source: "gfx\explosions\*.*";            DestDir: "{app}\gfx\explosions"; Components: main;
Source: "gfx\tanks\*.*";                 DestDir: "{app}\gfx\tanks";      Components: main;
Source: "gfx\logo\*.*";                  DestDir: "{app}\gfx\logo";       Components: main;
Source: "gfx\splash\*.*";                DestDir: "{app}\gfx\splash";     Components: main;
Source: "gfx\targets\*.*";               DestDir: "{app}\gfx\targets";    Components: main;
Source: "gfx\terrain\*.*";               DestDir: "{app}\gfx\terrain";    Components: main;
Source: "snd\*.*";                       DestDir: "{app}\snd";            Components: main;
Source: "mus\*.*";			     DestDir: "{app}\mus";		  Components: main;
Source: "fonts\*.*";			     DestDir: "{app}\fonts";		  Components: main;

[Icons]
Name: "{group}\Scorch Went Bonkers"; Filename: "{app}\swb.exe"; WorkingDir: "{app}"
Name: "{group}\Uninstall Wriggle"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Scorch Went Bonkers"; Filename: "{app}\swb.exe"; WorkingDir: "{app}"; Tasks: desktopicon