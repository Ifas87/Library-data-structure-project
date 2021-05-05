#ifndef CLASSES_H
#define CLASSES_H
#include<string>
#include<vector>
/*
	classes.h
	Author: M00692387
	Created: 13/03/2021
	Updated: 17/04/2021
*/

/*
	A Book class that stores related attributes, 
	printing function set/get functions
*/
class book {
	private:
		std::string title;
		std::string author;
		std::string ISBN;
		int remainingQty;
	public:
		book(std::string title, std::string author, std::string ISBN, int remainingQty);
		book();
		
		std::string getTitle();
		std::string getAuthor();
		std::string getISBN();
		int getQty();
		
		void setTitle( std::string title );
		void setAuthor( std::string author );
		void setISBN( std::string ISBN );
		void setQty( int remainingQty );
		
		std::string printObject();
		std::string printPretty();
};

// Upper case function
std::string convertUpper( std::string &sample );
// int input check function
int check(int minimum, int maximum);

// Structure definition for BST
struct b_node {
  std::string key;
  book value;
  b_node* left;
  b_node* right;
  
	b_node( b_node* &reference );
	b_node( b_node* &start, b_node* &entry );
  b_node( std::string key, book value );
	b_node();
};

// BST class
class BST {
	public:
		b_node* root = NULL;
		BST();
		BST(b_node* &root);
		
		b_node* lookup( b_node* &start, std::string &key );
		
		void printPreorder(struct b_node* &root, std::ofstream &writer);
		void collision( b_node* &start, b_node* &entry, std::string &key);
		
		b_node* insertNode( b_node* &start, b_node* &entry );
		b_node* getMinNode( b_node* &rootNode );
		b_node* deleteNode( b_node* &root, std::string &key );
};
#endif