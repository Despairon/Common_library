/*
* This file contains XML reader definitions
*/

#include "XmlReader.h"
#include <regex>
#include <functional>

namespace Xml
{   
    XmlReader::XmlReader(const std::string &xmlFile)
    {
        tree = nullptr;

        if (xmlFile != "")
        {
            auto iStream = std::fstream(xmlFile, std::ios::in);

            if (iStream.good())
            {
                parseTree(&tree, iStream);
            }
            else
                throw XmlReadException("Bad file provided!");
        }
        else
            throw XmlReadException("No file provided!");
    }

    XmlReader::~XmlReader() 
    {
        deleteTree(&tree);
    }

    void XmlReader::parseTree(XmlNode **node, std::fstream &stream)
    {
        if (node)
        {
            if (!stream.eof())
            { 
                auto regex_search = [](std::string str, const std::string &regex) -> auto
                {
                    std::smatch match;

                    if (std::regex_search(str, match, std::regex(regex)))
                        return std::string(match[0]);
                    else
                        return std::string("");
                };

                auto parseNodeName = [regex_search](XmlNode *node, const std::string &str)
                {
                    if (node != nullptr)
                    {
                        auto name = regex_search(str, "<[a-zA-Z0-9_]*");

                        if (!name.empty())
                        {
                            node->name = name.substr(1);
                        }
                    }
                };

                auto parseNodeParameter = [regex_search](XmlNode *node, const std::string &str)
                {
                    if (node != nullptr)
                    {
                        auto name  = regex_search(str, ".*=");
                        auto value = regex_search(str, "['\"].*['\"]");

                        if (!name.empty() && !value.empty())
                        {
                            name.pop_back();
                            value.pop_back();
                            value = value.substr(1);

                            if (node->params.find(name) == node->params.end())
                            {
                                node->params[name] = value;
                            }
                        }
                    }
                };

                auto parseNodeValue = [regex_search](XmlNode *node, const std::string &str)
                {
                    if (node != nullptr)
                    {
                        auto value = regex_search(str, ">.*<");

                        if (!value.empty())
                        {
                            node->value = value.substr(1);
                            node->value.pop_back();
                        }
                    }
                };

                auto str = std::string("");

                stream >> str;

                if (!regex_search(str, "<\\?").empty())                    // skip version tag
                {
                    do
                    {
                        stream >> str;
                    } while (regex_search(str, "\\?").empty());           // look for a version close tag

                    parseTree(node, stream);                              // continue looking for the node
                }
                else if (!regex_search(str, "<!--").empty())              // skip comment tag
                {
                    do
                    {
                        stream >> str;
                    } 
                    while (regex_search(str, "-->").empty());             // look for a comment close tag

                    parseTree(node, stream);                              // continue looking for the node
                }
                else if (!regex_search(str, "<[a-zA-Z0-9_].*>?").empty()) // look for a node with opening tag     
                {
                    if ((*node) == nullptr)
                    {
                        (*node) = new XmlNode();                          // create new node

                        (*node)->name     = "";
                        (*node)->value    = "";
                        (*node)->next     = nullptr;
                        (*node)->children = nullptr;

                        parseNodeName(*node, str);
                    }

                    while (regex_search(str, ">").empty())                // find closing bracket
                    {
                        stream >> str;

                        parseNodeParameter(*node, str);                   // parse node's parameters
                    }

                    if (!regex_search(str, ">.*</").empty())              // if node contains value - parse it
                    {
                        parseNodeValue(*node, str);
                    }

                    if (regex_search(str, "(</.*>)|(/>)").empty())         // if node does not contain closing tocken -> its parent node
                    {
                        parseTree(&(*node)->children, stream);             // parse children tree
                    }

                    parseTree(&(*node)->next, stream);                     // parse next node
                }
                else if (!regex_search(str, "</.*>").empty())              // look for a parent closing node
                {
                    return;                                                // just return so parent could parse it's next node
                }
                else parseTree(node, stream);                              // continue parsing if matches not found
            }
            else return;
        }
        else return;
    }

    void XmlReader::deleteTree(XmlNode **node)
    {
        if (node)
        {
            if (*node)
            {
                if ((*node)->children != nullptr)
                    deleteTree(&(*node)->children);

                if ((*node)->next != nullptr)
                    deleteTree(&(*node)->next);
                
                delete *node;
                *node = nullptr;
            }
        }
    }

    XmlNode *XmlReader::findNodeByName(const std::string &name) const
    {
        if (tree != nullptr)
        {
            std::function<XmlNode**(XmlNode**, const std::string&)> seekNode;

            seekNode = [&seekNode](XmlNode **node, const std::string &name) -> XmlNode**
            {
                if (node != nullptr)
                {
                    if (*node != nullptr)
                    {
                        if ((*node)->name == name)
                            return node;
                        else
                        {
                            XmlNode **lookUpNode = nullptr;

                            if ((*node)->next != nullptr)
                                lookUpNode = seekNode(&(*node)->next, name);

                            if (lookUpNode == nullptr)
                            {
                                if ((*node)->children != nullptr)
                                    return seekNode(&(*node)->children, name);
                            }
                            else
                                return lookUpNode;
                        }
                    }
                    else return nullptr;
                }
                else return nullptr;
            };

            return *seekNode(&tree, name);
        }
        else
            return nullptr;
    }
}