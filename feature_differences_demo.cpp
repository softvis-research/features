/*! \file feature_differences_demo.cpp
 *
 * \author Ulrich W. Eisenecker
 * \date December 21, 2022
 * \warning This program is a research prototype only. It comes with
    no warranty and no liability. Use it only at your own risk!
 * \license This program is released under the Apache License 2.0.
 */

#include <iostream>
#include <fstream>
#include <string>
#include "features.hpp"

using namespace std;
using namespace features;

int main()
{
   feature_id_t number_of_features;
   model_id_t model_id;
   cout << "Number of features: ";
   cin >> number_of_features;
   cout << "Model id: ";
   cin >> model_id;
   feature_location_differences_t spl { number_of_features,model_id };
   string file_name { "feature_differences_for_" + to_string(number_of_features) +
                      "_model_" + to_string(model_id) + ".csv" };
   ofstream output { file_name };
   spl.print_header(output);
   output << endl;
   spl.print_systems(output);
   output << endl;
   spl.print_results(output);
   cout << "Results written to " << file_name << " ... Finished!" << endl;
}
