
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
const double eps = 1e-5;

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

double dMax(double a, double b)
{
    if(dGreater(a,b))
        return a;
    else
        return b;
}
double dMin(double a, double b)
{
    if(dLess(a,b))
        return a;
    else
        return b;
}

// return random integer in the range [L,R] inclusive

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int random(int L,int R) {
	return uniform_int_distribution<int>(L, R)(rng);
}

/*** Square Matrix Data Structure ***/

template<class T>
class Matrix{
public:
    int n, zero_indexed;
    vector<vector<T>>M;

    Matrix(int size, T def = 0)
    {
        n = size;
        M.resize(n+5,vector<T>(n+5,def));
        zero_indexed = 0;
    }
    void makeZeroIndexed()
    {
        zero_indexed = 1;
    }
    Matrix operator * (Matrix other)
    {
        assert(n == other.n);
        Matrix<T>ret(n,inf);
        int start,finish;
        if(zero_indexed)
        {
            start = 0; finish = n-1;
        }
        else
        {
            start = 1; finish = n;
        }
        for(int i=start;i<=finish;i++)
        {
            for(int j=start;j<=finish;j++)
            {
                for(int k=start;k<=finish;k++)
                {
                    ret.M[i][j] = dMin( ret.M[i][j], (M[i][k] + (other.M[k][j])));
                }
            }
        }
        return ret;
    }
    Matrix operator^(int k)
    {
        if(k==1)
            return *this;
        Matrix temp = (*this)^(k/2);
        temp = temp*temp;
        if(k%2 == 0)
        {
            return temp;
        }
        else
        {
            return temp*(*this);
        }
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
    vector<vector<double>>sp;  // All Pair Shortest Path

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

    void Matrix_Multiplication_APSP()
    {
        hasNegativeCycle = 0;

        sp.clear();
        sp.resize(n+5,vector<double>(n+5,inf));

        Matrix<double>dist(n);
        if(V[0] == 0)
        {
            dist.makeZeroIndexed();
        }
        for(int u: V)
        {
            for(int v: V)
            {
                dist.M[u][v] = inf;
            }
        }

        for(int u: V)
        {
            dist.M[u][u] = 0;
            for(auto e: adj[u])
            {
                int v = e.first;
                double w = e.second;

                dist.M[u][v] = w;
            }
        }

        dist = dist^(n);

        for(int u: V)
        {
            for(int v: V)
            {
                sp[u][v] = dist.M[u][v];
            }
        }

        for(int u: V)
        {
            if( dLess( sp[u][u] , 0 ) )
            {
                hasNegativeCycle = 1;
                break;
            }
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
        cin>>u>>v>>w;

        G.addEdge(u,v,w);
    }

    G.Matrix_Multiplication_APSP();

    if(G.hasNegativeCycle)
    {
        cout<<"Negative Cycle Detected\n";
        return;
    }

    cout<<"Shortest distance matrix"<<endl;

    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            double distance = G.sp[i][j];
            if(distance >= inf)
            {
                cout<<"INF"<<gap;
            }
            else
            {
                cout<<distance<<gap;
            }
        }
        cout<<br;
    }

}

int32_t main()
{
    //freopen("in.txt", "r", stdin);
    //freopen("out.txt", "w", stdout);
    solve();
}

/*

Sample Input:

4 6
1 2 8
1 4 1
2 3 1
3 1 4
4 2 2
4 3 9

*/



