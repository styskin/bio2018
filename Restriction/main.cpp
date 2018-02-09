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

using namespace std;

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

int code(char c) {
    switch(c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'U': return 2;
        case 'G': return 3;
    }
    return 0;
}

int codeA(const string& s) {
    int x = 0;
    for (int i = 0; i < s.length(); ++i) {
        x = ((x << 2) | code(s[i]));
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



// FIRST PROBLEM WITH NON INTERSECTING INTERVALS
void calc(const string& s, vii& intervals) {
    struct {
        bool operator()(pii a, pii b) const
        {
            return a.second < b.second;
        }
    } customLess;
    sort(intervals.begin(), intervals.end(), customLess);
    
    const int M = 1 << 3;
    vi state(M), newstate(M);
    int curStart = 0;
    int maxc = M;
    for (int i = 1; i < s.length() / 3; ++i) {
        int pos = i * 3;
        string cur = s.substr(i * 3, 3);
        // SHIFT DONE INTERVALS
        {
            int shift = 0;
            for (; curStart < intervals.size() && intervals[curStart].second - 1 < pos; ++curStart) {
                ++shift;
            }
            if (shift) {
                int m = (1 << shift) - 1;
                int checkm = 0;
                for (int i = 0; i < M; ++i) {
                    state[i >> shift] = (i & m) == m ? state[i] : 0;
                    if (state[i >> shift]) {
                        checkm = (i >> shift) + 1;
                    }
                    state[i] = 0;
                }
                if (checkm == 0) {
                    // cannot build block
                    cout << -1 << endl;
                    return;
                }
            }
        }
        if (curStart == intervals.size()) {
            // ALL DONE
            break;
        }
        vs& amins = codon[codeA(cur)];
        
        for (int j = 0; j < maxc; ++j) {
            newstate[j] = state[j];
        }
        for (int ai = 0; ai < amins.size(); ++ai) {
            int change = 0;
            for (int j = 0; j < 3; ++j) {
                if (amins[ai][j] != cur[j]) {
                    ++change;
                }
            }
            
            int curstate = 0;
            // find start for pos instead of continue
            for (int l = curStart; l < intervals.size() && intervals[l].first - 1 <= pos + 2; ++l) {
                bool ch = false;
                for (int j = 0; j < 3; ++j) {
                    if (amins[ai][j] != cur[j] && intervals[l].first - 1 <= pos + j && pos + j <= intervals[l].second - 1) {
                        ch = true;
                        break;
                    }
                }
                if (ch) {
                    curstate |= 1 << (l - curStart);
                }
                if (intervals[l].first - 1 > pos + 3) {
                    break;
                }
            }
            if (curstate > 0) {
                for (int j = 0; j < maxc; ++j) {
                    if (state[j] || (j == 0 && curStart == 0)) {
                        int nr = state[j | curstate] ? min(state[j | curstate], state[j] + change) : state[j] + change;
                        newstate[j | curstate] = newstate[j | curstate] ? min(newstate[j | curstate], nr) : nr;
                    }
                }
            }
        }
        state.swap(newstate);
        fill(newstate.begin(), newstate.end(), 0);
    }
    

    int shift = intervals.size() - curStart;
    maxc = 0;
    if (shift) {
        int m = (1 << shift) - 1;
        for (int i = 0; i < M; ++i) {
            state[i >> shift] = (i & m) == m ? state[i] : 0;
            if (state[i >> shift]) {
                maxc = (i >> shift) + 1;
            }
            state[i] = 0;
        }
        if (maxc == 0) {
            cout << -1 << endl;
            return;
        }
    }
    cout << state[0] << endl;
}




int main(int argc, const char * argv[]) {
    ifstream input("/Users/styskin/s2018/2/input.txt");
//    ifstream input("/Users/styskin/s2018/2/test.in");
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
        vii intervals;
        input >> s;
        input >> k;
        for (int j = 0; j < k; ++j) {
            int x, y;
            input >> x >> y;
            intervals.push_back(make_pair(x, y));
        }
//        cout << s << " " << k << endl;
        calc(s, intervals);
    }
    return 0;
}
