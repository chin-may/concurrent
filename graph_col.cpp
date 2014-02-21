#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <sys/time.h>


using namespace std;

int* color(bool **graph, int nodes, int *numcol){
    int* colors = new int[nodes];
    int* weights = new int[nodes];
    int current_color = 1;
    bool* flag = new bool[nodes];

    for(int i=0; i<nodes; i++){
        colors[i] = -1;
        weights[i] = random();
        //cout<<weights[i]<<" ";
    }
    //cout<<endl;
    bool remain = true;
    while(remain){
        cilk_for(int i=0; i<nodes; i++){
            if(colors[i]==-1){
                flag[i] = true;
                for(int j=0; j<nodes;j++){
                    if(graph[i][j] && colors[j]==-1 && (weights[j]>weights[i] 
                                || (weights[j]==weights[i] && j>i) )){
                        //cout<<"made false\n";
                        flag[i] = false;
                    }
                }
            }
        }
        cilk_for(int i=0; i<nodes; i++){
            if(flag[i]){
                colors[i] = current_color;
                flag[i] = false;
            }
        }
        current_color++;
        remain = false;
        for(int i=0; i<nodes; i++){
            if(colors[i] == -1)
                remain = true;
        }
    }
    *numcol = current_color-1;
    return colors;
}



int* color_deg(bool **graph, int nodes, int *numcol){
    int* colors = new int[nodes];
    int* weights = new int[nodes];
    int current_color = 1;
    bool* flag = new bool[nodes];

    for(int i=0; i<nodes; i++){
        colors[i] = -1;
        weights[i] = 0;
        //cout<<weights[i]<<" ";
    }
    //cout<<endl;
    bool remain = true;
    while(remain){
        cilk_for(int i=0; i<nodes; i++){
            weights[i] = 0;
            for(int j=0; j<nodes; j++){
                if(graph[i][j] && colors[j]==-1 )
                    weights[i]++;
            }
        }
        cilk_for(int i=0; i<nodes; i++){
            if(colors[i]==-1){
                flag[i] = true;
                for(int j=0; j<nodes;j++){
                    if(graph[i][j] && colors[j]==-1 && (weights[j]>weights[i] 
                                || (weights[j]==weights[i] && j>i) )){
                        //cout<<"made false\n";
                        flag[i] = false;
                    }
                }
            }
        }
        cilk_for(int i=0; i<nodes; i++){
            if(flag[i]){
                int chosen=1;
                for(int j=0; j< nodes; j++){
                    if(graph[i][j] && colors[j]>chosen){
                        chosen = colors[j];
                    }
                }
                colors[i] = chosen+1;
                flag[i] = false;
            }
        }
        current_color++;
        remain = false;
        for(int i=0; i<nodes; i++){
            if(colors[i] == -1)
                remain = true;
        }
    }
    *numcol = current_color-1;
    return colors;
}



bool check_coloring(bool** graph, int* colors, int nodes){
        for(int i=0; i<nodes; i++){
            for(int j=0; j<nodes; j++){
                if(graph[i][j] && colors[i]==colors[j])
                    return false;
            }
        }
        return true;
}

int main(){
    srand(time(NULL));
    int nodes, edges;
    struct timeval start,end;
    int numcol;
    cin>>nodes;
    cin>>edges;
    bool **graph = new bool*[nodes];
    for(int i=0; i<nodes; i++){
        graph[i] = new bool[nodes];
        for(int j=0;j<nodes; j++){
            graph[i][j] = false;
        }
    }
    int n1,n2;
    for(int i=0; i<edges; i++){
        cin>>n1;
        cin>>n2;
        graph[n1-1][n2-1]=true;
        graph[n2-1][n1-1]=true;
    }
    gettimeofday(&start,NULL);
    int *colors = color_deg(graph,nodes, &numcol);
    gettimeofday(&end,NULL);
    int max=0;
    for(int i=0;i<nodes;i++){
        if(colors[i]>max)
            max=colors[i];
    }
    cout<<check_coloring(graph,colors,nodes)<<endl;
    cout<<end.tv_sec - start.tv_sec + (end.tv_usec-start.tv_usec)/1000000.0<<endl;
    cout<<max<<endl;
    for(int i=0; i<nodes; i++){
        cout<<colors[i]<<" ";
    }
    cout<<endl;
}
