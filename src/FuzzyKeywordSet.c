//
//  FuzzyKeywordSet.c
//  fuzzy
//
//  Created by Lee on 2017/5/3.
//  Copyright © 2017年 Lee. All rights reserved.
//

#include "FuzzyKeywordSet.h"
#include <string.h>
#include <stdlib.h>

void substitution(char *src, char** target, int num) {
    int len = strlen(src);
    int *mark = (int*)malloc(sizeof(int)*num);
    int i;
    int count = 0;
    char *tmp;
    
    // index mark array
    // initialize mark array
    mark[0] = 0;
    for (i=1; i<num; i++) {
        mark[i] = mark[i-1]+1;
    }
    
    int k = num-1;// moving wildcard's index
    int p = num-1;// bottom's index
    
    while (1) {
        if (mark[p] >= len) {
            do {
                k--;
                if (k<0) {
                    return;
                }
            } while (mark[k]+1 == mark[k+1]);
            mark[k]++;
        
            for (i=k+1; i<num; i++) {
                mark[i] = mark[i-1]+1;
            }
            k = num-1;
        }
        else {
            // generate fuzzy keywords
            tmp = (char*)malloc(sizeof(char)*(len)+1);
            strcpy(tmp, src);
            for (i=0; i<num; i++) {
                tmp[mark[i]] = '*';
            }
            strcpy(target[count], tmp);
            free(tmp);
            
            mark[k]++;
            count++;
        }
    }
}


void insert(char *src, char **target, int num) {
    int len = strlen(src);
    int *mark = (int*)malloc(sizeof(int)*(len+1));
    int i,j;
    int count = 1;
    int zero = 0;
    int wildcardIndex = 0;
    int wildcardCount = 0;
    char *tmp = NULL;
    char *wildcard;
    
    // when num == 1
    if (num == 1) {
        char *one = "*";
        for (i=0; i<len+1; i++) {
            tmp = (char*)malloc(sizeof(char)*(len+1+num));
            strcpy(tmp, src);
            connectString(tmp, one, i);
            strcpy(target[i], tmp);
        }
        free(tmp);
        return;
    }
    
    // when num > 1
    // wildcard count mark array
    // initialize mark array
    mark[0] = num;
    for (i=1; i<num; i++) {
        mark[i] = 0;
    }
    
    int k = 0;// moving wildcard's index
    int p = 0;// former index
    
    // generate first fuzzy keyword
    tmp = (char*)malloc(sizeof(char)*(len+1+num));
    strcpy(tmp, src);
    wildcard = (char*)malloc(sizeof(char)*(num+1));
    for (j=0; j<num; j++) {
        wildcard[j] = '*';
    }
    wildcard[num] = '\0';
    connectString(tmp, wildcard, 0);
    strcpy(target[0], tmp);
    free(wildcard);
    free(tmp);
    
    do {
        do {
            mark[k]--;
            mark[++k]++;
            
            for (i=k+1; i<len+1; i++) {
                mark[k] += mark[i];
                mark[i] = 0;
            }
            
            for (i=k-1,zero=0; i>=0; i--) {
                if (mark[i] != 0) {
                    p = i;
                    break;
                }
                else {
                    zero++;
                    if (zero == k) {
                        p = k;
                        break;
                    }
                }
            }
            
            // generate fuzzy keywords
            tmp = (char*)malloc(sizeof(char)*(len+1+num));
            strcpy(tmp, src);
            
            for (i=0,wildcardIndex=0; i<len+1; i++) {
                if ((wildcardCount = mark[i]) != 0) {
                    wildcard = (char*)malloc(sizeof(char)*(wildcardCount+1));
                    for (j=0; j<wildcardCount; j++) {
                        wildcard[j] = '*';
                    }
                    wildcard[wildcardCount] = '\0';
                    connectString(tmp, wildcard, wildcardIndex);
                    free(wildcard);
                    wildcardIndex += wildcardCount;
                }
                else;
                wildcardIndex++;
            }
            strcpy(target[count], tmp);
            free(tmp);
            count++;
            
        } while (k != len);
        
        k = p;
        
    } while (p != len);
}

void connectString(char *des, char *src, int index){
    int i;
    int srcLen = strlen(src);
    int desLen = strlen(des);
    for (i=desLen; i>=index; i--) {
        des[i+srcLen] = des[i];
    }
    for (i=index; i<index+srcLen; i++) {
        des[i] = src[i-index];
    }
}

int combination(int m, int n) {
    int count,i,mx;
    for(mx=m,count=1,i=m-n;m>i;m--)
        count*=m;
    for(i=mx-m;i>1;count/=i--);
    return count;
}

int factorial(int n){
    int sum = 1;
    int i;
    for (i=1; i<=n; i++) {
        sum *= i;
    }
    return sum;
}

int substituteCount(int len, int num) {
    return combination(len, num);
}

int insertCount(int len, int num) {
    int sum = 1;
    int i;
    if (len > num) {
        for (i=len+num; i>len; i--) {
            sum *= i;
        }
    }
    else {
        for (i=len+num; i>num; i--) {
            sum *= i;
        }
    }
    return sum/factorial(num);
}
