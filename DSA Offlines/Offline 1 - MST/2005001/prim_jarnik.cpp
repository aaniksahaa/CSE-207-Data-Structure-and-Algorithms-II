
#include <bits/stdc++.h>
using namespace std;

/*** Definitions ***/

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define gap " "
#define br "\n"
#define int long long int
#define double long double
#define pb push_back
#define all(a) a.begin(),a.end()
#define fo(i,n) for(int i=0;i<n;i++)

/*** Constants ***/

const int inf = INT_MAX - 100;
const double eps = 1e-9;

/*** Mathematical Utilities ***/

bool dGreater(double a, double b)
{
    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * eps);
}

bool dLess(double a, double b)
{
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * eps);
}

bool dEqual(double a, double b)
{
    return fabs(a-b)<=eps;
}

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

class edge{
public:
    int u,v;
    double w;

    void show()
    {
        cout<<"u = "<<u<<", v = "<<v<<"  ->  w = "<<w<<endl;
    }
};
bool operator<(edge p, edge q)
{
    return dLess(p.w , q.w);
}

/*** Undirected Graph Data Structure ***/

class UndirectedGraph{
public:
    int n, m;
    int hasCycle, isBipartite, isConnected, componentCount;
    vector<int>d,parent,vis,color,start,finish;
    vector<int>V;                         // Vertices
    vector<vector<pair<int,double>>>adj;  // Adjacency List
    vector<vector<double>>mat;            // Adjacency Matrix
    vector<edge>E;                        // List of Undirected Edges

    void clearAll()
    {
        d.clear();
        parent.clear();
        vis.clear();
        color.clear();
        start.clear();
        finish.clear();
        V.clear();
        E.clear();
        adj.clear();
        mat.clear();
    }

    void clearArrays()
    {
        d.clear();
        parent.clear();
        vis.clear();
        color.clear();
        start.clear();
        finish.clear();
    }

    void resetArrays(int n)
    {
        d.resize(n+5,inf);
        parent.resize(n+5,-1);
        vis.resize(n+5,WHITE);
        color.resize(n+5,0);
        start.resize(n+5,0);
        finish.resize(n+5,0);
    }

    UndirectedGraph(int n, int m)
    {
        clearAll();

        this->n = n;
        this->m = m;

        resetArrays(n);
        adj.resize(n+5,vector<pair<int,double>>());

        setVerticesDefault(n);
    }

    // Cannot handle multiple edges
    UndirectedGraph(int n, int m, vector<edge>&edgeList)
    {
        clearAll();

        this->n = n;
        this->m = m;

        resetArrays(n);
        adj.resize(n+5,vector<pair<int,double>>());

        setVerticesDefault(n);
        for(auto e: edgeList)
        {
            int u,v,w;
            u = e.u;
            v = e.v;
            w = e.w;
            E.pb(e);
            adj[u].pb({v,w});
        }
    }

    void constructAdjMatrix()  // O(n^2) Memory needed
    {
        mat.clear();
        mat.resize(n+5,vector<double>(n+5,inf));
        for(auto e: E)
        {
            int u,v;
            double w;
            u = e.u;
            v = e.v;
            w = e.w;
            mat[u][v] = w;
        }
    }

    void setVertices(vector<int>&vertices)
    {
        V.clear();
        for(int v: vertices)
        {
            V.push_back(v);
        }
    }

    void setVerticesDefault(int n)
    {
        V.clear();
        for(int i=1;i<=n;i++)
        {
            V.push_back(i);
        }
    }

    void makeVerticesZeroIndexed()
    {
        V.clear();
        for(int i=0;i<n;i++)
        {
            V.push_back(i);
        }
    }

    void addEdge(int u, int v, double w)
    {
        E.pb({u,v,w});

        adj[u].pb({v,w});
        adj[v].pb({u,w});
    }

    vector<edge>MST_Prim_Jarnik(int root_node)
    {
        vector<edge>MST;
        vector<int>parent(n+5,-1),taken(n+5,0);
        vector<double>key(n+5,inf);

        priority_queue<pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;

        int root = root_node;  // Can be changed to any

        pq.push({0,root});
        key[root] = 0;

        while(!pq.empty())
        {
            auto p = pq.top();
            pq.pop();

            int u = p.second;

            if(taken[u] || dGreater(p.first , key[u]))
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
                if(!taken[v] && dLess( w , key[v] ))
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

};

void solve()
{
    //global initializations like "vector clear", "sum = 0"
    int n,m,p,q,maxm,minm,u,v,choice,src,dst;

    double w,ans = 0;

    cin>>n>>m;

    UndirectedGraph G(n,m);

    G.makeVerticesZeroIndexed();

    for(int i=0;i<m;i++)
    {
        cin>>u>>v>>w;

        G.addEdge(u,v,w);
    }
    int root = 0;
    auto MST = G.MST_Prim_Jarnik(root);

    for(auto e: MST)
    {
        ans += e.w;
    }

    cout<<"Prim-Jarnik Algorithm:"<<endl;
    cout<<"Total weight = "<<ans<<endl;
    cout<<"Root node = "<<root<<endl;
    for(auto e: MST)
    {
        cout<<e.u<<" "<<e.v<<endl;
    }

}

void pre()
{

}

int32_t main() {

    freopen("in.txt", "r", stdin);
    freopen("prim.txt", "w", stdout);
    solve();
}


