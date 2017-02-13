#ifndef _SIMULATIONCONFIG_H
#define _SIMULATIONCONFIG_H

#include "fmt/format.h"
#include "../io/log.h"
#include "../platform/Platform.h"
#include "CSVTable.h"
#include "../util/DebugOnly.h"
#include "cpptoml.h"
#include "Simtypes.h"
#include "SimulationSettings.h"
#include <cassert>
#include <string>

using namespace io;

class SimulationConfig : public SimulationSettings {
public:
    inline SimulationConfig(SimulationSettings s) : SimulationSettings(s) {
        write_table();
    }

    // build a config from preallcated arrays and known values
    inline SimulationConfig(Simtypes::SIZE bodycount, Simtypes::FLOAT endTime,
                            Simtypes::FLOAT dt, Simtypes::FLOAT * masses,
                            Simtypes::v3 * positions,
                            Simtypes::v3 * velocities) noexcept
        : SimulationConfig(SimulationSettings{bodycount, endTime, dt, masses,
                                              positions, velocities}) {}

    // read a config from file
    inline SimulationConfig(std::string filename) noexcept {
        try {
            table = cpptoml::parse_file(filename);

            endTime   = *table->get_qualified_as<double>("simulation.endTime");
            dt        = *table->get_qualified_as<double>("simulation.dt");
            bodycount = *table->get_qualified_as<long>("simulation.bodycount");

            positions  = Platform::allocate<Simtypes::v3>(bodycount);
            velocities = Platform::allocate<Simtypes::v3>(bodycount);
            masses     = Platform::allocate<Simtypes::FLOAT>(bodycount);

            log::debug("read configfile: {}", filename);
            log::debug("endTime: {}", endTime);
            log::debug("dt: {}", dt);
            log::debug("bodycount: {}", bodycount);

            // one single data file
            if(table->contains_qualified("data.file")) {
                std::string filename =
                    *table->get_qualified_as<std::string>("data.file");

                log::debug("Using single data file: {}", filename);

                readSingle(filename);

                // split data files
            } else {
                std::string masses_filename =
                    *table->get_qualified_as<std::string>("data.masses");
                std::string positions_filename =
                    *table->get_qualified_as<std::string>("data.positions");
                std::string velocities_filename =
                    *table->get_qualified_as<std::string>("data.velocities");

                log::debug("Using split data files: ");
                log::debug("masses file: {}", masses_filename);
                log::debug("positions file: {}", positions_filename);
                log::debug("velocities file: {}", velocities_filename);

                readSplit(masses_filename, positions_filename,
                          velocities_filename);
            }
        } catch(const cpptoml::parse_exception & e) {
            log::err("Failed to parse config file: {}", e.what());
			std::exit(EXIT_FAILURE);
        } catch(std::ifstream::failure & failure) {
            log::err("Error during reading data files occured: {}",
                     failure.what());
			std::exit(EXIT_FAILURE);
        }

        log::debug("Data read: ");
        printBodies();
    }

    inline void printBodies() noexcept {
        log::debug("MASS\tX\tY\tZ\tVX\tVY\tVZ");
        for(Simtypes::SIZE i = 0; i < bodycount; i++) {
            log::debug("{}\t{}\t{}\t{}\t{}\t{}\t{}", masses[i],
                       positions[i].pos[0], positions[i].pos[1],
                       positions[i].pos[2], velocities[i].pos[0],
                       velocities[i].pos[1], positions[i].pos[2]);
        }
    }

    // template <typename... Args>
    // inline void write_to_file(std::string configfile, Args...) noexcept {
    //     static_assert((sizeof...(Args) == 1) || (sizeof...(Args) == 3),
    //                   "write to file needs ether two file (single data file),
    //                   "
    //                   "or four arguments (split data file)");
    // }

