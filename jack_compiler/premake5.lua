
-- global output paths
output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

-- includes directories
includes = {};

-- workspace
workspace "jcom"
    architecture "x64"
    startproject "jcom2"

    configurations {
        "Debug",
        "Release",
    }

    project "jcom"
        location "jcom";
        kind "ConsoleApp";
        language "C++";
        cppdialect "C++20";
        staticruntime "on";

        targetdir ("bin/"..output.."/%{prj.name}");
        objdir ("bin-int/"..output.."/%{prj.name}");

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/main.cpp"
        };

        includedirs {
            "%{prj.name}/src",
        }

        links {

        }

        defines {

        }

        filter "system:windows"
            systemversion "latest"

        filter "configurations:Debug"
            defines {"JCOM_DEBUG"}
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
    
        filter "configurations:Dist"
            runtime "Release"
            optimize "On"

    project "jcom2"
        location "jcom2";
        kind "ConsoleApp";
        language "C++";
        cppdialect "C++20";
        staticruntime "on";

        targetdir ("bin/"..output.."/%{prj.name}");
        objdir ("bin-int/"..output.."/%{prj.name}");

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/main.cpp"
        };

        includedirs {
            "%{prj.name}/src",
        }

        links {

        }

        defines {

        }

        filter "system:windows"
            systemversion "latest"

        filter "configurations:Debug"
            defines {"JCOM_DEBUG"}
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
    
        filter "configurations:Dist"
            runtime "Release"
            optimize "On"
            