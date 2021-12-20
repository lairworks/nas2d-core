#pragma once

#include "XmlSubSectionRange.h"
#include "XmlNamedSubSectionRange.h"

#include "../StringValue.h"

#include <string>
#include <vector>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}


	class XmlSection
	{
	public:
		explicit XmlSection(const NAS2D::Xml::XmlElement& xmlElement);

		const std::string& name() const;

		void throwError(const std::string& errorMessage) const;
		void verifySubSections(const std::vector<std::string>& required, const std::vector<std::string>& optional = {}) const;
		void verifyKeys(const std::vector<std::string>& required, const std::vector<std::string>& optional = {}) const;

		std::vector<std::string> subSectionNames() const;
		std::vector<std::string> keys() const;

		bool hasSubSection(const std::string& name) const;
		bool hasKey(const std::string& key) const;

		XmlSection subSection(const std::string& name) const;
		XmlSubSectionRange subSections() const;
		XmlNamedSubSectionRange subSections(std::string name) const;

		NAS2D::StringValue valueOrEmpty(const std::string& key) const;

		template <typename T = std::string>
		T value(const std::string& key) const
		{
			if (!hasKey(key))
			{
				throwError("Required key name not found: " + key);
			}
			return valueOrEmpty(key).to<T>();
		}

		template <typename T = std::string>
		T value(const std::string& key, T defaultValue) const
		{
			return hasKey(key) ? valueOrEmpty(key).to<T>() : defaultValue;
		}

	protected:
		const NAS2D::Xml::XmlElement& mXmlElement;
	};
}
