#include "gravity_naive.h"

namespace mons {
    gravity_naive::gravity_naive(simulation_data * data) noexcept {
        bodycount = data->bodycount;
        mass      = data->mass;
        px        = data->px;
        py        = data->py;
        pz        = data->pz;
        ax        = data->ax;
        ay        = data->ay;
        az        = data->az;

        // premultiply all masses with gamma, saves some mul's later on
        for(size i = 0; i < bodycount; i++) {
            mass[i] = mass[i] * constants::physics::gamma;
        }

        // apply one time to init the values for the old position
        apply();
    }

    void gravity_naive::apply() noexcept {
        this->apply(numeric_limits<f>::max());
    }

    // ToDo(robin): build support for flexsteps -> simTime
    // ToDo(robin): SIMD
    /* __attribute__((target("default"))) void */
    void gravity_naive::apply(f /*simTime*/) noexcept {
        const auto count = bodycount;

        // naive force summation (uses counterforce force equality)
        for(size i = 0; i < count; i++) {
            /*
                         * __m256d axi = _mm256_set_pd(ax[i], 0, 0, 0);
             * __m256d ayi = _mm256_set_pd(ay[i], 0, 0, 0);
             * __m256d azi = _mm256_set_pd(az[i], 0, 0, 0);
                         */
            /* __m256d ayi = ay[i]; */
            /* __m256d azi = az[i]; */

            for(size j = i + 1; j < 4 * (i / 4) + 4; j++) {
                f dx, dy, dz, dist, p;
                dx = px[i] - px[j];
                dy = py[i] - py[j];
                dz = pz[i] - pz[j];

                dist = 1.0 / sqrt(dx * dx + dy * dy + dz * dz);
                dist = dist * dist * dist;

                p = dx * dist;
                ax[i] -= p * mass[j];
                ax[j] += p * mass[i];

                p = dy * dist;
                ay[i] -= p * mass[j];
                ay[j] += p * mass[i];

                p = dz * dist;
                az[i] -= p * mass[j];
                az[j] += p * mass[i];
            }

            __m256d axi = _mm256_set1_pd(0);
            __m256d ayi = _mm256_set1_pd(0);
            __m256d azi = _mm256_set1_pd(0);
            __m256d m   = _mm256_set1_pd(mass[i]);
            __m256d pxi = _mm256_set1_pd(px[i]);
            __m256d pyi = _mm256_set1_pd(py[i]);
            __m256d pzi = _mm256_set1_pd(pz[i]);

            /* #pragma clang loop distribute(enable) vectorize(assume_safety) */
            for(size j = 4 * (i / 4) + 4; j < count; j += 4) {
                /* f dx, dy, dz, dist, p; */
                // calculate distance
                __m256d dx = _mm256_sub_pd(pxi, _mm256_load_pd(&px[j]));
                __m256d dy = _mm256_sub_pd(pyi, _mm256_load_pd(&py[j]));
                __m256d dz = _mm256_sub_pd(pzi, _mm256_load_pd(&pz[j]));
                /* dy = py[i] - py[j]; */
                /* dz = pz[i] - pz[j]; */

                /* dist = sqrt(dx * dx + dy * dy + dz * dz); */
                __m256d dist =
                    _mm256_div_pd(_mm256_set1_pd(1.0),
                                  _mm256_sqrt_pd(_mm256_add_pd(
                                      _mm256_mul_pd(dx, dx),
                                      _mm256_add_pd(_mm256_mul_pd(dy, dy),
                                                    _mm256_mul_pd(dz, dz)))));

                dist = _mm256_mul_pd(_mm256_mul_pd(dist, dist), dist);

                // add the respective force components (as acceleration) to the
                // total acceleration

                dx  = _mm256_mul_pd(dx, dist);
                axi = _mm256_fmadd_pd(dx, _mm256_load_pd(&mass[j]), axi);
                _mm256_store_pd(&ax[j],
                                _mm256_fmadd_pd(dx, m, _mm256_load_pd(&ax[j])));

                dy  = _mm256_mul_pd(dy, dist);
                ayi = _mm256_fmadd_pd(dy, _mm256_load_pd(&mass[j]), ayi);
                _mm256_store_pd(&ay[j],
                                _mm256_fmadd_pd(dy, m, _mm256_load_pd(&ay[j])));

                dz  = _mm256_mul_pd(dz, dist);
                azi = _mm256_fmadd_pd(dz, _mm256_load_pd(&mass[j]), azi);
                _mm256_store_pd(&az[j],
                                _mm256_fmadd_pd(dz, m, _mm256_load_pd(&az[j])));
            }

            __m256d sum      = _mm256_hadd_pd(axi, ayi);
            __m128d sum_high = _mm256_extractf128_pd(sum, 1);
            __m128d result = _mm_add_pd(sum_high, _mm256_castpd256_pd128(sum));

            ax[i] -= result[0];
            ay[i] -= result[1];
            az[i] -= azi[0] + azi[1] + azi[2] + azi[3];

            for(size j = count - count % 4; j < count; j++) {
                f dx, dy, dz, dist, p;
                dx = px[i] - px[j];
                dy = py[i] - py[j];
                dz = pz[i] - pz[j];

                dist = 1.0 / sqrt(dx * dx + dy * dy + dz * dz);
                dist = dist * dist * dist;

                p = dx * dist;
                ax[i] -= p * mass[j];
                ax[j] += p * mass[i];

                p = dy * dist;
                ay[i] -= p * mass[j];
                ay[j] += p * mass[i];

                p = dz * dist;
                az[i] -= p * mass[j];
                az[j] += p * mass[i];
            }
        }
    }

