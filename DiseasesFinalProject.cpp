#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>

#include <chrono>

#include "HashTable.h"
#include "RBTree.h"

using namespace std;
using namespace std::chrono;


// tick tock functions for timing
/**
 * Start timing a process by setting the start time point.
 * @param start Reference to the time point to set to the current time.
 */
void tick(steady_clock::time_point &start) {
    start = steady_clock::now();
}

/**
 * Stop timing a process and calculate the duration.
 * @param start The starting time point of the process.
 * @param operation Name of the operation to display in the output.
 * @param duration Reference to the variable to store the duration in microseconds.
 */
void tock(steady_clock::time_point &start, const string& operation, long long &duration) {
    auto end = steady_clock::now();
    duration = duration_cast<microseconds>(end - start).count();
    cout << endl << operation << " took " << duration << " microseconds\n";
}

// Helper functions for display
/**
 * Display the menu options for the user to choose a data structure.
 */
void displayMenu() {
    cout << "Choose a data structure for querying:\n";
    cout << "1. Hash Table\n";
    cout << "2. Red-Black Tree\n";
    cout << "3. Both\n";
    cout << "Enter your choice (1, 2, or 3): ";
}

/**
 * Get the user's choice of data structure and validate input.
 * @return The user's choice as an integer (1, 2, or 3).
 */
int getUserChoice() {
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;

        // Clear the input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Check for valid input
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cout << "Invalid input. Please enter a number (1, 2, or 3).\n";
        } else if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Please enter a number (1, 2, or 3).\n";
        } else {
            break; // Valid choice, exit loop
        }
    }
    return choice;
}


/**
 * Build the specified data structures (Hash Table and/or Red-Black Tree) using data from a CSV file.
 * @param ht HashTable object to populate (if buildHashTable is true).
 * @param rbt RBTree object to populate (if buildRBTree is true).
 * @param buildHashTable Boolean indicating whether to build the Hash Table.
 * @param buildRBTree Boolean indicating whether to build the Red-Black Tree.
 * @param buildTimeHT Reference to the variable to store the build time for the Hash Table.
 * @param buildTimeRBT Reference to the variable to store the build time for the Red-Black Tree.
 */
void buildDataStructures(HashTable &ht, RBTree &rbt, bool buildHashTable, bool buildRBTree,
                         long long &buildTimeHT, long long &buildTimeRBT) {
    ifstream file("USDiseases.csv");
    string line;
    if (!file.is_open()) {
        cout << "Can't open file" << endl;
        return;
    }

    getline(file, line); // Skip header line

    int count = 0;

    if (buildHashTable) {
        // Timing for hash table
        steady_clock::time_point startHT = steady_clock::now();
        while (getline(file, line)) {
            istringstream ss(line);
            string year, state, diseaseType, isMortality, deathCount;
            int yearAsint, deathAsInt;

            getline(ss, year, ',');
            yearAsint = stoi(year);

            getline(ss, state, ',');
            getline(ss, diseaseType, ',');
            getline(ss, isMortality, ',');
            getline(ss, deathCount, ',');
            deathAsInt = stoi(deathCount);

            // Convert isMortality to lowercase
            transform(isMortality.begin(), isMortality.end(), isMortality.begin(), ::tolower);

            if (isMortality.find("mortality") != string::npos) {
                ht.insertItem(state, hashTableVars(diseaseType, yearAsint, deathAsInt, isMortality));
            }

            count += 1;
        }
        file.clear(); // Clear EOF flag
        file.seekg(0); // Rewind file stream
        getline(file, line); // Skip header line
        tock(startHT, "Hash Table build", buildTimeHT);
    }

    if (buildRBTree) {
        // Timing for red-black tree
        steady_clock::time_point startRBT = steady_clock::now();
        while (getline(file, line)) {
            istringstream ss(line);
            string year, state, diseaseType, isMortality, deathCount;
            int yearAsint, deathAsInt;

            getline(ss, year, ',');
            yearAsint = stoi(year);

            getline(ss, state, ',');
            getline(ss, diseaseType, ',');
            getline(ss, isMortality, ',');
            getline(ss, deathCount, ',');
            deathAsInt = stoi(deathCount);

            // Convert isMortality to lowercase
            transform(isMortality.begin(), isMortality.end(), isMortality.begin(), ::tolower);

            if (isMortality.find("mortality") != string::npos) {
                rbt.insert(state, hashTableVars(diseaseType, yearAsint, deathAsInt, isMortality));
            }

            count += 1;
        }
        file.close();
        tock(startRBT, "Red-Black Tree build", buildTimeRBT);
    }

    cout << "Total records processed: " << count << "\n\n";
}

