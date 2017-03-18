// ==================================================================================
// = www.sourceforge.net/projects/tinyxml
// = Original code by Lee Thomason (www.grinninglizard.com)
// ==================================================================================
// = This software is provided 'as-is', without any express or implied warranty. In
// = no event will the authors be held liable for any damages arising from the use of
// = this software.
// = 
// = Permission is granted to anyone to use this software for any purpose, including
// = commercial applications, and to alter it and redistribute it freely, subject to
// = the following restrictions:
// = 
// = 1. The origin of this software must not be misrepresented; you must not claim
// = that you wrote the original software. If you use this software in a product, an
// = acknowledgment in the product documentation would be appreciated but is not
// = required.
// = 
// = 2. Altered source versions must be plainly marked as such, and must not be
// = misrepresented as being the original software.
// = 
// = 3. This notice may not be removed or altered from any source distribution.
// ==================================================================================
#include <assert.h>
#include <ctype.h>

#include "NAS2D/XML/Xml.h"

using namespace NAS2D::Xml;

bool TiXmlBase::condenseWhiteSpace = true;

const std::string TIXML_EMPTY_STR = "";


// ==================================================================================
// = XmlBase Implementation
// ==================================================================================
void TiXmlBase::EncodeString(const std::string& str, std::string& outString)
{
	size_t i = 0;

	while (i < str.length())
	{
		unsigned char c = (unsigned char)str[i];

		if (c == '&'
			&& i < (str.length() - 2)
			&& str[i + 1] == '#'
			&& str[i + 2] == 'x')
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while (i < str.length() - 1)
			{
				outString.append(str[i], 1);
				++i;
				if (str[i] == ';')
					break;
			}
		}
		else if (c == '&')
		{
			outString.append(entity[0].str, entity[0].strLength);
			++i;
		}
		else if (c == '<')
		{
			outString.append(entity[1].str, entity[1].strLength);
			++i;
		}
		else if (c == '>')
		{
			outString.append(entity[2].str, entity[2].strLength);
			++i;
		}
		else if (c == '\"')
		{
			outString.append(entity[3].str, entity[3].strLength);
			++i;
		}
		else if (c == '\'')
		{
			outString.append(entity[4].str, entity[4].strLength);
			++i;
		}
		else if (c < 32)
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[32];

			#if defined(TIXML_SNPRINTF)		
			TIXML_SNPRINTF(buf, sizeof(buf), "&#x%02X;", (unsigned)(c & 0xff));
			#else
			sprintf(buf, "&#x%02X;", (unsigned)(c & 0xff));
			#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString.append(buf, (int)strlen(buf));
			++i;
		}
		else
		{
			outString += c;
			++i;
		}
	}
}


void TiXmlBase::fillErrorTable()
{
	TiXmlBase::errorString.push_back("No error");
	TiXmlBase::errorString.push_back("Unspecified Error");
	TiXmlBase::errorString.push_back("Error: Failed to open file");
	TiXmlBase::errorString.push_back("Error parsing Element.");
	TiXmlBase::errorString.push_back("Failed to read Element name.");
	TiXmlBase::errorString.push_back("Error reading Element value.");
	TiXmlBase::errorString.push_back("Error reading Attributes.");
	TiXmlBase::errorString.push_back("Error: Empty tag.");
	TiXmlBase::errorString.push_back("Error reading end tag.");
	TiXmlBase::errorString.push_back("Error parsing Unknown.");
	TiXmlBase::errorString.push_back("Error parsing Comment.");
	TiXmlBase::errorString.push_back("Error parsing Declaration.");
	TiXmlBase::errorString.push_back("Error: Document empty.");
	TiXmlBase::errorString.push_back("Error: Unexpected EOF found in input stream.");
	TiXmlBase::errorString.push_back("Error parsing CDATA.");
	TiXmlBase::errorString.push_back("Error adding XmlDocument to document: XmlDocument can only be at the root.");
}



// ==================================================================================
// = XmlNode Implementation
// ==================================================================================
TiXmlNode::~TiXmlNode()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = nullptr;

	while (node)
	{
		temp = node;
		node = node->next;
		delete temp;
	}
}


void TiXmlNode::CopyTo(TiXmlNode* target) const
{
	target->SetValue(value);
	target->userData = userData;
	target->location = location;
}


void TiXmlNode::Clear()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = nullptr;

	while (node)
	{
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = nullptr;
	lastChild = nullptr;
}


