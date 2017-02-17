//////////////////////////////////////////////////////////////////////
// XMLFile.cpp
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

#include "stdafx.h"
#include "xmlFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace Mirage
{
	void XMLFile::load(const char * filename)
	{
		FILE *file=fopen(filename,"rt");
		if (!file) 
			FATALERROR(String("Error opening XML file ")+filename);
		String fileString;
		fileString.readFile(file); //dump the file into a string
		fclose(file);
		int startPos=0;
        if (!root.parse(fileString,startPos,0)) //parse it
		{
			FATALERROR(String("Error parsing XML file ")+String(filename)+String("\nThe XML file is not valid"));
		}
	}

	void XMLFile::save(const char * filename)
	{
		FILE *file=fopen(filename,"wt");
		if (!file) 
			FATALERROR(String("Error opening XML file for writing: ")+filename);
		String fileString;

		if (!root.writeFileString(fileString))
		{
			FATALERROR(String("Error outputting XML file string"));
		}

		fileString.writeFile(file); //dump the file into a string
		fclose(file);
	}

	//returns an attribute value, or NULL if not found
	String* XMLElement::getAttribPtr(const char * attribName)
	{
		for (int a=0;a<attribs.size();a++)
		{
			if (strcmp(attribs[a].name.cStr(),attribName)==0)
			{
				return &attribs[a].value;
			}
		}
		return NULL;
	}

	XMLElement* XMLElement::getElementPtr(const char * elementName)
	{
		for (int a=0;a<elements.size();a++)
		{
			if (strcmp(elements[a].name.cStr(),elementName)==0)
			{
				return &elements[a];
			}
		}
		return NULL;

	}

	String& XMLElement::getAttrib(const char *attribName)
	{
		static String emptyString;
		String *sp=getAttribPtr(attribName); 
		if (sp!=NULL)
			return *sp;
		else
		{
			emptyString="";
			return emptyString;
		}
	}
	XMLElement& XMLElement::getElement(const char *elementName)
	{
		static XMLElement emptyElement;
		XMLElement *ep=getElementPtr(elementName); 
		if (ep!=NULL)
			return *ep;
		else
		{
			emptyElement=XMLElement();
			return emptyElement;
		}
	}

	XMLElement* XMLElement::findFirstElementPtr(const char * elementName)	
	{
		elementSearchIndex=0;
		for (int a=0;a<elements.size();a++)
		{
			if (strcmp(elements[a].name.cStr(),elementName)==0)
			{
				elementSearchIndex=a+1;
				elementSearchString=elementName;
				return &elements[a];
			}
		}
		return NULL;
	}
	XMLElement* XMLElement::findNextElementPtr()
	{
		XMLElement* el=NULL;
		for (int a=elementSearchIndex;a<elements.size();a++)
		{
			if (strcmp(elements[a].name.cStr(),elementSearchString)==0)
			{
				elementSearchIndex=a+1;
				return &elements[a];
			}
		}
		return NULL;
	}

	bool XMLElement::parse(String& fileStr, int &offset, int depth)
	{
		//count the number of child elements
		int nChildren=0;
		int pos=offset;
		int currentDepth=depth;

		int tagType;
		do
		{
			tagType=parseTag(fileStr,pos,false);
			if (tagType==TAG_INVALID) return false; //failure (invalid XML)
			if (tagType==TAG_OPEN) 
			{
				if (currentDepth==depth+1) nChildren++;
				currentDepth++;
			}
			else if (tagType==TAG_CLOSE) 
			{
				currentDepth--;
				if (currentDepth==depth) break;
			}
			else if (tagType==TAG_OPENCLOSE) 
			{
				if (currentDepth==depth) break;
				if (currentDepth==depth+1) nChildren++;
			}
		}while (tagType!=TAG_EMPTY);

		if (currentDepth!=depth) return false; //tag mismatch
		
		elements.resize(nChildren); //preallocate the child elements
		int end=pos;
		pos=offset;
		offset=end;

		//parse through the file again, this time storing the information
		//read the first element's attributes
		do
		{
			tagType=parseTag(fileStr,pos,true); //read the opening tag
		}while (tagType==TAG_COMMENT);

		if (tagType==TAG_OPENCLOSE) return true;
		if (tagType!=TAG_OPEN) return false;

		if (nChildren>0)  	//read the child elements
		{
			for (int a=0;a<nChildren;a++)
			{
				if (!elements[a].parse(fileStr,pos,depth+1)) return false;
			}
		}
		do
		{
			tagType=parseTag(fileStr,pos,false); //read the closing tag
		}while (tagType==TAG_COMMENT);

		if (tagType!=TAG_CLOSE) return false;
		return true;
	}

	//parses an XML tag. If storeAttribs is true then attributes, the element name and preceeding content
	//- in the case of closing tags - is stored. 
	int XMLElement::parseTag(String& fileStr, int &offset, bool storeAttribs)
	{
		int numElements=0;

		int a=offset;
		for (;a<fileStr.length();a++) { if (fileStr[a]=='<') break; } //skip whitespace and other content
		if (a==fileStr.length()) return TAG_EMPTY; //no tag found
		if (fileStr[a]!='<') return TAG_INVALID;

		a++; //skip to start of tag name
		int tagStartPos=a;  //position of the character after the opening <
		int tagEndPos=a;	//position of the closing >

		//get the tag end pos
		bool inQuotes=false;
		int level=1;
		for (int b=a;b<fileStr.length();b++)
		{
			if (fileStr[b]=='\"') 
				inQuotes=!inQuotes;

			if (!inQuotes)
			{
				if (fileStr[b]=='<')
					level++;
				if (fileStr[b]=='>') 
				{
					level--;
					if (level==0)
					{
						tagEndPos=b;
						break;
					}
				}
			}
		}
		int tagLength=tagEndPos-tagStartPos;
		if (tagLength<1) return TAG_INVALID; //no > or tag too short
		offset=tagEndPos+1; //set the offset for the next tag

		if (fileStr[a]=='!' || fileStr[a]=='?') return TAG_COMMENT; //ignore comments and processing tags

		int type=(fileStr[a]=='/') ? TAG_CLOSE : TAG_OPEN;
		if (fileStr[tagEndPos-1]=='/') //detect an openclose tag (ie <foo="bar" />)
		{
			if (type==TAG_CLOSE) return TAG_INVALID;
			else 
			{
				type=TAG_OPENCLOSE;
				tagEndPos--; 
			}
		}
		if (!storeAttribs || type==TAG_CLOSE) return type;
		else //open or openclose
		{
			//store the element name
			for (;a<tagEndPos;a++)
			{
				char c=fileStr[a];
				if (isspace(c)) break;
				name.append(c);
			}

			//read and store the attributes
			int r;
			do 
			{
				r=parseAttrib(fileStr,a,tagEndPos);
				if (r==ATTRIB_INVALID) return TAG_INVALID;
			} while (r!=ATTRIB_EMPTY);
		}
		return type;
	}

	int XMLElement::parseAttrib(String &fileStr, int &offset, int tagEndPos)
	{
		XMLAttrib newAttrib;
		int a=offset;
		for (;a<tagEndPos;a++) { if (!isspace(fileStr[a])) break;} //skip whitespace
		if (a==tagEndPos) return ATTRIB_EMPTY;

		//read the name
		char c;
		for (;a<tagEndPos;a++)
		{
			c=fileStr[a];
			if (isspace(c) || c=='=') break;
			newAttrib.name.append(c);
		}
		if (newAttrib.name.length()==0) return ATTRIB_INVALID;

		//find the = 
		for (;a<tagEndPos;a++) { if (!isspace(fileStr[a])) break;} //skip whitespace
		if (fileStr[a++]!='=') return ATTRIB_INVALID; //no =
		for (;a<tagEndPos;a++) { if (!isspace(fileStr[a])) break;} //skip whitespace

		if (fileStr[a++]!='\"') return ATTRIB_INVALID; //no "
		//read the value 
		for (;a<tagEndPos;a++)
		{
			c=fileStr[a];
			if (c=='\"' ) 
			{
				offset=a+1;
				attribs.push_back(newAttrib);
				return ATTRIB_VALID;
			}
			newAttrib.value.append(c);
		}
		return ATTRIB_INVALID;
	}

	bool XMLElement::writeFileString(String& fileStringOut, int depth)
	{
		for (int a=0;a<depth;a++)
		{
			fileStringOut.printf("    ");
		}
		fileStringOut.printf("<%s",name.cStr());

		for (int a=0;a<attribs.size();a++)
		{
			fileStringOut.printf(" %s=\"%s\"",attribs[a].name.cStr(),attribs[a].value.cStr());
		}
		if (elements.size()==0)
		{
			fileStringOut.printf("/>\n");
		}
		else
		{
			fileStringOut.printf(">\n");
			for (int a=0;a<elements.size();a++)
			{
				elements[a].writeFileString(fileStringOut,depth+1);
			}
			for (int a=0;a<depth;a++)
			{
				fileStringOut.printf("    ");
			}
			fileStringOut.printf("</%s>\n",name.cStr());
		}
		return true;
	}
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