/**
 * Process user queries to search for disease data in the selected data structures.
 * @param ht HashTable object to query (if buildHashTable is true).
 * @param rbt RBTree object to query (if buildRBTree is true).
 * @param buildHashTable Boolean indicating whether the Hash Table was built.
 * @param buildRBTree Boolean indicating whether the Red-Black Tree was built.
 */
void processUserChoice(HashTable &ht, RBTree &rbt, bool buildHashTable, bool buildRBTree) {
    while (true) {
        string userState, userDisease;
        cout << "Enter a state you would like to look up a disease for (or type 'Exit' to quit): ";
        getline(cin, userState);
        if (userState == "Exit") break;

        cout << "Enter the disease/cause of death you want to know about (or type 'Example' for a list): ";
        getline(cin, userDisease);
        if (userDisease == "Example") {
            cout << "Alcohol\n";
            cout << "Asthma\n";
            cout << "Cancer\n";
            cout << "Chronic Kidney Disease\n";
            cout << "Chronic Obstructive Pulmonary Disease\n";
            cout << "Cardiovascular Disease\n";
            cout << "Overarching Conditions\n";
            cout << "Please enter your disease you would like to search for: ";
            getline(cin, userDisease);
        }

        if (buildHashTable && buildRBTree) {
            // Time the query operation for hash table
            steady_clock::time_point startHT = steady_clock::now();
            ht.displayDeathCount(userState, userDisease);
            long long durationHT;
            tock(startHT, "Hash Table search", durationHT);

            // Time the query operation for red-black tree
            steady_clock::time_point startRBT = steady_clock::now();
            rbt.displayDeathCount(userState, userDisease);
            long long durationRBT;
            tock(startRBT, "Red-Black Tree search", durationRBT);

            if (durationHT < durationRBT) {
                cout << "Hash Table was faster in searching by " << abs(durationHT - durationRBT) << " microseconds.\n\n";
            } else {
                cout << "Red-Black Tree was faster in searching by " << abs(durationRBT - durationHT) << " microseconds.\n\n";
            }

        } else if (buildHashTable) {
            // Time the query operation for hash table only
            steady_clock::time_point startHT = steady_clock::now();
            ht.displayDeathCount(userState, userDisease);
            long long durationHT;
            tock(startHT, "Hash Table search", durationHT);
        } else if (buildRBTree) {
            // Time the query operation for red-black tree only
            steady_clock::time_point startRBT = steady_clock::now();
            rbt.displayDeathCount(userState, userDisease);
            long long durationRBT;
            tock(startRBT, "Red-Black Tree search", durationRBT);
        }
    }
}

/**
 * Main function to execute the program.
 * - Displays menu.
 * - Builds selected data structures.
 * - Processes user queries.
 */
int main() {
    HashTable ht;
    RBTree rbt;

    // Display menu and process user's choice for data structure
    int choice = getUserChoice();

    bool buildHashTable = (choice == 1 || choice == 3);
    bool buildRBTree = (choice == 2 || choice == 3);

    long long buildTimeHT = 0;
    long long buildTimeRBT = 0;

    // Build the selected data structures
    buildDataStructures(ht, rbt, buildHashTable, buildRBTree, buildTimeHT, buildTimeRBT);

    // Display build times and differences if both structures are built
    if (buildHashTable && buildRBTree) {
        cout << "Build time for Hash Table: " << buildTimeHT << " microseconds" << endl;
        cout << "Build time for Red-Black Tree: " << buildTimeRBT << " microseconds" << endl;

        if (buildTimeHT < buildTimeRBT) {
            cout << "Hash Table was faster in building by " << abs(buildTimeHT - buildTimeRBT) <<  " microseconds\n\n";
        } else {
            cout << "Red-Black Tree was faster in building by " << abs(buildTimeRBT - buildTimeHT) <<  " microseconds\n\n";
        }
    }

    // Process user queries
    processUserChoice(ht, rbt, buildHashTable, buildRBTree);

    return 0;
}

