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

using namespace std;

/*
 GCU, GCC, GCA, GCG
 CGU, CGC, CGA, CGG, AGA, AGG
 AAU, AAC
 GAU, GAC
 UGU, UGC
 CAA, CAG
 GAA, GAG
 GGU, GGC, GGA, GGG
 CAU, CAC
 AUU, AUC, AUA
 AUG
 UUA, UUG, CUU, CUC, CUA, CUG
 AAA, AAG
 AUG
 UUU, UUC
 CCU, CCC, CCA, CCG
 UCU, UCC, UCA, UCG, AGU, AGC
 ACU, ACC, ACA, ACG
 UGG
 UAU, UAC
 GUU, GUC, GUA, GUG
 UAA, UGA, UAG
 */


typedef pair<int, int> pii;
typedef vector<pii> vii;

int main(int argc, const char * argv[]) {
    ifstream input("/Users/styskin/s2018/2/test.in");
    if(!input.good()){
        cerr << "Error opening. Bailing out." << std::endl;
        return -1;
    }
    string line, name, content;
    int N;
    input >> N;
    for (int i = 0; i < N; ++i) {
        string s;
        int k;
        vii intervals;
        input >> s;
        input >> k;
        for (int j = 0; j < k; ++j) {
            int x, y;
            input >> x >> y;
            intervals.push_back(make_pair(x, y));
        }
        cout << s << " " << k << endl;
    }
    return 0;
}
