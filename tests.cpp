#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "classes.h"

#include <fstream>
#include <vector>
#include <string>
#include <chrono>
/*
	tests.cpp
	Author: M00692387
	Created: 13/03/2021
	Updated: 17/04/2021
*/

using namespace std::chrono;

/*
	Supporting functions from the CW2.cpp program to facilitate the tests
	The remaining libraries are takn from classes.cpp
*/

std::string validation(){
	std::string entry, yesNo;
	while(true){
		std::cin.ignore();
		std::getline(std::cin, entry);
		std::cout << "You entered: \"" << entry << "\" is this fine?(Y/N): ";
		std::cin >> yesNo;
		
		if(yesNo == "y" || yesNo == "yes" || yesNo == "Yes" || yesNo == "Y")
			return entry;
		std::cout << "Enter again: ";
	}
}

void addBook(BST &hope){
	std::string title, author, ISBN; int Qty;
	
	std::cout << "\nEnter the following details:" << std::endl;
	std::cout << "Please enter the title of the book: ";
	title = validation();
	std::cout << "\nPlease enter the author/authors separated by ';': ";
	author = validation();
	std::cout << "\nPlease enter the ISBN number: ";
	ISBN = validation();
	std::cout << "\nPlease enter the current or additional stock of this book: ";
	Qty = check(-12, 1000);
	
	title = convertUpper(title);
	author = convertUpper(author);
	book newEntry(title, author, ISBN, Qty);
	b_node* record = new b_node(title, newEntry);
	
	std::cout << "Stuff" << std::endl;
	b_node* temp = (hope.lookup(hope.root, title));
	
	hope.root = hope.insertNode(hope.root, record);
}

b_node* objectLoad(std::string record){
	std::istringstream iss(record);
	std::string title, author, ISBN; int Qty, count=1;
	while (iss.good()) {
		std::string substr;
		getline(iss, substr, '\t');
		
		if(count == 1)
			title = convertUpper(substr);
		else if(count == 2)
			author = convertUpper(substr);
		else if(count == 3)
			ISBN = substr;
		else if(count == 4)
			Qty = std::stoi(substr);
		
		count++;
	}
	book temp(title, author, ISBN, Qty);
	b_node* tempNode = new b_node(title, temp);
	return tempNode;
}

int initialRead(BST &sample, std::ifstream &reader, std::string &filename){
	int lineCount = 0;
	reader.open(filename, std::ios::in);
	std::string temp; int count = 1;
	
	while (std::getline(reader, temp) ){
		lineCount++;
    b_node* obj = objectLoad(temp);
		sample.root = sample.insertNode(sample.root, obj);
		count++;
	}
	reader.close();
	return lineCount;
}

int timedRead(BST &sample, std::ifstream &reader, std::string &filename){
	int lineCount = 0;
	std::vector<b_node *> vec;
	reader.open(filename, std::ios::in);
	std::string temp; int count = 1;
	
	while (std::getline(reader, temp) ){
		lineCount++;
    b_node* obj = objectLoad(temp);
		vec.push_back(obj);
		count++;
	}
	reader.close();
	
	auto start = high_resolution_clock::now();
	for(int i=0; i < vec.size(); i++)
	 sample.root = sample.insertNode(sample.root, vec[i]);
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
  
  std::cout << "Time taken for inserting 419 objects: " << duration.count() << " microseconds" << std::endl;
	return lineCount;
}

void endingWrite(std::ofstream &writer, std::ofstream &truncater, 
			std::string &filename, BST &hope){
	truncater.open(filename, std::ofstream::out | std::ofstream::trunc);
	truncater.close();
	
	writer.open(filename, std::ios_base::app);
	hope.printPreorder(hope.root, writer);
	writer.close();
}


TEST_CASE("Testing read function and insert function for BST", "[insertion]"){
	BST storage;
	std::ifstream reader;
	std::string filename = "books";
	
	int check = timedRead(storage, reader, filename);
	
	// 419 is the number of lines in a normal "books" file
	// So the Check is to see if data from the file is read from the file
	// and Inserted into the BST properly
	REQUIRE(check == 419);
	
}

