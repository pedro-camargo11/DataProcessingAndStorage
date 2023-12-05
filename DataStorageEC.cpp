#include <iostream>
#include <unordered_map>
#include <stdexcept>

using namespace std;

class InMemoryDB {
private:
    unordered_map<string, int> mainDB;
    unordered_map<string, int> transactionDB;
    bool inTransaction = false;

public:
    void begin_transaction() {
        if (inTransaction) {
            throw runtime_error("Transaction already in progress");
        }
        inTransaction = true;
        transactionDB = mainDB;
    }

    void put(const string& key, int value) {
        if (!inTransaction) {
            throw runtime_error("No transaction in progress");
        }
        transactionDB[key] = value;
    }

    int get(const string& key) {
        if (mainDB.find(key) != mainDB.end()) {
            return mainDB[key];
        }
        return -1;
    }

    void commit() {
        if (!inTransaction) {
            throw runtime_error("No transaction in progress");
        }
        mainDB = transactionDB;
        inTransaction = false;
    }

    void rollback() {
        if (!inTransaction) {
            throw runtime_error("No transaction in progress");
        }
        inTransaction = false;
    }
};

int main() {
    InMemoryDB db;

    try {
        db.begin_transaction();
        db.put("A", 100); //set initial balance of account A
        db.put("B", 50); //set initial balance of account B
        db.commit();
    } catch (const exception& e) {
        cout << "Error during initialization: " << e.what() << endl;
        return 1;
    }

    cout << "Initial Balances:" << endl;
    cout << "Account A: $" << db.get("A") << endl;
    cout << "Account B: $" << db.get("B") << endl;

    try {
        cout << "Starting transaction: Transferring $10 from A to B..." << endl;
        db.begin_transaction();
        db.put("A", db.get("A") - 10); //deduct $10 from account A
        db.put("B", db.get("B") + 10); //add $10 to account B
        db.commit();
        cout << "Transaction completed successfully." << endl;
    } catch (const exception& e) {
        cout << "Error during transaction: " << e.what() << endl;
    }

    cout << "Final Balances:" << endl;
    cout << "Account A: $" << db.get("A") << endl;
    cout << "Account B: $" << db.get("B") << endl;

    return 0;
}
