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
#include "NAS2D/XML/Xml.h"

// The goal of the seperate error file is to make the first
// step towards localization. tinyxml (currently) only supports
// english error messages, but the could now be translated.
//
// It also cleans up the code a bit.
//

void TiXmlBase::fillErrorTable()
{
	TiXmlBase::errorString.push_back("No error");
	TiXmlBase::errorString.push_back("Error");
	TiXmlBase::errorString.push_back("Failed to open file");
	TiXmlBase::errorString.push_back("Error parsing Element.");
	TiXmlBase::errorString.push_back("Failed to read Element name");
	TiXmlBase::errorString.push_back("Error reading Element value.");
	TiXmlBase::errorString.push_back("Error reading Attributes.");
	TiXmlBase::errorString.push_back("Error: empty tag.");
	TiXmlBase::errorString.push_back("Error reading end tag.");
	TiXmlBase::errorString.push_back("Error parsing Unknown.");
	TiXmlBase::errorString.push_back("Error parsing Comment.");
	TiXmlBase::errorString.push_back("Error parsing Declaration.");
	TiXmlBase::errorString.push_back("Error document empty.");
	TiXmlBase::errorString.push_back("Error null (0) or unexpected EOF found in input stream.");
	TiXmlBase::errorString.push_back("Error parsing CDATA.");
	TiXmlBase::errorString.push_back("Error when TiXmlDocument added to document, because TiXmlDocument can only be at the root.");
}