#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include "branch_predictor.h"

using namespace std;

#define endl '\n' // Use '\n' instead of std::endl for faster output

template <typename T> void print_vector(const vector<T> &v) {for (const auto &elem : v) {cout << elem << ' ';}cout << endl;}

int main(int argc, char** argv)
{
    // Input validation
    string trace_file;
    int M, N;
    string mode = argv[1];
    if(argc == 4 && (string)argv[1] == "bimodal")
    {
        M = stoi(argv[2]);
        trace_file = string(argv[3]);
    }
    else if (argc == 5 && (string)argv[1] == "gshare")
    {
        M = stoi(argv[2]);
        N = stoi(argv[3]);
        trace_file = string(argv[4]);
    }
    else
    {
        cout << "Usage: ./bpsim <bimodal> <m> <trace>" << endl;
        cout << "       ./bpsim <gshare> <m> <n> <trace>" << endl;
        return 0;
    }
    
    std::unique_ptr<Predictor> predictor;
    if ((string)argv[1] == "bimodal")
    {
        predictor = make_unique<BimodalPredictor>(M);
    }
    else
    {
        predictor = make_unique<GsharePredictor>(M, N);
    }

    // Read trace
    ifstream inputFile(trace_file);
    string line;
    if(!inputFile)
    {
        cout << "Error opening file" << endl;
        return 0;
    }

    int no_predictions = 0;
    if(inputFile.is_open())
    {
        while (getline(inputFile, line))
        {
            no_predictions++;
            pair<uint32_t, int> result = trace_decoder(line);
            // cout << hex << result.first << ' ' << result.second << endl;

            int prediction = predictor->predict(result.first, result.second);
            // cout << hex << result.first << " " << "Prediction: " << prediction << endl;
            // cout << endl;
        }
    }


    // Printing results
    cout << "COMMAND" << endl;
    if(mode == "bimodal")
    {
        cout << " ./bpsim " << (string)argv[1] << " " << M << " " << trace_file << endl;
    }
    else if(mode == "gshare")
    {
        cout << " ./bpsim " << (string)argv[1] << " " << M << " " << N << " " << trace_file << endl;
    }
    // cout << "OUTPUT" << endl;
    // cout << " number of predictions:\t" << no_predictions << endl;
    // cout << " number of mispredictions:\t" << predictor->mispredictions() << endl;
    printf("misprediction rate:\t%.2f%%\n", (double)((predictor->mispredictions() * 1.0)/ no_predictions) * 100.0);
    // predictor->print_counter_states();

    return 0;
}