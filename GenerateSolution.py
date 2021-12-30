import subprocess
import sys
sys.path.insert(0, '.\\3rdParty\\BuildScripts')

import DgBuild

DgBuild.Make_vpaths("src", "DgLib_vpaths.lua")

subprocess.call("3rdParty\premake\premake5.exe vs2022")
