#pragma once

#include "XmlSection.h"

#include "../Xml/XmlDocument.h"

#include <string>


namespace NAS2D
{
	class XmlFile
	{
	public:
		explicit XmlFile(const std::string& fileData);

		XmlSection root() const;
		XmlSection root(const std::string& name) const;

	protected:
		NAS2D::Xml::XmlDocument mDocument;
	};
}
