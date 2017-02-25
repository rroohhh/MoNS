#ifndef __CSVTABLE_H_
#define __CSVTABLE_H_

#include "fmt/ostream.h"
#include "types.h"

namespace mons {
    class csv_table {
    public:
        csv_table(const char * filename, char seperator = ',') noexcept;

        static void write_file(const string &         filename,
                               vector<vector<string>> data,
                               char                   seperator = ',') noexcept;
        vector<vector<string>> getRows() noexcept;

    private:
        string m_fileContent;
        char   m_seperator;
    };
}

#endif
