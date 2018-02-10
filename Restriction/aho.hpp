//
//  aho.hpp
//  Restriction
//
//  Created by Andrey Styskin on 10.02.18.
//  Copyright © 2018 Andrey Styskin. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <stdio.h>

const int MAXS = 3 * 100000 + 10000;
const int MAXC = 4;

char code(char c);

class TAho {
    int out[MAXS];
    // Used internally in the algorithm.
    int f[MAXS]; // Failure function
    int g[MAXS][MAXC]; // Goto function, or -1 if fail.

    
public:
    TAho() {
        
    }
    
    int buildMatchingMachine(const std::vector<std::string> &words,
                         char lowestChar = 0,
                         char highestChar = 4);
    
    int findNextState(int currentState, char nextInput,
                      char lowestChar = 0);
    
    
    bool checkOut(int currentState);
    
};
