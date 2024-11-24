#include<bits/stdc++.h>
using namespace std;


int k , idCnt;
vector< int > dist , parent , startConfig , endConfig , rowPos , colPos;
vector< vector< int > > configurations;
map< vector< int > , int > id;
int explore_cnt , expanded_cnt;
const int dx[] = {-1,1,0,0};
const int dy[] = {0,0,-1,1};


int linearPos(int r , int c){
    return r * k + c;
}


pair< int , int > rowcolPos(int ix){
    int row = ix / k;
    int col = ix % k;
    return {row,col};
}


bool isSolvable(vector< int >&Config){
    int inversion_cnt = 0;
    for(int i = 0 ; i < Config.size() ; i ++ ){
        for(int j = i + 1 ; j < Config.size() ; j ++ ){
            if(Config[i] && Config[j] && Config[i] > Config[j]){
                inversion_cnt ++ ;
            }
        }
    }
    if(k % 2){
        return (inversion_cnt % 2 == 0);
    }
    int zero_pos = -1;
    for(int i = 0 ; i < Config.size() ; i ++ ){
        if(Config[i] == 0){
            zero_pos = i;
        }
    }
    int row = rowcolPos(zero_pos).first;
    return (inversion_cnt % 2 != row % 2);
}


int hammingDistance(vector< int >&Config){
    int sum = 0;
    for(int i = 0 ; i < Config.size() ; i ++ ){
        if(Config[i] == 0) continue;
        sum += (Config[i] != i + 1);
    }
    return sum;
}


int manhattanDistance(vector< int >&Config){
    int sum = 0;
    for(int i = 0 ; i < Config.size() ; i ++ ){
        if(Config[i] == 0) continue;
        pair< int , int > u = rowcolPos(i);
        pair< int , int > v ;
        v.first = rowPos[Config[i]];
        v.second = colPos[Config[i]];
        sum += abs(u.first - v.first) + abs(u.second - v.second);
    }
    return sum;
}


void preProcess(){
    idCnt = 2;
    id.clear();
    dist.clear();
    parent.clear();
    configurations.push_back(startConfig);
    configurations.push_back(endConfig);
    id[startConfig] = 0;
    id[endConfig] = 1;
    expanded_cnt = explore_cnt = 0;
    dist.push_back(0);
    dist.push_back(-1);
    parent.push_back(-1);
    parent.push_back(-1);
}


void AStar(int t){
    preProcess();
    priority_queue< pair<int,int> , vector< pair<int,int> > , greater< pair<int,int> > > pq;
    pq.push({0,0});
    dist[0] = 0;
    while(pq.size()){
        int u = pq.top().second;
        // cout << u << endl;
        pq.pop();
        if(u == 1){
            cout << "Minimum number of moves = " << dist[u] << '\n';
            return;
        }
        explore_cnt ++ ;
        int zero_pos = -1;
        for(int i = 0 ; i < configurations[u].size() ; i ++ ){
            if(configurations[u][i] == 0){
                zero_pos = i;
            }
        }
        

        pair< int , int > z = rowcolPos(zero_pos);
        for(int ix = 0 ; ix < 4 ; ix ++ ){
            int x = z.first + dx[ix];
            int y = z.second + dy[ix];

            if(x < 0 || x >= k || y < 0 || y >= k) continue;

            vector< int > nextConfig = configurations[u];
            int newPos = linearPos(x,y);
            swap(nextConfig[zero_pos] , nextConfig[newPos]);

            if(id.find(nextConfig) == id.end()){
                id[nextConfig] = idCnt ++ ;
                dist.push_back(-1);
                parent.push_back(-1);
                configurations.push_back(nextConfig);
            }

            int v = id[nextConfig];
            if(dist[v] == -1 || dist[v] > dist[u] + 1){
                dist[v] = dist[u] + 1;
                parent[v] = u;
                int d = dist[v] + (t? manhattanDistance(nextConfig):hammingDistance(nextConfig));
                pq.push({d , v});
                expanded_cnt ++ ;                
            }
        }
    }
}


void print_path(int node = 1){
    if(node) print_path(parent[node]);
    cout << '\n';
    for(int i = 0 ; i < configurations[node].size() ; i ++ ){
        cout << configurations[node][i] << ' ' ;
        if((i + 1)%k == 0) cout << '\n';
    }
    cout << '\n';
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> k;
    for(int i = 0 ; i < k * k ; i ++ ){
        int j;
        cin >> j;
        startConfig.push_back(j);
    }
    
    
    if(!isSolvable(startConfig)){
        cout << "Unsolvable puzzle";
        return 0;
    }


    int n = k * k - 1;
    for(int i = 1 ; i <= n ; i ++ ) endConfig.push_back(i);
    endConfig.push_back(0);


    rowPos = vector< int > (n + 1);
    colPos = vector< int > (n + 1);
    for(int i = 1 ; i <= n ; i ++ ){
        pair< int , int > z = rowcolPos(i - 1);
        rowPos[i] = z.first;
        colPos[i] = z.second;
    }
    rowPos[0] = colPos[0] = k - 1;

    //hamming
    // AStar(0);
    // print_path();
    // cout << explore_cnt << ' ' << expanded_cnt << '\n';

    //manhattan
    AStar(1);
    print_path();
    cout << explore_cnt << ' ' << expanded_cnt << '\n';
    
    return 0;
}