TiXmlNode* TiXmlNode::LinkEndChild(TiXmlNode* node)
{
	assert(node->parent == nullptr || node->parent == this);
	assert(node->GetDocument() == nullptr || node->GetDocument() == this->GetDocument());

	if (node->Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		delete node;
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = nullptr;

	if (lastChild)
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertEndChild(const TiXmlNode& addThis)
{
	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}
	TiXmlNode* node = addThis.Clone();
	if (!node)
		return nullptr;

	return LinkEndChild(node);
}


TiXmlNode* TiXmlNode::InsertBeforeChild(TiXmlNode* beforeThis, const TiXmlNode& addThis)
{
	if (!beforeThis || beforeThis->parent != this)
		return nullptr;

	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}

	TiXmlNode* node = addThis.Clone();
	if (!node)
		return nullptr;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if (beforeThis->prev)
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert(firstChild == beforeThis);
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertAfterChild(TiXmlNode* afterThis, const TiXmlNode& addThis)
{
	if (!afterThis || afterThis->parent != this) {
		return nullptr;
	}
	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}

	TiXmlNode* node = addThis.Clone();
	if (!node)
		return nullptr;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if (afterThis->next)
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert(lastChild == afterThis);
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


TiXmlNode* TiXmlNode::ReplaceChild(TiXmlNode* replaceThis, const TiXmlNode& withThis)
{
	if (!replaceThis)
		return nullptr;

	if (replaceThis->parent != this)
		return nullptr;

	if (withThis.ToDocument()) {
		// A document can never be a child.	Thanks to Noam.
		TiXmlDocument* document = GetDocument();
		if (document)
			document->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}

	TiXmlNode* node = withThis.Clone();
	if (!node)
		return nullptr;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if (replaceThis->next)
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if (replaceThis->prev)
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool TiXmlNode::RemoveChild(TiXmlNode* removeThis)
{
	if (!removeThis)
		return false;

	if (removeThis->parent != this)
	{
		assert(0); // ??? Not really a great way to do this.
		return false;
	}

	if (removeThis->next)
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if (removeThis->prev)
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const TiXmlNode* TiXmlNode::FirstChild(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;
	for (node = firstChild; node; node = node->next)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const TiXmlNode* TiXmlNode::LastChild(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;
	for (node = lastChild; node; node = node->prev)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const TiXmlNode* TiXmlNode::IterateChildren(const TiXmlNode* previous) const
{
	if (!previous)
	{
		return FirstChild();
	}
	else
	{
		assert(previous->parent == this);
		return previous->NextSibling();
	}
}


const TiXmlNode* TiXmlNode::IterateChildren(const std::string& val, const TiXmlNode* previous) const
{
	if (!previous)
	{
		return FirstChild(val);
	}
	else
	{
		assert(previous->parent == this);
		return previous->NextSibling(val);
	}
}


const TiXmlNode* TiXmlNode::NextSibling(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;
	for (node = next; node; node = node->next)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const TiXmlNode* TiXmlNode::PreviousSibling(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;
	for (node = prev; node; node = node->prev)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const TiXmlElement* TiXmlNode::FirstChildElement() const
{
	const TiXmlNode* node = nullptr;

	for (node = FirstChild(); node; node = node->NextSibling())
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const TiXmlElement* TiXmlNode::FirstChildElement(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;

	for (node = FirstChild(_value);	node; node = node->NextSibling(_value))
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const TiXmlElement* TiXmlNode::NextSiblingElement() const
{
	const TiXmlNode* node = nullptr;

	for (node = NextSibling(); node; node = node->NextSibling())
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const TiXmlElement* TiXmlNode::NextSiblingElement(const std::string&  _value) const
{
	const TiXmlNode* node = nullptr;

	for (node = NextSibling(_value); node; node = node->NextSibling(_value))
	{
		if (node->ToElement())
			return node->ToElement();
	}

	return nullptr;
}


const TiXmlDocument* TiXmlNode::GetDocument() const
{
	const TiXmlNode* node;

	for (node = this; node; node = node->parent)
	{
		if (node->ToDocument())
			return node->ToDocument();
	}
	return nullptr;
}


// ==================================================================================
// = XmlElement Implementation
// ==================================================================================
TiXmlElement& TiXmlElement::operator=( const TiXmlElement& base )
{
	ClearThis();
	base.CopyTo( this );
	return *this;
}


TiXmlElement::~TiXmlElement()
{
	ClearThis();
}


void TiXmlElement::ClearThis()
{
	Clear();
	while (attributeSet.First())
	{
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove(node);
		delete node;
	}
}


std::string TiXmlElement::Attribute(const std::string& name) const
{
	const TiXmlAttribute* node = attributeSet.Find(name);
	if (node)
		return node->Value();
	return "";
}


std::string TiXmlElement::Attribute(const std::string& name, int& i) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	std::string result;

	if (attrib)
	{
		result = attrib->Value();
		if (i)
		{
			attrib->QueryIntValue(i);
		}
	}
	return result;
}


std::string TiXmlElement::Attribute(const std::string& name, double& d) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	std::string result;

	if (attrib)
	{
		result = attrib->Value();
		if (d)
		{
			attrib->QueryDoubleValue(d);
		}
	}
	return result;
}


int TiXmlElement::QueryIntAttribute(const std::string& name, int& ival) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	if (!attrib)
		return TIXML_NO_ATTRIBUTE;

	return attrib->QueryIntValue(ival);
}


int TiXmlElement::QueryDoubleAttribute(const std::string& name, double& dval) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	if (!attrib)
		return TIXML_NO_ATTRIBUTE;

	return attrib->QueryDoubleValue(dval);
}


void TiXmlElement::SetAttribute(const std::string& name, int val)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	
	if (attrib)
		attrib->SetIntValue(val);
}


void TiXmlElement::SetDoubleAttribute(const std::string& name, double val)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib)
		attrib->SetDoubleValue(val);
}


void TiXmlElement::SetAttribute(const std::string& cname, const std::string& cvalue)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(cname);
	if (attrib)
	{
		attrib->SetValue(cvalue);
	}
}


void TiXmlElement::Print(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; ++i)
		buf += "\t";

	buf += "<" + value;

	const TiXmlAttribute* attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next())
	{
		buf += " ";
		attrib->Print(buf, depth);
	}

	TiXmlNode* node;
	if (!firstChild)
	{
		buf += " />";
	}
	else if (firstChild == lastChild && firstChild->ToText())
	{
		buf += ">";
		firstChild->Print(buf, depth + 1);
		buf += "</" + value + ">";
	}
	else
	{
		buf += ">";

		for (node = firstChild; node; node = node->NextSibling())
		{
			if (!node->ToText())
				buf += "\n";

			node->Print(buf, depth + 1);
		}

		buf += "\n";

		for (int i = 0; i < depth; ++i)
			buf += "\t";

		buf += "</" + value + ">";
	}
}


