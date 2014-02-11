#include <iostream>
#include <cilk/cilk.h>

using namespace std;

bool check(bool** a, int row, int col){
    if(row>=0 && col>=0 && row<10000 && col<10000)
        return a[row][col];
    else return false;
}

int get_num_neighbours(bool** a,int row, int col){
    int num = 0;
    if(check(a,row-1,col)) num++;
    if(check(a,row,col-1)) num++;
    if(check(a,row-1,col-1)) num++;

    if(check(a,row+1,col)) num++;
    if(check(a,row,col+1)) num++;
    if(check(a,row+1,col+1)) num++;

    if(check(a,row-1,col+1)) num++;
    if(check(a,row+1,col-1)) num++;
}

bool** run_game(bool** a, bool** b, int num_steps){
    bool** cur, **prev;
    for(int step = 0; step<num_steps; step++){
        cur = step%2!=0?a:b;
        prev = step%2!=0?b:a;
        for(int i=0; i<10000; i++){
            for(int j=0; j<10000; j++){
                int neighbours = get_num_neighbours(prev,i,j);
                if(neighbours < 2 || neighbours >  3)
                    cur[i][j] = false;
                else if(neighbours == 3)
                    cur[i][j] = true;
                else 
                    cur[i][j] = prev[i][j];
            }
        }
    }
    return cur;
}


int main(){
    bool **a = new bool*[10000];
    bool **b = new bool*[10000];
    for(int i=0; i<10000; i++){
        a[i] = new bool[10000];
        b[i] = new bool[10000];
    }
    for(int i=0; i<10000; i++){
        for(int j=0; j<10000; j++){
        cin>>a[i][j];
        }
    }
    int steps;
    cin>>steps;
    bool** result = run_game(a,b,steps);
    for(int i=0; i<10000; i++){
        for(int j=0; j<10000; j++){
            cout<<a[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}




