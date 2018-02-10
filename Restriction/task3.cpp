//
//  main.cpp
//  Restriction
//
//  Created by Andrey Styskin on 08.02.18.
//  Copyright © 2018 Andrey Styskin. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>

#include "aho.hpp"

using namespace std;

namespace {

const char * CODON = "\
 GCU, GCC, GCA, GCG \n\
 CGU, CGC, CGA, CGG, AGA, AGG \n\
 AAU, AAC \n\
 GAU, GAC \n\
 UGU, UGC \n\
 CAA, CAG \n\
 GAA, GAG \n\
 GGU, GGC, GGA, GGG \n\
 CAU, CAC \n\
 AUU, AUC, AUA \n\
 AUG \n\
 UUA, UUG, CUU, CUC, CUA, CUG \n\
 AAA, AAG \n\
 AUG \n\
 UUU, UUC \n\
 CCU, CCC, CCA, CCG \n\
 UCU, UCC, UCA, UCG, AGU, AGC \n\
 ACU, ACC, ACA, ACG \n\
 UGG \n\
 UAU, UAC \n\
 GUU, GUC, GUA, GUG \n\
 UAA, UGA, UAG \n";

typedef pair<int, int> pii;
typedef vector<pii> vii;
typedef vector<int> vi;
typedef vector<string> vs;
typedef unordered_map<int, vs> TCodon;

TCodon codon;


    int codeA(const string& s) {
        int x = 0;
        for (int i = 0; i < s.length(); ++i) {
            x = ((x << 2) | (int) code(s[i]));
        }
        return x;
    }


    vector<string> parseString(string s, const string& delimiter) {
        size_t pos = 0;
        vector<string> v;
        while (s.find_first_of(delimiter) == 0) {
            s.erase(0, 1);
        }
        while ((pos = s.find_first_of(delimiter)) != string::npos) {
            v.push_back(s.substr(0, pos));
            s.erase(0, pos + 1);
            while (s.find_first_of(delimiter) == 0) {
                s.erase(0, 1);
            }
        }
        return v;
    }


    void parseCodon(TCodon& codon) {
        string s(CODON);
        size_t pos;
        while ( (pos = s.find('\n')) != string::npos) {
            string line = s.substr(0, pos);
            vs cods = parseString(line, " ,");
            for (int i = 0; i < cods.size(); ++i) {
                codon[codeA(cods[i])] = cods;
            }
            s = s.substr(pos + 1);
        }
    }

    
    void calc(const string& s, const vs& patterns) {
        TAho aho;
        aho.buildMatchingMachine(patterns, 0, 3);
        vii iteration;
        map<int, int> nextiteration;
        {
            int cur = 0;
            for (int i = 0; i < 3; ++i) {
                cur = aho.findNextState(cur, code(s[i]));
            }
            iteration.push_back(pii(cur, 0));
        }
        // Check all variants
        for (int i = 1; i < s.length() / 3; ++i) {
            int pos = i * 3;
            string cur = s.substr(i * 3, 3);
//            cout << cur << endl;
            vs& amins = codon[codeA(cur)];
            for (int ai = 0; ai < amins.size(); ++ai) {
                int change = 0;
                for (int j = 0; j < 3; ++j) {
                    if (amins[ai][j] != cur[j]) {
                        ++change;
                    }
                }
                // Check amins[ai] as suffix for
                for (int it = 0; it < iteration.size(); ++it) {
                    int size = iteration[it].second;
                    int newState = iteration[it].first;
                    bool correct = true;
                    for (int j = 0; j < 3; ++j) {
                        newState = aho.findNextState(newState, code(amins[ai][j]));
                        if (!aho.checkOut(newState)) {
                            correct = false;
                            break;
                        }
                    }
                    if (correct) {
                        map<int, int>::iterator nit = nextiteration.find(newState);
                        if (nit != nextiteration.end()) {
                            nit->second = min(nit->second, size + change);
                        } else {
                            nextiteration[newState] = size + change;
                        }
                    }
                }
            }
            iteration.clear();
            for (const auto& it : nextiteration) {
                iteration.push_back(it);
            }
            nextiteration.clear();
        }
        if (iteration.size() == 0) {
            cout << -1 << endl;
        } else {
            int size = iteration[0].second;
            for (int i = 1; i < iteration.size(); ++i) {
                size = min(size, iteration[i].second);
            }
            cout << size << endl;
        }
    }
};


int main(int argc, const char * argv[]) {
    ifstream input("/Users/styskin/s2018/2/input.txt");
//    ifstream input("/Users/styskin/s2018/2/test3.in");
    if(!input.good()){
        cerr << "Error opening. Bailing out." << std::endl;
        return -1;
    }
    parseCodon(codon);
    string line, name, content;
    int N;
    input >> N;
    for (int i = 0; i < N; ++i) {
        string s;
        int k;
        input >> s;
        input >> k;
        vs pattens;
        for (int j = 0; j < k; ++j) {
            string x;
            input >> x;
            pattens.push_back(x);
        }
        calc(s, pattens);
    }
    return 0;
}
