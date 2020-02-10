#include "NAS2D/MessageBox.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <SDL2/SDL.h>
#include <array>
#endif

namespace NAS2D
{
	int GetMessageBoxIconFlagFromSeverityLevel(MessageBoxSeverityLevel level)
	{
#if defined(_WIN32)
		switch (level)
		{
		case MessageBoxSeverityLevel::Information: return MB_ICONASTERISK;
		case MessageBoxSeverityLevel::Question: return MB_ICONQUESTION;
		case MessageBoxSeverityLevel::Warning: return MB_ICONEXCLAMATION;
		case MessageBoxSeverityLevel::Fatal: return MB_ICONHAND;
		default: return MB_ICONEXCLAMATION;
		}
#else
		switch (level)
		{
		case MessageBoxSeverityLevel::Information: return SDL_MESSAGEBOX_INFORMATION;
		case MessageBoxSeverityLevel::Question: return SDL_MESSAGEBOX_INFORMATION;
		case MessageBoxSeverityLevel::Warning: return SDL_MESSAGEBOX_WARNING;
		case MessageBoxSeverityLevel::Fatal: return SDL_MESSAGEBOX_ERROR;
		default: return SDL_MESSAGEBOX_ERROR;
		}
#endif
	}

	void MessageBox_Okay(std::string_view messageTitle, std::string_view messageText, MessageBoxSeverityLevel severity)
	{
		auto type = GetMessageBoxIconFlagFromSeverityLevel(severity);
#if defined(_WIN32)
		::MessageBoxA(nullptr, messageText.data(), messageTitle.data(), MB_OK | type | MB_TOPMOST | MB_TASKMODAL);
#else
	const std::array<SDL_MessageBoxButtonData, 1> buttons = {
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Ok"}
	};

		const SDL_MessageBoxData messageboxdata =
		{
			type,
			nullptr,
			messageTitle.data(),
			messageText.data(),
			buttons.size(),
			buttons.data(),
			nullptr
		};

		int buttonId = -1;
		auto errorCode = SDL_ShowMessageBox(&messageboxdata, &buttonId);
		if (errorCode == -1)
		{
			return false;
		}
		return buttonId == 0;
#endif
	}

	bool MessageBox_OkayCancel(std::string_view messageTitle, std::string_view messageText, MessageBoxSeverityLevel severity)
	{
		auto type = GetMessageBoxIconFlagFromSeverityLevel(severity);
#if defined(_WIN32)
		auto buttonId = ::MessageBoxA(nullptr, messageText.data(), messageTitle.data(), MB_OKCANCEL | type | MB_TOPMOST | MB_TASKMODAL);
		return buttonId == IDOK;
#else
	const std::array<SDL_MessageBoxButtonData, 1> buttons = {
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Ok"}
		,{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Cancel"}
	};

	const SDL_MessageBoxData messageboxdata =
	{
		type,
		nullptr,
		messageTitle.data(),
		messageText.data(),
		buttons.size(),
		buttons.data(),
		nullptr
	};

	int buttonId = -1;
	auto errorCode = SDL_ShowMessageBox(&messageboxdata, &buttonId);
	if (errorCode == -1)
	{
		return false;
	}
	return buttonId == 0;
#endif
	}


	bool MessageBox_YesNo(std::string_view messageTitle, std::string_view messageText, MessageBoxSeverityLevel severity)
	{
		auto type = GetMessageBoxIconFlagFromSeverityLevel(severity);
#if defined(_WIN32)
		auto buttonId = ::MessageBoxA(nullptr, messageText.data(), messageTitle.data(), MB_YESNO | type | MB_TOPMOST | MB_TASKMODAL);
		return buttonId == IDYES;
#else
	const std::array<SDL_MessageBoxButtonData, 1> buttons = {
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Yes"}
		,{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "No"}
	};

	const SDL_MessageBoxData messageboxdata =
	{
		type,
		nullptr,
		messageTitle.data(),
		messageText.data(),
		buttons.size(),
		buttons.data(),
		nullptr
	};

	int buttonId = -1;
	auto errorCode = SDL_ShowMessageBox(&messageboxdata, &buttonId);
	if (errorCode == -1)
	{
		return false;
	}
	return buttonId == 0;
#endif
	}

} // namespace NAS2D
