#pragma once

#include "Surface.h"
#include "Chromosome.h"
#include "Surface.h"
#include "Lander.h"

#ifdef SVG
    #include "SVG.h"
    #include <iostream>
#endif // SVG

#include <vector>

constexpr int CHROMOSOME_ID_INVALID = -1;

class Population {
public:
    void init(int population_size, const Surface& planet_surface, const Lander& lander) noexcept {
        population_size = population_size;
        surface = planet_surface;
        initial_lander = lander;

        populations.resize(2);

        for (auto& population : populations) {
            population.resize(population_size);

            for (auto& subject : population) {
                subject.init(initial_lander);
            }
        }

    }

    void insertGene(int population_id, int chromosome_id, int gene_ig, const Gene& gene) noexcept {
        populations[population_id][chromosome_id][gene_ig] = gene;
    }

    void initRandomPopulation() noexcept {
        int angle_max_rand = (2 * ANGLE_STEP_MAX) + 1;
        int power_max_rand = (2 * POWER_STEP_MAX) + 1;

        for (auto& chromosome : populations[0]) {
            for (auto& gene : chromosome.genes) {
                gene = {
                        rand() % angle_max_rand + ANGLE_STEP_MIN,
                        rand() % power_max_rand + POWER_STEP_MIN
                       };
            }
        }
	}

    void simulateSingleThread(int id_from, int id_to) noexcept {
        /// TODO
    }

    void sumulate(int threads = 0) noexcept {
        return simulateSingleThread(0, population_size);
    }

public:
    Surface surface;
    Lander initial_lander;

    std::vector < std::vector < Chromosome >  > populations;

    int solution_chromosome_id = CHROMOSOME_ID_INVALID;
    int population_size = 0;
    int population_id = 0;
    float score_sum = 0;
};
