project "DgLib"
  location ""
  kind "StaticLib"
  targetdir ("%{wks.location}/build/%{prj.name}-%{cfg.buildcfg}")
  objdir ("%{wks.location}/build/intermediate/%{prj.name}-%{cfg.buildcfg}")
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "src/**.h",
    "src/**.cpp",
    "src/impl/**.h",
    "src/impl**.cpp"
  }
  
  include "DgLib_vpaths.lua"
    
  filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    runtime "Release"
    optimize "on"