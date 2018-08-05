os.execute("mkdir bin")
os.execute("cp -r dll/x64/* bin/")

local project_list = {
  "EJ03.Pathfinding"
}

local function new_project(name)
  project (name)   
    kind ("ConsoleApp")
    includedirs {"include"}
    files { "tests/"..name.."/*.cpp", 
            "tests/"..name.."/*.h",
            "include/**.h",
            "src/**.cpp",
            "src/**.cc",
            "src/**.h"
          }       
    excludes {"src/mathlib/*_sse.cc"}
    vpaths{ ["Headers"] = "include/*.h",
            ["Headers/MathLib"] = "include/mathlib/*.h",
            ["Source"] = {"src/*.cpp", "src/*.cc"},
            ["Source/MathLib"] = "src/mathlib/*.cc",
            ["Tests"] = { "tests/"..name.."/*.cpp",
                          "tests/"..name.."/*.h"}
          }
    libdirs {"lib/x64"}           
    links { "SDL2", "SDL2main", "SDL2_ttf","SDL2_image"}             
    configuration {"debug"}
      flags {"Symbols"}
      targetsuffix ("_d")
    configuration {"release"}
      flags {"Optimize"}
      targetsuffix ("_r")
    configuration {"windows"}
      flags {"NoEditAndContinue"}
      windowstargetplatformversion "10.0.17134.0"
end

solution "05MVID"
  configurations { "debug", "release"}
  language ("c++")
  platforms {"x64"}
  location ("build")
  debugdir ("build")
  targetdir ("bin")
  flags {"ExtraWarnings"}

for k,v in pairs(project_list) do
  new_project(v)
end