#include "CSVTable.h"

CSVTable::CSVTable(const char * filename, char seperator) noexcept
    : m_seperator(seperator) {
    std::ifstream file(filename);

    // enable exceptions
    // ToDo(robin): actually handle exceptions
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // reserve enough space in the buffer string
    file.seekg(0, std::ios::end);
    m_fileContent.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    // read the whole file to memory
    m_fileContent.assign(std::istreambuf_iterator<char>(file),
                         std::istreambuf_iterator<char>());

    file.close();
}

void CSVTable::write_file(std::string                           filename,
                          std::vector<std::vector<std::string>> data,
                          char seperator) noexcept {
    std::ofstream file(filename);

    for(const auto & line : data) {
        if(line.size() > 0) {
            unsigned int i = 0;
            for(i = 0; i < line.size() - 1; i++) {
                fmt::print(file, "{}{}", line[i], seperator);
            }
            fmt::print(file, "{}\n", line[i]);
        }
    }

    file.close();
}

std::vector<std::vector<std::string>> CSVTable::getRows() noexcept {
    std::stringstream                     file(m_fileContent);
    std::string                           line;
    std::string                           cell;
    std::vector<std::string>              cells;
    std::vector<std::vector<std::string>> rows;

    // read all lines
    while(std::getline(file, line)) {
        std::stringstream lineStream(line);

        // hack getline to use the cell seperator to read all cells in one line
        while(std::getline(lineStream, cell, m_seperator)) {
            cells.push_back(cell);
        }

        // push copy of cells to the total rows
        rows.push_back(cells);

        // clear this copy of all cells
        cells.clear();
    }

    return rows;
}
