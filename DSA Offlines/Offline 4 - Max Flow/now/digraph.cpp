
#include <bits/stdc++.h>
using namespace std;

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define gap " "
#define br "\n"
#define pb push_back
#define all(a) a.begin(),a.end()

const int inf = INT_MAX - 100;
const double eps = 1e-9;

// Disjoint Set Union Find

vector<int>par,csize;  // csize = component size

void initUF(int n)
{
    par.clear();
    csize.clear();
    par.resize(n+5);
    csize.resize(n+5);
}

void make_set(int v) {
    par[v] = v;
    csize[v] = 1;
}

int find(int v) {
    if (v == par[v])
        return v;
    return par[v] = find(par[v]);
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b)
    {
        if(csize[a] < csize[b])
            swap(a,b);
        csize[a] += csize[b];
        par[b] = a;
    }
}

bool isUnited(int a, int b)
{
    return find(a)==find(b);
}

// End of DSU

// Edge structure

struct edge{
    int u,v;
    double w;
};
bool operator<(edge a, edge b)
{
    return a.w < b.w;
}
void show(edge e)
{
    cout<<"("<<e.u<<","<<e.v<<") -> "<<e.w<<br;
}

// End of Endge struct

// Capacity structure

struct capacity{
    int u,v;
    double cap;
};
bool operator<(capacity a, capacity b)
{
    return a.cap < b.cap;
}
void show(capacity e)
{
    cout<<"("<<e.u<<","<<e.v<<") -> "<<e.cap<<br;
}

// End of Capacity struct

// directed weighted graph
class DirectedGraph{
public:
    int n, m;
    int time;
    int hasCycle, hasNegativeCycle;
    vector<double>d;
    vector<int>parent;
    vector<int>vis;
    vector<int>color;
    vector<int>start;
    vector<int>finish;
    vector<int>V;                         // Vertices
    vector<vector<pair<int,double>>>adj;  // Adjacency List
    vector<vector<double>>mat;            // Adjacency Matrix
    vector<edge>E;                        // List of Undirected Edges
    vector<capacity>C;
    vector<vector<double>>residual_capacity;
    vector<vector<double>>sp;  // All Pair Shortest Path
    vector<vector<int>>sp_parent;  // For APSP

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
        C.clear();
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

    DirectedGraph(int n, int m)
    {
        clearAll();

        this->n = n;
        this->m = m;

        resetArrays(n);
        adj.resize(n+5,vector<pair<int,double>>());

        setVerticesDefault(n);
    }

    void initFlowNetwork()
    {
        residual_capacity.resize(n+5,vector<double>(n+5,0));
    }

    void addEdge(int u, int v, double w)
    {
        E.pb({u,v,w});

        adj[u].pb({v,w});
    }

    void addCapacity(int u, int v, double cap)
    {
        C.pb({u,v,cap});

        adj[u].pb({v,1});
        adj[v].pb({u,1});

        residual_capacity[u][v] = cap;
    }

    void addBidirectionalCapacity(int u, int v, double cap)
    {
        C.pb({u,v,cap});

        adj[u].pb({v,1});
        adj[v].pb({u,1});

        residual_capacity[u][v] = cap;
        residual_capacity[v][u] = cap;
    }

