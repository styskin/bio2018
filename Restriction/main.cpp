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
typedef std::bitset<32> TBS;

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



void calc(const string& s, vii& intervals) {
    // FIRST PROBLEM WITH NON INTERSECTING INTERVALS
    
    
    // TODO: SORT BY THE END OF INTERVAL
    struct {
        bool operator()(pii a, pii b) const
        {
            return a.second < b.second;
        }
    } customLess;
    sort(intervals.begin(), intervals.end(), customLess);
    
    unordered_map<TBS, int> state;
    
    state[TBS()] = 0;
    TBS curMask;
    int curStart = 0;
    for (int i = 1; i < s.length() / 3; ++i) {
        int pos = i * 3;
//        cout << "POS: " << pos << endl;
//        cout << "MASK: " << curMask << endl;
        string cur = s.substr(i * 3, 3);
        vs& amins = codon[codeA(cur)];
        
        for (int ai = 0; ai < amins.size(); ++ai) {
            TBS curstate;
            int change = 0;
            for (int j = 0; j < 3; ++j) {
                if (amins[ai][j] != cur[j]) {
                    ++change;
                }
            }
            
            // find start for pos instead of continue
            for (int l = curStart; l < intervals.size(); ++l) {
                if(intervals[l].second - 1 < pos) {
                    curStart = l;
                    curMask.set(l);
                    continue;
                }
                int a = max(intervals[l].first - 1, pos);
                int b = min(intervals[l].second - 1, pos + 2);
                
                if (a <= b) {
                    // check presicely intersection
                    bool ch = false;
                    for (int j = 0; j < 3; ++j) {
                        if (amins[ai][j] != cur[j] && intervals[l].first - 1 <= pos + j && pos + j <= intervals[l].second - 1) {
                            ch = true;
                            break;
                        }
                    }
                    if (ch) {
                        // interval[l] has change of size: change
                        curstate.set(l);
                    }
                }
                
                if (intervals[l].first - 1 > pos + 3) {
                    break;
                }
            }
            // we have curstate and size of change
//            cout << "C: " << curstate << " " << change << endl;
            unordered_map<TBS, int>::iterator nit = state.find(curstate);
            if (nit != state.end()) {
                nit->second = min(nit->second, change);
            }
            unordered_map<TBS, int> newState;
            for (const auto& it : state) {
                if ((it.first & curMask) == curMask) {
                    newState.insert(it);
                }
            }
            for (const auto& it : state) {
                if ((it.first & curMask) != curMask) {
                    continue;
                }
                TBS s = it.first | curstate;
//                cout << "U: " << s << " " << it.second + change << endl;
                unordered_map<TBS, int>::iterator nit = newState.find(s);
                if (nit != newState.end()) {
                    nit->second = min(nit->second, it.second + change);
                } else {
                    newState[s] = it.second + change;
                }
            }
            state.swap(newState);
//            cout << state.size() << endl;
        }
    }
    
    TBS finalS;
    for (int i = 0; i < intervals.size(); ++i) {
        finalS.set(i);
    }
    
//    for (const auto&  it : state) {
//        cout << it.first << " " << it.second << endl;
//    }
    
    unordered_map<TBS, int>::iterator nit = state.find(finalS);
    if (nit != state.end()) {
        cout << nit->second << endl;
    } else {
        cout << -1 << endl;
    }
    
}




int main(int argc, const char * argv[]) {
    ifstream input("/Users/styskin/s2018/2/test.in");
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
//        cerr << s << " " << k << endl;
        calc(s, intervals);
    }
    return 0;
}
