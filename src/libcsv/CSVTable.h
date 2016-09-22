#ifndef _CSVTABLE_H
#define _CSVTABLE_H

#include "fmt/ostream.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class CSVTable {
public:
    CSVTable(const char * filename, char seperator = ',') noexcept;

    static void write_file(std::string                           filename,
                           std::vector<std::vector<std::string>> data,
                           char seperator = ',') noexcept;
    std::vector<std::vector<std::string>> getRows() noexcept;

private:
    std::string m_fileContent;
    char        m_seperator;
};

#endif
