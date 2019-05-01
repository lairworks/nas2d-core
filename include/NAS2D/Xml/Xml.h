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

// ==================================================================================
// = This is a heavily modified version of TinyXML based on v2.6.2.
// ==================================================================================
// = Notable Modifications:
// =	- Force use of STL containers.
// =	- Removed redundant calls that convert between basic types.
// =	- Removed file input/output and substituted buffer input/output
// =	- Removed 'Ti' from all object names.
// =	- Renamed class functions to conform with NAS2D naming conventions.
// =	- Separated source code into individual source and header files.
// =	- Removed processing of 'declaration' tag -- not needed for NAS2D apps.
// =	- Removed support for UTF encoding -- not needed for NAS2D apps.
// =	- Updated documentation throughout the entire library.
// ==================================================================================
#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4530)
#pragma warning(disable : 4786)
#endif

#include "NAS2D/Xml/XmlAttribute.h"
#include "NAS2D/Xml/XmlAttributeSet.h"
#include "NAS2D/Xml/XmlBase.h"
#include "NAS2D/Xml/XmlComment.h"
#include "NAS2D/Xml/XmlDocument.h"
#include "NAS2D/Xml/XmlElement.h"
#include "NAS2D/Xml/XmlHandle.h"
#include "NAS2D/Xml/XmlMemoryBuffer.h"
#include "NAS2D/Xml/XmlNode.h"
#include "NAS2D/Xml/XmlText.h"
#include "NAS2D/Xml/XmlVisitor.h"
#include "NAS2D/Xml/XmlUnknown.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif
