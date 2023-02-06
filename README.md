[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![Documentation](https://img.shields.io/badge/Documentation-Doxygen-blue.svg)](https://softvis-research.github.io/features/)

# Introduction

This repository contains four programs for feature location analysis in Software Product Lines (SPLs).
1. **feature_isolation_demo.cpp**<br/>
Calculates set differences to isolate features without IDs.
2. **feature_differences_demo.cpp**<br/>
Calculates all set differences with IDs.
3. **feature_calculation_demo.cpp**<br/>
Calculates only valid set differences with IDs.
4. **fl.cpp**<br/>
Calculates valid set differences for F independent features according to model M19.

# Compilation
1. `$ g++ src/features/feature_isolation_demo.cpp -std=c++20 -o fid.exe`
2. `$ g++ src/features/feature_differences_demo.cpp -std=c++20 -o fdd.exe`
3. `$ g++ src/features/feature_calculation_demo.cpp -std=c++20 -o fcd.exe`
4. `$ g++ src/fl/fl.cpp -std=c++20 -o fl.exe`

# Usage
1. `$ ./fid.exe`
2. `$ ./fdd.exe`
3. `$ ./fcd.exe`
4. `$ ./fl.exe`

The programs 1, 2 and 3 accept two values as input, namely the number of independent features and the selected model (M1 to M19, entered as an integer in the interval 1..19). Afterwards it stores the following results in a comma-separated value (CSV) file.

- M, selected model (repetition of input)
- T, actual total number of features
- F, number of independent features (repetition of the input)
- DF, actual total number of inherently dependent features
- O, actual number of or-features
- A, actual number of and-features
- N, actual number of not-features
- ON, actual number of or-not-features
- AN, actual number of and-not-features
- S, number of systems of SPL 
- D, number of all set differences of SPL systems
- List of all systems with system names and features
- List of all features plus the set difference that isolates this feature with systems as elements

Program 4 asks for the number of independent features. Afterwards it creates six
CSV files in the working directory. Their filenames begin with fl_ followed by
the number of independent features, followed by _ and the symbol for the
corresponding feature category, for example, F for independent features. Each line in
each file contains two values separated by a tab.
1. A string that contains the feature name.
2. A bitstring that represents a difference expression. The most significant bit (MSB) at the most left position indicates that the system has to be
intersected if it has value 1 or that it has to be united if it has value 0.

# Author
[Ulrich Eisenecker](https://www.wifa.uni-leipzig.de/personenprofil/mitarbeiter/prof-dr-ulrich-eisenecker)
