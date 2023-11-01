#include<bits/stdc++.h>
using namespace std;

/*** Definitions ***/

#define int long long int

/*** Constants ***/

const int inf = LLONG_MAX - 100;

/*** Utility Function for Random ***/

// return random integer in the range [L,R] inclusive

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int random(int L,int R) {
	return uniform_int_distribution<int>(L, R)(rng);
}

/*** Union Find Data Structure ***/

class UnionFind{
    int n;
    vector<int>par,csize;
public:
    void clearArrays()
    {
        par.clear();
        csize.clear();
    }
    void resetArrays(int n)
    {
        par.resize(n+5);
        csize.resize(n+5);
    }
    UnionFind(int n)
    {
        clearArrays();

        this->n = n;
        resetArrays(n);

        for(int i=0;i<n+5;i++)
            make_set(i);
    }
    void make_set(int p)
    {
        par[p] = p;
        csize[p] = 1;
    }
    int find(int u)
    {
        if(u == par[u])
            return u;
        return par[u] = find(par[u]);
    }
    void unite(int u, int v)
    {
        u = find(u);
        v = find(v);
        if(u != v)
        {
            if(csize[u] < csize[v])
                swap(u,v);
            csize[u] += csize[v];
            par[v] = u;
        }
    }
    bool isUnited(int u, int v)
    {
        return find(u)==find(v);
    }
};

/*** Edge Data Structure ***/

class Edge{
public:
    int u,v;
    double w;

    void show()
    {
        cout<<"u = "<<u<<", v = "<<v<<"  ->  w = "<<w<<endl;
    }
};
bool operator<(Edge p, Edge q)
{
    return p.w < q.w;
}

/*** Start ***/

int n,m;
vector<vector<pair<int,double>>>adj;
vector<Edge>E;

int root_node = 0;

vector<Edge>MST_Prim_Jarnik()
{
    vector<Edge>MST;
    vector<int>parent(n+5,-1),taken(n+5,0),key(n+5,inf);

    priority_queue<pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;

    int root = root_node;  // Can be changed to any

    pq.push({0,root});
    key[root] = 0;

    while(!pq.empty())
    {
        auto p = pq.top();
        pq.pop();

        int u = p.second;

        if(taken[u] || p.first > key[u])
            continue;

        taken[u] = 1;

        if(u != root)
        {
            MST.push_back({parent[u],u,p.first});
        }
        for(auto e: adj[u])
        {
            int v = e.first;
            double w = e.second;
            if(!taken[v] && w < key[v])
            {
                key[v] = w;
                parent[v] = u;
                pq.push({key[v],v});
            }
        }
    }

    if(MST.size() != n-1)  // Checking whether a valid tree has been formed
    {
        MST.clear();
        return MST;  // returning empty vector if not possible
    }

    return MST;

}

void solve()
{
    int u,v;
    double w;

    bool random_root = false;

    cin>>n>>m;

    adj.resize(n+5,vector<pair<int,double>>());

    for(int i=0;i<m;i++)
    {
        cin>>u>>v>>w;

        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

    if(random_root)
    {
        root_node = random(0,n-1);
    }

    vector<Edge>MST = MST_Prim_Jarnik();

    double MST_weight = 0;
    for(auto e: MST)
    {
        MST_weight += e.w;
    }

    cout<<"Prim-Jarnik Algorithm:"<<endl;
    cout<<"Total weight = "<<MST_weight<<endl;
    cout<<"Root node = "<<root_node<<endl;
    for(auto e: MST)
    {
        cout<<e.u<<" "<<e.v<<endl;
    }


}

int32_t main()
{
    freopen("in.txt", "r", stdin);
    freopen("prim.txt", "w", stdout);
    solve();
}

/* Inputs

Input - 1

7 16
0 2 12
0 3 13
0 4 5
0 5 14
0 6 6
1 2 7
1 3 9
1 4 5
1 5 7
1 6 9
2 4 11
2 5 5
2 6 8
3 4 7
3 5 13
3 6 6

*/

