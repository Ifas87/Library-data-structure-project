#include "classes.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>
/*
	classes.cpp
	Author: M00692387
	Created: 13/03/2021
	Updated: 26/04/2021
*/


// Book class methods
// Standard book class constructor
book::book( std::string title, std::string author, std::string ISBN, 
																									 int remainingQty ){
	this->title = title;
	this->author = author;
	this->ISBN = ISBN;
	this->remainingQty = remainingQty;
}

book::book(){}

// Getters / Accessors
std::string book::getTitle(){
	return this->title;
}

std::string book::getAuthor(){
	return this->author;
}

std::string book::getISBN(){
	return this->ISBN;
}

int book::getQty(){
	return this->remainingQty;
}

// Setters / Mutators		
void book::setTitle( std::string title ){
	this->title = title;
}

void book::setAuthor( std::string author ){
	this->author = author;
}

void book::setISBN( std::string ISBN ){
	this->ISBN = ISBN;
}

void book::setQty( int remainingQty ){
	this->remainingQty = remainingQty;
}

/*
	Print a string containing all the properties similar to the file record.
	@param none
	@return a std::string of all attributes
*/
std::string book::printObject(){
	std::string strQty = std::to_string(this->remainingQty);
	return (convertUpper(this->title) + "\t" +convertUpper(this->author) + "\t" + 
					this->ISBN + "\t" + strQty+"\t\n");
}

/*
	Print a string containing all records in an organised readable manner.
	@param none all attributes are already present
	@return a std::string of all attributes
*/
std::string book::printPretty(){
	std::string strQty = std::to_string(this->remainingQty);
	return ("\nBook name: " + this->title + "\nWritten by: " + this->author +
					"\nISBN search number: " + this->ISBN + "\nRemaining books available: "
					+ strQty + "\n");
}

/*
	Check function
	@param maximum and minimum int values
	@return an integer within the range specified
*/
int check(int minimum, int maximum) {
	while(true){
		int testValue;
		std::cin >> testValue;
		if(!std::cin){ // checking cin failure by a char
			std::cin.clear(); // reset failbit
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Wrong input try again: ";
		}
		else
			if(testValue > minimum && testValue <maximum)
				return testValue;
			else
				std::cout << "Input is out of bounds should be within, try again: ";
	}
}

/*
	Binary node (b_node) hybrid constructor for the BST delete function
	@param Two existing nodes to take the respective properties from
	@return A new b_node that can be attached to a pointer
*/
b_node::b_node( b_node* &start, b_node* &entry ){
	this->key = entry->key;
	this->value = entry->value;
	this->left = start->left;
  this->right = start->right;
}

/*
	Binary node (b_node) copying constructor for the BST collision function
	@param An existing node pointer to take the respective properties from
	@return A new b_node that can be attached to a pointer
*/
b_node::b_node( b_node* &reference ){
	this->key = reference->key;
	this->value = reference->value;
	this->left = NULL;
  this->right = NULL;
}

// binary node structure functions
b_node::b_node( std::string key, book value ){
	this->key = key;
	this->value = value;
	this->left = NULL;
  this->right = NULL;
}

/*
	Print an all uppercase version of the input string
	@param std::string that needs to be upper case
	@return a std::string
*/
std::string convertUpper( std::string &sample ){
	std::for_each(sample.begin(), sample.end(), [](char &c){
    c = ::toupper(c);
	});
	return sample;
}

// Binary tree class constructors
BST::BST(){}

// Class constuctor with an existing start node
BST::BST( b_node* &root ){
	this->root = root;
}

/*
	deals with collision events with same books by providing users with options
	@param the original node and new duplicate node and the title of both book nodes
	@return nothing just edits the BST in the background
*/
void BST::collision( b_node* &start, b_node* &entry, std::string &key){
	
	std::cout << "\nThat book already exists" << std::endl;
	std::cout << "Press \"1\" to overwrite the existing book and all of its" <<
							 " details with the duplicate. \nOR" << std::endl;
	std::cout << "Press \"2\" to update the existing's book quantity," << 
							 " the book will be restocked with the quantity of the duplicate" 
							 << std::endl;
	std::cout << "What do you wish to do? ";
	int option = check(0, 3);
	
	if(option == 1){
		this->root = this->deleteNode(this->root, start->key);
		this->root = this->insertNode(this->root, entry);
		std::cout << "Overwriting done: " << std::endl;
	}
	else{
		int tempQty = (start->value.getQty() + entry->value.getQty());
		b_node* temps = new b_node(start); temps->value.setQty(tempQty);
		this->root = this->deleteNode(this->root, start->key);
		this->root = this->insertNode(this->root, temps);
		std::cout << "Adding done: " << std::endl;
	}
}

/*
	Insert function for adding new data to the BST
	@param starting node to insert from and the data node to insert
	@return returning the node to update the parent node in that recursion layer
*/

b_node* BST::insertNode( b_node* &start, b_node* &entry ){
	std::string s1 = convertUpper(entry->key);
	if (start == NULL)
		start = entry;
	else if(s1 == start->key){
		std::cout << "\nThe record: \n" << entry->value.printPretty() << "\nis already present" <<
		" as, \n" << start->value.printPretty() << "In the database\n" << std::endl;
		this->collision(start, entry, s1);
	}
	else if (start->key > s1)
		start->left = insertNode(start->left, entry);
  else if (start->key < s1)
		start->right = insertNode(start->right, entry);
	
	return start;
}

/*
	Search function to return a node from the BST using a key
	@param starting node to search from and the title of the book to search
	@return the node that was found
*/
b_node* BST::lookup(b_node* &start, std::string &key){
	std::string s1 = convertUpper(key);
	if (start == NULL)
		return NULL;
	else if(s1 == start->key)
		return start;
	else if(key > start->key)
		return lookup(start->right, key);
	else
		return lookup(start->left, key);
}

/*
	Printing function for writing the BST's contents to a file in sorted order
	@param root starting node or any parent node, the output filestream to write
	data from
	@return none there is no need to
*/
void BST::printPreorder(b_node* &root, std::ofstream &writer){
  if (root == NULL)
    return;
  
	std::string temp = root->value.printObject();
	writer << temp;
	printPreorder(root->left, writer);
	printPreorder(root->right, writer);
}

/*
	Data node deletion function
	@param root starting node or any parent node, and title of the book
	@return the replacement node after deleting to update parent nodes.
*/
b_node* BST::deleteNode(b_node* &start, std::string &key){
	std::string s1 = convertUpper(key);
	if(start == NULL)
		return start;
	// Recursive search for finding the node with the right key / title
	if (s1 < start->key)
		start->left = deleteNode(start->left, s1);
	else if(s1 > start->key)
		start->right = deleteNode(start->right, s1);
  // Node has been discovered scenario
	// Further testing for any child nodes present and replacing them
	// Only one node present
	else{
		if(start->left == NULL){
			b_node* temp = start->right;
      delete start;
      return temp;
    }
    
		else if(start->right == NULL){
      b_node* temp = start->left;
      delete start;
      return temp;
    }
		// Two child nodes scenario
		// Solved by choosing and replacing with the smallest element in right subtree
		b_node* temp = getMinNode(start->right);
    start = new b_node(start, temp);
    start->right = deleteNode(start->right, temp->key);
  }
  return start;
}

/*
	Function to retrieve the smallest node from a parent node or subtree
	@param starting node to search from
	@returns the smallest node found within that subtree
*/
b_node* BST::getMinNode(b_node* &rootNode){
	b_node* sample = rootNode;
  while (sample && sample->left != NULL)
		sample = sample->left;
	return sample;
}