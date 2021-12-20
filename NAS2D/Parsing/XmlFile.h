#pragma once

#include "XmlSection.h"

#include "../Xml/XmlDocument.h"

#include <string>


namespace NAS2D
{
	class XmlFile
	{
	public:
		static XmlFile Open(const std::string& fileName);

		explicit XmlFile(const std::string& fileData);

		XmlSection root() const;
		XmlSection root(const std::string& name) const;

	protected:
		NAS2D::Xml::XmlDocument mDocument;
	};
}
