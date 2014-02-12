#include <iostream>
#include <cilk/cilk.h>

using namespace std;

int lcs(char *a, char* b, int n1, int n2){
    int** tab = new int*[n1];
    for(int i=0; i<n1; i++){
        tab[i] = new int[n2];
    }
    for(int i = 0; i<n1; i++){
        for(int j = 0; j<n1; j++){
            if(a[i]==b[j]){
                tab[i][j] = tab[i-1][j-1] + 1;
            }
            else{
                tab[i][j] = max(tab[i-1][j-1], tab[i-1][j],tab[i][j-1]);
            }
        }
    }
}