void TiXmlElement::CopyTo(TiXmlElement* target) const
{
	// superclass:
	TiXmlNode::CopyTo(target);

	// Element class: 
	// Clone the attributes, then clone the children.
	const TiXmlAttribute* attribute = nullptr;
	for (attribute = attributeSet.First(); attribute; attribute = attribute->Next())
	{
		target->SetAttribute(attribute->Name(), attribute->Value());
	}

	TiXmlNode* node = nullptr;
	for (node = firstChild; node; node = node->NextSibling())
	{
		target->LinkEndChild(node->Clone());
	}
}

bool TiXmlElement::Accept(XmlVisitor* visitor) const
{
	if (visitor->VisitEnter(*this, attributeSet.First()))
	{
		for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
		{
			if (!node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit(*this);
}


TiXmlNode* TiXmlElement::Clone() const
{
	TiXmlElement* clone = new TiXmlElement(Value());
	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}


const std::string& TiXmlElement::GetText() const
{
	const TiXmlNode* child = this->FirstChild();
	if (child)
	{
		const TiXmlText* childText = child->ToText();
		if (childText)
		{
			return childText->Value();
		}
	}
	return TIXML_EMPTY_STR;
}


void TiXmlElement::RemoveAttribute(const std::string& name)
{
	TiXmlAttribute* node = attributeSet.Find(name);
	if (node)
	{
		attributeSet.Remove(node);
		delete node;
	}
}


// ==================================================================================
// = XmlDocument Implementation
// ==================================================================================
TiXmlDocument::TiXmlDocument() : TiXmlNode(TiXmlNode::TINYXML_DOCUMENT)
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}


TiXmlDocument::TiXmlDocument(const std::string& documentName) : TiXmlNode(TiXmlNode::TINYXML_DOCUMENT)
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}


TiXmlDocument::TiXmlDocument(const TiXmlDocument& copy) : TiXmlNode(TiXmlNode::TINYXML_DOCUMENT)
{
	copy.CopyTo(this);
}


TiXmlDocument& TiXmlDocument::operator=(const TiXmlDocument& copy)
{
	Clear();
	copy.CopyTo(this);
	return *this;
}


void TiXmlDocument::CopyTo(TiXmlDocument* target) const
{
	TiXmlNode::CopyTo(target);

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	TiXmlNode* node = nullptr;
	for (node = firstChild; node; node = node->NextSibling())
	{
		target->LinkEndChild(node->Clone());
	}
}


TiXmlNode* TiXmlDocument::Clone() const
{
	TiXmlDocument* clone = new TiXmlDocument();
	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}


void TiXmlDocument::Print(std::string& buf, int depth) const
{
	for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
	{
		node->Print(buf, depth);
		buf += "\n";
	}
}


bool TiXmlDocument::Accept(XmlVisitor* visitor) const
{
	if (visitor->VisitEnter(*this))
	{
		for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
		{
			if (!node->Accept(visitor))
				break;
		}
	}

	return visitor->VisitExit(*this);
}



// ==================================================================================
// = XmlAttribute Implementation
// ==================================================================================
const TiXmlAttribute* TiXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	if (next->value.empty() && next->name.empty())
		return nullptr;

	return next;
}


