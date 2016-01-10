#ifndef _SIMULATIONCONFIG_H
#define _SIMULATIONCONFIG_H

#include <string>
#include "Simtypes.h"
#include "../util/cpptoml.h"
#include "../util/DebugOutput.h"
#include "../util/DebugOnly.h"
#include "../util/CSVTable.h"
#include "../platform/Platform.h"

class SimulationConfig {
public:
	SimulationConfig(Simtypes::SIZE bodycount, Simtypes::FLOAT endTime, Simtypes::FLOAT dt,
					 Simtypes::v3 * positions, Simtypes::v3 * velocities) :
		bodycount(bodycount), endTime(endTime), dt(dt), positions(positions),
		velocities(velocities)  {}

	SimulationConfig(std::string filename) {
		try {
			std::shared_ptr<cpptoml::table> table = cpptoml::parse_file("config.toml");

			endTime = *table->get_qualified_as<double>("simulation.endTime");
			dt = *table->get_qualified_as<double>("simulation.dt");
			bodycount = *table->get_qualified_as<long>("simulation.bodycount");

			positions = Platform::allocate<Simtypes::v3>(bodycount);
		    velocities = Platform::allocate<Simtypes::v3>(bodycount);
		    masses = Platform::allocate<Simtypes::FLOAT>(bodycount);

			DEBUGOUTPUT("read configfile: " << filename << std::endl);
			DEBUGOUTPUT("endTime: " << endTime << std::endl);
			DEBUGOUTPUT("dt: " << dt << std::endl);
			DEBUGOUTPUT("bodycount: " << bodycount << std::endl);

			if(table->contains_qualified("data.file")) {
				std::string filename = *table->get_qualified_as<std::string>("data.file");

				DEBUGOUTPUT("Using single data file: " << filename << std::endl);

				readSingle(filename);
			} else {
			    std::string masses_filename = *table->get_qualified_as<std::string>("data.masses");
			    std::string positions_filename = *table->get_qualified_as<std::string>("data.positions");
			    std::string velocities_filename = *table->get_qualified_as<std::string>("data.velocities");

				DEBUGOUTPUT("Using split data files: " << std::endl);
				DEBUGOUTPUT("masses file: " << masses_filename << std::endl);
				DEBUGOUTPUT("positions file: " << positions_filename << std::endl);
				DEBUGOUTPUT("velocities file: " << velocities_filename << std::endl);

				readSplit(masses_filename, positions_filename, velocities_filename);
			}
		} catch(const cpptoml::parse_exception& e) {
			std::cerr << "Failed to parse config file: " << e.what() << std::endl;
		} catch(std::ifstream::failure& failure) {
			std::cerr << "Error during reading data files occured: " << failure.what() << std::endl;
		}

		DEBUGONLY(
			std::cout << "Data read: " << std::endl;
			printBodies();
			)
	}

	void printBodies() {
		std::cout << "MASS\tX\tY\tZ\tVX\tVY\tVZ"  << std::endl;
		for (int i = 0; i < bodycount; i++) {
			std::cout << masses[i] << "\t"
					  << positions[i].x << "\t"
					  << positions[i].y << "\t"
					  << positions[i].z << "\t"
					  << velocities[i].x << "\t"
					  << velocities[i].y << "\t"
					  << velocities[i].z << std::endl;
		}
	}

	Simtypes::SIZE bodycount;
	Simtypes::FLOAT endTime;
	Simtypes::FLOAT dt;
	Simtypes::FLOAT * masses;
	Simtypes::v3 * positions;
	Simtypes::v3 * velocities;
private:
	void checkRows(std::vector<std::vector<std::string>> rows, Simtypes::SIZE bodycount, Simtypes::SIZE expectedColums, std::string dataName) {
		if(rows.size() < bodycount) {
			std::cerr << dataName << " too short, only " << rows.size() << " rows found, expected " << bodycount << std::endl;
			std::abort();
		}

		for (int i = 0; i < bodycount; i++) {
			if(rows[i].size() != expectedColums) {
				std::cerr << dataName << " for body " << bodycount << " invalid!" << std::endl;
				std::abort();
			}
		}
	}

	void readSingle(std::string filename) {
	    auto rows = CSVTable(filename.c_str()).getRows();

		checkRows(rows, bodycount, 7 /* mass, x, y, z, vx, vy, vz */, "Data");

		for (int i = 0; i < bodycount; i++) {
			masses[i] = std::stod(rows[i][0]);

			positions[i].x = std::stod(rows[i][1]);
			positions[i].y = std::stod(rows[i][2]);
			positions[i].z = std::stod(rows[i][3]);

			velocities[i].x = std::stod(rows[i][4]);
			velocities[i].y = std::stod(rows[i][5]);
			velocities[i].z = std::stod(rows[i][6]);
		}
	}

	void readSplit(std::string masses_filename, std::string positions_filename, std::string velocities_filename) {
		std::vector<std::vector<std::string>> rows;
		auto masses_rows = CSVTable(masses_filename.c_str()).getRows();
		auto positions_rows = CSVTable(positions_filename.c_str()).getRows();
		auto velocities_rows = CSVTable(velocities_filename.c_str()).getRows();

		checkRows(masses_rows, bodycount, 1 /* mass */, "Mass data");
		checkRows(positions_rows, bodycount, 3 /* x, y, z*/, "Position data");
		checkRows(velocities_rows, bodycount, 3 /* vx, vy, vz*/, "Velocity data");

		for (int i = 0; i < bodycount; i++) {
			masses[i] = std::stod(masses_rows[i][0]);

			positions[i].x = std::stod(positions_rows[i][0]);
			positions[i].y = std::stod(positions_rows[i][1]);
			positions[i].z = std::stod(positions_rows[i][2]);

			velocities[i].x = std::stod(velocities_rows[i][0]);
			velocities[i].y = std::stod(velocities_rows[i][1]);
			velocities[i].z = std::stod(velocities_rows[i][2]);
		}

	}
};


#endif
