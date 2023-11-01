
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

int get_nearest_prime_down(int n)  // nearest greater or equal prime
{
	// Handling base small cases

	if(n==2) return 2;
	if(n<=4) return 3;
	if(n<=6) return 5;

	// Performing binary search on the primes vector

	int lo = 0, hi = primes.size()-1, mid;

	while(hi - lo >= 5)
	{
		mid = (lo+hi)/2;
		if(primes[mid] <= n)
		{
			lo = mid;
		}
		else
		{
			hi = mid;
		}
	}

	int nearest_prime_down = -1;

	for(int i=hi; i>=lo; i--)
	{
		if(primes[i] <= n)
		{
			nearest_prime_down = primes[i];
			break;
		}
	}

	return nearest_prime_down;
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

/*** HashTable Class ***/

class HashTable{

	vector<Node*>hash_table;         // Array of head pointers of linked lists
	int collision_resolution_method;
	int hash_function_type;
	int table_size;
	int key_count;
	int c1,c2;                       // constants for custom probing
	Hash hash;
	int collision_count;             // calculates collisions occured during insertion
	int probe_count;                 // calculates collisions occured during search

	void add_node(int index, string key, int value)
	{
		Node*new_node = new Node(key,value,hash_table[index]);
		hash_table[index] = new_node;
	}
	void destroy_hash_table(vector<Node*>&table)
	{
		for(auto head: table)
		{
			delete_list(head);
		}
		table.clear();
	}
public:

	HashTable(int table_size, int collision_resolution_method, int hash_function_type, int c1 = small_primes[2], int c2 = small_primes[3]): hash_table(table_size, NULL), hash(table_size, hash_function_type)
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
		destroy_hash_table(hash_table);
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

	int get_key_count()
	{
		return key_count;
	}

	void insert(string key, int value)
	{
		if(collision_resolution_method == SEPARATE_CHAINING)
		{
			int hash_value = hash.get_hash(key);

			int index = hash_value;

			Node*curr = hash_table[index];

			while(curr != NULL)
			{
				collision_count++;
				if(curr->key == key)  // The given key already exists
				{
					curr->value = value;  // update the value
					return;
				}
				curr = curr->next;
			}

			add_node(index,key,value);
			key_count++;
		}
		else if(collision_resolution_method == DOUBLE_HASHING)
		{
			if(key_count < table_size)  // otherwise inserting is not possible
			{
				int hash_value = hash.get_hash(key);
				int aux_hash = hash.get_aux_hash(key);
				
				int index = hash_value;

				while(hash_table[index]!=NULL && hash_table[index]->status != DELETED)
				{
					collision_count++;
					if(hash_table[index]->key == key)  // already exists
					{
						hash_table[index]->value = value;  // update the value
						return;
					}
					index = (index + aux_hash)%table_size;
				}

				add_node(index,key,value);
				key_count++;

			}
		}
		else if(collision_resolution_method == CUSTOM_PROBING)
		{
			if(key_count < table_size)  // otherwise inserting is not possible
			{

				int hash_value = hash.get_hash(key);
				int aux_hash = hash.get_aux_hash(key);
				
				int index = hash_value;

				int iter_count = 0;
				int offset = 1;
				while(hash_table[index]!=NULL && hash_table[index]->status != DELETED && iter_count<table_size)
				{
					collision_count++;
					if(hash_table[index]->key == key)      // already exists
					{
						hash_table[index]->value = value;  // update the value
						return;
					}
					index = (index + c1*aux_hash + c2*offset)%table_size;
					offset+=2;
					iter_count++;
				}
				if(hash_table[index] == NULL || hash_table[index]->status == DELETED)
				{
					add_node(index,key,value);
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

			Node*curr = hash_table[index];

			while(curr != NULL)
			{
				probe_count++;
				if(curr->key == key)  // The given key exists, return the value
				{
					return curr->value;
				}
				curr = curr->next;
			}

			return NOT_FOUND;  // element not found

		}
		else if(collision_resolution_method == DOUBLE_HASHING)
		{
			int hash_value = hash.get_hash(key);
			int aux_hash = hash.get_aux_hash(key);
			
			int index = hash_value;

			int iter_count = 0;

			while(hash_table[index]!=NULL && iter_count < table_size)
			{
				probe_count++;
				if(hash_table[index]->key == key)  
				{
					if(hash_table[index]->status == DELETED)
					{
						return NOT_FOUND;  // element not found
					}
					else // key found, return the value
					{
						return hash_table[index]->value;
					}
				}
				index = (index + aux_hash)%table_size;
				iter_count++;
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

			while(hash_table[index]!=NULL && iter_count < table_size)
			{
				probe_count++;
				if(hash_table[index]->key == key) 
				{
					if(hash_table[index]->status == DELETED)
					{
						return NOT_FOUND;  // element not found
					}
					else // key found, return the value
					{
						return hash_table[index]->value;
					}
				}
				index = (index + c1*aux_hash + c2*offset)%table_size;
				offset+=2;
				iter_count++;
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

			Node*curr = hash_table[index];

			if(curr->key == key)
			{
				Node*temp_node = curr;
				hash_table[index] = curr->next;
				delete temp_node;
				key_count--;
				return;
			}

			while(curr->next != NULL)
			{
				if(curr->next->key == key)  // The given key exists, delete it
				{
					Node*temp_node = curr->next;
					curr->next = curr->next->next;
					delete temp_node;
					key_count--;
					return;
				}
				curr = curr->next;
			}

			// key not found
		}
		else if(collision_resolution_method == DOUBLE_HASHING)
		{
			int hash_value = hash.get_hash(key);
			int aux_hash = hash.get_aux_hash(key);
			
			int index = hash_value;

			int iter_count = 0;

			while(hash_table[index]!=NULL && iter_count < table_size)
			{
				if(hash_table[index]->key == key)  
				{
					if(hash_table[index]->status == DELETED)
					{
						return;  // already deleted
					}
					else // key found, perform soft delete
					{
						hash_table[index]->status = DELETED;
						return;
					}
				}
				index = (index + aux_hash)%table_size;
				iter_count++;
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

			while(hash_table[index]!=NULL && iter_count < table_size)
			{
				if(hash_table[index]->key == key)  
				{
					if(hash_table[index]->status == DELETED)
					{
						return; // already deleted
					}
					else // key found, perform soft delete
					{
						hash_table[index]->status = DELETED;
						return;
					}
				}
				index = (index + c1*aux_hash + c2*offset)%table_size;
				offset+=2;
				iter_count++;  
			}

			// key not found

		}

		// key not found
	}
	void rehash()
	{
		int new_table_size = get_nearest_prime_up(2*table_size);

		vector<Node*>temp_hash_table = hash_table;

		hash_table.clear();

		for(int i=0;i<new_table_size;i++)
		{
			hash_table.push_back(NULL);
		}

		table_size = new_table_size;
		key_count = 0;
		collision_count = 0;
		probe_count = 0;
		hash = Hash(table_size, hash_function_type);

		if(collision_resolution_method == SEPARATE_CHAINING)
		{
			for(Node*head: temp_hash_table)
			{
				Node*curr = head;
				while(curr != NULL)
				{
					insert(curr->key,curr->value);
					curr = curr->next;
				}
			}
			destroy_hash_table(temp_hash_table); // Freeing the memory
		}
		else
		{
			for(Node*curr: temp_hash_table)
			{
				if(curr != NULL && curr->status != DELETED)
				{
					insert(curr->key,curr->value);
				}
			}
			destroy_hash_table(temp_hash_table); // Freeing the memory
			cout<<"Done"<<endl;
		}
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
	initialize_random_seed(seed_1);

	int n,m,p,q,u,v,x,y,ans;

	int methods[] = { SEPARATE_CHAINING, DOUBLE_HASHING, CUSTOM_PROBING };

	n = 10000;

	int table_size = get_nearest_prime_up(n);

	int unique_word_count = 10000;  // default

	int search_count = 1000;  // default

	auto unique_words = generate_random_unique_words(unique_word_count,5,10);

	HashTable H(table_size,SEPARATE_CHAINING,1);
	int i = 1;
	for(auto word: unique_words)
	{
		H.insert(word,i);
		i++;
	}

	for(int i=0; i<search_count; i++)
	{
		int word_index = random(0,min(table_size-1,unique_word_count-1));
		string word = unique_words[word_index];
		int value = H.find(word);
	}

	double avg_probes = (H.get_probe_count()*1.0)/search_count;

	cout<<fixed<<setprecision(4)<<avg_probes<<br;

	H.rehash();

	for(int i=0; i<search_count; i++)
	{
		int word_index = random(0,min(table_size-1,unique_word_count-1));
		string word = unique_words[word_index];
		int value = H.find(word);
	}

	avg_probes = (H.get_probe_count()*1.0)/search_count;

	cout<<fixed<<setprecision(4)<<avg_probes<<br;

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