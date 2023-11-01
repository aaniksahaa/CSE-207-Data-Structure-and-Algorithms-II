
#include <bits/stdc++.h>
using namespace std;

/*** Definitions ***/

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define gap " "
#define br "\n"
#define pb push_back
#define all(a) a.begin(),a.end()
#define int long long int
#define double long double

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

/*** Directed Graph Data Structure ***/

class DirectedGraph{
public:
    int n, m;
    int time;
    int hasCycle, hasNegativeCycle;
    vector<double>d;
    vector<int>parent,vis,color,start,finish;
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

    DirectedGraph(int n, int m)
    {
        clearAll();

        this->n = n;
        this->m = m;

        resetArrays(n);
        adj.resize(n+5,vector<pair<int,double>>());

        setVerticesDefault(n);
    }

    void addEdge(int u, int v, double w)
    {
        E.pb({u,v,w});

        adj[u].pb({v,w});
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

    void dijkstra(int src)
    {
        clearArrays();
        resetArrays(n);

        priority_queue<pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>>pq;

        d[src] = 0;

        pq.push({d[src],src});

        while(!pq.empty())
        {
            auto curr_min = pq.top();
            pq.pop();

            int u = curr_min.second;
            double cost = curr_min.first;

            if(dGreater(cost , d[u]))  // Ignoring unnecessary previous entries
            {
                continue;
            }

            for(auto e: adj[u])
            {
                int v = e.first;
                double w = e.second;

                double newcost = cost + w;

                if(dLess(newcost , d[v]))
                {
                    d[v] = cost + w;
                    parent[v] = u;
                    pq.push({d[v],v});
                }
            }
        }
    }

    void bellmanford(int src)
    {
        hasNegativeCycle = 0;

        clearArrays();
        resetArrays(n);

        d[src] = 0;

        for(int i=0;i<n-1;i++)
        {
            for(edge e: E)
            {
                int u,v;
                double w;

                u = e.u;
                v = e.v;
                w = e.w;

                if(dLess(d[u] + w , d[v]))
                {
                    d[v] = d[u] + w;
                    parent[v] = u;
                }
            }
        }

        for(edge e: E)
        {
            int u,v;
            double w;

            u = e.u;
            v = e.v;
            w = e.w;

            if(dLess(d[u] + w , d[v]))
            {
                hasNegativeCycle = 1;
                break;
            }
        }
    }

    vector<int>get_path(int src, int dst)
    {
        vector<int>path;

        int now = dst;

        while(now != src)
        {
            path.push_back(now);
            now = parent[now];
        }

        if(src != dst)
            path.push_back(src);

        reverse(all(path));

        return path;
    }

};

void solve()
{
    //global initializations like "vector clear", "sum = 0"
    int n,m,p,q,maxm,minm,u,v,choice,src,dst;

    double w,ans = 0;

    cin>>n>>m;

    // Two graphs GB for Bellman-Ford, GD for Dijkstra
    DirectedGraph GB(n,m), GD(n,m);

    for(int i=0;i<m;i++)
    {
        cin>>u>>v>>w;
        GB.addEdge(u,v,w);
        GD.addEdge(u,v,fabs(w));
    }
    cin>>src>>dst;

    GB.bellmanford(src);

    cout<<"\nBellman Ford Algorithm:\n";
    if(GB.hasNegativeCycle)
    {
        cout<<"Negative weight cycle present\n";
    }
    else
    {
        cout<<GB.d[dst]<<br;
        auto path = GB.get_path(src,dst);

        for(int i=0;i<path.size()-1;i++)
        {
            cout<<path[i]<<" -> ";
        }
        cout<<path[path.size()-1]<<br;
    }

    GD.dijkstra(src);

    cout<<"\nDijkstra Algorithm:\n";
    cout<<GD.d[dst]<<br;

    auto path = GD.get_path(src,dst);

    for(int i=0;i<path.size()-1;i++)
    {
        cout<<path[i]<<" -> ";
    }
    cout<<path[path.size()-1]<<br;

}

void pre()
{

}

int32_t main() {

    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    solve();
}


