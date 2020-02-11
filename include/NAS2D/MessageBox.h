#pragma once

#include <string_view>

namespace NAS2D
{

	enum class MessageBoxSeverityLevel
	{
		Information,
		Question,
		Warning,
		Fatal
	};

	void MessageBox_Okay(std::string_view messageTitle, std::string_view messageText, MessageBoxSeverityLevel severity);
	bool MessageBox_OkayCancel(std::string_view messageTitle, std::string_view messageText, MessageBoxSeverityLevel severity);
	
	bool MessageBox_YesNo(std::string_view messageTitle, std::string_view messageText, MessageBoxSeverityLevel severity);
	int MessageBox_YesNoCancel(std::string_view messageTitle, std::string_view messageText, MessageBoxSeverityLevel severity);

	bool MessageBox_RetryCancel(std::string_view messageTitle, std::string_view messageText, MessageBoxSeverityLevel severity);

} // namespace NAS2D
