#ifndef BRANCH_PREDICTOR_H
#define BRANCH_PREDICTOR_H

#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;
#define endl '\n'

uint32_t reverseBits(uint32_t n)
{
    uint32_t reversed = 0;
    int bit_size = 32;
    
    for (int i = 0; i < bit_size; i++)
    {
        reversed <<= 1;
        reversed |= (n & 1);
        n >>= 1;
    }
    
    return reversed;
}

// If taken returns 1, else returns 0
pair <uint32_t, int> trace_decoder(string line)
{
    stringstream ss(line);
    uint32_t pcAddress = 0;
    char decision;
    ss >> hex >> pcAddress >> decision;
    return make_pair(pcAddress, decision == 't' ? 1 : 0);
}

class Predictor {
public:
    virtual ~Predictor() {} // Virtual destructor
    virtual int predict(uint32_t, int) = 0; // Pure virtual function
    virtual void print_counter_states() = 0; // Pure virtual function
    virtual int mispredictions() { return MISPREDICTIONS; }
    int MISPREDICTIONS = 0;
};


class BimodalPredictor : public Predictor
{
public:

    BimodalPredictor(int m)
    {
        M = m;
        bimodal_history_size = 1 << M;
        bimodal_history.resize(bimodal_history_size, 2);
    }

    int predict(uint32_t pcAddress, int decision)
    {
        // Selecting M PC bits
        int start_bit = 2;
        uint32_t mask = ((1 << M) - 1) << start_bit;
        uint32_t index = (pcAddress & mask) >> start_bit;

        // Predictions
        int prediction = 0;
        if (bimodal_history[index] == 2 || bimodal_history[index] == 3) prediction = 1;
        else if (bimodal_history[index] == 1 || bimodal_history[index] == 0) prediction = 0;
        else
        {
            cout << "[ERROR] Counter value out of range" << endl;
            exit(1);
        }
        
        if(prediction != decision) MISPREDICTIONS++;

        // Update counters
        if (decision == 1 && bimodal_history[index] < 3) bimodal_history[index]++;
        if (decision == 0 && bimodal_history[index] > 0) bimodal_history[index]--;

        return prediction;
    }

    int mispredictions() { return MISPREDICTIONS; }

    void print_counter_states()
    {
        cout << "FINAL\t" << "BIMODAL CONTENTS" << endl;
        for (uint32_t i = 0; i < bimodal_history_size; i++)
        {
            cout << " " << dec << i << "\t" << bimodal_history[i] << endl;
        }
    }


private:
    int M;
    uint32_t bimodal_history_size;
    vector<int> bimodal_history;
    int MISPREDICTIONS = 0;
};



class GsharePredictor : public Predictor
{
public:
    GsharePredictor(int m, int n)
    {
        M = m;
        N = n;
        gshare_history_size = 1 << (M);
        gshare_history.resize(gshare_history_size, 2);
        BHR = 0;
    }
    
    int predict(uint32_t pcAddress, int decision)
    {
        // Selecting M PC bits
        int start_bit = 2;
        uint32_t mask = ((1 << M) - 1) << start_bit; // For selecting upper M bits from PC
        uint32_t pc_m_bits = (pcAddress & mask) >> start_bit;
        uint32_t pc_n_bits = pc_m_bits >> (M - N);
        uint32_t mask2 = ((1 << (M-N)) - 1); // For selecting lower M-N bits from PC_M
        uint32_t pc_m_n_bits = pc_m_bits & mask2;
        
        uint32_t index = ((pc_n_bits ^ (reverseBits(BHR) >> (32 - N))) << (M - N)) | pc_m_n_bits;

        // Predictions
        int prediction = 0;
        if (gshare_history[index] == 2 || gshare_history[index] == 3) prediction = 1;
        else if (gshare_history[index] == 1 || gshare_history[index] == 0) prediction = 0;
        else
        {
            cout << "[ERROR] Counter value out of range" << endl;
            exit(1);
        }
        
        if(prediction != decision) MISPREDICTIONS++;

        // Update counters
        if (decision == 1 && gshare_history[index] < 3) gshare_history[index]++;
        if (decision == 0 && gshare_history[index] > 0) gshare_history[index]--;

        // Update BHR
        uint32_t BHR_mask = ((1 << N) - 1);
        if (decision == 1)
        {
            BHR = (BHR << 1) | 1;
            BHR &= BHR_mask;
        }
        else
        {
            BHR = (BHR << 1);
            BHR &= BHR_mask;
        }

        // cout << "pcAddress:\t" << bitset<32>(pcAddress) << endl;
        // cout << "pc_m_bits:\t" << bitset<32>(pc_m_bits) << endl;
        // cout << "pc_n_bits:\t" << bitset<32>(pc_n_bits) << endl;
        // cout << "pc_m_n_bits:\t" << bitset<32>(pc_m_n_bits) << endl;
        // cout << "decision\t\t" << decision << endl;
        // cout << "BHR:\t\t" << bitset<32>(BHR) << endl;
        // cout << "Reversed BHR:\t" << bitset<32>(reverseBits(BHR) >> (32 - N)) << endl;
        // cout << "index:\t\t" << bitset<32>(index) << endl;
        // cout << endl;

        return prediction;
    }


    int mispredictions() { return MISPREDICTIONS; }

    void print_counter_states()
    {
        cout << "FINAL\t" << "GSHARE CONTENTS" << endl;
        for (uint32_t i = 0; i < gshare_history_size; i++)
        {
            cout << " " << dec << i << "\t" << gshare_history[i] << endl;
        }
    }
    
private:
    int M;
    int N;
    uint32_t gshare_history_size;
    vector<int> gshare_history;
    uint32_t BHR;
};

#endif
