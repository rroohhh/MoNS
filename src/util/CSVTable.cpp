#include "CSVTable.h"

CSVTable::CSVTable(const char * filename, char seperator) : m_seperator(seperator) {
 	std::ifstream file(filename);

    file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

	file.seekg(0, std::ios::end);
	m_fileContent.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	m_fileContent.assign((std::istreambuf_iterator<char>(file)),
			   std::istreambuf_iterator<char>());

	file.close();
}

std::vector<std::vector<std::string>> CSVTable::getRows() {
	std::stringstream file(m_fileContent);
	std::string line;
	std::string cell;
	std::vector<std::string> cells;
	std::vector<std::vector<std::string>> rows;

	while(std::getline(file, line)) {
		std::stringstream lineStream(line);

		while(std::getline(lineStream, cell, m_seperator)) {
			cells.push_back(cell);
		}

		rows.push_back(cells);
		cells.clear();
	}

	return rows;
}

CSVTable::~CSVTable() {}
