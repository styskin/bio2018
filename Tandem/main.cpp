//
//  tandem.cpp
//  Tandem
//
//  Created by Andrey Styskin on 08.02.18.
//
//

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

const int M = 14;

char decodec(int x) {
    switch(x) {
        case 0: return 'A';
        case 1: return 'C';
        case 2: return 'T';
        case 3: return 'G';
    }
    return 0;
}

string decode(int x) {
    string s;
    for (int i = 0; i < M; ++i) {
        s = decodec(x % 4) + s;
        x >>= 2;
    }
    return s;
}

int code(char c) {
    switch(c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'T': return 2;
        case 'G': return 3;
    }
    return 0;
}

typedef vector<int> vii;

void calcDirect(const std::string& line) {
    unordered_map<int, vii> hash;
    vii shingled;
    
    
    int MASK = (1 << (M * 2)) - 1;
    int x = 0;
    for (int i = 0; i < M; ++i) {
        x = (x << 2 | code(line[i])) & MASK;
    }
    for (int i = M; i < line.length(); ++i) {
        shingled.push_back(x);
        hash[x].push_back(i - M);
        x = (x << 2 | code(line[i])) & MASK;
    }
    unordered_map<int, int> len;
    for (unordered_map<int, vii>::const_iterator it = hash.begin(); it != hash.end(); ++it) {
        if (it->second.size() == 2) {
            len[it->second[1] - it->second[0]] ++;
        }
    }
    vector<pair<int, int> > len_s;
    
    for (unordered_map<int, int>::const_iterator it = len.begin(); it != len.end(); ++it) {
        len_s.push_back(make_pair(-it->second, it->first));
    }
    // need to get the biggest distances
    sort(len_s.begin(), len_s.end());
    for (int i = 0; i < 50; ++i) {
//        cout << len_s[i].first << " " << len_s[i].second << endl;
    }
    // CASE OF ONE TOP
    if (0) {
        int size = len_s[0].second;
    //    cout << "SIZE: " << size << endl;
        // XXX: check  -len_s[0].first > size
        
        vii l;
        for (int ind = 0; ind < shingled.size(); ++ind) {
            if (ind + size < shingled.size() && shingled[ind] == shingled[ind + size]) {
                l.push_back(ind);
            }
        }
        sort(l.begin(), l.end());
    //    for (int i = 0; i < l.size(); ++i) {
    //        cout << " " << l[i];
    //    }
    //    cout << endl;
        cout << *l.begin() << " " << size << " " << size << endl;
    }
    
    // LEVENSTEIN DISTANCE
    
    vii vm;
    for (int i = 0; i < 30; ++i) {
        // TODO: 30 -- strange constant
        if (-len_s[i].first > 30) {
            vm.push_back(len_s[i].second);
        }
    }
    sort(vm.begin(), vm.end());

    for (int j = vm.size() - 1; j >= 0 ; --j) {
        for (int i = 0; i < shingled.size(); ++i) {
            if (shingled[i] == shingled[i + vm[j]]) {
                // CHECK as start
                // i -- start
                // j -- size
                for (int l = i + vm[j] - 1; l >= i + vm[j] - vm[j] / 10; --l) {
                    for (int k = 0; k < vm.size(); ++k) {
                        if (shingled[l] == shingled[l + vm[k]]) {
                            
                            cout << i << " " << vm[j] << " " << vm[k] << endl;
                            return;
                            
                        }
                    }

                }
                
            }
            
        }
    }
    cout << "=========" << endl;
}


int main(int argc, char **argv) {
//    if( argc <= 1 ){
//        std::cerr << "Usage: "<<argv[0]<<" [infile]" << std::endl;
//        return -1;
//    }
    
    //    std::ifstream input(argv[1]);
    
    
    std::ifstream input("/Users/styskin/s2018/3/test6.fasta");
    if(!input.good()){
        std::cerr << "Error opening '"<<argv[1]<<"'. Bailing out." << std::endl;
        return -1;
    }
    
    std::string line, name, content;
    while( std::getline( input, line ) ){
        if( line.empty() || line[0] == '>' ){ // Identifier marker
            if( !name.empty() ){ // Print out what we read from the last entry
                calcDirect(content);
//                calcInverted(content);
                name.clear();
            }
            if( !line.empty() ){
                name = line.substr(1);
            }
            content.clear();
        } else if( !name.empty() ){
            if( line.find(' ') != std::string::npos ){ // Invalid sequence--no spaces allowed
                name.clear();
                content.clear();
            } else {
                content += line;
            }
        }
    }
    if( !name.empty() ){ // Print out what we read from the last entry
        calcDirect(content);
//        calcInverted(content);
    }
    return 0;
}



