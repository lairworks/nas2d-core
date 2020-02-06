#include "NAS2D/DataUtils.h"

#include "NAS2D/Xml/XmlAttribute.h"
#include "NAS2D/Xml/XmlElement.h"

#include <algorithm>
#include <sstream>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

void NAS2D::Xml::validateXMLElement(
	const NAS2D::Xml::XmlElement& element,
	const std::string& name,
	const std::string& requiredChildElements,
	const std::string& requiredAttributes,
	const std::string& optionalChildElements,
	const std::string& optionalAttributes)
{
	if (name.empty())
	{
		// TODO: Make this a function with [[noreturn]] attribute
#if defined(_WIN32)
		std::ostringstream err_ss;
		err_ss << "Xml Validation failed. Element name is required.";
		::MessageBoxA(nullptr, err_ss.str().c_str(), "Fatal Error", MB_ICONERROR | MB_OK);
#else
		// TODO: Add non-windows version.
#endif
		exit(0);
	}

	const std::string elemName = element.value();
	if (elemName != name)
	{
		// TODO: Make this a function with [[noreturn]] attribute
#if defined(_WIN32)
		std::ostringstream err_ss;
		err_ss << "Xml Validation failed. Element name " << elemName << " does not match valid name " << name;
		::MessageBoxA(nullptr, err_ss.str().c_str(), "Fatal Error", MB_ICONERROR | MB_OK);
#else
		// TODO: Add non-windows version.
#endif
		exit(0);
	}

	// Get list of required/optional attributes/children
	// Sort
	// Remove duplicates
	// Rational for not using std:set:
	// Profiled code takes average of 10 microseconds to complete.
	auto requiredChildNames = NAS2D::split(requiredChildElements);
	std::sort(std::begin(requiredChildNames), std::end(requiredChildNames));
	requiredChildNames.erase(std::unique(std::begin(requiredChildNames), std::end(requiredChildNames)), std::end(requiredChildNames));

	auto requiredAttributeNames = NAS2D::split(requiredAttributes);
	std::sort(std::begin(requiredAttributeNames), std::end(requiredAttributeNames));
	requiredAttributeNames.erase(std::unique(std::begin(requiredAttributeNames), std::end(requiredAttributeNames)), std::end(requiredAttributeNames));

	auto optionalChildNames = NAS2D::split(optionalChildElements);
	std::sort(std::begin(optionalChildNames), std::end(optionalChildNames));
	optionalChildNames.erase(std::unique(std::begin(optionalChildNames), std::end(optionalChildNames)), std::end(optionalChildNames));

	auto optionalAttributeNames = NAS2D::split(optionalAttributes);
	std::sort(std::begin(optionalAttributeNames), std::end(optionalAttributeNames));
	optionalAttributeNames.erase(std::unique(std::begin(optionalAttributeNames), std::end(optionalAttributeNames)), std::end(optionalAttributeNames));

	auto actualChildNames = getChildElementNames(element);
	std::sort(std::begin(actualChildNames), std::end(actualChildNames));
	actualChildNames.erase(std::unique(std::begin(actualChildNames), std::end(actualChildNames)), std::end(actualChildNames));

	// getAttributeNames
	auto actualAttributeNames = getAttributeNames(element);
	std::sort(std::begin(actualAttributeNames), std::end(actualAttributeNames));
	actualAttributeNames.erase(std::unique(std::begin(actualAttributeNames), std::end(actualAttributeNames)), std::end(actualAttributeNames));

	StringList extraChildElementNames{};
	std::set_difference(
		std::begin(actualChildNames), std::end(actualChildNames),
		std::begin(requiredChildNames), std::end(requiredChildNames),
		std::back_inserter(extraChildElementNames));
	std::sort(std::begin(extraChildElementNames), std::end(extraChildElementNames));

	StringList extraAttributeNames{};
	std::set_difference(
		std::begin(actualAttributeNames), std::end(actualAttributeNames),
		std::begin(requiredAttributeNames), std::end(requiredAttributeNames),
		std::back_inserter(extraAttributeNames));
	std::sort(std::begin(extraAttributeNames), std::end(extraAttributeNames));

	StringList missingRequiredChildElements{};
	std::set_difference(
		std::begin(requiredChildNames), std::end(requiredChildNames),
		std::begin(actualChildNames), std::end(actualChildNames),
		std::back_inserter(missingRequiredChildElements));
	std::sort(std::begin(missingRequiredChildElements), std::end(missingRequiredChildElements));

	if (!missingRequiredChildElements.empty())
	{
		std::ostringstream err_ss;
		err_ss << "Xml Validation failed. Missing required children:\n";
		for (const auto& child : missingRequiredChildElements)
		{
			err_ss << '\t' << child << '\n';
		}
		// TODO: Make this a function with [[noreturn]] attribute
#if defined(_WIN32)
		::MessageBoxA(nullptr, err_ss.str().c_str(), "Fatal Error", MB_ICONERROR | MB_OK);
#else
		// TODO: Add non-windows version.
#endif
		exit(0);
	}

	StringList missingRequiredAttributes{};
	std::set_difference(
		std::begin(requiredAttributeNames), std::end(requiredAttributeNames),
		std::begin(actualAttributeNames), std::end(actualAttributeNames),
		std::back_inserter(missingRequiredAttributes));
	std::sort(std::begin(missingRequiredChildElements), std::end(missingRequiredChildElements));

	if (!missingRequiredAttributes.empty())
	{
		std::ostringstream err_ss;
		err_ss << "Xml Validation failed. Missing required attributes:\n";
		for (const auto& attrb : missingRequiredAttributes)
		{
			err_ss << '\t' << attrb << '\n';
		}
		// TODO: Make this a function with [[noreturn]] attribute
#if defined(_WIN32)
		::MessageBoxA(nullptr, err_ss.str().c_str(), "Fatal Error", MB_ICONERROR | MB_OK);
#else
		// TODO: Add non-windows version.
#endif
		exit(0);
	}

#if defined(_DEBUG)

	StringList extraOptionalChildElementNames{};
	std::set_difference(
		std::begin(extraChildElementNames), std::end(extraChildElementNames),
		std::begin(optionalChildNames), std::end(optionalChildNames),
		std::back_inserter(extraOptionalChildElementNames));

	if (!extraOptionalChildElementNames.empty())
	{
		std::ostringstream err_ss;
		err_ss << "Optional child element validation failed. Verify child elements are correct. Found unknown children:\n";
		for (const auto& child : extraOptionalChildElementNames)
		{
			err_ss << '\t' << child << '\n';
		}
		// Maybe should log instead? This would get annoying quickly.

		// TODO: Make this a function with [[noreturn]] attribute
#if defined(_WIN32)
		::MessageBoxA(nullptr, err_ss.str().c_str(), "Warning", MB_ICONWARNING | MB_OK);
#else
		// TODO: Add non-windows version.
#endif
	}

	StringList extraOptionalAttributeNames{};
	std::set_difference(
		std::begin(extraAttributeNames), std::end(extraAttributeNames),
		std::begin(optionalAttributeNames), std::end(optionalAttributeNames),
		std::back_inserter(extraOptionalAttributeNames));

	if (!extraOptionalAttributeNames.empty())
	{
		std::ostringstream err_ss;
		err_ss << "Optional attribute validation failed. Verify attributes are correct. Found unknown attributes:\n";
		for (const auto& attrb : extraOptionalAttributeNames)
		{
			err_ss << '\t' << attrb << '\n';
		}
		// Maybe should log instead? This would get annoying quickly.

		// TODO: Make this a function with [[noreturn]] attribute
#if defined(_WIN32)
		::MessageBoxA(nullptr, err_ss.str().c_str(), "Warning", MB_ICONWARNING | MB_OK);
#else
		// TODO: Add non-windows version.
#endif
	}

#endif
}

NAS2D::StringList NAS2D::Xml::getChildElementNames(const XmlElement& element)
{
	NAS2D::StringList result{};
	for (auto iter = element.firstChildElement();
		 iter != nullptr;
		 iter = iter->nextSiblingElement())
	{
		result.push_back(iter->value());
	}
	return result;
}

NAS2D::StringList NAS2D::Xml::getAttributeNames(const XmlElement& element)
{
	NAS2D::StringList result{};
	for (auto iter = element.firstAttribute();
		 iter != nullptr;
		 iter = iter->next())
	{
		result.push_back(iter->name());
	}
	return result;
}
