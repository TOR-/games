#!lua

if os.execute("clang -v") then
	premake.gcc.cc = "clang"
	premake.gcc.cxx = "clang++"
end

solution "Own"
	location "."
	targetdir "bin"
	configurations { "debug" }
	platforms { "native" }

	project "ChapterOne"
		kind "ConsoleApp"
		language "C++"
		files { "ch1/**.h", "ch1/**.cpp" }
		links { "SDL2" }
		buildoptions {"--std=c++17", "-Wall"}

		configuration "debug"
			defines { "DEBUG" }
			flags { "Symbols" }

