
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
#define fo(i,n) for(int i=0;i<n;i++)
#define pii pair<int,int>

/*** Constants ***/

const int inf = LLONG_MAX - 100;

// return random integer in the range [L,R] inclusive

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int random(int L,int R) {
	return uniform_int_distribution<int>(L, R)(rng);
}

// Helper Functions

void show(vector<int>&a)
{
    cout<<endl;
    for(int u: a) cout<<u<<" ";
    cout<<endl<<endl;
}

void print_path(vector<int>&path)
{
    int m = path.size();
    fo(i,m-1)
    {
        cout<<path[i]<<" -> ";
    }
    cout<<path[m-1]<<endl;
}

/*** Edge Data Structure ***/

class edge{
public:
    int u,v;
    int w;

    void show()
    {
        cout<<"u = "<<u<<", v = "<<v<<"  ->  w = "<<w<<endl;
    }
};
bool operator<(edge p, edge q)
{
    return p.w < q.w;
}

/*** Directed Graph Data Structure ***/

class DirectedGraph{
public:
    int n, m;
    int time;
    int hasCycle, hasNegativeCycle;
    vector<int>d;
    vector<int>parent,vis,color,start,finish;
    vector<int>V;                         // Vertices
    vector<vector<pair<int,int>>>adj;  // Adjacency List
    vector<vector<int>>mat;            // Adjacency Matrix
    vector<edge>E;                        // List of Undirected Edges
    vector<edge>C;
    vector<vector<int>>residual_capacity;

    void clearAll()
    {
        clearArrays();
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
        clearArrays();
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
        adj.resize(n+5,vector<pair<int,int>>());

        setVerticesDefault(n);
    }

    // Just initializes residual_capacity matrix 
    void init_flow_network(int n)
    {
        residual_capacity.clear();
        residual_capacity.resize(n+5,vector<int>(n+5,0));
    }

    void addEdge(int u, int v, int w)
    {
        E.pb({u,v,w});

        adj[u].pb({v,w});
    }

    void addCapacity(int u, int v, int cap)
    {
        C.push_back({u,v,cap});

        adj[u].pb({v,cap});
        adj[v].pb({u,0});

        residual_capacity[u][v] = cap;
        residual_capacity[v][u] = 0;
    }

    // cap1 for u=>v and cap2 for v=>u
    void addBidirectionalCapacity(int u, int v, int cap1, int cap2)
    {
        C.push_back({u,v,cap1});
        C.push_back({v,u,cap2});

        adj[u].pb({v,cap1});
        adj[v].pb({u,cap2});

        residual_capacity[u][v] = cap1;
        residual_capacity[v][u] = cap2;
    }

