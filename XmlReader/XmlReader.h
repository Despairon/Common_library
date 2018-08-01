#pragma once

/*
* This file contains XML reader declarations
*/

#include <string>
#include <map>
#include <list>
#include <fstream>

namespace Xml
{
    class XmlReadException : public std::exception          // XML read exception
    { 
    public:
        XmlReadException(const std::string &errStr) 
        : std::exception(errStr.c_str()) { }
    };

    struct XmlNode;                                                // declare XML node struct so it could be used in name alias
    
    using XmlParams        = std::map<std::string, std::string>;    // XML node parameters type name alias
    using XmlNodePtr       = XmlNode*;                              // XML node pointer type name alias
    using XmlNodeConstPtr  = const XmlNode*;                        // XML node const pointer type name alias
    using XmlNodeDoublePtr = XmlNode**;                             // XML node double pointer type name alias
    using XmlNodeList      = std::list<XmlNodeConstPtr>;            // XML node list type name alias

    struct XmlNode                                             // XML node structure
    {
        friend class XmlReader;                                // mark XmlReader class as friend so it could access 'new' and 'delete' operators
    private:
        static void *operator new(std::size_t sz)              // overloaded operator 'new' so it is private
        {
            return ::operator new(sz);
        }

        static void operator delete(void *ptr)                 // overloaded operator 'delete' so it is private
        {
            ::operator delete(ptr);
        }

    public:
        std::string name;                                      // node name
        std::string value;                                     // node value
        XmlParams   params;                                    // node parameters
        XmlNodePtr  next;                                      // node next pointer
        XmlNodePtr  children;                                  // node children pointer

        XmlNodeConstPtr findNode(const std::string&) const;    // find node by name method
        XmlNodeList findChildren(const std::string&) const;    // find all nodes by name method
    };

    class XmlReader                                            // XML reader class
    {
    private:
        mutable XmlNode *tree;                                 // pointer to the head of XML tree

        void parseTree(XmlNodeDoublePtr, std::fstream&);       // method to parse the tree
        void deleteTree(XmlNodeDoublePtr);                     // method to delete the XML tree and free memory
    public:
        explicit XmlReader(const std::string&);                // explicit constructor
        ~XmlReader();                                          // destructor

        XmlNodePtr getTree() const;                            // get tree method
    };
}