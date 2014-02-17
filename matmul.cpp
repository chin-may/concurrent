#include <iostream>
#include <cilk/cilk.h>
#include <stdlib.h>
#include <sys/time.h>

/*
void mat_square(int* a, int* b, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<n; k++){
            }
        }
    }
}
*/
#define BLOCK_SIZE 1024
void matmul(float** a, float** b, float** c, int sia, int sja, int sib,
        int sjb,int sic, int sjc,int n){
    //float **tmp = malloc(n*sizeof(float*));
    if(n<=BLOCK_SIZE){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                for(int k=0;k<n;k++){
                    c[sic+i][sjc+j]+=a[sia+i][sja+k]*b[sib+k][sjb+j];
                }
            }
        }
    }
    else{
        int hn = n/2;
        cilk_spawn matmul(a,b,c,sia,sja,sib,sjb,sic,sjc,n/2); //c11 b11 a11
        cilk_spawn matmul(a,b,c,sia,sja,sib,sjb+hn,sic,sjc+hn,n/2); //a11 b12 c12
        cilk_spawn matmul(a,b,c,sia+hn,sja,sib,sjb+hn,sic+hn,sjc+hn,n/2);//a21 b12 c22
        cilk_spawn matmul(a,b,c,sia+hn,sja,sib,sjb,sic+hn,sjc,n/2);//a21 b11 c21
        
        cilk_sync;

        cilk_spawn matmul(a,b,c,sia+hn,sja+hn,sib+hn,sjb,sic+hn,sjc,n/2);//a22 b21 c21
        cilk_spawn matmul(a,b,c,sia+hn,sja+hn,sib+hn,sjb+hn,sic+hn,sjc+hn,n/2);//a22 b22 c22
        cilk_spawn matmul(a,b,c,sia,sja+hn,sib+hn,sjb+hn,sic,sjc+hn,n/2);//a12 b22 c12
        cilk_spawn matmul(a,b,c,sia,sja+hn,sib+hn,sjb,sic,sjc,n/2);//a12 b21 c11
    }
}

using namespace std;
int main(){
    int n;
    cin>>n;
    struct timeval start, end;
    srand(time(NULL));
    float **a,**c;
    a = new float*[n];
    c = new float*[n];
    for(int i=0;i<n;i++){
        a[i] = new float[n];
        c[i] = new float[n];
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            //cin>>a[i][j];
            a[i][j] = ((float)rand())/RAND_MAX;
            c[i][j] = 0;
        }
    }
    gettimeofday(&start,NULL);
    matmul(a,a,c,0,0,0,0,0,0,n);
    gettimeofday(&end,NULL);
    cout<<end.tv_sec-start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0<<endl;
    //cout<<end.tv_sec-start.tv_sec<<endl;
    /*for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<c[i][j]<<" ";
        }
        cout<<endl;
    }
    */
}