const TiXmlAttribute* TiXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	if (prev->value.empty() && prev->name.empty())
		return nullptr;

	return prev;
}


void TiXmlAttribute::Print(std::string& buf, int /*depth*/) const
{
	std::string n, v;

	EncodeString(name, n);
	EncodeString(value, v);

	if (value.find('\"') == std::string::npos)
	{
		buf += n;
		buf += "=\"";
		buf += v;
		buf += "\"";
	}
	else
	{
		buf += n;
		buf += "='";
		buf += v;
		buf += "'";
	}
}


int TiXmlAttribute::QueryIntValue(int& ival) const
{
	if (TIXML_SSCANF(value.c_str(), "%d", &ival) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}


int TiXmlAttribute::QueryDoubleValue(double& dval) const
{
	if (TIXML_SSCANF(value.c_str(), "%lf", &dval) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}


void TiXmlAttribute::SetIntValue(int _value)
{
	SetValue(std::to_string(_value));
}


void TiXmlAttribute::SetDoubleValue(double _value)
{
	SetValue(std::to_string(_value));
}


int TiXmlAttribute::IntValue() const
{
	return std::stoi(value);
}


double  TiXmlAttribute::DoubleValue() const
{
	return std::stof(value);
}



// ==================================================================================
// = XmlComment Implementation
// ==================================================================================
TiXmlComment& TiXmlComment::operator=(const TiXmlComment& base)
{
	Clear();
	base.CopyTo(this);
	return *this;
}


void TiXmlComment::Print(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; i++)
		buf += "\t";

	buf += "<!--" + value + "-->";
}


TiXmlNode* TiXmlComment::Clone() const
{
	TiXmlComment* clone = new TiXmlComment();

	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}



// ==================================================================================
// = XmlText Implementation
// ==================================================================================
void TiXmlText::Print(std::string& buf, int depth) const
{
	if (cdata)
	{
		buf += "\n";
		for (int i = 0; i < depth; i++)
			buf += "\t";

		buf += "<![CDATA[" + value + "]]>\n"; // unformatted output
	}
	else
	{
		std::string buffer;
		EncodeString(value, buffer);
		buf += buffer;
	}
}


void TiXmlText::CopyTo( TiXmlText* target ) const
{
	TiXmlNode::CopyTo( target );
	target->cdata = cdata;
}


TiXmlNode* TiXmlText::Clone() const
{
	TiXmlText* clone = new TiXmlText("");
	CopyTo(clone);

	return clone;
}



// ==================================================================================
// = XmlDeclaration Implementation
// ==================================================================================
TiXmlDeclaration& TiXmlDeclaration::operator=( const TiXmlDeclaration& copy )
{
	Clear();
	copy.CopyTo( this );
	return *this;
}


void TiXmlDeclaration::Print(std::string& buf, int /*depth*/) const
{
	buf += "<?xml ";

	if (!version.empty())
	{
		buf += "version=\"";
		buf += version;
		buf += "\" ";
	}
	if (!encoding.empty())
	{
		buf += "encoding=\"";
		buf += encoding;
		buf += "\" ";
	}
	if (!standalone.empty())
	{
		buf += "standalone=\"";
		buf += standalone;
		buf += "\" ";
	}
	buf += "?>";
}


void TiXmlDeclaration::CopyTo(TiXmlDeclaration* target) const
{
	TiXmlNode::CopyTo(target);

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


TiXmlNode* TiXmlDeclaration::Clone() const
{
	TiXmlDeclaration* clone = new TiXmlDeclaration();

	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}



// ==================================================================================
// = XmlUnknown Implementation
// ==================================================================================
void TiXmlUnknown::Print(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; i++)
		buf += "\t";

	buf += "<" + value + ">";
}


TiXmlNode* TiXmlUnknown::Clone() const
{
	TiXmlUnknown* clone = new TiXmlUnknown();

	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}




// ==================================================================================
// = XmlAttributeSet Implementation
// ==================================================================================
TiXmlAttributeSet::TiXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


TiXmlAttributeSet::~TiXmlAttributeSet()
{
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}


void TiXmlAttributeSet::Add(TiXmlAttribute* addMe)
{
	assert(!Find(addMe->Name()));	// Shouldn't be multiply adding to the set.

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev = addMe;
}


void TiXmlAttributeSet::Remove( TiXmlAttribute* removeMe )
{
	TiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}


TiXmlAttribute* TiXmlAttributeSet::Find(const std::string& name) const
{
	for (TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (node->name == name)
			return node;
	}
	return nullptr;
}


TiXmlAttribute* TiXmlAttributeSet::FindOrCreate( const std::string& _name )
{
	TiXmlAttribute* attrib = Find( _name );
	if ( !attrib ) {
		attrib = new TiXmlAttribute();
		Add( attrib );
		attrib->SetName( _name );
	}
	return attrib;
}



// ==================================================================================
// = TiXmlHandle Implementation
// ==================================================================================
TiXmlHandle TiXmlHandle::FirstChild() const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild();
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::FirstChild(const std::string& value) const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild(value);
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::FirstChildElement() const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement();
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::FirstChildElement(const std::string& value) const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement(value);
		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::Child(int count) const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild();
		for (int i = 0; child && i < count; child = child->NextSibling(), ++i)
			; // Nothing
		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::Child(const std::string& value, int count) const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild(value);
		for (int i = 0;	child && i < count;	child = child->NextSibling(value), ++i)
			; // Nothing
		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::ChildElement(int count) const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement();
		for (int i = 0; child && i < count; child = child->NextSiblingElement(), ++i)
			; // Nothing

		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::ChildElement(const std::string& value, int count) const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement(value);
		for (int i = 0; child && i < count; child = child->NextSiblingElement(value), ++i)
			; // Nothing
		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}



