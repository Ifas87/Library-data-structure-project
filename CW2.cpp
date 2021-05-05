#include "classes.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>
#include <limits>

/*
	CW2.cpp
	Author: M00692387
	Created: 13/03/2021
	Updated: 26/04/2021
*/

/*
	Function for loading each line in the file into a book object
	@param record line from the file through initiaRead
	@return returns the encapsulated object within the b_node pointer
*/
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

/*
	Reading function that reads the entire file from start to end
	@param the BST to store each node in, the input filestream and filename from
					console
	@return none
*/
void initialRead(BST &sample, std::ifstream &reader, std::string &filename, bool loopLogic
	,bool writeConfirm){
	int lineCount = 0;
	reader.open(filename, std::ios::in);
	std::string temp; int count = 1;
	
	while (std::getline(reader, temp) ){
		lineCount++;
		// Substring that represents a 0 stock value in the file, used to remove
		// records with no stock
		std::string s1 = "\t0\t";
		if (temp.find(s1) != std::string::npos)
			continue;
    
		b_node* obj = objectLoad(temp);
		sample.root = sample.insertNode(sample.root, obj);
		count++;
	}
	reader.close();
}

/*
	Simple string checking function that asks users for validation
	@param none
	@return the final string that the user wants to send
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

/*
	For adding a book to the record, asks users about details and creates the
	necessary objects to insert into the BST
	@param The BST to insert data into
	@return none there is no need to
*/
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
	Qty = check(-100, 1000);
	
	//.wonk uoy ,krow smetsys kcots woh t'nsi sihT
	if (Qty <= 0){
		std::cout << "\nStock of a new book cannot be less than 0." << std::endl;
		return;
	}
	title = convertUpper(title);
	author = convertUpper(author);
	book newEntry(title, author, ISBN, Qty);
	b_node* record = new b_node(title, newEntry);

	hope.root = hope.insertNode(hope.root, record);
}

/*
	Function for writing the BST back to file, uses one output to stream to
	empty the file and another to write data
	@param two output filestreams; writer to write data which will be passed to
					the printPreorder() function of the BST
					The Truncater filestream to empty the file before uploading,
					The name of the file to write back to
	@return none there is no need to
*/
void endingWrite(std::ofstream &writer, std::ofstream &truncater, std::string &filename, BST &hope){
	truncater.open(filename, std::ofstream::out | std::ofstream::trunc);
	truncater.close();
	
	writer.open(filename, std::ios_base::app);
	hope.printPreorder(hope.root, writer);
	writer.close();
}

int main( int argc, char** argv ){ 
	// Storage unit for books
	BST storage;
	// File related variables
	std::ifstream reader;
	std::ofstream writer, truncater;
	std::string filename;
	// TUI related elements 
	int option, count = 0;
	bool loopLogic = true;
	bool writeConfirm = true;
	
	if(argc > 1 and argc < 3){
		filename = argv[1];
		std::cout << "\nFile is being read now" << std::endl;
		initialRead(storage, reader, filename, loopLogic, writeConfirm);
	}
	else{
		std::cout<< "\nNo filename provided, program closing" << std::endl;
		loopLogic = false;
		writeConfirm = false;
	}
	
	while(loopLogic){
		std::cout << "\n------------------------------------------------------------" << std::endl;
		std::cout << "Library management system" << std::endl;
		std::cout << "Please add the number for the operation you want to perform:" << std::endl;
		std::cout << "\"1\" for searching for a particular book." << std::endl;
		std::cout << "\"2\" for inserting a new book into the library system." << std::endl;
		std::cout << "\"3\" for deleting a particular book from the library system." << std::endl;
		std::cout << "\"4\" To close the program." << std::endl;
		std::cout << "What do you wish to do? ";
		
		option = check(0, 5);

		switch(option){
			case 1: {
				std::cout << "Enter the name of the book: ";
				std::string tempKey = validation();
				b_node* tempNode = storage.lookup(storage.root, tempKey);
				
				if(tempNode == NULL)
					std::cout << "\nNo book found with that title" << std::endl;
				else
					std::cout << "\nRecord found: " << tempNode->value.printPretty() << std::endl;
			break;
			}
			case 2:
				addBook(storage);
			break;
			
			case 3: {
				std::cout << "Enter the name of the book to delete: ";
				std::string tempKey = validation();
				storage.root = storage.deleteNode(storage.root, tempKey);
				std::cout << "\nBook record deleted" << std::endl;
			break;
			}
			case 4:
				loopLogic = false;
			break;
		}
	}
	if(writeConfirm){
		endingWrite(writer, truncater, filename, storage);
		std::cout<< "Writing done, Program is closing" << std::endl;
		std::cout << "\n------------------------------------------------------------" << std::endl;
	}
}