//
//  aho.cpp
//  Restriction
//
//  Created by Andrey Styskin on 10.02.18.
//  Copyright © 2018 Andrey Styskin. All rights reserved.
//

#include "aho.hpp"

#include <queue>
#include <algorithm>

using namespace std;


char code(char c) {
    switch(c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'U': return 2;
        case 'G': return 3;
    }
    return 0;
}

///////////////////////////////////////////////////////////////
//         Aho-Corasick's algorithm, as explained in         //
//          http://dx.doi.org/10.1145/360825.360855          //
///////////////////////////////////////////////////////////////

// Max number of states in the matching machine.
// Should be equal to the sum of the length of all keywords.

// Builds the string matching machine.
//
// words - Vector of keywords. The index of each keyword is
//         important:
//         "out[state] & (1 << i)" is > 0 if we just found
//          word[i] in the text.
// lowestChar - The lowest char in the alphabet.
//              Defaults to 'a'.
// highestChar - The highest char in the alphabet.
//               Defaults to 'z'.
//               "highestChar - lowestChar" must be <= MAXC,
//               otherwise we will access the g matrix outside
//               its bounds and things will go wrong.
//
// Returns the number of states that the new machine has.
// States are numbered 0 up to the return value - 1, inclusive.
int TAho::buildMatchingMachine(const vector<string> &words,
                         char lowestChar,
                         char highestChar) {
    memset(out, 0, sizeof out);
    memset(f, -1, sizeof f);
    memset(g, -1, sizeof g);
    
    int states = 1; // Initially, we just have the 0 state
    
    for (int i = 0; i < words.size(); ++i) {
        const string &keyword = words[i];
        int currentState = 0;
        for (int j = 0; j < keyword.size(); ++j) {
            int c = code(keyword[j]) - lowestChar;
            if (g[currentState][c] == -1) {
                // Allocate a new node
                g[currentState][c] = states++;
            }
            currentState = g[currentState][c];
        }
        // There's a match of keywords[i] at node currentState.
//        out[currentState] |= (1 << i);
        out[currentState] |= 1;
    }
    
    // State 0 should have an outgoing edge for all characters.
    for (int c = 0; c < MAXC; ++c) {
        if (g[0][c] == -1) {
            g[0][c] = 0;
        }
    }
    
    // Now, let's build the failure function
    queue<int> q;
    // Iterate over every possible input
    for (int c = 0; c <= highestChar - lowestChar; ++c) {
        // All nodes s of depth 1 have f[s] = 0
        if (g[0][c] != -1 and g[0][c] != 0) {
            f[g[0][c]] = 0;
            q.push(g[0][c]);
        }
    }
    while (q.size()) {
        int state = q.front();
        q.pop();
        for (int c = 0; c <= highestChar - lowestChar; ++c) {
            if (g[state][c] != -1) {
                int failure = f[state];
                while (g[failure][c] == -1) {
                    failure = f[failure];
                }
                failure = g[failure][c];
                f[g[state][c]] = failure;
                
                // Merge out values
                out[g[state][c]] |= out[failure];
                q.push(g[state][c]);
            }
        }
    }
    
    return states;
}

// Finds the next state the machine will transition to.
//
// currentState - The current state of the machine. Must be
//                between 0 and the number of states - 1,
//                inclusive.
// nextInput - The next character that enters into the machine.
//             Should be between lowestChar and highestChar,
//             inclusive.
// lowestChar - Should be the same lowestChar that was passed
//              to "buildMatchingMachine".

// Returns the next state the machine will transition to.
// This is an integer between 0 and the number of states - 1,
// inclusive.


int TAho::findNextState(int currentState, char nextInput,
                  char lowestChar) {
    int answer = currentState;
    int c = nextInput - lowestChar;
    while (g[answer][c] == -1) answer = f[answer];
    return g[answer][c];
}

bool TAho::checkOut(int currentState) {
    return out[currentState] == 0;
}



///////////////////////////////////////////////////////////////
//               End of Aho-Corasick's algorithm             //
///////////////////////////////////////////////////////////////
