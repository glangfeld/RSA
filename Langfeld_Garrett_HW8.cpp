/*
 * hw8.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: carola
 */



#include<iostream>
using namespace std;



/***********************************************************************
 * Function and class headers. This should ideally be in a separate file cryptoLib.h
 ***********************************************************************/


/*
 * TODO:
 *
 * Class or struct definitions go here for:
 *   User
 *   PublicKeyDB
 */

//Creating struct for public_key
struct public_key{
		int e;
		int n;
	};


class User{
	int d;
	//char *user;
	int decryptNumber(int shift_encrypted);
public:
	struct public_key pubKey;
	char *user;
	//constructor
	User(char *name);
	void receiveMessage(int shift_encrypted, char *shifted_message);
};

struct record{
	struct public_key K;
	char *userName;
};


class PublicKeyDB{
	int counter;
	struct record *array;
	public_key getPublicKey(User *user);
	//int encryptNumber(User *recipient, int shift);
public:
	//constructor
	PublicKeyDB();
	void setKey(public_key K, char *user);
	int encryptNumber(User *recipient, int shift);
	
};


/*************************************************************************
 * Global variables and constants
 *************************************************************************/
// Mysterious library function that returns public and private encryption keys. Those should be generated from large prime numbers.
int c=0;
const int keys[15]= {95, 5, 29, 119, 7, 55, 115, 7, 63, 161, 7, 19, 121, 11, 91};
void next_n_e_d(int* n, int* e, int* d){
	*n=keys[c++];
	*e=keys[c++];
	*d=keys[c++];
	c=c%15;
}

// Default capacity for strings.
#define STRING_CAPACITY  256
#define ARRAY_CAPACITY 5

// Global storage for all public keys for all users.
PublicKeyDB publicKeys = PublicKeyDB();


/**********************************************************************************************
 * Function and class implementations. This should ideally be in a separate file cryptoLib.cpp
 **********************************************************************************************/


int pow_mod(int a, int b, int m){
	// Computes a^b mod m. Assume b>0.
	int result = 1;
	int i;
	for(i=0; i<b; i++)
		result = (result*a)%m;

	return result;
}

// Utility functions for string manipulation:
char shiftChar(int shift, char c){
	return (c-32+95+shift)%95+32;
}


char* shiftString(int shift, char* input){
	//Finding length of input
	int l = 0;
	while(input[l] != '\0'){
		l++;
	}
	char *output = new char[STRING_CAPACITY];
	int i;
	for(i = 0; i < l; i++){
		output[i] = shiftChar(shift, input[i]);
	}
	output[l] = '\0';
	return output;
}

/*
 * TODO:
 *
 * Class implementations go here for:
 *   PublicKeyDB
 *   User
 */


/*
 * Implementations for class PublicKeyDB
 */

PublicKeyDB::PublicKeyDB(){
	array = new struct record[ARRAY_CAPACITY];
	counter = 0;
}


void PublicKeyDB::setKey(public_key K, char *user){
	array[counter].K = K;
	array[counter].userName = user;
	counter++;
}

public_key PublicKeyDB::getPublicKey(User *user){
	int i;
	for(i = 0; i <= counter; i++){
		if(array[i].userName == user->user){
			return array[i].K;
		}
	}
	//defining bogus struct to return if user is not in database
	struct public_key bogus;
	bogus.n = 0;
	bogus.e = 0;
	return bogus;
}


int PublicKeyDB::encryptNumber(User *recipient, int shift){
	struct public_key key = getPublicKey(recipient);
	int shiftAmt = pow_mod(shift, key.e, key.n);	
	return shiftAmt;
}


/*
 * Implementations for class User
 */



	User::User(char *name){
		//pubKey.n = n;
		//pubKey.e = e;
		//int *n = new_value;
		//int *e = other_value;
		user = name;
		next_n_e_d(&pubKey.n, &pubKey.e, &d);
		//pubKey.n = n;
		//pubKey.e = e;
		publicKeys.setKey(pubKey, name);
		//pubKey = {e, n};
	}

	int User::decryptNumber(int shift_encrypted){
		return pow_mod(shift_encrypted, d, pubKey.n);
	}

	void User::receiveMessage(int shift_encrypted, char *shifted_message){
		int shift = decryptNumber(shift_encrypted);
		cout << shift << endl;
		char *real_message = shiftString(shift*(-1), shifted_message);
		cout << real_message << endl;
	}
		



/**********************************************************************************************
 * main()
 **********************************************************************************************/


int main(){

	/*
	 * TODO:
	 *
	 * Test code goes here.
	 *
	 */
User Alice("Alice");
User Bob("Bob");

char *secretMessageForAlice = "^ro*~o}~*q|kno}*k|o*oxmynon*sx*K]MSS8";
char *hello = "Hello World";
char *hello2 = shiftString(10, hello);
cout << hello2 << endl;
char *hello3 = shiftString(-10, hello2);
cout << hello3 << endl;

char *messageForAlice2 = "Hello hot stuff!";
char *messageForAlice2Encrypted = shiftString(8, messageForAlice2);
int s2 = publicKeys.encryptNumber(&Alice, 8);
Alice.receiveMessage(s2, messageForAlice2Encrypted);

char *messageForBob = "Come get it while it's hot.";
char *messageForBobEncrypted = shiftString(15, messageForBob);
int s3 = publicKeys.encryptNumber(&Bob, 15);
Bob.receiveMessage(s3, messageForBobEncrypted);

//char *messageForAlice2Encrypted = shiftString(8, messageForAlice2);
Alice.receiveMessage(60, secretMessageForAlice);








	return 0;
}

