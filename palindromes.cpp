#include <iostream>
#include <cstring>
#include <cmath>

#define BASE 256
#define LIMIT 25000

using namespace std;

long long int lower_prime_limit = 1000000000; // B
long long int upper_prime_limit = 2000000000; // 2*B

// pointers to hash arrays
long long int *hash_value, *reverse_hash, *base_value;

long long int prime, string_length;

// check if a number is prime
bool is_prime(long long int p){
	if(!(p%2)) 
		return false;
	long long int sq_root = (long long int)(pow(p, 1.0/2.0));
	for(int i = 3; i <= sq_root; i += 2){
		if(!(p%i)) 
			return false;
	}
	return true;
}

// get random prime number between B and 2B
long long int get_random_prime(){
	int counter = 0;
	while(counter < LIMIT){
		long long int rnd = (long long int)(lower_prime_limit + 2 * (rand() % (upper_prime_limit/2 - lower_prime_limit/2)) + 1);
		if(is_prime(rnd)){
			return rnd;
		}
		counter++;
	}
	return -1;
}

// get the hash value from i to j
long long int get_hash_value(int i, int j){
	if(j == i-1)
		return 0;
	if(i <= j)
		return (hash_value[j+1] + prime - (hash_value[i] * base_value[j+1-i]) % prime) % prime;
}

// get the reverse hash value from i to j
long long int get_reverse_hash(int i, int j){
	if(j == i-1)
		return 0;
	if(i <= j)
		return (reverse_hash[i] + prime - (reverse_hash[j+1] * base_value[j+1-i]) % prime) % prime;
}

// check if substring of length l is palindrome using hashes
int test(string &s, int l){
	for(int i = 0; i <= string_length - l; i++){
		int j = i + l - 1;
		if(get_hash_value(i,j) == get_reverse_hash(i,j)) return i;
	}
	return -1;
}

// search for substring in add string
void odd_binary_search(string &s, int *low, int *high, int *current_odd){
	int mid, temp;
	while(*low < *high){	
		mid = (*low + *high + 1) / 2;
		if((temp = test(s, 2*mid+1)) != -1){
			*low = mid;
			*current_odd = temp;
		}
		else{
			*high = mid-1;
		}
	}
}

// search for substring in even string
void even_binary_search(string &s, int *low, int *high, int *current_even){
	int mid, temp;
	while(*low < *high){
		mid = (*low + *high + 1) / 2;
		if((temp = test(s, 2*mid)) != -1){
			*low = mid;
			*current_even = temp;
		}
		else{
			*high = mid - 1;
		}		
	}
}

// function to find the largest substring which is a palindrome.
// It stores the start and length of the substring in substart and sub_length.
void largest_substring_palindrome(string &s, long long int *sub_start, long long int *sub_length){

	string_length = s.length();
	prime = get_random_prime();
	if(prime == -1)
		return;
	cout << "trying modulus " << prime << endl;

	//hash value of powers of BASE
	base_value = new long long int[string_length+1];
	base_value[0] = 1;
	for(int i = 0; i < string_length; i++){
		base_value[i+1] = (base_value[i] * BASE) % prime;
	}

	//hash value of prefixes
	hash_value = new long long int[string_length+1];
	hash_value[0] = 0;
	for(int i = 0; i < string_length; i++){
		hash_value[i+1] = (s[i] + hash_value[i] * BASE) % prime;
	}

	//hash value of suffixes
	reverse_hash = new long long int[string_length+1];
	reverse_hash[string_length] = 0;
	for(int i = string_length-1; i >= 0; i--){
		reverse_hash[i] = (reverse_hash[i+1] * BASE + s[i]) % prime;
	}

    // low and high of odd and even parts
	int low_of_odd = 0, low_of_even = 0;
	int high_of_odd = (string_length - 1) / 2, high_of_even = string_length / 2;
	int current_odd = 0, current_even = 0;

	// searching in odd part
	odd_binary_search(s, &low_of_odd, &high_of_odd, &current_odd);

	// searching in even part
	even_binary_search(s, &low_of_even, &high_of_even, &current_even);

	// calculating length from results of binary search
	int odd_result = 2 * low_of_odd + 1, even_result = 2 * low_of_even;

	// selecting longer substring from even or odd part
	if(odd_result > even_result){
		*sub_start = (long long int)current_odd;
		*sub_length = (long long int)odd_result;
	}
	else{
		*sub_start = (long long int)current_even;
		*sub_length = (long long int)even_result;
	}
	
	//free space
	delete [] base_value;
	delete [] hash_value;
	delete [] reverse_hash;
}

// checks if a substring is a palindrome
bool is_palindrome(string &s, long long int pos, long long int sub_length){
	for(int i = pos; i < pos + sub_length/2; i++){
		if(s[i] != s[2*pos+sub_length-1-i]) return false;
	}
	return true;
}

// main function
int main(int argc, char **argv) {
	string s;
	int B;
	
	// Taking input for B
	cin >> B;
	lower_prime_limit = B;
	upper_prime_limit = 2 * B;
	if(B%2)
		lower_prime_limit--;	
	
	// Taking input for string to search in
	cin >> s;

	// finding largest substring	
	while(1){
		long long int sub_start, sub_length;
		largest_substring_palindrome(s, &sub_start, &sub_length);
		if(is_palindrome(s, sub_start, sub_length)){
			cout << "start=" << sub_start << " length=" << sub_length << endl;
			break;
		}
	}
	return 0;
}