TEST_CASE("Testing Delete function and writing function", "[deletion]"){
	BST storage1, postStorage;
	std::ifstream reader;
	std::ifstream reader2;
	std::ofstream writer;
	std::ofstream truncater;
	std::string filename = "books";
	int check = initialRead(storage1, reader, filename);
	
	// Sample books to removed, the goal is to remove 3 books and see if the line
	// count is reduced to 415
	std::string s1 = "PThreads Programming";
	std::string s2 = "Microsoft SQL Server 2005 Reporting Services For Dummies";
	std::string s3 = "Advances in Neural Networks - ISNN 2007";
	
	auto start = high_resolution_clock::now();
	storage1.root = storage1.deleteNode(storage1.root, s1);
	storage1.root = storage1.deleteNode(storage1.root, s2);
	storage1.root = storage1.deleteNode(storage1.root, s3);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
  
  std::cout << "Time taken for deleting 3 objects: " << duration.count() << " microseconds" << std::endl;
	endingWrite(writer, truncater, filename, storage1);
	
	int recheck = initialRead(postStorage, reader, filename);
	
	// Check if the records are actually deleted
	// End answer should be 416 as 419 - 3
	REQUIRE(recheck == 416);
}


TEST_CASE("Testing lookup function", "[lookup]"){
	BST storage2;
	std::ifstream reader;
	std::string filename = "books";
	int check = initialRead(storage2, reader, filename);
	
	//Checking the book entries from users
	// The book record is title-ASOIAF
	// The program would check if this works and is able to be searched
	std::cout << "Custom new book addtion testcase, using \"ASOIAF\" as the title" << std::endl;
	addBook(storage2);
	// Custom record check
	std::string s6 = "ASOIAF";
	b_node* tempNode6 = storage2.lookup(storage2.root, s6);
	bool t6 = (tempNode6 != NULL);
	
	// Check is to see the check is not case sensitive
	std::string s1 = "Algorithm Design with Haskell";
	std::string s2 = "Urban Operating Systems";
	std::string s3 = "Advanced Graph Theory and Combinatorics";
	std::string s4 = "Transfer in Reinforcement Learning Domains";
	std::string s5 = "Pro SQL Server 2005 Assemblies";
	
	// Checks to see if an actual object was found and returned
	b_node* tempNode1 = storage2.lookup(storage2.root, s1);
	bool t1 = (tempNode1 != NULL);
	b_node* tempNode2 = storage2.lookup(storage2.root, s2);
	bool t2 = (tempNode2 != NULL);
	b_node* tempNode3 = storage2.lookup(storage2.root, s3);
	bool t3 = (tempNode3 != NULL);
	b_node* tempNode4 = storage2.lookup(storage2.root, s4);
	bool t4	= (tempNode4 != NULL);
	b_node* tempNode5 = storage2.lookup(storage2.root, s5);
	bool t5 = (tempNode5 != NULL);
	
	
	REQUIRE(t1);
	REQUIRE(t2);
	REQUIRE(t3);
	REQUIRE(t4);
	REQUIRE(t5);
	REQUIRE(t6);

	// Last check to know whether the record received is balid or not
	std::cout << "\nRecord found: " << tempNode2->value.printPretty() << std::endl;	
}

TEST_CASE("Testing collision function", "[lookup]"){
	BST storage3;
	std::ifstream reader;
	std::string filename = "books";
	int check = initialRead(storage3, reader, filename);
	
	std::string s1 = "A River";
	
	// Checking the book entries collisions function
	// Sample testing record is A River
	// This testcase is for the overwrite / restock functionality
	std::cout << "\" A River \" Qty testcase restock or overwrite" << std::endl;
	addBook(storage3);
	// The entry would be the same name to trigger the collision
	
	b_node* tempNode1 = storage3.lookup(storage3.root, s1);
	bool t1 = (tempNode1 != NULL);
	
	int stocks = tempNode1->value.getQty();
	REQUIRE(t1);
	// Checking if overwriting or adding is done properly
	REQUIRE((stocks == 9 || stocks == 5));

	// Last check to know whether the record received is balid or not
	std::cout << "\nRecord found: " << tempNode1->value.printPretty() << std::endl;	
}