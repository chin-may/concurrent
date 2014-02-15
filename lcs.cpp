#include <iostream>
#include <algorithm>
#include <cilk/cilk.h>

using namespace std;

inline int get(int** a,int i, int j){
    if(i>=0 && j>=0)
        return a[i][j];
    else 
        return 0;
}

int p_lcs(string a, string b, int n1, int n2){
    int** tab = new int*[n1];
    for(int i=0; i<n1; i++){
        tab[i] = new int[n2];
    }
    int iter = 1;
    int ns = 1;
    while(ns>0){
        cilk_for(int sp=0; sp<ns; sp++){
            int sum = iter - 1;
            int i,j;
            if(iter<=n1){
                j=sp;
                i=sum-sp;
            }
            else if(iter>n1){
                j = sp+iter-n1;
                i = sum-j;
            }
            if(a[i]==b[j]){
                tab[i][j] = get(tab,i-1,j-1) + 1;
                //cout<<a[i];
                //cout<<i<<" "<<j<<" "<<tab[i][j]<<endl;
            }
            else{
                tab[i][j] = max(get(tab,i-1,j-1), max(get(tab,i-1,j),get(tab,i,j-1)));
                //cout<<i<<" "<<j<<" "<<tab[i][j]<<endl;
            }
        }
        if(iter<n1 && iter<n2)
            ns++;
        else if(iter>=n1 && iter>=n2)
            ns--;
        iter++;
    }
    //cout<<endl;
    /*for(int y=0;y<n1;y++){
        for(int z=0;z<n2;z++){
            cout<<tab[y][z]<<" ";
        }
        cout<<endl;
    }
    */
    return tab[n1-1][n2-1];
}


int main(){
    string s1,s2;
    getline(cin,s1,'$');
    s1.erase(remove(s1.begin(), s1.end(), '\n'), s1.end());

    getline(cin,s2,'$');
    s2.erase(remove(s2.begin(), s2.end(), '\n'), s2.end());
    int size;
    if(s1.length() <= s2.length())
        size=p_lcs(s1,s2,s1.length(),s2.length());
    else
        size=p_lcs(s2,s1,s2.length(),s1.length());
    cout<<size<<endl;

}




