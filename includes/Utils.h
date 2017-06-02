//
// Created by Theo Fanet on 23/10/2016.
//

#ifndef TESTPROG_UTILS_H_H
#define TESTPROG_UTILS_H_H

#include <string>
#include <vector>

class Utils{
public:

    /**
     * Split a string every where a certain char is found
     * stores results in vector
     * Returns the size of the vector
     */
    static unsigned long split(const std::string &txt, std::vector<std::string> &strs, char ch){
        unsigned long pos = txt.find(ch);
        unsigned long initialPos = 0;
        strs.clear();

        while(pos != std::string::npos){
            strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
            initialPos = pos + 1;

            pos = txt.find(ch, initialPos);
        }

        strs.push_back(txt.substr(initialPos, std::min<unsigned long>(pos, txt.size() - (initialPos + 1))));

        return strs.size();
    }

    /**
     * Replace string occurence by an other one in a string
     */
    static void replace(std::string& subject, const std::string& search, const std::string& replace) {
        size_t pos = 0;
        while((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }
};

#endif //TESTPROG_UTILS_H_H
