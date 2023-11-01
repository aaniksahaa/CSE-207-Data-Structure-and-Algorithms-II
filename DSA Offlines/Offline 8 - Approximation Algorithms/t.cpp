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

/*** Seeds for Reproducibility ***/

const int seed_1 = 10000019;

// return random integer in the range [L,R] inclusive

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void initialize_random_seed(int seed)
{
	rng.seed(seed);
}

double get_random_double(double a, double b)
{
	return uniform_real_distribution<double>(a,b)(rng);
}

int32_t main()
{
	initialize_random_seed(seed_1);
	for(int i=0;i<100;i++)
	{
		cout<<get_random_double(-1.0,1.0)<<endl;
	}
}