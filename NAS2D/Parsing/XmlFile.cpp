#include "XmlFile.h"

#include "../Xml/XmlElement.h"
#include "../Utility.h"
#include "../Filesystem.h"

#include <stdexcept>


using namespace NAS2D;


XmlFile XmlFile::Open(const std::string& fileName)
{
	return XmlFile{Utility<Filesystem>::get().read(fileName)};
}


XmlFile::XmlFile(const std::string& fileData)
{
	mDocument.parse(fileData.c_str());
	if (mDocument.error())
	{
		const auto positionString = " at (line " + std::to_string(mDocument.errorRow()) + ", column " + std::to_string(mDocument.errorCol()) + ")";
		throw std::runtime_error(mDocument.errorDesc() + positionString);
	}
}


XmlSection XmlFile::root() const
{
	const auto* rootElement = mDocument.rootElement();
	if (!rootElement){
		throw std::runtime_error("No root element found in XML data");
	}
	return XmlSection{*rootElement};
}


XmlSection XmlFile::root(const std::string& name) const
{
	const auto rootElement = root();
	if (rootElement.name() != name)
	{
		rootElement.throwError("Unexpected XML root tag. Expected: '" + name + "', Found: '" + rootElement.name() + "'");
	}
	return rootElement;
}
