#!/bin/sh
./random.O3 Naive > time/random_naive.time
./random.O3 LS    > time/random_larsson.time
./random.O3 ISorg > time/random_induced_org.time
./random.O3 IS    > time/random_induced.time
