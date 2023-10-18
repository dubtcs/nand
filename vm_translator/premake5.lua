
-- global output paths
output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

-- includes directories
includes = {};

-- workspace
workspace "Translator"
    architecture "x64"
    startproject "vmtranslator"

    configurations {
        "Debug",
        "Release",
    }

    -- ENGINE PROJECT
    project "vmtranslator"
        location "translator";
        kind "ConsoleApp";
        language "C++";
        cppdialect "C++20";
        staticruntime "on";

        targetdir ("bin/"..output.."/%{prj.name}");
        objdir ("bin-int/"..output.."/%{prj.name}");

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        };

        includedirs {
            "%{prj.name}/src",
        }

        links {

        }

        defines {

        }

        filter "system:windows"
            systemversion "latest";
            filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
    
        filter "configurations:Dist"
            runtime "Release"
            optimize "On"
    -- END ENGINE
