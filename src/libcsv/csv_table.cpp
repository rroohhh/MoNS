#include "csv_table.h"

namespace mons {
    csv_table::csv_table(const char * filename, char seperator) noexcept
        : m_seperator(seperator) {
        ifstream file(filename);

        // enable exceptions
        // ToDo(robin): actually handle exceptions
        file.exceptions(ifstream::failbit | ifstream::badbit);

        // reserve enough space in the buffer string
        file.seekg(0, ios::end);
        m_fileContent.reserve(file.tellg());
        file.seekg(0, ios::beg);

        // read the whole file to memory
        m_fileContent.assign(istreambuf_iterator<char>(file),
                             istreambuf_iterator<char>());

        file.close();
    }

    void csv_table::write_file(const string &                   filename,
                               vector<vector<string>> data,
                               char seperator) noexcept {
        ofstream file(filename);

        for(const auto & line : data) {
            if(!line.empty()) {
                u32 i = 0;
                for(i = 0; i < line.size() - 1; i++) {
                    fmt::print(file, "{}{}", line[i], seperator);
                }
                fmt::print(file, "{}\n", line[i]);
            }
        }

        file.close();
    }

    vector<vector<string>> csv_table::getRows() noexcept {
        stringstream                     file(m_fileContent);
        string                           line;
        string                           cell;
        vector<string>              cells;
        vector<vector<string>> rows;

        // read all lines
        while(getline(file, line)) {
            stringstream lineStream(line);

            // hack getline to use the cell seperator to read all cells in one
            // line
            while(getline(lineStream, cell, m_seperator)) {
                cells.push_back(cell);
            }

            // push copy of cells to the total rows
            rows.push_back(cells);

            // clear this copy of all cells
            cells.clear();
        }

        return rows;
    }
}
