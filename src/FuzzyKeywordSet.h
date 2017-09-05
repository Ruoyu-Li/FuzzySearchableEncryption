//
//  FuzzyKeywordSet.h
//  fuzzy
//
//  Created by Lee on 2017/5/3.
//  Copyright © 2017年 Lee. All rights reserved.
//

#ifndef FuzzyKeywordSet_h
#define FuzzyKeywordSet_h

#include <stdio.h>

void substitution(char *src, char **target, int num);

void insert(char *src, char **target, int num);

void connectString(char *des, char *src, int index);

int combination(int m, int n);

int factorial(int n);

int substituteCount(int len, int num);

int insertCount(int len, int num);

#endif /* FuzzyKeywordSet_h */
