
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

/*** Seeds for Reproducibility ***/

const int seed_1 = 10000019;

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

void initialize_random_seed(int seed)
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

    void print_graph()
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


};


