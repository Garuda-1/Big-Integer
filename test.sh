#!/bin/bash

cp big_integer* /home/oktet/CPP/HW2_TEST
cmake /home/oktet/CPP/HW2_TEST . -DCMAKE-BUILD-TYPE=Release
make /home/oktet/CPP/HW2_TEST
/home/oktet/CPP/HW2_TEST/big_integer_testing