
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

/*** Random Number Genertor ***/

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void initialize_random_seed(int seed)
{
	rng.seed(seed);
}

int random(int l,int r) {
	return uniform_int_distribution<int>(l, r)(rng);
}

/*** Random Word Generator ***/

string generate_random_word(int min_len, int max_len)
{

	int len = random(min_len, max_len);

	string word = "";

	for(int j=0; j<len; j++)
	{
		int id = random(0,25);
		char ch = 'a' + id;
		word += ch;
	}

	return word;
}

/*** Prime Sieve - O(nloglogn) ***/

vector<int>primes;

void prime_sieve(int n) // n = upper_bound
{
	primes.clear();

	vector<bool>is_prime(n+5,1);  // assume all numbers to be prime at first

	is_prime[1] = 0;  // 1 is neither prime nor composite

	for(int i=4;i<=n;i+=2) // even numbers except 2 are non-prime
	{
		is_prime[i] = 0;
	}

	int LIM = sqrt(n) + 5;

	for(int i=3; i<=n; i++)
	{
		if(is_prime[i])
		{
			if(i<=LIM)
			{
				for(int j=i*i ; j<=n ; j=j+2*i) // only examining odd numbers
				{
					is_prime[j] = 0;
				}
			}
		}
	}

	for(int i=1; i<=n; i++)
	{
		if(is_prime[i])
		{
			primes.push_back(i);
		}
	}

}

/*** Nearest Prime Generator from given Number - Binary Search ***/

int get_nearest_prime_up(int n)  // nearest greater or equal prime
{
	// Handling base small cases

	if(n<=2) return 2;
	if(n==3) return 3;
	if(n<=5) return 5;

	// Performing binary search on the primes vector

	int lo = 0, hi = primes.size()-1, mid;

	while(hi - lo >= 5)
	{
		mid = (lo+hi)/2;
		if(primes[mid] < n)
		{
			lo = mid;
		}
		else
		{
			hi = mid;
		}
	}

	int nearest_prime_up = -1;

	for(int i=lo; i<=hi; i++)
	{
		if(primes[i] >= n)
		{
			nearest_prime_up = primes[i];
			break;
		}
	}

	return nearest_prime_up;
}

/*** Node Class for Maintaining Linked List in Separate Chaining ***/

class Node{
public:
	string key;
	int value;
	Node*next;
	int status;
	Node(string K, int V, Node*nxt = NULL, int s = ACTIVE)
	{
		key = K;
		value = V;
		next = nxt;
		status = s;
	}
};

void delete_list(Node*head)
{
	Node*curr = head;
	while(head != NULL)
	{
		Node*temp_node = head;
		head = head->next;
		delete temp_node;
	}
}

/*** Prestored Primes for Easier Choice ***/

int small_primes[] = {29,31,37,43,47,53,59,61,67,71};
int big_primes[] = {999999929,999999937,1000000007,1000000009,1000000021,1000000033,1000000087,1000000103};

/*** Hash Class ***/

// Wrapper Class for hashing related functions

class Hash{

	int type;                       // type of hash function
	int prime_1, prime_2, prime_3;  // prime_1 < prime_2 < prime_3
	int mod;                        // will be equal to the table size

	int get_polynomial_hash(string s, int p, int m)
	{
		int n = s.length();
		int hash_value = 0;
		for(int i=0;i<n;i++)
		{
			char ch = s[i];
			hash_value = (hash_value*p + (ch - 'a'))%m;
		}
		return hash_value;
	}

	int get_bitwise_interleaved_hash(string s, int p, int m)
	{
		int n = s.length();
		int hash_value = p;
		for(int i=0;i<n;i++)
		{
			char ch = s[i];
			hash_value = (hash_value<<5)^(hash_value>>27)^ch;
		}
		while(hash_value<0)  // make negative values positive
		{
			hash_value = (hash_value + m)%m;
		}
		return hash_value%m;
	}

public:
	Hash()
	{
		mod = type = prime_1 = prime_2 = -1;
	}
	Hash(int m, int t = 1, int p1 = small_primes[4], int p2 = small_primes[9], int p3 = big_primes[2])  // m = table_size
	{
		if(!(t==1 || t==2))
		{
			t = 1; // default set to 1 in case of invalid request
		}
		this->mod = m;
		this->type = t;
		this->prime_1 = p1;
		this->prime_2 = p2;
		this->prime_3 = p3;
	}
	int get_hash(string s)   // O(n)
	{
		if(type == 1)
		{
			return get_polynomial_hash(s,prime_1,mod);  // passing small prime
		}
		else
		{
			return get_bitwise_interleaved_hash(s,prime_3,mod);  // passing big prime
		}
	}

	int get_aux_hash(string s) // O(1) // Can never be zero // prime_1 must be less than table_size
	{
		int n = s.length();
		int salt = prime_3*(s[0] - 'a') + s[n-1] - 'a';// getting a big number from first and last two letters
		return prime_2 - ( salt % prime_2 );   // ensuring that aux_hash < m
	}

};

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
void show(Entry e)
{
	cout<<"(\""<<e.key<<"\","<<e.value<<")";
}

/*** HashTable Class ***/

class HashTable{

	vector<list<Entry>>hash_table;         // Array of head pointers of linked lists
	int collision_resolution_method;
	int hash_function_type;
	int table_size;
	int key_count;
	int c1,c2;                       // constants for custom probing
	Hash hash;
	int collision_count;             // calculates collisions occured during insertion
	int probe_count;                 // calculates collisions occured during search

public:

	HashTable(int table_size, int collision_resolution_method, int hash_function_type, int c1 = small_primes[2], int c2 = small_primes[3]): hash_table(table_size), hash(table_size, hash_function_type)
	{
		this->collision_resolution_method = collision_resolution_method;
		this->hash_function_type = hash_function_type;
		this->table_size = table_size;
		this->c1 = c1;
		this->c2 = c2;
		key_count = 0;
		collision_count = 0;
		probe_count = 0;
	}
	~HashTable()
	{
		hash_table.clear();
	}

	void set_collision_count()
	{
		collision_count = 0;
	}
	int get_collision_count()
	{
		return collision_count;
	}
	
	void set_probe_count()
	{
		probe_count = 0;
	}
	int get_probe_count()
	{
		return probe_count;
	}

