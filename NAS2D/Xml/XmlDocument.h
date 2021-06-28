// ==================================================================================
// = NAS2D
// = Copyright � 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "XmlNode.h"

namespace NAS2D {
namespace Xml {

class XmlDocument : public XmlNode
{
public:
	XmlDocument();
	explicit XmlDocument(const std::string& documentName);

	XmlDocument(const XmlDocument& copy);
	XmlDocument& operator=(const XmlDocument& copy);

	const char* parse(const char* p, void* data = nullptr) override;

	const XmlElement* rootElement() const;
	XmlElement* rootElement();

	bool error() const;
	const std::string& errorDesc() const;
	XmlErrorCode errorId() const;
	int errorRow() const;
	int errorCol() const;

	void clearError();

	const XmlDocument* toDocument() const override { return this; }
	XmlDocument* toDocument() override { return this; }

	bool accept(void* content) const override;
	void write(std::string& buf, int depth = 0) const override;

public:
	void error(XmlErrorCode err, const char* errorLocation, void* prevData);

protected:
	XmlNode* clone() const override;
	void streamIn(std::istream& in, std::string& tag) override;

private:
	void copyTo(XmlDocument* target) const;

private:
	XmlErrorCode _errorId;

	bool _error;

	std::string _errorDesc;

	XmlBase::ParseLocation _errorLocation;
};

} // namespace Xml
} // namespace NAS2D
