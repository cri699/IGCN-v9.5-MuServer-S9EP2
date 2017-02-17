//////////////////////////////////////////////////////////////////////
// save_stream.cpp
#include "pugixml.hpp"

#include <iostream>

int main()
{
    // get a test document
    pugi::xml_document doc;
    doc.load_string("<foo bar='baz'><call>hey</call></foo>");

    // tag::code[]
    // save document to standard output
    std::cout << "Document:\n";
    doc.save(std::cout);
    // end::code[]
}

// vim:et


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

