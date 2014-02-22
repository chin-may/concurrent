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



int* color_deg(bool **graph, int nodes){
    int* colors = new int[nodes];
    int* weights = new int[nodes];
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
                int chosen=0;
                bool used=true;
                while(used){
                    chosen++;
                    used=false;
                    for(int j=0; j< nodes; j++){
                        if(graph[i][j] && colors[j]==chosen){
                            used=true;
                            break;
                        }
                    }
                }
                colors[i] = chosen;
                flag[i] = false;
            }
        }
        remain = false;
        for(int i=0; i<nodes; i++){
            if(colors[i] == -1)
                remain = true;
        }
    }
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
    //srand(time(NULL));
    int nodes, edges;
    struct timeval start,end;
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
    int *colors = color_deg(graph,nodes );
    gettimeofday(&end,NULL);
    int max=0;
    for(int i=0;i<nodes;i++){
        if(colors[i]>max)
            max=colors[i];
    }
    //cout<<check_coloring(graph,colors,nodes)<<endl;
    cout<<(end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec)<<endl;
    cout<<max<<endl;
    for(int i=0; i<nodes; i++){
        cout<<i<<" "<<colors[i]<<endl;
    }
    cout<<endl;
}
