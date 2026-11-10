#pragma once

#include <string_view>


namespace NAS2D
{
	namespace Xml
	{
		class XmlElement;
	}

	class ReaderObject;


	class ReaderArray
	{
	public:
		bool hasNext() const;

		ReaderObject object();

	protected:
		friend ReaderObject;

		explicit ReaderArray(const Xml::XmlElement& parentElement, std::string_view arrayElementName);

	private:
		const Xml::XmlElement& mParentElement;
		std::string_view mArrayElementName;
		const Xml::XmlElement* mCurrentElement;
	};
}