	void insert(string key, int value)
	{
		if(collision_resolution_method == SEPARATE_CHAINING)
		{
			int hash_value = hash.get_hash(key);

			int index = hash_value;

			auto it = hash_table[index].begin();

			while(it != hash_table[index].end())
			{
				collision_count++;
				if(it->key == key)    // The given key already exists
				{
					it->value = value;  // update the value
					return;
				}
				it++;
			}

			hash_table[index].push_front(Entry(key,value));
			key_count++;
		}
		else if(collision_resolution_method == DOUBLE_HASHING)
		{
			if(key_count < table_size)  // otherwise inserting is not possible
			{
				int hash_value = hash.get_hash(key);
				int aux_hash = hash.get_aux_hash(key);
				
				int index = hash_value;

				auto it = hash_table[index].begin();

				while(!hash_table[index].empty() && it->status != DELETED)
				{
					collision_count++;
					if(it->key == key)  // already exists
					{
						it->value = value;  // update the value
						return;
					}
					index = (index + aux_hash)%table_size;
					it = hash_table[index].begin();
				}

				hash_table[index].push_front(Entry(key,value));
				key_count++;

			}
		}
		else if(collision_resolution_method == CUSTOM_PROBING)
		{

			if(key_count < table_size)  // otherwise inserting is not possible
			{
				//cout<<key<<gap<<value<<br;
				int hash_value = hash.get_hash(key);
				int aux_hash = hash.get_aux_hash(key);
				
				int index = hash_value;

				int iter_count = 0;
				int offset = 1;

				auto it = hash_table[index].begin();

				while(!hash_table[index].empty() && it->status != DELETED && iter_count<table_size)
				{
					collision_count++;
					if(it->key == key)      // already exists
					{
						it->value = value;  // update the value
						return;
					}
					index = (index + c1*aux_hash + c2*offset)%table_size;
					offset+=2;
					iter_count++;
					it = hash_table[index].begin();
				}
				if(hash_table[index].empty() || it->status == DELETED)
				{
					hash_table[index].push_front(Entry(key,value));
					key_count++;
				}
			}
		}
	}
	int find(string key)
	{
		if(collision_resolution_method == SEPARATE_CHAINING)
		{
			int index = hash.get_hash(key);

			auto it = hash_table[index].begin();  

			while(it != hash_table[index].end())
			{
				probe_count++;
				if(it->key == key)  // The given key exists, return the value
				{
					return it->value;
				}
				it++;
			}

			return NOT_FOUND;  // element not found

		}
		else if(collision_resolution_method == DOUBLE_HASHING)
		{
			int hash_value = hash.get_hash(key);
			int aux_hash = hash.get_aux_hash(key);
			
			int index = hash_value;

			int iter_count = 0;

			auto it = hash_table[index].begin();

			while(!hash_table[index].empty() && iter_count < table_size)
			{
				probe_count++;
				if(it->key == key)  
				{
					if(it->status == DELETED)
					{
						return NOT_FOUND;  // element not found
					}
					else // key found, return the value
					{
						return it->value;
					}
				}
				index = (index + aux_hash)%table_size;
				iter_count++;
				it = hash_table[index].begin();
			}

			return NOT_FOUND;  // element not found
		}
		else if(collision_resolution_method == CUSTOM_PROBING)
		{
			int hash_value = hash.get_hash(key);
			int aux_hash = hash.get_aux_hash(key);
			
			int index = hash_value;

			int iter_count = 0;
			int offset = 1;

			auto it = hash_table[index].begin();

			while(!hash_table[index].empty() && iter_count < table_size)
			{
				probe_count++;
				if(it->key == key) 
				{
					if(it->status == DELETED)
					{
						return NOT_FOUND;  // element not found
					}
					else // key found, return the value
					{
						return it->value;
					}
				}
				index = (index + c1*aux_hash + c2*offset)%table_size;
				offset+=2;
				iter_count++;
				it = hash_table[index].begin();
			}

			return NOT_FOUND;  // element not found

		}

		return NOT_FOUND;  // element not found

	}
	void remove(string key)
	{
		if(collision_resolution_method == SEPARATE_CHAINING)
		{
			int index = hash.get_hash(key);

			auto it = hash_table[index].begin();

			if(it->key == key)
			{
				hash_table[index].erase(it);
				key_count--;
				return;
			}

			while(it != hash_table[index].end())
			{
				if(it->key == key)
				{
					hash_table[index].erase(it); // The given key exists, delete it
					key_count--;
					return;
				}
				it++;
			}

			// key not found
		}
		else if(collision_resolution_method == DOUBLE_HASHING)
		{
			int hash_value = hash.get_hash(key);
			int aux_hash = hash.get_aux_hash(key);
			
			int index = hash_value;

			int iter_count = 0;

			auto it = hash_table[index].begin();

			while(!hash_table[index].empty() && iter_count < table_size)
			{
				if(it->key == key)  
				{
					if(it->status == DELETED)
					{
						return;  // already deleted
					}
					else // key found, perform soft delete
					{
						it->status = DELETED;
						return;
					}
				}
				index = (index + aux_hash)%table_size;
				iter_count++;
				it = hash_table[index].begin();
			}

			// key not found
		}
		else if(collision_resolution_method == CUSTOM_PROBING)
		{
			int hash_value = hash.get_hash(key);
			int aux_hash = hash.get_aux_hash(key);
			
			int index = hash_value;

			int iter_count = 0;
			int offset = 1;

			auto it = hash_table[index].begin();

			while(!hash_table[index].empty() && iter_count < table_size)
			{
				if(it->key == key)  
				{
					if(it->status == DELETED)
					{
						return; // already deleted
					}
					else // key found, perform soft delete
					{
						it->status = DELETED;
						return;
					}
				}
				index = (index + c1*aux_hash + c2*offset)%table_size;
				offset+=2;
				iter_count++;  
				it = hash_table[index].begin();
			}

			// key not found

		}

		// key not found
	}

	void rehash(int new_table_size)
	{
		auto temp_hash_table = hash_table;

		hash_table.clear();
		hash_table.resize(new_table_size);

		table_size = new_table_size;
		key_count = 0;
		collision_count = 0;
		probe_count = 0;
		hash = Hash(table_size, hash_function_type);

		if(collision_resolution_method == SEPARATE_CHAINING)
		{
			for(auto list: temp_hash_table)
			{
				for(auto entry: list)
				{
					insert(entry.key, entry.value);
				}
			}
			temp_hash_table.clear();
		}
		else
		{
			for(auto list: temp_hash_table)
			{
				for(auto entry: list)
				{
					if(entry.status != DELETED)
					{
						insert(entry.key, entry.value);
					}
				}
			}
			temp_hash_table.clear();
		}
	}

