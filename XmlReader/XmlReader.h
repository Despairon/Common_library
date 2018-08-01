#pragma once

/*
* This file contains XML reader declarations
*/

#include <string>
#include <map>
#include <fstream>

namespace Xml
{
    class XmlReadException : public std::exception          // XML read exception
    { 
    public:
        XmlReadException(const std::string &errStr) 
        : std::exception(errStr.c_str()) { }
    };

    using XmlParams = std::map<std::string, std::string>;   // XML node parameters type name alias

    struct XmlNode                                          // XML node structure
    {
        friend class XmlReader;                             // mark XmlReader class as friend so it could access 'new' and 'delete' operators
    private:
        static void *operator new(std::size_t sz)           // overloaded operator 'new' so it is private
        {
            return ::operator new(sz);
        }

        static void operator delete(void *ptr)              // overloaded operator 'delete' so it is private
        {
            ::operator delete(ptr);
        }

    public:
        std::string name;                                   // node name
        std::string value;                                  // node value
        XmlParams   params;                                 // node parameters
        XmlNode    *next;                                   // node next pointer
        XmlNode    *children;                               // node children pointer
    };

    class XmlReader                                         // XML reader class
    {
    private:
        mutable XmlNode *tree;                              // pointer to the head of XML tree

        void parseTree(XmlNode**, std::fstream&);           // method to parse the tree
        void deleteTree(XmlNode**);                         // method to delete the XML tree and free memory
    public:
        explicit XmlReader(const std::string&);             // explicit constructor
        ~XmlReader();                                       // destructor

        XmlNode *findNodeByName(const std::string&) const;  // find node by name method
    };
}