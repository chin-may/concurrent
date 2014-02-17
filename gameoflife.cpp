#include <iostream>
#include <cilk/cilk.h>
#include <sys/time.h>

using namespace std;
#define SIZE 10000

inline bool check(bool** a, int row, int col){
    if(row>=0 && col>=0 && row<SIZE && col<SIZE)
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
    return num;
}

bool** run_game(bool** a, bool** b, int num_steps){
    bool** cur, **prev;
    for(int step = 0; step<num_steps; step++){
        //cout<<"step"<<endl;
        cur = step%2!=0?a:b;
        prev = step%2!=0?b:a;
        cilk_for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                int neighbours = get_num_neighbours(prev,i,j);
                //cout<<neighbours<<" ";
                if(neighbours < 2 || neighbours >  3){
                    cur[i][j] = false;
                }
                else if(neighbours == 3){
                    cur[i][j] = true;
                }
                else {
                    cur[i][j] = prev[i][j];
                }
            }
            //cout<<endl;
        }
    }
    return cur;
}


int main(){
    bool **a = new bool*[SIZE];
    bool **b = new bool*[SIZE];
    for(int i=0; i<SIZE; i++){
        a[i] = new bool[SIZE];
        b[i] = new bool[SIZE];
    }
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
        cin>>a[i][j];
        }
    }
    int steps;
    cin>>steps;
    struct timeval start,end;
    gettimeofday(&start,NULL);
    bool** result = run_game(a,b,steps);
    gettimeofday(&end,NULL);
    cout<<end.tv_sec-start.tv_sec<<endl;

    //cout<<"res\n";
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            cout<<result[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}



