//
//  EditDistance.c
//  fuzzy
//
//  Created by Lee on 2017/5/2.
//  Copyright © 2017年 Lee. All rights reserved.
//

#include "EditDistance.h"
#include <string.h>

int editDistance(char *s, char *t) {
    // for all i and j, d[i,j] will hold the Levenshtein distance between
    // the first i characters of s and the first j characters of t;
    // note that d has (m+1)x(n+1) values
    int m = strlen(s);
    int n = strlen(t);
    int d[m+1][n+1];
    int i,j=0;
    
    for (i=0; i<=m; i++) {
        d[i][0] = i;
    }
    for (j=0; j<=n; j++) {
        d[0][j] = j;
    }
    
    for (i=1; i<=m; i++) {
        for (j=1; j<=n; j++) {
            if (s[i-1] == t[j-1])
                d[i][j] = d[i-1][j-1];
            else
                d[i][j] = minimum(d[i-1][j]+1, d[i][j-1]+1, d[i-1][j-1]+1);
            // a deletion, an insertion, a substitution
        }
    }
    return d[m][n];
}

int minimum(int a, int b, int c) {
    int min = a;
    if (b < min)
        min = b;
    else if (c < min)
        min = c;
    return min;
}
