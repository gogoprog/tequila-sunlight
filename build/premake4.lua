
solution "tequila-sunlight"
	configurations { "debug", "release" }

	-- A project defines one build target
	project "tequila-sunlight"
		kind "ConsoleApp"
		language "C++"
		files { "../src/daemon/*.h", "../src/daemon/*.c*" }
		links { "pthread", "boost_system", "boost_thread", "dl" }

		configuration "debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "release"
			defines { "NDEBUG" }
			flags { "Optimize" }    
