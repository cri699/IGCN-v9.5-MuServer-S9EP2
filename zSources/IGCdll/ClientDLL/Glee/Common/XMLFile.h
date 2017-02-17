//////////////////////////////////////////////////////////////////////
// XMLFile.h
// Description:		A simplified XML parser
//
//					This parser breaks an XML file into a quick readable
//					data structure consisting of elements and 
//					attributes. Currently processing blocks, CDATA, and 
//					content	in elements (other than child elements) is 
//					ignored.
//
// By:				Ben Woodhouse 
// Date:			04/2004
//////////////////////////////////////////////////////////////////////

#pragma once

namespace Mirage
{
	struct XMLFile;
	struct XMLAttrib
	{ 
		XMLAttrib()									{}
		XMLAttrib(const XMLAttrib& src)				{*this=src;}
		XMLAttrib(const char *nameIn, const char * valueIn)	{name=nameIn; value=valueIn;}
		XMLAttrib& operator=(const XMLAttrib& src)	{name=src.name; value=src.value; return *this;}

		String name;
		String value;
	};

	struct XMLElement
	{ 
	
		friend XMLFile;
		XMLElement()								{elementSearchIndex=0;}
		XMLElement(const XMLElement& src)			{*this=src;}
		XMLElement& operator=(const XMLElement& src) {name=src.name; elements=src.elements; attribs=src.attribs; return *this;}

		void setName(const char *nameIn)			{name=nameIn;}

		//name-based data retrieval. The element searches return only the first element of that name
		String* getAttribPtr(const char * attribName);
		XMLElement* getElementPtr(const char * elementName);
		String& getAttrib(const char *attribName);
		XMLElement& getElement(const char *elementName);

		//for repeat searches of the same element name. Both methods return NULL when no more elements can be found
		XMLElement* findFirstElementPtr(const char * elementName);
		XMLElement* findNextElementPtr();

		//writing 
		void addElement(const XMLElement& src)	{elements.add(src);}
		void addAttrib(const XMLAttrib& src)	{attribs.add(src);}
		bool writeFileString(String& fileStringOut, int depth=0);

	private:
		enum TagTypes{TAG_OPEN, TAG_CLOSE, TAG_OPENCLOSE, TAG_COMMENT, TAG_INVALID, TAG_EMPTY};
		enum AttribTypes{ATTRIB_EMPTY, ATTRIB_VALID, ATTRIB_INVALID};
	
		bool parse(String& fileStr, int &offset, int depth);
		int parseTag(String& fileStr, int &offset, bool storeAttribs);
		int parseAttrib(String &fileStr, int &offset, int tagEndPos);
	public:
		String name;
		ArrayList<XMLElement> elements;
		ArrayList<XMLAttrib> attribs;

		int elementSearchIndex;
		String elementSearchString;
	};

	struct XMLFile 
	{
	public:
		void load(const char * filename);
		void save(const char * filename);

		XMLElement root;
	private:
	};
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

