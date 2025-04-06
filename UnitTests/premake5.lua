project "UnitTests"
    kind "SharedLib"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}") 
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "%{wks.location}/ArduinoConnection/source",
        "D:/VisualStudio22/VC/UnitTest/include"
    }

    files {
        "./source/**.cpp",
        "./source/**.h"
    }

    libdirs {
        "D:/VisualStudio22/VC/UnitTest/lib"
    }

    links { "ArduinoConnection" }