
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
#define pii pair<int,int>
#define isSet(n, i) bool(n & (1LL << i))
#define setBit(n, i) (n | (1LL << i))
#define unsetBit(n, i) (n & ~(1LL << i))
#define print(x) cout<< #x <<" = "<< x<<endl
#define fo(i,n) for(int i=0;i<n;i++)

/*** Constants ***/

const double inf = 1.0*(INT_MAX - 100);
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

void initialize_seed(int seed)
{
    rng.seed(seed);
}

int get_random_int(int L,int R) {
	return uniform_int_distribution<int>(L, R)(rng);
}

double get_random_double(double a, double b)
{
    return uniform_real_distribution<double>(a,b)(rng);
}

/*** Calculate Euclidean Distance ***/

double dist(double ax, double ay, double bx, double by)
{
    double dx = (ax-bx);
    double dy = (ay-by);
    return sqrt(dx*dx + dy*dy);
}

int Log(int n, int r)    // returns r base log n
{
    return (n > r - 1) ? 1 +
       Log(n / r, r) : 0;
}

/*** Count number of bits ***/

int countBits(int n)
{
    return Log(n,2)+1;
}

/*** int to Binary string ***/

string bin(int n, int digits = -1)
{
    int cnt = countBits(n);
    string ans = "";
    while(n>0)
    {
        int r = n%2;
        ans += ('0'+r);
        n/=2;
    }

    if(digits != -1)
    {
        fo(i,(digits - cnt))
        {
            ans += '0';
        }
    }

    reverse(all(ans));
    return ans;
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

/*** Tour Class ***/

class Tour{
public:
    vector<int>path;
    Tour()
    {
        path.clear();
    }
    void add(int v)
    {
        path.push_back(v);
    }
    void rev()
    {
        reverse(all(path));
    }
    void print_tour()
    {
        for(int u: path)
        {
            cout<<u<<gap;
        }
        cout<<endl;
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
        for(int u: V)
        {
            mat[u][u] = 0;
        }
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

    // Works only when edge weights are reflexive as it is the case in Metric properties
    vector<edge>MST_Kruskal_for_Undirected_Version()  // O(m*log(m))
    {
        vector<edge>EU,MST;

        // transforming E into EU

        for(auto e: E)
        {
            if(e.u < e.v)  // Assuming no multi-edge or self-loop
            {
                EU.push_back(e);
            }
        }

        sort(EU.begin(),EU.end());

        UnionFind uf(n);

        for(auto e: EU)
        {
            int u,v;
            double w;

            u = e.u;
            v = e.v;
            w = e.w;

            if(!uf.isUnited(u,v))
            {
                MST.push_back(e);
                uf.unite(u,v);
                if(MST.size() == n-1)
                {
                    break;
                }
            }
        }
        if(MST.size() != n-1) // Checking whether a valid tree has been formed
        {
            MST.clear();
            return MST; // return empty vector if not possible
        }
        return MST;
    }

    double Calculate_Tour_Length(Tour T)
    {
        double cost = 0.0;
        int len = T.path.size();
        for(int i=0;i<len-1;i++)
        {
            cost += mat[T.path[i]][T.path[i+1]];
        }
        return cost;
    }

    void print_node_and_edge_list()
    {
        cout<<"Nodes: ";
        for(int u: V)
        {
            cout<<u<<" ";
        }
        cout<<endl;
        cout<<"\nEdges:\n\n";
        for(auto e: E)
        {
            cout<<e.u<<gap<<e.v<<gap<<fixed<<setprecision(4)<<e.w<<endl;
        }
        cout<<endl<<endl;
    }

    void print_adj_matrix()
    {
        cout<<"\nAdjacency Matrix:\n\n";
        for(int u: V)
        {
            for(int v: V)
            {
                cout<<setw(8)<<fixed<<setprecision(2)<<mat[u][v]<<gap;;
            }
            cout<<br;
        }
        cout<<br<<br;
    }


};

// Create random graph obeying the Metric properties
DirectedGraph Create_Random_Graph(int V)
{
    int m = (V*(V-1))/2;
    DirectedGraph G(V,m);

    for(int i=1;i<=V;i++)
    {
        for(int j=1;j<=V;j++)
        {
            if(j<=i) continue;

            // It can be proven that  
            // taking edges in the range [a,b] where 2a > b 
            // will always satisfy the metric properties
            //
            // Proof:
            //
            // The minimum sum of weights can be = 2a
            // The maximum weight = b
            // So, if 2a > b, the sum of no two edges can be less than any other edge.
            
            double d = get_random_double(75.0,145.0); 

            G.addEdge(i,j,d);
            G.addEdge(j,i,d);
        }
    }

    G.constructAdjMatrix();

    return G;


}

Tour Exact_TSP(DirectedGraph G)
{
    int n = G.n;

    // dp[i][mask] = minimum cost to go from 1 to i traversing through the nodes denoted by mask
    // Recursion: dp[i][mask] = min over all set vertices j in mask except 1( dp[j][unset(mask,j)]+d[j][i] )

    // penultimate_vertex = the second last vertex in the min-cost path from 1 to i visiting the nodes denoted by mask

    vector<vector<double>>dp(n+1,vector<double>(1<<(n+1),inf));
    vector<vector<int>>penultimate_vertex(n+1,vector<int>(1<<(n+1),-1));

    for(int mask=0;mask<(1<<(n+1));mask++)
    {
        for(int i=2;i<=n;i++)
        {
            if(isSet(mask,0) || (!isSet(mask,1)) || (!isSet(mask,i)))
            {
                continue; // not possible
            }
            if(mask == ( (1<<1) | (1<<i) ))
            {
                dp[i][mask] = G.mat[1][i];
            }
            for(int j=2; j<=n; j++)  // choosing the penultimate vertex j
            {
                if(j == i) continue;
                if(isSet(mask,j))
                {
                    double candidate = dp[j][unsetBit(mask,i)] + G.mat[j][i];
                    if(candidate < dp[i][mask])
                    {
                        dp[i][mask] = candidate;
                        penultimate_vertex[i][mask] = j;
                    }
                }
            }

            //cout<<i<<gap<<bin(mask,5)<<gap<<dp[i][mask]<<endl;

        }
    }

    // a mask indicating visiting all vertices from 1 to n but exclusing 0, since 0 does not exist
    int mask_all_visit = unsetBit((1<<(n+1))-1,0);

    int last_vertex = -1;

    double min_cost = inf;

    for(int u=2;u<=n;u++) // choosing optimal last vertex so that the cost of the cycle is minimized
    {
        double candidate = dp[u][mask_all_visit] + G.mat[u][1] ;
        if(candidate < min_cost)
        {
            min_cost = candidate;
            last_vertex = u;
        }
    }

    Tour T;

    int added_count = 0;

    int current_vertex = last_vertex;
    int current_mask = mask_all_visit;

    while(current_vertex != -1)
    {
        T.add(current_vertex);
        int v = penultimate_vertex[current_vertex][current_mask];
        current_mask = unsetBit(current_mask,current_vertex);
        current_vertex = v;
    }

    T.add(1);

    T.rev();

    T.add(1); // completing the cycle by returning to 1

    return T;

}

void run_dfs_on_tree(int u, int p, vector<vector<int>>&adj, Tour&T)
{
    T.add(u);          // Each vertex is added upon discovery, thus removing duplicate vertices
    for(int v: adj[u])
    {
        if(v^p)
        {
            run_dfs_on_tree(v,u,adj,T);
        }
    }
}

Tour Metric_Approximation_TSP(DirectedGraph G)
{
    int n = G.n;
    vector<vector<int>>adj(n+1,vector<int>());
    vector<edge>MST = G.MST_Kruskal_for_Undirected_Version();
    
    // Form adjacency list for undirected tree
    for(edge e: MST)
    {
        adj[e.u].push_back(e.v);
        adj[e.v].push_back(e.u);
    }

    Tour T;
    run_dfs_on_tree(1,0,adj,T); // Perform Euler Tour
    T.add(1);  // to complete the cycle, add the first vertex again
    return T;
}


void solve()
{
    initialize_seed(2005001);

    //global initializations like "vector clear", "sum = 0"
    int n,m,p,q,maxm,minm,u,v,choice,src,dst;

    double w,ans = 0;

    int V = 20;

    int X;

    cout<<"\nEnter the number of runs: ";

    cin>>X;

    cout<<"\n\nApproximation Ratios:\n\n";

    for(int x=1;x<=X;x++)
    {

        auto G = Create_Random_Graph(V);

        //G.print_adj_matrix();

        Tour exact_tour = Exact_TSP(G);
        double exact_tour_length = G.Calculate_Tour_Length(exact_tour);

        // cout<<"Exact Tour : ";
        // exact_tour.print_tour();
        // cout<<"Exact Tour length : "<<exact_tour_length<<"\n\n";

        Tour approx_tour = Metric_Approximation_TSP(G);
        double approx_tour_length = G.Calculate_Tour_Length(approx_tour);

        // cout<<"Approx Tour: ";
        // approx_tour.print_tour();
        // cout<<"Approx Tour length: "<<approx_tour_length<<"\n\n";

        double ratio = approx_tour_length/exact_tour_length;

        cout<<"( "<<x<<" , "<<fixed<<setprecision(6)<<ratio<<" )"<<endl;
    }


}

void pre()
{

}

int32_t main() {

    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    pre();
    solve();
}


