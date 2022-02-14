
publicIncludedirs =
{
   "$(ProjectDir)",
   "$(CIMG)",
   "$(EIGEN)",
   "$(BOOST)",
   "$(ASSIMP)/include",
   "$(STB_IMAGE)/include",
   "$(GLM)",
   "$(OPENGL)/include/",
   "../FRAME"
}

publicLibdirs =
{
   
   "$(PARTIO)/Release",
   "$(BOOST)/x64",
   "$(ASSIMP)/lib/x64",
   "$(OPENGL)/lib/x64"
}

publicLinks =
{
   "FRAME.lib",
   "opengl32.lib",
   "glfw3-vc140.lib",
   "assimp-vc140-mt.lib",
   "kernel32.lib",
   "user32.lib",
   "gdi32.lib",	
   "winspool.lib",
   "comdlg32.lib",
   "advapi32.lib",
   "shell32.lib",
   "ole32.lib",
   "oleaut32.lib",
   "uuid.lib",
   "odbc32.lib",
   "odbccp32.lib"   
}



  
project("FitLTCMatrix")          
	kind "ConsoleApp"  
	systemversion "latest"		
	language "C++"
	
	filter { "platforms:x64" }
		system "Windows"
		architecture "x86_64"
		
	files { "**.h", "**.cpp","**.glsl","**.cu","**.cuh" }
	vpaths {
		["Headers"] = "**.h",
		["Sources/*"] = {"**.cpp"},
		["Shader"] = {"**.glsl"},
		["CUDA"] = {"**.cu","**.cuh"}
	}
	debugenvs {"PATH=$(OPENGL)/bin/x64;$(ASSIMP)/bin/x64;%PATH"}
	disablewarnings { 
		"4251",
		"4305",
		"4244"
	}
		
	includedirs {publicIncludedirs}
	links {publicLinks}	  
	libdirs {publicLibdirs}	

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		targetdir "../x64/Debug"
		objdir "x64/Debug"
		links { "glew32d.lib"}	
		libdirs { "../x64/Debug" }
		

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		targetdir "../x64/Release"
		objdir "x64/Release"
		links { "glew32.lib"}	
		libdirs { "../x64/Release" }
  