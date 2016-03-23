#ifndef _CSVTABLE_H
#define _CSVTABLE_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class CSVTable {
public:
	CSVTable(const char * filename, char seperator = ',');

	std::vector<std::vector<std::string>> getRows();

	~CSVTable();
private:
	std::string m_fileContent;
	char m_seperator;
};

#endif
