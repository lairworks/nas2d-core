#pragma once

#include <string>
#include <vector>


namespace NAS2D {
	void reportMissingOrUnexpected(const std::vector<std::string>& missing, const std::vector<std::string>& unexpected);
	void reportMissingOrUnexpected(const std::vector<std::string>& names, const std::vector<std::string>& required, const std::vector<std::string>& optional);
}
