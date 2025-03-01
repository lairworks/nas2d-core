#include "XmlVisitor.h"


using namespace NAS2D::Xml;


bool XmlVisitor::visitEnter(const XmlDocument&)
{
	return true;
}


bool XmlVisitor::visitExit(const XmlDocument&)
{
	return true;
}


bool XmlVisitor::visitEnter(const XmlElement&, const XmlAttribute*)
{
	return true;
}


bool XmlVisitor::visitExit(const XmlElement&)
{
	return true;
}


bool XmlVisitor::visit(const XmlText&)
{
	return true;
}


bool XmlVisitor::visit(const XmlComment&)
{
	return true;
}


bool XmlVisitor::visit(const XmlUnknown&)
{
	return true;
}
