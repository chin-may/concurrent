#include <iostream>
#include <cilk/cilk.h>

using namespace std;



int p_lcs(char *a, char* b, int n1, int n2){
    int** tab = new int*[n1];
    for(int i=0; i<n1; i++){
        tab[i] = new int[n2];
    }
    int ci=0, cj=0;
    int sum=0;
    int sp_num = 1;
    while(true){
        cilk_for(int sp=0; sp<sp_num; sp++){
            int i,j;
            if(ci<n1 && cj<n2){
                i = sp;
                j = sum-sp;
            }


            }
        }
    }
}
