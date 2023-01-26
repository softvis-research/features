# Features

This repository contains three programs for feature location analysis in Software Product Lines (SPLs).
1. **feature_isolation_demo.cpp**<br/>
Calculates set differences to isolate features without IDs.
2. **feature_differences_demo.cpp**<br/>
Calculates all set differences with IDs.
3. **feature_calculation_demo.cpp**<br/>
Calculates only valid set differences with IDs.

## Compilation
1. `$ g++ feature_isolation_demo.cpp -std=c++20 -o fid.exe`
2. `$ g++ feature_differences_demo.cpp -std=c++20 -o fdd.exe`
2. `$ g++ feature_calculation_demo.cpp -std=c++20 -o fcd.exe`

## Usage
1. `$ ./fid.exe`
2. `$ ./fdd.exe`
2. `$ ./fcd.exe`

The programs accept two values as input, namely the number of independent features and the selected model (M1 to M19, entered as an integer in the interval 1..19). Afterwards it stores the following results in a comma-separated value (CSV) file.

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

## Author
[Ulrich Eisenecker](https://www.wifa.uni-leipzig.de/personenprofil/mitarbeiter/prof-dr-ulrich-eisenecker)
