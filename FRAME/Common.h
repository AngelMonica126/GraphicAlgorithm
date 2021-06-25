#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <iostream>
#include <GLM/glm.hpp>
#include "FRAME_EXPORTS.h"

namespace ElayGraphics
{
#ifdef _DEBUG
	#define _WARNING(condition, response) if(condition) {std::clog << response << std::endl;}
#else
	#define _WARNING(condition, response) ((void)0)
#endif

#define MAX_VALUE 3.402823466e+38F
#define MIN_VALUE 1.175494351e-38F
	const float PI = 3.14159265359f;

	namespace WINDOW_KEYWORD
	{
		extern int WINDOW_WIDTH;
		extern int WINDOW_HEIGHT;
		extern int VIEWPORT_WIDTH;
		extern int VIEWPORT_HEIGHT;
		extern int VIEWPORT_LEFTBOTTOM_X;
		extern int VIEWPORT_LEFTBOTTOM_Y;
		extern int NUM_SAMPLES;
		extern bool CURSOR_DISABLE;
		extern std::string WINDOW_TITLE;
	}

	namespace COMPONENT_CONFIG
	{
		extern bool IS_ENABLE_GUI;
	}

	struct FRAME_DLLEXPORTS STexture
	{
		enum class ETextureType
		{
			Texture2D = GL_TEXTURE_2D,
			Texture3D = GL_TEXTURE_3D,
			Texture2DArray = GL_TEXTURE_2D_ARRAY,
			TextureCubeMap = GL_TEXTURE_CUBE_MAP,
			DepthCubeMap = GL_DEPTH_COMPONENT,
		};
		enum class ETextureAttachmentType
		{
			ColorTexture,
			DepthTexture,
			CubeDepthTexture,
			StencilTexture,
			DepthAndStencilTexture,
			DepthArrayTexture
		};
		GLint					InternalFormat;
		GLenum					ExternalFormat;
		GLenum					DataType;
		GLsizei					Width;
		GLsizei					Height;
		GLsizei					Depth;
		std::vector<GLvoid*>	pDataSet;
		GLint					Type4WrapS;
		GLint					Type4WrapT;
		GLint					Type4WrapR;
		GLint					Type4MinFilter;
		GLint					Type4MagFilter;
		GLboolean				isMipmap;
		GLboolean				isSRGBSpace;
		GLint					TextureID;
		GLint					ImageBindUnit;
		GLint					FrameBufferID;
		GLint					AttachmentID;
		std::string				TextureName;
		ETextureType			TextureType;
		ETextureAttachmentType	TextureAttachmentType;
		std::vector<float>		BorderColor;

		STexture() : TextureID(-1), InternalFormat(GL_RGBA), ExternalFormat(GL_RGBA), DataType(GL_UNSIGNED_BYTE), Width(WINDOW_KEYWORD::WINDOW_WIDTH),
			Height(WINDOW_KEYWORD::WINDOW_HEIGHT), Depth(1), Type4WrapS(GL_REPEAT), Type4WrapT(GL_REPEAT), Type4WrapR(GL_REPEAT), Type4MinFilter(GL_LINEAR),
			Type4MagFilter(GL_LINEAR), isMipmap(GL_FALSE), isSRGBSpace(GL_FALSE), TextureType(ETextureType::Texture2D), TextureAttachmentType(ETextureAttachmentType::ColorTexture), 
			BorderColor{ 1,1,1,1 }, ImageBindUnit(-1), FrameBufferID(-1), AttachmentID(-1)
		{
		}

		bool operator<(const ElayGraphics::STexture& vAnotherTexture) const
		{
			return TextureID < vAnotherTexture.TextureID;
		}
	};

	struct FRAME_DLLEXPORTS SPointLight
	{
		glm::vec4 Position;
		glm::vec4 Color;
	};

	struct FRAME_DLLEXPORTS SDirectionalLight
	{
		glm::vec4 Direction;
		glm::vec4 Color;
	};

	enum class EDataType
	{
		DataType_I32,      // int
		DataType_U32,      // unsigned int
		DataType_I64,      // long long, __int64
		DataType_U64,      // unsigned long long, unsigned __int64
		DataType_Float,    // float
		DataType_Double,   // double
	};

	enum class EGUIItemColor
	{
		GUIItemColor_Text,
		GUIItemColor_TextDisabled,
		GUIItemColor_WindowBg,              // Background of normal windows
		GUIItemColor_ChildBg,               // Background of child windows
		GUIItemColor_PopupBg,               // Background of popups, menus, tooltips windows
		GUIItemColor_Border,
		GUIItemColor_BorderShadow,
		GUIItemColor_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
		GUIItemColor_FrameBgHovered,
		GUIItemColor_FrameBgActive,
		GUIItemColor_TitleBg,
		GUIItemColor_TitleBgActive,
		GUIItemColor_TitleBgCollapsed,
		GUIItemColor_MenuBarBg,
		GUIItemColor_ScrollbarBg,
		GUIItemColor_ScrollbarGrab,
		GUIItemColor_ScrollbarGrabHovered,
		GUIItemColor_ScrollbarGrabActive,
		GUIItemColor_CheckMark,
		GUIItemColor_SliderGrab,
		GUIItemColor_SliderGrabActive,
		GUIItemColor_Button,
		GUIItemColor_ButtonHovered,
		GUIItemColor_ButtonActive,
		GUIItemColor_Header,
		GUIItemColor_HeaderHovered,
		GUIItemColor_HeaderActive,
		GUIItemColor_Separator,
		GUIItemColor_SeparatorHovered,
		GUIItemColor_SeparatorActive,
		GUIItemColor_ResizeGrip,
		GUIItemColor_ResizeGripHovered,
		GUIItemColor_ResizeGripActive,
		GUIItemColor_PlotLines,
		GUIItemColor_PlotLinesHovered,
		GUIItemColor_PlotHistogram,
		GUIItemColor_PlotHistogramHovered,
		GUIItemColor_TextSelectedBg,
		GUIItemColor_DragDropTarget,
		GUIItemColor_NavHighlight,          // Gamepad/keyboard: current highlighted item
		GUIItemColor_NavWindowingHighlight, // Highlight window when using CTRL+TAB
		GUIItemColor_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
		GUIItemColor_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
		GUIItemColor_COUNT

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		, GUIItemColor_ChildWindowBg = GUIItemColor_ChildBg, GUIItemColor_Column = GUIItemColor_Separator, GUIItemColor_ColumnHovered = GUIItemColor_SeparatorHovered, GUIItemColor_ColumnActive = GUIItemColor_SeparatorActive
		, GUIItemColor_ModalWindowDarkening = GUIItemColor_ModalWindowDimBg
#endif
	};

	enum class ERenderPassType
	{
		RenderPassType_Normal,     
		RenderPassType_Once,      
		RenderPassType_Parallel,     
		RenderPassType_ParallelOnce,     
		RenderPassType_Delay,     
	};
}