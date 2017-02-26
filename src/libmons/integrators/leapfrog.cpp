#include "leapfrog.h"

namespace mons {
    leapfrog::leapfrog(simulation_data * data) noexcept {
        bodycount = data->bodycount;
        px        = data->px;
        py        = data->py;
        pz        = data->pz;
        opx       = data->opx;
        opy       = data->opy;
        opz       = data->opz;
        ax        = data->ax;
        ay        = data->ay;
        az        = data->az;
        dt        = data->dt;

        init(data->vx, data->vy, data->vz);
    }

    // ToDo(robin): SIMD
    // calculate first step with euler's method, because no old position is
    // known
    inline void leapfrog::init(f * vx, f * vy, f * vz) noexcept {
        platform::copy(px, opx, bodycount);
        platform::copy(py, opy, bodycount);
        platform::copy(pz, opz, bodycount);

        f   this_dt, dt2;

        for(size i = 0; i < bodycount; i++) {
            this_dt = dt[i];
            dt2     = 0.5 * this_dt * this_dt;

            // one iteration of euler's method
            px[i] = opx[i] + vx[i] * this_dt + ax[i] * dt2;
            py[i] = opy[i] + vy[i] * this_dt + ay[i] * dt2;
            pz[i] = opz[i] + vz[i] * this_dt + az[i] * dt2;
        }
    }

    // ToDo(robin): SIMD
    // ToDo(robin): Flexsteps
    // now use actual leapfrog method
    f leapfrog::step(f simTime) noexcept {
        f  dt2;
        v3 tmp_position{};

        for(size i = 0; i < bodycount; i++) {
            dt2 = dt[i] * dt[i];

            tmp_position = {px[i], py[i], pz[i]};
            // tmp_position.pos[0] = positions[i].pos[0];
            // tmp_position.pos[1] = positions[i].pos[1];
            // tmp_position.pos[2] = positions[i].pos[2];

            px[i] = 2 * px[i] - opx[i] + ax[i] * dt2;
            py[i] = 2 * py[i] - opy[i] + ay[i] * dt2;
            pz[i] = 2 * pz[i] - opz[i] + az[i] * dt2;

            opx[i] = tmp_position.pos[0];
            opy[i] = tmp_position.pos[1];
            opz[i] = tmp_position.pos[2];
            // old_positions[i].pos[0] = tmp_position.pos[0];
            // old_positions[i].pos[1] = tmp_position.pos[1];
            // old_positions[i].pos[2] = tmp_position.pos[2];

            // reset acceleration to zero for the next step
            ax[i] = 0;
            ay[i] = 0;
            az[i] = 0;
        }

        return simTime;
    }
}