	vector<Entry>get_all_entries()
	{
		vector<Entry>entries;
		for(auto L: hash_table)
		{
			for(auto entry: L)
			{
				if(entry.status != DELETED)
				{
					entries.push_back(entry);
				}
			}
		}
		return entries;
	}
};

// HashTable(int table_size, int collision_resolution_method, int hash_function_type, int c1 = small_primes[2], int c2 = small_primes[3]): hash_table(table_size), hash(table_size, hash_function_type)
/*** 2DHashTable ***/

class Two_Layered_HashTable{
	int N;
	vector<HashTable>child_tables;
	Hash hash;
public:
	Two_Layered_HashTable(int table_size = 100, int collision_resolution_method = SEPARATE_CHAINING, int hash_function_type = 1, int c1 = small_primes[2], int c2 = small_primes[3])
	{
		child_tables = vector<HashTable>(table_size,HashTable(table_size,collision_resolution_method,hash_function_type,c1,c2));
		hash = Hash(table_size, hash_function_type);
		N = table_size;
	}
	void insert(string key1, string key2, int value)
	{
		int hash_value1 = hash.get_hash(key1);
		child_tables[hash_value1].insert(key2,value);
	}
	int find(string key1, string key2)
	{
		int hash_value1 = hash.get_hash(key1);
		return child_tables[hash_value1].find(key2);
	}
	vector<Entry>find(string key1)
	{
		int hash_value1 = hash.get_hash(key1);
		return child_tables[hash_value1].get_all_entries();
	}

};

/*** Performance Analysis of Hash Functions ***/

double get_unique_hash_percentage(int hash_type, int table_size, vector<string>&unique_words)
{
	Hash hash(table_size,hash_type);
	int key_count = unique_words.size();

	set<int>unique_hash_values;

	for(int i=0;i<key_count;i++)
	{
		string s = unique_words[i];

		int hash_value = hash.get_hash(s);

		unique_hash_values.insert(hash_value);
	}

	int unique_hash_count = (int)(unique_hash_values.size());

	double unique_hash_ratio = (unique_hash_count*1.0)/(key_count);

	return 100.0*unique_hash_ratio;
}

void report_hash_function_performance(double load_factor, vector<string>&unique_words)
{
	int key_count = unique_words.size();
	int table_size = (int)(key_count/load_factor);
	for(int type = 1; type<=2; type++)
	{
		double d = get_unique_hash_percentage(type, table_size, unique_words);
		cout<<"Uniqueness Percentage for Hash Function "<<type<<":  "<<fixed<<setprecision(2)<<d<<" %"<<endl;
	}
}

/*** Gnerating n random unique words with the help of above HashTable class ***/

vector<string>generate_random_unique_words(int n, int min_len, int max_len)
{
	vector<string>unique_words;

	int table_size = get_nearest_prime_up(2*n); 
	HashTable HelperTable(table_size,SEPARATE_CHAINING,1);
	int unique_word_count = 0;

	while(unique_word_count < n)
	{
		string word = generate_random_word(min_len, max_len);
		if(HelperTable.find(word) == NOT_FOUND)  // so this can be inserted
		{
			unique_words.push_back(word);
			HelperTable.insert(word,unique_word_count+1);
			unique_word_count++;
		}
	}

	return unique_words;
}

void solve()
{
	// Output Formatting

	int width = 10;
	int hyphens = 82;

	initialize_random_seed(seed_1);

	int n,m,p,q,u,v,x,y,ans;

	int N = 100;

	Two_Layered_HashTable H(get_nearest_prime_up(N));

	H.insert("abc","def",1);
	H.insert("abc","xyz",2);

	cout<<H.find("abc","def")<<br;

	auto entries = H.find("abc");

	cout<<"[";
	m = entries.size();
	int i = 0;
	for(auto entry: entries)
	{
		show(entry);
		if(i < m-1) cout<<",";
		i++;
	}
	cout<<"]"<<endl;


}

void precalculate()
{
	int MAXN = 1000000;
	prime_sieve(MAXN);
}

int32_t main()
{
	//freopen("in.txt","r",stdin);
	//freopen("out.txt","w",stdout);
	precalculate();
	solve();
	return 0;
}