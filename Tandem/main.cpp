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


void calcInverted(const string& line) {
    vii shingled;
    vii ishingled;
    
    int MASK = (1 << (M * 2)) - 1;
    int x = 0;
    int ix = 0;
    for (int i = 0; i < M; ++i) {
        x = (x << 2 | code(line[i])) & MASK;
        ix = (ix | (code(line[i]) << (2 * i) )) & MASK;
    }
    for (int i = M; i < line.length(); ++i) {
        shingled.push_back(x);
        ishingled.push_back(ix);
        x = (x << 2 | code(line[i])) & MASK;
        ix = (ix >> 2 | (code(line[i]) << (2 * (M - 1)) )) & MASK;
    }
    shingled.push_back(x);
    ishingled.push_back(ix);
//    cout << line << endl;
//    for (int i = 0; i < shingled.size(); ++i) {
//        cout << shingled[i] << " = " << decode(shingled[i]) << " - " << ishingled[i] << " = " << decode(ishingled[i])<< endl;
//    }
    
    
    int mstart = 0;
    int msize = -1;
    for (int i = 0; i < shingled.size() - M; ++i) {
        if (shingled[i] == ishingled[i + M]) {
            int size = 0;
            while (i - size >= 0 && i + M + size < shingled.size() && shingled[i - size] == ishingled[i + M + size]) {
                ++size;
            }
            --size;
            if (size > msize) {
                msize = size + M;
                mstart = i - size;
            }
        }
    }
    
    cout << mstart << " " << msize << " " << msize << endl;
    
    cout << line.substr(mstart, msize) << endl;
    cout << line.substr(mstart + msize, msize) << endl;
    
    
}

void calcDirect(const std::string& line) {
    unordered_map<int, vii> hash;
    vii shingled;
    
    
    int MASK = (1 << (M * 2)) - 1;
    int x = 0;
    for (int i = 0; i < M; ++i) {
        x = (x << 2 | code(line[i])) & MASK;
    }
    for (int i = M; i < line.length(); ++i) {
        // current hash is x
        shingled.push_back(x);
        hash[x].push_back(i - M);
        x = (x << 2 | code(line[i])) & MASK;
    }
    unordered_map<int, int> len;
    for (unordered_map<int, vii>::const_iterator it = hash.begin(); it != hash.end(); ++it) {
        if (it->second.size() == 2) {
//            cout << "$$ "<< it->second[1] - it->second[0] << " " << shingled[it->second[0]] << " " << shingled[it->second[1]] << endl;
            len[it->second[1] - it->second[0]] ++;
        }
    }
    vector<pair<int, int> > len_s;
    
    for (unordered_map<int, int>::const_iterator it = len.begin(); it != len.end(); ++it) {
        len_s.push_back(make_pair(-it->second, it->first));
    }
    // need to get the biggest distances
    sort(len_s.begin(), len_s.end());
    for (int i = 0; i < 5; ++i) {
        cout << len_s[i].first << " " << len_s[i].second << endl;
    }
    // Get TOP
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


int main(int argc, char **argv) {
//    if( argc <= 1 ){
//        std::cerr << "Usage: "<<argv[0]<<" [infile]" << std::endl;
//        return -1;
//    }
    
    //    std::ifstream input(argv[1]);
    
    
    if (0) {
        calcInverted("AAACTGGTCAG");
        return 0;
    }
    
    
    std::ifstream input("/Users/styskin/s2018/3/test5.fasta");
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



