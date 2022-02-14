
imguiMaster = {
	"../SDK/imgui_master/imconfig.h",
	"../SDK/imgui_master/imgui.cpp",
	"../SDK/imgui_master/imgui.h",
	"../SDK/imgui_master/imgui_draw.cpp",
	"../SDK/imgui_master/examples/imgui_impl_glfw.cpp",
	"../SDK/imgui_master/examples/imgui_impl_glfw.h",
	"../SDK/imgui_master/examples/imgui_impl_opengl3.cpp",
	"../SDK/imgui_master/examples/imgui_impl_opengl3.h",
	"../SDK/imgui_master/imgui_internal.h",
	"../SDK/imgui_master/imgui_widgets.cpp",

}


project("FRAME")          
	kind "SharedLib"  
	systemversion "latest"		
	language "C++"	
	
	files {imguiMaster}
	files { "**.h", "**.cpp","**.hpp","**.glsl","**.cuh"}
	vpaths {
		["Headers"] = {"**.h","**.hpp"},
		["Sources/*"] = {"**.cpp"},
		["Libs/ImGui/*"] = {imguiMaster},
		["Shader"] = {"**.glsl"},
	}
	
	disablewarnings { 
		"4477",
		"4267",
		"4251",
		"4305",
		"4244"
	}
		
	includedirs {
		"../SDK/CUDAHelper/",
		"../SDK/Other/",
		"../SDK/MPMUtility/",
		"../SDK/imgui_master",
		"$(OPENGL)/include/",
		"$(GLM)",
		"$(SOIL)/include",
		"$(STB_IMAGE)/include",
		"$(ASSIMP)/include",
		"$(BOOST)",
		"$(GLI)/include",
		
	}
	links {
		"SOIL-vc140.lib",
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
	
	libdirs { 
		"$(BOOST)/x64",
		"$(ASSIMP)/lib/x64",
		"$(OPENGL)/lib/x64",
		"$(BOOST)/x64",
		"$(SOIL)/lib/x64"
	}

	
	filter { "platforms:Win64" }
		system "Windows"
		architecture "x86_64"
	
	filter "configurations:Debug"
		defines {
			"_DEBUG",
			"_CONSOLE",
			"FRAME_EXPORTS",
			"_SCL_SECURE_NO_WARNINGS",
			"_CRT_SECURE_NO_WARNINGS"
		}
		symbols "On"
		targetdir "../x64/Debug"
		objdir "x64/Debug"
		links { "glew32d.lib" }	  
		ignoredefaultlibraries {"msvcrt.lib"}

	filter "configurations:Release"
		defines {
			"NDEBUG",
			"_CONSOLE",
			"FRAME_EXPORTS",
			"_CRT_SECURE_NO_WARNINGS"
		}
		optimize "On"
		targetdir "../x64/Release"
		objdir "x64/Release"
		links { "glew32.lib" }	
		