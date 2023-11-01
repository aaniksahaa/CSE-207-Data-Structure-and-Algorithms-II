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
vector<Edge>E;

vector<Edge>MST_Kruskal()
{
    vector<Edge>MST;

    UnionFind uf(n);

    sort(E.begin(),E.end());  // sorting edges in increasing order

    for(Edge e: E)
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
            if(MST.size() == n)
            {
                break;
            }
        }
    }
    if(MST.size() != n) // Checking whether a valid tree has been formed
    {
        MST.clear();
        return MST; // return empty vector if not possible
    }
    return MST;
}

void solve(int tc)
{
    E.clear();
    int u,v;
    double w;

    double p,q;
    p = q = 0;

    cin>>n;

    while(1)
    {
        cin>>u>>v>>w;

        if(u==0 && v == 0 && w == 0)
        {
            break;
        }

        E.push_back({min(u,v),max(u,v),w});
    }

    auto MST = MST_Kruskal();
    for(auto e: MST)
    {
        p += e.w;
    }
    for(int i=0;i<E.size();i++)
    {
        E[i].w = -E[i].w;
    }
//    for(auto e: E)
//        e.show();
    MST = MST_Kruskal();
    for(auto e: MST)
    {
        q += e.w;
    }
    q = -q;
    //cout<<q<<endl;
    cout<<"Case "<<tc<<": ";
    if((int)(p+q)%2 == 0)
        cout<<(p+q)/2<<"\n";
    else
    {
        cout<<p+q<<"/"<<2<<"\n";
    }

}

int32_t main()
{
    //freopen("in.txt", "r", stdin);
    //freopen("out.txt", "w", stdout);
    int t;
    cin>>t;
    for(int i=1;i<=t;i++)
    {
        solve(i);
    }
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