    void constructAdjMatrix()  // O(n^2) Memory needed
    {
        mat.clear();
        mat.resize(n+5,vector<double>(n+5,inf));
        for(auto e: E)
        {
            int u,v,w;
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

    void callDFS(int u)
    {
        time++;
        vis[u] = GRAY;
        start[u] = time;
        for(auto p: adj[u])
        {
            int v = p.first;
            int w = p.second;

            if(vis[v] == WHITE)
            {
                parent[v] = u;
                callDFS(v);
            }
            else if(vis[v] == GRAY)
            {
                hasCycle = 1;
            }
        }
        time++;
        finish[u] = time;
        vis[u] = BLACK;
    }

    void DFS(int src)
    {
        time = 0;
        clearArrays();
        resetArrays(n);

        callDFS(src);
    }

    void DFSAll()
    {
        time = 0;
        clearArrays();
        resetArrays(n);

        for(int s: V)
        {
            if(vis[s] == WHITE)
            {
                callDFS(s);
            }
        }
    }

    vector<int>topsort()
    {
        DFSAll();

        if(hasCycle)
        {
            vector<int>empty_vector;
            return empty_vector;
        }

        vector<int>topsorted;

        vector<pair<int,int>>temp;

        for(auto u: V)
        {
            temp.push_back({finish[u],u});
        }

        sort(all(temp));

        for(auto p: temp)
        {
            topsorted.push_back(p.second);
        }

        reverse(all(topsorted));

        return topsorted;
    }

    void dijkstra(int s)
    {

        for(int i=0;i<n+5;i++)
        {
            d[i] = inf;
            parent[i] = -1;
        }

        d[s] = 0;

        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq; // Min Heap for {cost,v}

        pq.push({d[s], s});

        while (!pq.empty())
        {
            int u = pq.top().second;
            double cost = pq.top().first;

            pq.pop();

            if (cost > d[u])   // avoiding unnecessary previous entries
                continue;

            for (auto edge : adj[u])
            {
                int v = edge.first;
                double w = edge.second;

                int newcost = cost + w;

                if ( newcost < d[v])
                {
                    d[v] = newcost;
                    parent[v] = u;
                    pq.push({d[v], v});
                }
            }
        }
    }

    void bellmanford(int src)
    {
        int node;
        hasNegativeCycle = 0;

        for(int i=0;i<n+5;i++)
        {
            d[i] = inf;
            parent[i] = -1;
        }
        d[src] = 0;
        for(int step=0;step<n-1;step++)
        {
            for(auto e: E)
            {
                if((d[e.u] + e.w) < d[e.v])
                {
                    d[e.v] = d[e.u] + e.w;
                    parent[e.v] = e.u;
                }
            }
        }

        vector<int>involved,cycle;

        for(auto e: E)
        {
            if((d[e.u] + e.w) < d[e.v])
            {
                hasNegativeCycle = 1;
                d[e.v] = d[e.u] + e.w;
                parent[e.v] = e.u;
                involved.pb(e.v);
                node = e.v;
                //break;
                //the vertices in involved are reachable from negative cycles
                // but there are other involved vertices not explored in this step
            }
        }

        if(!hasNegativeCycle)
            return;

        for(int step=0;step<n;step++)
        {
            node = parent[node];
        }
        for(int v=node;;v = parent[v])
        {
            cycle.pb(v);
            if(v==node && cycle.size()>1)
            {
                break;
            }
        }
        reverse(all(cycle));

    }

    void floyd_warshall()
    {
        sp.clear();

        sp.resize(n+5,vector<double>(n+5,inf));
        sp_parent.resize(n+5,vector<int>(n+5,-1));

        for(int u: V)
        {
            sp[u][u] = 0;
            for(auto e: adj[u])
            {
                int v = e.first;
                sp[u][v] = e.second;
            }
        }

        for(int i: V)
        {
            for(int j: V)
            {
                sp_parent[i][j] = i;
            }
        }

        for(int k: V)
        {
            for(int i: V)
            {
                for(int j: V)
                {
                    if(sp[i][k]<inf && sp[k][j]<inf)
                    {
                        if(sp[i][k]+sp[k][j] < sp[i][j])
                        {
                            sp[i][j] = sp[i][k]+sp[k][j];
                            sp_parent[i][j] = sp_parent[k][j];
                        }
                    }
                }
            }
        }

    }

    vector<int> get_shortest_path(int s, int t) // for APSP
    {
        int now = t;
        vector<int>path;
        while(now != s)
        {
            path.push_back(now);
            now = sp_parent[s][now];
        }
        path.push_back(s);
        reverse(all(path));
        return path;
    }

    // returns the minimum residual capacity of any existing augmenting path from s to t
    double find_path_by_BFS(int s, int t)
    {
        for(int i=0;i<n+5;i++)
        {
            parent[i] = -1;
        }

        parent[s] = -101;

        queue<pair<int,double>>Q;

        Q.push({s,inf});  // vertex , minimum_residual_capacity_so_far

        while(!Q.empty())
        {
            auto p = Q.front();
            Q.pop();

            int u;

            double min_res_cap;

            u = p.first;
            min_res_cap = p.second;

            for(auto e: adj[u])
            {
                int v = e.first;
                if(parent[v]==-1 && residual_capacity[u][v])
                {
                    parent[v] = u;
                    int new_min_res_cap = min(min_res_cap,residual_capacity[u][v]);
                    if(v == t)
                    {
                        return new_min_res_cap;
                    }
                    Q.push({v,new_min_res_cap});
                }
            }
        }

        return 0;


    }

    // Edmond-Karp Implementation
    double ford_fulkerson_max_flow(int s, int t)  // O(VE^2)
    {
        double flow = 0;

        while(1)
        {
            double min_res_cap = find_path_by_BFS(s,t);

            flow += min_res_cap;

            if(min_res_cap <= eps)
                break;

            int now = t;

            while(now != s)
            {
                int prev = parent[now];

                residual_capacity[prev][now] -= min_res_cap;
                residual_capacity[now][prev] += min_res_cap;

                now = prev;
            }
        }

        return flow;
    }

    void printAdjList()
    {
        cout<<"\nAdjacency List Representation: \n\n";
        for(int u: V)
        {
            cout<<u<<" -> ";
            for(auto e: adj[u])
            {
                cout<<"("<<e.first<<","<<e.second<<") ";
            }
            cout<<br;
        }
    }

};

void solve()
{
    //global initializations like "vector clear", "sum = 0"
    int n,m,p,q,maxm,minm,u,v,choice,src,dst;

    double w,ans = 0;

    cin>>n>>m;

    DirectedGraph G(n,m);

    for(int i=0;i<m;i++)
    {

    }

}

void pre()
{

}

int32_t main() {

    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    solve();
}

