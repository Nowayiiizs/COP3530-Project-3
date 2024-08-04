#pragma once

using namespace std;

// Structure to hold information about a disease in a particular year and state.
struct hashTableVars {
    string disease;       // Name of the disease.
    int year;             // Year of the record.
    int deathCount;       // Number of deaths reported.
    string isMortality;   // Mortality status (e.g., "yes" or "no").

    // Constructor to initialize a disease record.
    hashTableVars(const string& d, int y, int dc, const string& im)
            : disease(d), year(y), deathCount(dc), isMortality(im) {}
};

// Class representing a hash table to store state-disease records.
class HashTable {
private:
    static const int hashGroups = 100;  // Number of hash groups (buckets) in the table.
    vector<list<pair<string, list<hashTableVars>>>> table; // Hash table represented as a vector of lists.

public:
    // Constructor to initialize the hash table with a predefined number of buckets.
    HashTable() {
        table.resize(hashGroups);
    }

    /**
     * Hash function to compute the index for a given key.
     * @param key The key to hash.
     * @return The hash index for the key.
     */
    int hashFunction(const string& key) {
        int hash = 0;
        for (char ch : key) {
            hash += ch;
        }
        return hash % hashGroups;
    }

    /**
     * Insert a new record into the hash table.
     * If the key (state) already exists, update the record if necessary.
     * @param key The key (state) for the record.
     * @param info The record to insert.
     */
    void insertItem(const string& key, const hashTableVars& info) {
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        auto bItr = begin(cell);

        bool keyExists = false;
        for (; bItr != end(cell); bItr++) {
            if (bItr->first == key) {
                keyExists = true;
                for (auto& entry : bItr->second) {
                    if (entry.disease == info.disease && entry.year == info.year && entry.isMortality == info.isMortality) {
                        if (info.deathCount > entry.deathCount) {
                            entry.deathCount = info.deathCount;
                        }
                        // Found the duplicate, no need to add a new entry
                        return;
                    }
                }
                // If no exact match found, add the new info
                bItr->second.push_back(info);
                return;
            }
        }

        if (!keyExists) {
            list<hashTableVars> diseaseList;
            diseaseList.push_back(info);
            cell.emplace_back(key, diseaseList);
        }
    }

    /**
     * Remove a record from the hash table by its key.
     * @param key The key (state) of the record to remove.
     */
    void removeItem(const string& key) {
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        auto bItr = begin(cell);
        bool keyExists = false;

        for (; bItr != end(cell); bItr++) {
            if (bItr->first == key) {
                keyExists = true;
                bItr = cell.erase(bItr);
                cout << "[INFO] Key removed.\n";
                break;
            }
        }

        if (!keyExists) {
            cout << "[WARNING] Key not found. Pair not removed.\n";
        }
    }

    /**
     * Display the death count for a given state and disease.
     * @param state The state to query.
     * @param disease The disease to query.
     */
    void displayDeathCount(const string& state, const string& disease) {
        int hashValue = hashFunction(state);
        auto& cell = table[hashValue];
        auto bItr = begin(cell);
        bool stateExists = false;

        for (; bItr != end(cell); bItr++) {
            if (bItr->first == state) {
                stateExists = true;
                bool diseaseFound = false;
                for (const auto& entry : bItr->second) {
                    if (entry.disease == disease) {
                        cout << "State: " << state << " Disease: " << disease
                             << " Year: " << entry.year
                             << " Death Count: " << entry.deathCount
                             << " Mortality: " << entry.isMortality << "\n";
                        diseaseFound = true;
                    }
                }
                if (!diseaseFound) {
                    cout << "Disease " << disease << " not found in state " << state << ".\n";
                }
                break;
            }
        }

        if (!stateExists) {
            cout << "State " << state << " not found.\n";
        }
    }
};