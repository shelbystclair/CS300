//************************************************
// Name			: ProjectTwo
// Author		: Shelby St. Clair
// Version		: 1.0
// Description	: Final Project
//
//************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
//created a structure to hold course details
//declared prerequisite courses as a vector to hold all prereqs if more than one
struct Courses {
	string courseNumber;
	string courseName;
	vector<string> coursePrereq;
};

//created an internal structure to hold Binary Search Tree details
struct Node {
	Courses course;
	Node* left;
	Node* right;

	//default constructor with left and right nodes assigned to null
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//initialized with a course
	Node(Courses aCourse) :
		Node() {
		course = aCourse;
	}
};

//created the CourseTree class to hold necessary methods for program functionality
class CourseTree {
private:
	Node* root;
	void inOrder(Node* node);

public:
	CourseTree();
	void InOrder();
	void addNode(Node* node, Courses course);
	void Insert(Courses course);
	Courses searchCourse(string courseNumber);
};

//create default constructor for CourseTree and set root equal to nullptr
CourseTree::CourseTree() {
	root = nullptr;
}

//set up in order tree traversal method
void CourseTree::InOrder() {
	this->inOrder(root);
}

//create function to traverse and print course details in alphanumeric order
//starting with the left-most node and working to the right-most node
void CourseTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseNumber << ", " << node->course.courseName << endl;
		inOrder(node->right);
	}
}

//create function to search CourseTree for a specific course number
//will return immediately if the course number is equal to the root value
//if the course number is LESS than the root value, we traverse left
//if the course number is MORE than the root value, we traverse right
Courses CourseTree::searchCourse(string courseNumber) {
	Node* current = root;

	while (current != nullptr) {
		if (current->course.courseNumber.compare(courseNumber) == 0) {
			return current->course;
		}
		if (courseNumber.compare(current->course.courseNumber) < 0) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}
}

//create function to add course details to BST from loaded file
//node will be assigned based on root value - if course is less than the root
//course will be added to left side of the BST. If course is more than the root,
//course will be added to the right side of the BST
void CourseTree::addNode(Node* node, Courses course) {
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}

	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}
}

//create function for insertion method into BST
//this assigns a node to the root point if root is empty
//otherwise, the previously created 'addNode' function will be called
void CourseTree::Insert(Courses course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}
}

//function to display courses when the Search function is chosen by the user
//in final program - will display course number, course name, and any prerequisites
void displayCourse(Courses course) {
	cout << endl;
	cout << course.courseNumber << ", " << course.courseName << endl;
	for (string preReq : course.coursePrereq) {
		cout << "Prerequisites: " << preReq << endl;
	}
}

//function to load file into program - used sstream to parse each line
//line will be separated with courseNumber and courseName at comma
//any prerequisites are added to the Prereq vector
//error will populate if file is not loaded properly
void loadCourses(string filename, CourseTree* bst) {
	string line;
	Node* root = nullptr;

	ifstream file(filename);

	if (file.is_open()) {
		while (getline(file, line)) {
			stringstream ss(line);

			Courses course;
			getline(ss, course.courseNumber, ',');
			getline(ss, course.courseName, ',');
			string str;
			while (getline(ss, str, ',')) {
				course.coursePrereq.push_back(str);
			}
			bst->Insert(course);
		}
		cout << "File loaded successfully." << endl;
	}
	else {
		cout << "Error opening file - please try again." << endl;
	}
	file.close();
}

//main functionality for program to run
int main() {

	//declared filename to load proper file into program and userCourse for user entry in search function
	string filename;
	string userCourse;

	//name of the file used in this program
	filename = "CS 300 ABCU_Advising_Program_Input.csv";

	//created a BST to hold all course details
	CourseTree* bst;
	bst = new CourseTree();
	Courses course;

	//switch statement to navigate through menu options
	//each option will be displayed for the user in the program console
	int userChoice = 0;
	while (userChoice != 9) {
		cout << "Welcome to the ABCU course planner." << endl;
		cout << endl;
		cout << "1. Load Data into Binary Search Tree." << endl;
		cout << "2. Print list of courses in alphanumeric order." << endl;
		cout << "3. Search for information about a specific course." << endl;
		cout << "9. Exit the course planner." << endl;
		cout << endl;
		cout << "What would you like to do?" << endl;
		cin >> userChoice;

		//start of switch statement with numerous case options
		switch (userChoice) {
		case 1:
			//loads file into program - must be done first for data to be available
			loadCourses(filename, bst);
			break;
		case 2:
			//prints all courses in alphanumeric order
			bst->InOrder();
			break;
		case 3:
			//accepts user entry for course and prints all course details if a match is found
			cout << "Which course would you like to search for?" << endl;
			cin >> userCourse;
			course = bst->searchCourse(userCourse);

			if (!course.courseNumber.empty()) {
				displayCourse(course);
			}
			else {
				cout << "Course not found - please try again." << endl;
			}
			break;
		case 9:
			//closes program
			cout << "Closing course planner - goodbye!" << endl;
			break;
		default:
			//returns an error message if any other number is entered
			cout << "Invalid selection - please try again." << endl;
			break;
		}
	}
}
