#!/bin/sh
rm icons.hpp icons.cxx
reswrap -k --header -o icons.hpp *.ico --source -k --include icons.hpp -o icons.cxx *.ico
