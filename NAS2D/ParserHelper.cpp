
#include "ParserHelper.h"
#include "StringUtils.h"
#include "ContainerUtils.h"


namespace NAS2D {
	void reportMissingOrUnexpected(const std::vector<std::string>& missing, const std::vector<std::string>& unexpected)
	{
		if (!missing.empty() || !unexpected.empty())
		{
			const auto missingString = !missing.empty() ? "Missing names: {" + join(missing, ", ") +"}" : "";
			const auto unexpectedString = !unexpected.empty() ? "Unexpected names: {" + join(unexpected, ", ") + "}" : "";
			const auto joinString = (!missingString.empty() && !unexpectedString.empty()) ? "\n" : "";
			throw std::runtime_error(missingString + joinString + unexpectedString);
		}
	}

	void reportMissingOrUnexpected(const std::vector<std::string>& names, const std::vector<std::string>& required, const std::vector<std::string>& optional)
	{
		const auto missing = missingValues(names, required);
		const auto unexpected = unexpectedValues(names, required, optional);
		reportMissingOrUnexpected(missing, unexpected);
	}
}
