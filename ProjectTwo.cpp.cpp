// Joseph Cassello Jr.
// Professor Gopalakrishnan
// CS 300 Project Two 
// December 15, 2024

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>

// Class to represent a course
class Course {
public:
	std::string courseNumber;
	std::string courseTitle;
	std::vector<std::string> prerequisites;

	// Constructor to initialize a Course object
	Course(const std::string& number, const std::string& title, const std::vector<std::string>& prereqs)
		: courseNumber(number), courseTitle(title), prerequisites(prereqs) {}
};

// Node for the binary search tree (BST)
class Node {
public:
	std::shared_ptr<Course> course;
	Node* left;
	Node* right;

	// Constructor to initialize a Node with a Course
	explicit Node(const std::shared_ptr<Course>& c) : course(c), left(nullptr), right(nullptr) {}
};

// BST implementation
class BST {
private:
	Node* root;

// Helper function to insert a node into the BST
	void insertNode(Node*& node, const std::shared_ptr<Course>& course) {
		// If the node is null, create a new node
		if (!node) {
			node = new Node(course);
		}
		// If the course number is smaller, go to the left subtree
		else if (course->courseNumber < node->course->courseNumber) {
			insertNode(node->left, course);
		}
		// Otherwise, go to the right subtree
		else {
			insertNode(node->right, course);
		}
	}

	// Helper function for in-order traversal
	void inOrderTraversal(Node* node) const {
		if (node) {
			inOrderTraversal(node->left);
			// Print course number and title is sorted order
			std::cout << node->course->courseNumber << ": " << node->course->courseTitle << std::endl;
			inOrderTraversal(node->right);
		}
	}
			
// Helper function to find a course in the BST
Node* findNode(Node* node, const std::string& courseNumber) const {
	// Base case: if node is null or the course number matches
	if (!node || node->course->courseNumber == courseNumber) {
		return node;
	}
	// If the course number is smaller, search the left subtree
	if (courseNumber < node->course->courseNumber) {
		return findNode(node->left, courseNumber);
	}
	// Otherwise, search the right subtree
	return findNode(node->right, courseNumber);
}

public:
	BST() : root(nullptr) {}
	// Insert a course into the BST
	void insertCourse(const std::shared_ptr<Course>& course) {
		insertNode(root, course);
	}
	// Print all courses in alphanumeric order
	void printCourses() const {
		inOrderTraversal(root);
	}
	// Print details of a specific course
	void printCourseDetails(const std::string& courseNumber) const {
		Node* node = findNode(root, courseNumber);
		if (node) {
			// Print the course number and title
			std::cout << node->course->courseNumber << ": " << node->course->courseTitle << std::endl;
			// Check if the course has prerequisites
			if (!node->course->prerequisites.empty()) {
				std::cout << "Prerequisites: ";
				for (size_t i = 0; i < node->course->prerequisites.size(); ++i) {
					std::cout << node->course->prerequisites[i];
					if (i < node->course->prerequisites.size() - 1) {
						std::cout << ", ";
					}
				}
				std::cout << std::endl;
			} else {
				std::cout << "Prerequisites: None" << std::endl;
			}
		} else {
			// If the course is not found, print an error message
			std::cout << "Error: Course not found." << std::endl;
		}
	}
};

// Function to load courses from a file
void loadCourses(const std::string& filePath, bool& isLoaded, BST& bst) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		// Handle file opening error
		std::cerr << "Error: Could not open the file. Please check the file path and try again." << std::endl;
		isLoaded = false;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream stream(line);
		std::string courseNumber, courseTitle;
		// Read the course number and title 
		std::getline(stream, courseNumber, ',');
		std::getline(stream, courseTitle, ',');
		// Read any prerequisites that follow
		std::vector<std::string> prerequisites;
		std::string prerequisite;
		while (std::getline(stream, prerequisite, ',')) {
			prerequisites.push_back(prerequisite);
		}
		// Create a course object and insert it into the BST
		auto course = std::make_shared<Course>(courseNumber, courseTitle, prerequisites);
		bst.insertCourse(course);
	}

	file.close();
	isLoaded = true;
}

// Main Program
int main() {
	BST bst;
	bool isLoaded = false;

	while (true) {
		std::cout << "\nWelcome to the course planner.\n"
			<< "1. Load Data Structure.\n"
			<< "2. Print Course List.\n"
			<< "3. Print Course.\n"
			<< "9. Exit\n"
			<< "What would you like to do? ";
		int choice;
		std::cin >> choice;

		// Validate menu input
		if (std::cin.fail() || choice < 1 || (choice > 3 && choice != 9)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid choice. Please try again." << std::endl;
			continue;
		}

		if (choice == 1) {
			// Prompt the user for the file path
			std::cout << "Enter file path: ";
			std::string filePath;
			std::cin.ignore();
			std::getline(std::cin, filePath);

			// Debug: Print the file path
			std::cout << "Attempting to open file from path: " << filePath << std::endl;
			loadCourses(filePath, isLoaded, bst);

			if (isLoaded) {
				std::cout << "Courses loaded successfully." << std::endl;
			}
			else {
				std::cout << "Error: Could not load courses." << std::endl;
			}

		} else if (choice == 2) {
			if (isLoaded) {
				// Print all courses if data is loaded
				bst.printCourses();
			} else {
				std::cout << "Error: No courses loaded. Please load courses first." << std::endl;
			}

		} else if (choice == 3) {
			if (isLoaded) {
				// Prompt the user for a course number to look up
				std::cout << "Enter course number: ";
				std::string courseNumber;
				std::cin >> courseNumber;
				bst.printCourseDetails(courseNumber);
			} else {
				std::cout << "Error: No courses loaded. Please load courses first." << std::endl;
			}

		} else if (choice == 9) {
			// Exit the program
			std::cout << "Thank you for using the course planner!" << std::endl;
			break;
		}
	}

	return 0;
}