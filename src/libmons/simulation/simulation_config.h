#ifndef __SIMULATIONCONFIG_H_
#define __SIMULATIONCONFIG_H_

#include "cpptoml.h"
#include "csv_table.h"
#include "fmt/format.h"
#include "io.h"
#include "platform.h"
#include "simulation_settings.h"
#include "types.h"
#include "util/debug_only.h"

namespace mons {
    class simulation_config : public simulation_settings {
    public:
        inline simulation_config(simulation_settings s)
            : simulation_settings(s) {
            write_table();
        }

        // build a config from preallcated arrays and known values
        inline simulation_config(size bodycount, f endTime, f dt, f * masses,
                                 v3 * positions, v3 * velocities) noexcept
            : simulation_config(simulation_settings{
                  bodycount, endTime, dt, masses, positions, velocities}) {}

        // read a config from file
        inline simulation_config(string filename) noexcept
            : simulation_settings() {
            try {
                table = cpptoml::parse_file(filename);

                endTime = *table->get_qualified_as<f>("simulation.endTime");
                dt      = *table->get_qualified_as<f>("simulation.dt");
                bodycount =
                    *table->get_qualified_as<size>("simulation.bodycount");

                positions  = platform::allocate<v3>(bodycount);
                velocities = platform::allocate<v3>(bodycount);
                masses     = platform::allocate<f>(bodycount);

                log::debug("read configfile: {}", filename);
                log::debug("endTime: {}", endTime);
                log::debug("dt: {}", dt);
                log::debug("bodycount: {}", bodycount);

                // one single data file
                if(table->contains_qualified("data.file")) {
                    string filename =
                        *table->get_qualified_as<string>("data.file");

                    log::debug("Using single data file: {}", filename);

                    read_single(filename);

                    // split data files
                } else {
                    string masses_filename =
                        *table->get_qualified_as<string>("data.masses");
                    string positions_filename =
                        *table->get_qualified_as<string>("data.positions");
                    string velocities_filename =
                        *table->get_qualified_as<string>("data.velocities");

                    log::debug("Using split data files: ");
                    log::debug("masses file: {}", masses_filename);
                    log::debug("positions file: {}", positions_filename);
                    log::debug("velocities file: {}", velocities_filename);

                    read_split(masses_filename, positions_filename,
                               velocities_filename);
                }
            } catch(const cpptoml::parse_exception & e) {
                log::err("Failed to parse config file: {}", e.what());
                exit(EXIT_FAILURE);
            } catch(ifstream::failure & failure) {
                log::err("Error during reading data files occured: {}",
                         failure.what());
                exit(EXIT_FAILURE);
            }

            log::debug("Data read: ");
            print_bodies();
        }

        inline void print_bodies() noexcept {
            log::debug("MASS\tX\tY\tZ\tVX\tVY\tVZ");
            for(size i = 0; i < bodycount; i++) {
                log::debug("{}\t{}\t{}\t{}\t{}\t{}\t{}", masses[i],
                           positions[i].pos[0], positions[i].pos[1],
                           positions[i].pos[2], velocities[i].pos[0],
                           velocities[i].pos[1], positions[i].pos[2]);
            }
        }

        // template <typename... Args>
        // inline void write_to_file(string configfile, Args...) noexcept {
        //     static_assert((sizeof...(Args) == 1) || (sizeof...(Args) == 3),
        //                   "write to file needs ether two file (single data
        //                   file),
        //                   "
        //                   "or four arguments (split data file)");
        // }

        inline void write_to_file(const string & configfile,
                                  string         single_data_file) noexcept {
            auto data = cpptoml::make_table();
            data->insert("file", single_data_file);
            table->insert("data", data);

            save_table(configfile);

            vector<vector<string>> out_data;

            for(unsigned int i = 0; i < bodycount; i++) {
                vector<string> line;
                line.push_back(fmt::format("{}", masses[i]));

                line.push_back(fmt::format("{}", positions[i].pos[0]));
                line.push_back(fmt::format("{}", positions[i].pos[1]));
                line.push_back(fmt::format("{}", positions[i].pos[2]));

                line.push_back(fmt::format("{}", velocities[i].pos[0]));
                line.push_back(fmt::format("{}", velocities[i].pos[1]));
                line.push_back(fmt::format("{}", velocities[i].pos[2]));

                out_data.push_back(line);
            }

            csv_table::write_file(single_data_file, out_data);
        }

        inline void write_to_file(const string & configfile, string masses_file,
                                  string positions_file,
                                  string velocities_file) noexcept {

            auto data = cpptoml::make_table();
            data->insert("masses", masses_file);
            data->insert("positions", positions_file);
            data->insert("velocities", velocities_file);
            table->insert("data", data);

            save_table(configfile);
        }

        inline size total_steps() { return endTime / dt; }

    private:
        // the config table
        shared_ptr<cpptoml::table> table;

        inline void save_table(const string & filename) {
            ofstream config(filename);
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
        inline void check_rows(vector<vector<string>> rows, size bodycount,
                               size           expected_colums,
                               const string & dataName) noexcept {
            if((size) rows.size() < bodycount) {
                log::err("{} too short, only {} rows found, expected {}",
                         dataName, rows.size(), bodycount);
                abort();
            }

            for(size i = 0; i < bodycount; i++) {
                if((size) rows[i].size() != expected_colums) {
                    log::err("{} for body {} invalid!", dataName, bodycount);
                    abort();
                }
            }
        }

        // read the particle data from a single data file
        inline void read_single(const string & filename) noexcept {
            auto rows = csv_table(filename.c_str()).getRows();

            check_rows(rows, bodycount, 7 /* mass, x, y, z, vx, vy, vz */,
                       "Data");

            for(size i = 0; i < bodycount; i++) {
                masses[i] = stod(rows[i][0]);

                positions[i].pos[0] = stod(rows[i][1]);
                positions[i].pos[1] = stod(rows[i][2]);
                positions[i].pos[2] = stod(rows[i][3]);

                velocities[i].pos[0] = stod(rows[i][4]);
                velocities[i].pos[1] = stod(rows[i][5]);
                velocities[i].pos[2] = stod(rows[i][6]);
            }
        }

        // read the particle data from split data files
        inline void read_split(const string & masses_filename,
                               const string & positions_filename,
                               const string & velocities_filename) noexcept {
            vector<vector<string>> rows;
            auto masses_rows = csv_table(masses_filename.c_str()).getRows();
            auto positions_rows =
                csv_table(positions_filename.c_str()).getRows();
            auto velocities_rows =
                csv_table(velocities_filename.c_str()).getRows();

            check_rows(masses_rows, bodycount, 1 /* mass */, "Mass data");
            check_rows(positions_rows, bodycount, 3 /* x, y, z*/,
                       "Position data");
            check_rows(velocities_rows, bodycount, 3 /* vx, vy, vz*/,
                       "Velocity data");

            for(size i = 0; i < bodycount; i++) {
                masses[i] = stod(masses_rows[i][0]);

                positions[i].pos[0] = stod(positions_rows[i][0]);
                positions[i].pos[1] = stod(positions_rows[i][1]);
                positions[i].pos[2] = stod(positions_rows[i][2]);

                velocities[i].pos[0] = stod(velocities_rows[i][0]);
                velocities[i].pos[1] = stod(velocities_rows[i][1]);
                velocities[i].pos[2] = stod(velocities_rows[i][2]);
            }
        }
    };
}

#endif