// ==================================================================================
// = XmlMemoryBuffer Implementation
// ==================================================================================
bool XmlMemoryBuffer::VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute)
{
	indent();
	_buffer += "<";
	_buffer += element.Value();

	for (const TiXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next())
	{
		_buffer += " ";
		attrib->Print(_buffer, 0);
	}

	if (!element.FirstChild())
	{
		_buffer += " />";
		line_break();
	}
	else
	{
		_buffer += ">";
		line_break();
	}
	++depth;
	return true;
}


bool XmlMemoryBuffer::VisitExit(const TiXmlElement& element)
{
	--depth;
	if (!element.FirstChild())
		;
	else
	{
		indent();
		_buffer += "</";
		_buffer += element.Value();
		_buffer += ">";
		line_break();
	}
	return true;
}


bool XmlMemoryBuffer::Visit(const TiXmlText& text)
{
	if (text.CDATA())
	{
		indent();
		_buffer += "<![CDATA[";
		_buffer += text.Value();
		_buffer += "]]>";
		line_break();
	}
	else
	{
		indent();
		std::string str;
		TiXmlBase::EncodeString(text.Value(), str);
		_buffer += str;
		line_break();
	}
	return true;
}


bool XmlMemoryBuffer::Visit(const TiXmlDeclaration& declaration)
{
	indent();
	declaration.Print(_buffer, 0);
	line_break();
	return true;
}


bool XmlMemoryBuffer::Visit( const TiXmlComment& comment )
{
	indent();
	_buffer += "<!--";
	_buffer += comment.Value();
	_buffer += "-->";
	line_break();
	return true;
}


bool XmlMemoryBuffer::Visit( const TiXmlUnknown& unknown )
{
	indent();
	_buffer += "<";
	_buffer += unknown.Value();
	_buffer += ">";
	line_break();
	return true;
}
