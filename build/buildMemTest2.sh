#!/bin/bash
g++ -o ../bin/memTestPt ../src/memTestBasic.cpp
g++ -o ../bin/memTestTc ../src/memTestBasic.cpp -ltcmalloc
g++ -o ../bin/memTestJe ../src/memTestBasic.cpp -ljemalloc