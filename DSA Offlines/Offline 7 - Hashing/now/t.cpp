
#include <bits/stdc++.h>
using namespace std;

/*** Definitions ***/

#define gap " "
#define br "\n"
#define pb push_back
#define all(a) a.begin(),a.end()
#define int long long int
#define fo(i,n) for(int i=0;i<n;i++)
#define pii pair<int,int>
#define double long double
#define print(x) cout<< #x <<" = "<< x<<endl

/*** Enumerations ***/

enum status_values : int{
	DELETED = -101,
	ACTIVE = -102,
	NOT_FOUND = -103
};

enum collision_resolution_methods : int{
	SEPARATE_CHAINING = -201,
	DOUBLE_HASHING = -202,
	CUSTOM_PROBING = -203,
	LINEAR_PROBING = -204
};

/*** Constants ***/

const int inf = LLONG_MAX;

/*** Seeds for Reproducibility ***/

const int seed_1 = 10000019;
const int seed_2 = 10000379;
const int seed_3 = 10010111;

class Entry{
public:
	string key;
	int value;
	int status;
	Entry(string K = "", int V = 0, int s = ACTIVE)
	{
		this->key = K;
		this->value = V;
		this->status = ACTIVE;
	}
};

int32_t main()
{
	vector<list<Entry>>hash_table(10);

	hash_table[3].push_back(Entry("a",1));
	hash_table[3].push_back(Entry("b",2));

	auto it = hash_table[3].begin();

	it = hash_table[3].begin();

	cout<<it->key<<gap<<it->value<<br;

	hash_table.clear();
	hash_table.resize(50);

	hash_table[45].push_back(Entry("abc",123));
	hash_table[45].push_back(Entry("abc",123));

	it = hash_table[45].begin();

	cout<<it->key<<gap<<it->value<<br;

	cout<<hash_table[45].size()<<br;



	return 0;
}