    void constructAdjMatrix()  // O(n^2) Memory needed
    {
        mat.clear();
        mat.resize(n+5,vector<int>(n+5,inf));
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

    vector<int> find_augmenting_path_by_BFS(int s, int t)
    {
        resetArrays(n);
        vector<int>path;

        bool done = 0;

        queue<int>Q;
        vis[s] = 1;
        Q.push(s);
        while(!Q.empty())
        {
            int u = Q.front();
            Q.pop();

            for(auto e: adj[u])
            {
                int v = e.first;

                if(!vis[v] && residual_capacity[u][v] > 0)
                {
                    vis[v] = 1;
                    Q.push(v);
                    parent[v] = u;
                    if(v == t)
                    {
                        done = 1;
                        break;
                    }
                }
            }

            if(done)
            {
                break;
            }
        }

        if(!done)
        {
            return path; // returning an empty vector
        }

        int now = t;
        while(now != s)
        {
            path.pb(now);
            now = parent[now];
        }
        path.pb(s);

        reverse(all(path));

        return path;
    }

    int max_flow_edmond_karp(int s, int t)
    {
        int flow = 0;

        while(1)
        {

            vector<int>path = find_augmenting_path_by_BFS(s,t);

            if(path.size() == 0)
            {
                break;
            }

            int minimum_residual_capacity = inf ;

            int m = path.size();

            fo(i,m-1)
            {
                int u = path[i];
                int v = path[i+1];
                minimum_residual_capacity = min(minimum_residual_capacity, residual_capacity[u][v]);
            }

            int possible_new_flow = minimum_residual_capacity;
            
            // cout<<"Augmenting Path: ";
            // print_path(path);
            // cout<<"Added Flow = "<<possible_new_flow<<endl;

            flow += possible_new_flow;

            fo(i,m-1)
            {
                int u = path[i];
                int v = path[i+1];

                residual_capacity[u][v] -= possible_new_flow;
                residual_capacity[v][u] += possible_new_flow;
            }
        }

        return flow;

    }

    // max-flow must have been called before
    vector<vector<int>> find_min_cut(int s, int t)
    {
        resetArrays(n);

        vector<vector<int>>cut(2,vector<int>());

        queue<int>Q;
        vis[s] = 1;
        Q.push(s);
        while(!Q.empty())
        {
            int u = Q.front();
            Q.pop();

            cut[0].push_back(u);  // cut[0] means the S part pf the S-T cut

            for(auto e: adj[u])
            {
                int v = e.first;

                if(!vis[v] && residual_capacity[u][v] > 0)
                {
                    vis[v] = 1;
                    Q.push(v);
                    parent[v] = u;
                }
            }
        }

        for(int u: V)
        {
            if(!vis[u]) cut[1].push_back(u);
        }

        return cut;
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

    map<pii,int>capacity;

    int w,cap,ans = 0;

    cin>>n;

    vector<int>W(n+1),L(n+1),R(n+1);
    vector<vector<int>>g(n+1,vector<int>(n+1));
    vector<vector<pair<int,int>>>flow_network;
    vector<string>teams(n+1);

    for(int u=0;u<n;u++)
    {
        cin>>teams[u]>>W[u]>>L[u]>>R[u];

        for(int v=0;v<n;v++)
        {
            cin>>g[u][v];
        }
    }

    // For debugging

    /*
    cout<<"Given Matrix: "<<endl;
    for(int u=0;u<n;u++)
    {
        cout<<teams[u].substr(0,1)<<" "<<W[u]<<" "<<L[u]<<" "<<R[u]<<" ";

        for(int v=0;v<n;v++)
        {
            cout<<g[u][v]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    */


    for(int focus = 0; focus < n; focus++)
    {

        flow_network.clear();

        map<pair<int,int>,int>id;
        vector<pair<int,int>>temp;
        int cnt = 0; // node count in flow network

        for(int u=0;u<n;u++)
        {
            temp.clear();
            flow_network.push_back(temp); // creating vertices for teams
            cnt++;
        }

        for(int u=0;u<n;u++)
        {
            for(int v = u+1;v<n;v++)
            {
                // creating vertices for games
                temp.clear();
                temp.push_back({u,inf});
                temp.push_back({v,inf});
                flow_network.push_back(temp);
                id[{u,v}] = cnt;
                cnt++;
            }
        }

        int s,t;

        s = cnt;

        temp.clear(); // adjacencies for start vertex

        for(int u=0;u<n;u++)
        {
            for(int v = u+1;v<n;v++)
            {
                if(u == focus || v == focus) continue;
                temp.push_back({ id[{u,v}] , g[u][v]});
            }
        }

        flow_network.push_back(temp);

        cnt++;

        t = cnt;

        temp.clear();
        flow_network.push_back(temp);

        for(int u =0; u<n; u++)
        {
            if(u == focus) continue;
            flow_network[u].push_back({ t , W[focus] + R[focus] - W[u] });
        }

        cnt++;
        
        /*
        int u = 0;
        for(auto v: flow_network)
        {
            for(auto p: v)
            {
                cout<<u<<" "<<p.first<<" "<<p.second<<endl;
            }
            u++;
        }
        */
        

        m = 0;
        for(auto v: flow_network) m += v.size();

        DirectedGraph G(cnt+1,m+1);

        G.init_flow_network(cnt+1);

        for(int u=0;u<cnt;u++)
        {
            for(auto e: flow_network[u])
            {
                int v = e.first;
                int w = e.second;

                G.addCapacity(u,v,w);
            }
        }

        int flow = G.max_flow_edmond_karp(s,t);
        //cout<<"Flow = "<<flow<<endl;

        //cout<<"Focus = "<<focus<<endl;
        vector<vector<int>>cut = G.find_min_cut(s,t);

        vector<int>wanted_group;

        bool done = 0;

        for(int u=0;u<n;u++)
        {
            if(W[u] > W[focus] + R[focus]) // Trivially done
            {
                wanted_group.push_back(u);
                done = 1;
                break;
            }
        }

        if(!done)
        {
            for(int u: cut[0])
            {
                if(u>=0 && u<n) // checking if team vertex
                {
                    wanted_group.push_back(u);
                }
            }
        }

        if(wanted_group.size() != 0)
        {

            cout<<teams[focus]<<" is eliminated."<<endl;

            cout<<"They can win at most "<<W[focus]<< " + "<<R[focus]<<" = "<<W[focus] + R[focus]<<" games."<<endl;
            
            int cnt = wanted_group.size();

            int total_win = 0;
            for(auto u: wanted_group) total_win += W[u];
            for(int i=0;i<cnt-2;i++)
            {
                cout<<teams[i]<<", ";
            }

            if(cnt > 1)
            {
                cout<<teams[cnt-2]<<" and ";
            }

            cout<<teams[cnt-1]<<" have won a total of "<<total_win<<" games."<<endl;

            int play_each_other = 0;

            for(auto u: wanted_group)
            {
                for(auto v: wanted_group)
                {
                    if(u<v) // avoiding double count
                    {
                        play_each_other += g[u][v];
                    }
                }
            }

            cout<<"They play each other "<<play_each_other<<" times."<<endl;

            double max_win_on_average = ((total_win + play_each_other)*1.0)/cnt;

            cout<<"So on average, each of the teams in this group wins "<<total_win + play_each_other<<"/"<<cnt<<" = "<<max_win_on_average<<" games."<<endl;

            cout<<endl;

        }

    }




}

int32_t main()
{
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    solve();
}

/*

Test Cases

4
Atlanta 83 71 8 0 1 6 1
Philadelphia 80 79 3 1 0 0 2
New_York 78 78 6 6 0 0 0
Montreal 77 82 3 1 2 0 0

5
New_York 75 59 28 0 3 8 7 3
Baltimore 71 63 28 3 0 2 7 4
Boston 69 66 27 8 2 0 0 0
Toronto 63 72 27 7 7 0 0 0
Detroit 49 86 27 3 4 0 0 0


*/



