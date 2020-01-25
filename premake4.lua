#!lua

if os.execute("clang -v") then
	premake.gcc.cc = "clang"
	premake.gcc.cxx = "clang++"
end

solution "Own"
	location "."
	targetdir "bin"
	configurations { "debug", "release" }
	platforms { "native" }

	project "PONG"
		kind "ConsoleApp"
		language "C++"
		files { "pong/**.h", "pong/**.cpp" }
		links { "SDL2" }
		buildoptions {"--std=c++17", "-Wall"}

	project "SideScroll"
		kind "ConsoleApp"
		language "C++"
		files { "scroll/**.h", "scroll/**.cpp" }
		links { "SDL2", "SDL2_image"}
		buildoptions {"--std=c++17", "-Wall"}

	project "AAAsteroids"
		kind "ConsoleApp"
		language "C++"
		files { "aaasteroids/**.h", "aaasteroids/**.cpp" }
		links { "SDL2", "SDL2_image"}
		buildoptions {"--std=c++17", "-Wall"}

	configuration "debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "release"
		defines { "NDEBUG" }
		flags { "Optimize" }
