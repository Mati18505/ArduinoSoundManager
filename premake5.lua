workspace "ArduinoSoundManager"
    configurations { "Debug", "Release", "Dist" }
    platforms { "Win64" }
    startproject "ArduinoSoundManager"

    flags { "MultiProcessorCompile" }

    outputdir = "%{cfg.platform}-%{cfg.buildcfg}"

    systemversion "latest"

    filter "configurations:Debug" 
        defines { "DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"

    filter "platforms:Win64"
        cppdialect "C++20"
        architecture "x86_64"
        buildoptions { "/ZI" }
        
    include "ArduinoConnection"
    include "ArduinoSoundManager"
    include "UnitTests"


workspace "*"
    newaction {
        trigger     = "clean",
        description = "Remove binaries, intermediate files and vs project files",
        execute     = function ()
        print("Removing binaries")
        os.rmdir("./bin")
        print("Removing intermediate files")
        os.rmdir("./bin-int")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
        end
    }