    inline void write_to_file(std::string configfile,
                              std::string single_data_file) noexcept {
        auto data = cpptoml::make_table();
        data->insert("file", single_data_file);
        table->insert("data", data);

        save_table(configfile);

        std::vector<std::vector<std::string>> out_data;

        for(unsigned int i = 0; i < bodycount; i++) {
            std::vector<std::string> line;
            line.push_back(fmt::format("{}", masses[i]));

            line.push_back(fmt::format("{}", positions[i].pos[0]));
            line.push_back(fmt::format("{}", positions[i].pos[1]));
            line.push_back(fmt::format("{}", positions[i].pos[2]));

            line.push_back(fmt::format("{}", velocities[i].pos[0]));
            line.push_back(fmt::format("{}", velocities[i].pos[1]));
            line.push_back(fmt::format("{}", velocities[i].pos[2]));

			out_data.push_back(line);
        }

		CSVTable::write_file(single_data_file, out_data);
    }

    inline void write_to_file(std::string configfile, std::string masses_file,
                              std::string positions_file,
                              std::string velocities_file) noexcept {

        auto data = cpptoml::make_table();
        data->insert("masses", masses_file);
        data->insert("positions", positions_file);
        data->insert("velocities", velocities_file);
        table->insert("data", data);

        save_table(configfile);
    }

private:
    // the config table
    std::shared_ptr<cpptoml::table> table;

    inline void save_table(std::string filename) {
        std::ofstream config(filename);
        config << (*table);
        config.close();
    }

    inline void write_table() {
        table           = cpptoml::make_table();
        auto simulation = cpptoml::make_table();
        simulation->insert("endTime", endTime);
        simulation->insert("dt", dt);
        simulation->insert("bodycount", (long)bodycount);
        table->insert("simulation", simulation);
    }

    // check if all rows contain the expected number of colums, and if
    // enough
    // row for all particles are present
    inline void checkRows(std::vector<std::vector<std::string>> rows,
                          Simtypes::SIZE                        bodycount,
                          Simtypes::SIZE                        expectedColums,
                          std::string dataName) noexcept {
        if(rows.size() < bodycount) {
            log::err("{} too short, only {} rows found, expected {}", dataName,
                     rows.size(), bodycount);
            std::abort();
        }

        for(Simtypes::SIZE i = 0; i < bodycount; i++) {
            if(rows[i].size() != expectedColums) {
                log::err("{} for body {} invalid!", dataName, bodycount);
                std::abort();
            }
        }
    }

    // read the particle data from a single data file
    inline void readSingle(std::string filename) noexcept {
        auto rows = CSVTable(filename.c_str()).getRows();

        checkRows(rows, bodycount, 7 /* mass, x, y, z, vx, vy, vz */, "Data");

        for(Simtypes::SIZE i = 0; i < bodycount; i++) {
            masses[i] = std::stod(rows[i][0]);

            positions[i].pos[0] = std::stod(rows[i][1]);
            positions[i].pos[1] = std::stod(rows[i][2]);
            positions[i].pos[2] = std::stod(rows[i][3]);

            velocities[i].pos[0] = std::stod(rows[i][4]);
            velocities[i].pos[1] = std::stod(rows[i][5]);
            velocities[i].pos[2] = std::stod(rows[i][6]);
        }
    }

    // read the particle data from split data files
    inline void readSplit(std::string masses_filename,
                          std::string positions_filename,
                          std::string velocities_filename) noexcept {
        std::vector<std::vector<std::string>> rows;
        auto masses_rows     = CSVTable(masses_filename.c_str()).getRows();
        auto positions_rows  = CSVTable(positions_filename.c_str()).getRows();
        auto velocities_rows = CSVTable(velocities_filename.c_str()).getRows();

        checkRows(masses_rows, bodycount, 1 /* mass */, "Mass data");
        checkRows(positions_rows, bodycount, 3 /* x, y, z*/, "Position data");
        checkRows(velocities_rows, bodycount, 3 /* vx, vy, vz*/,
                  "Velocity data");

        for(Simtypes::SIZE i = 0; i < bodycount; i++) {
            masses[i] = std::stod(masses_rows[i][0]);

            positions[i].pos[0] = std::stod(positions_rows[i][0]);
            positions[i].pos[1] = std::stod(positions_rows[i][1]);
            positions[i].pos[2] = std::stod(positions_rows[i][2]);

            velocities[i].pos[0] = std::stod(velocities_rows[i][0]);
            velocities[i].pos[1] = std::stod(velocities_rows[i][1]);
            velocities[i].pos[2] = std::stod(velocities_rows[i][2]);
        }
    }
};

#endif
