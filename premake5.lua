workspace "cdcEngine"
    architecture "x86"
    language "C++"
    configurations { "Debug", "Release" }

project "tr7"
    location "tr7"
    kind "WindowedApp"

    files {
        "tr7/**/*.cpp",
        "tr7/**/*.c"
    }

    characterset "ASCII"
    defines { "_CRT_SECURE_NO_WARNINGS" }

    includedirs { "tr7" }
    
    symbols "On"
    
    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"