    /* __attribute__((target("avx2"))) void */
    /*
         * void gravity_naive::apply(f /\*simTime*\/) noexcept {
     *     auto count = *bodycount;
         *
     *     /\* f dx[4], dy[4], dz[4], dist[4], p[4]; *\/
         *
     *     // naive force summation (uses counterforce force equality)
     *     for(size i = 0; i < count; i++) {
     *         size j;
     *         for(j = i + 1; j < count / 4; j++) {
         * 			/\* TIMED_BLOCK(avx_force); *\/
         * 			__m256d dx, dy, dz, dist, p;
     *             size jj = 4 * j;
     *             dx      =
     * _mm256_sub_pd(_mm256_set1_pd(positions[i].pos[0]),
     *                                _mm256_set_pd(positions[jj].pos[0],
     *                                              positions[jj +
     * 1].pos[0],
     *                                              positions[jj +
     * 2].pos[0],
     *                                              positions[jj +
     * 3].pos[0]));
         *
     *             dy = _mm256_sub_pd(_mm256_set1_pd(positions[i].pos[1]),
     *                                _mm256_set_pd(positions[jj].pos[1],
     *                                              positions[jj +
     * 1].pos[1],
     *                                              positions[jj +
     * 2].pos[1],
     *                                              positions[jj +
     * 3].pos[1]));
         *
     *             dz = _mm256_sub_pd(_mm256_set1_pd(positions[i].pos[2]),
     *                                _mm256_set_pd(positions[jj].pos[2],
     *                                              positions[jj +
     * 1].pos[2],
     *                                              positions[jj +
     * 2].pos[2],
     *                                              positions[jj +
     * 3].pos[2]));
         *
     *             dist = _mm256_add_pd(
     *                 _mm256_add_pd(_mm256_mul_pd(dx, dx),
     * _mm256_mul_pd(dy,
     * dy)),
     *                 _mm256_mul_pd(dz, dz));
         *
         * 			dist = _mm256_div_pd(_mm256_set1_pd(1.0),
     * _mm256_sqrt_pd(dist));
         *
         * 			dist = _mm256_mul_pd(_mm256_mul_pd(dist, dist),
     * dist);
         *
         * 			p = _mm256_mul_pd(dx, dist);
         * 			__m256d tmp = _mm256_mul_pd(p,
     * _mm256_load_pd(&mass[jj]));
     *             accelerations[i].pos[0] -= tmp[0];
     *             accelerations[i].pos[0] -= tmp[1];
     *             accelerations[i].pos[0] -= tmp[2];
     *             accelerations[i].pos[0] -= tmp[3];
         *
         * 			tmp = _mm256_mul_pd(p, _mm256_set1_pd(mass[i]));
     *             accelerations[jj + 0].pos[0] += tmp[0];
     *             accelerations[jj + 1].pos[0] += tmp[1];
     *             accelerations[jj + 2].pos[0] += tmp[2];
     *             accelerations[jj + 3].pos[0] += tmp[3];
         *
         * 			p = _mm256_mul_pd(dy, dist);
         * 			tmp = _mm256_mul_pd(p,
     * _mm256_load_pd(&mass[jj]));
     *             accelerations[i].pos[1] -= tmp[0];
     *             accelerations[i].pos[1] -= tmp[1];
     *             accelerations[i].pos[1] -= tmp[2];
     *             accelerations[i].pos[1] -= tmp[3];
         *
         * 			tmp = _mm256_mul_pd(p, _mm256_set1_pd(mass[i]));
     *             accelerations[jj + 0].pos[1] += tmp[0];
     *             accelerations[jj + 1].pos[1] += tmp[1];
     *             accelerations[jj + 2].pos[1] += tmp[2];
     *             accelerations[jj + 3].pos[1] += tmp[3];
         *
         * 			p = _mm256_mul_pd(dz, dist);
         * 			tmp = _mm256_mul_pd(p,
     * _mm256_load_pd(&mass[jj]));
     *             accelerations[i].pos[2] -= tmp[0];
     *             accelerations[i].pos[2] -= tmp[1];
     *             accelerations[i].pos[2] -= tmp[2];
     *             accelerations[i].pos[2] -= tmp[3];
         *
         * 			tmp = _mm256_mul_pd(p, _mm256_set1_pd(mass[i]));
     *             accelerations[jj + 0].pos[2] += tmp[0];
     *             accelerations[jj + 1].pos[2] += tmp[1];
     *             accelerations[jj + 2].pos[2] += tmp[2];
     *             accelerations[jj + 3].pos[2] += tmp[3];
     *         }
         *
     *         for(; j < count; j++) {
         * 			/\* TIMED_BLOCK(scalar_force); *\/
     *             f dx, dy, dz, dist, p;
         *
     *             // calculate distance
     *             dx   = positions[i].pos[0] - positions[j].pos[0];
     *             dy   = positions[i].pos[1] - positions[j].pos[1];
     *             dz   = positions[i].pos[2] - positions[j].pos[2];
     *             dist = sqrt(dx * dx + dy * dy + dz * dz);
     *             dist = dist * dist * dist;
         *
     *             // add the respective force components (as acceleration)
     * to
     * the
     *             // total acceleration
     *             p = dx / dist;
     *             accelerations[i].pos[0] -= p * mass[j];
     *             accelerations[j].pos[0] += p * mass[i];
         *
     *             p = dy / dist;
     *             accelerations[i].pos[1] -= p * mass[j];
     *             accelerations[j].pos[1] += p * mass[i];
         *
     *             p = dz / dist;
     *             accelerations[i].pos[2] -= p * mass[j];
     *             accelerations[j].pos[2] += p * mass[i];
     *         }
     *     }
     * }
         */
}
