#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

using namespace std;

// Define possible sizes for packages and slots.
enum class Size { SMALL, MEDIUM, LARGE };

// Package class
class Package {
public:
    string id;
    Size size;

    Package(const string& id, Size size) : id(id), size(size) {}
};

// Slot class: each slot has a capacity, can be occupied, and if allocated stores the package id, OTP, and allocation day.
class Slot {
public:
    int slotID;
    Size capacity; // maximum package size that can fit
    bool occupied;
    string packageID;
    string otp;      // generated OTP code when a package is stored
    int allocationDay; // simulation: day when the package was stored

    Slot(int id, Size capacity)
        : slotID(id), capacity(capacity), occupied(false), packageID(""), otp(""), allocationDay(-1) {}

    // Check if the slot is free and if the package size can be accommodated.
    bool canFit(Size packageSize) {
        if (occupied)
            return false;
        // Define a simple rule: a SMALL package fits in any slot;
        // a MEDIUM package fits in a MEDIUM or LARGE slot;
        // a LARGE package only fits in a LARGE slot.
        if (packageSize == Size::SMALL)
            return true;
        if (packageSize == Size::MEDIUM && (capacity == Size::MEDIUM || capacity == Size::LARGE))
            return true;
        if (packageSize == Size::LARGE && capacity == Size::LARGE)
            return true;
        return false;
    }

    // Allocate the slot with the package.
    void allocatePackage(const Package &pkg, const string &otp, int currentDay) {
        if (occupied)
            throw runtime_error("Slot already occupied");
        packageID = pkg.id;
        occupied = true;
        this->otp = otp;
        allocationDay = currentDay;
    }

    // Empty the slot.
    void emptySlot() {
        packageID = "";
        otp = "";
        allocationDay = -1;
        occupied = false;
    }
};

// Locker class: each locker has an ID, a location, and contains several slots.
class Locker {
public:
    int lockerID;
    string location;
    vector<Slot> slots;

    // The constructor accepts a vector of slot capacities.
    Locker(int id, const string &location, const vector<Size>& slotCapacities)
        : lockerID(id), location(location) {
        int slotID = 1;
        for (auto cap : slotCapacities) {
            slots.push_back(Slot(slotID++, cap));
        }
    }
};

// Receipt class: produced when a package is stored.
class Receipt {
public:
    string packageID;
    int lockerID;
    int slotID;
    string otp;
    string location;

    Receipt(const string& pkg, int lID, int sID, const string& otp, const string& location)
        : packageID(pkg), lockerID(lID), slotID(sID), otp(otp), location(location) {}

    void printReceipt() const {
        cout << "\n--- Receipt ---\n";
        cout << "Package: " << packageID << "\n";
        cout << "Locker: " << lockerID << ", Slot: " << slotID << "\n";
        cout << "OTP: " << otp << "\n";
        cout << "Location: " << location << "\n";
        cout << "----------------\n";
    }
};

// LockerSystem class: manages multiple lockers and provides operations for allocation, retrieval, and cleanup.
class LockerSystem {
private:
    vector<Locker> lockers;
    // Map OTP -> pair {locker index, slot index} for quick lookup during retrieval.
    unordered_map<string, pair<int, int>> otpToLockerSlot;
    int currentDay; // Simulation of the current day.

    // Utility function: generate a random 6-digit OTP.
    string generateOTP() {
        int otpInt = rand() % 900000 + 100000; // random 6-digit number
        return to_string(otpInt);
    }

public:
    LockerSystem(const vector<Locker>& lockers) : lockers(lockers), currentDay(0) {}

    // Advance the simulation day.
    void incrementDay() {
        currentDay++;
    }

    // Use case 1 & 2: Allocate a locker slot that fits the package's size, generate an OTP, and produce a receipt.
    // Returns a dynamically allocated Receipt (for demo purposes).
    Receipt* allocateLocker(const Package &pkg) {
        for (size_t i = 0; i < lockers.size(); i++) {
            Locker &locker = lockers[i];
            for (size_t j = 0; j < locker.slots.size(); j++) {
                Slot &slot = locker.slots[j];
                if (slot.canFit(pkg.size)) {
                    string otp = generateOTP();
                    slot.allocatePackage(pkg, otp, currentDay);
                    otpToLockerSlot[otp] = { static_cast<int>(i), static_cast<int>(j) };
                    // Create and return a receipt containing locker id, slot id, OTP, and location.
                    Receipt* receipt = new Receipt(pkg.id, locker.lockerID, slot.slotID, otp, locker.location);
                    cout << "Package " << pkg.id << " stored in Locker " << locker.lockerID
                         << ", Slot " << slot.slotID << ". OTP: " << otp << "\n";
                    return receipt;
                }
            }
        }
        cout << "No available locker slot fits package " << pkg.id << "\n";
        return nullptr;
    }

    // Use case 3: Validate the OTP and (if valid) open and empty the slot.
    bool validateOTP(const string &otp) {
        if (otpToLockerSlot.find(otp) != otpToLockerSlot.end()) {
            auto [lockerIndex, slotIndex] = otpToLockerSlot[otp];
            Locker &locker = lockers[lockerIndex];
            Slot &slot = locker.slots[slotIndex];
            // For simulation, "opening" means emptying the slot.
            slot.emptySlot();
            otpToLockerSlot.erase(otp);
            cout << "Locker " << locker.lockerID << " Slot " << slot.slotID << " opened and emptied.\n";
            return true;
        }
        cout << "Invalid OTP provided.\n";
        return false;
    }

    // Use case 5: Clear any package that has been in a locker for 3 days.
    void clearExpiredSlots() {
        for (size_t i = 0; i < lockers.size(); i++) {
            Locker &locker = lockers[i];
            for (size_t j = 0; j < locker.slots.size(); j++) {
                Slot &slot = locker.slots[j];
                if (slot.occupied && (currentDay - slot.allocationDay >= 3)) {
                    cout << "Clearing expired package " << slot.packageID << " from Locker "
                         << locker.lockerID << " Slot " << slot.slotID << "\n";
                    if (!slot.otp.empty()) {
                        otpToLockerSlot.erase(slot.otp);
                    }
                    slot.emptySlot();
                }
            }
        }
    }

    // For debugging: print the current state of the locker system.
    void printSystemStatus() {
        cout << "\n=== Locker System Status (Day " << currentDay << ") ===\n";
        for (auto &locker : lockers) {
            cout << "Locker " << locker.lockerID << " (" << locker.location << "):\n";
            for (auto &slot : locker.slots) {
                cout << "  Slot " << slot.slotID << " (";
                if (slot.capacity == Size::SMALL)
                    cout << "Small";
                else if (slot.capacity == Size::MEDIUM)
                    cout << "Medium";
                else if (slot.capacity == Size::LARGE)
                    cout << "Large";
                cout << ") - ";
                if (slot.occupied) {
                    cout << "Occupied, Package: " << slot.packageID
                         << ", OTP: " << slot.otp
                         << ", Allocated Day: " << slot.allocationDay;
                } else {
                    cout << "Empty";
                }
                cout << "\n";
            }
        }
        cout << "============================================\n";
    }
};

//
// Main function demonstrating use cases:
// 1. Allocate a locker slot for a package based on size.
// 2. Generate and send an OTP and locker location (via Receipt).
// 3. Validate an OTP to retrieve a package.
// 4. Empty a locker after use (via OTP validation).
// 5. Automatically empty a locker after 3 days.
//
int main() {
    srand(static_cast<unsigned int>(time(0)));  // seed random generator

    // Create two lockers with different slot configurations:
    // Locker 1: Slots with capacities: Small, Medium, and Large.
    // Locker 2: Slots with capacities: Medium, Medium, and Large.
    Locker locker1(1, "Location A", {Size::SMALL, Size::MEDIUM, Size::LARGE});
    Locker locker2(2, "Location B", {Size::MEDIUM, Size::MEDIUM, Size::LARGE});
    vector<Locker> lockers = {locker1, locker2};

    // Initialize the locker system.
    LockerSystem system(lockers);

    // Allocate a package of SMALL size.
    Package pkg1("PKG001", Size::SMALL);
    Receipt* receipt1 = system.allocateLocker(pkg1);
    if (receipt1)
        receipt1->printReceipt();

    // Allocate a package of MEDIUM size.
    Package pkg2("PKG002", Size::MEDIUM);
    Receipt* receipt2 = system.allocateLocker(pkg2);
    if (receipt2)
        receipt2->printReceipt();

    // Allocate a package of LARGE size.
    Package pkg3("PKG003", Size::LARGE);
    Receipt* receipt3 = system.allocateLocker(pkg3);
    if (receipt3)
        receipt3->printReceipt();

    // Print current system status.
    system.printSystemStatus();

    // Use case 3 & 4: Validate the OTP to retrieve (open and empty) the locker for pkg2.
    if (receipt2) {
        cout << "\nUser presents OTP " << receipt2->otp << " to retrieve package " << pkg2.id << "\n";
        system.validateOTP(receipt2->otp);
    }
    system.printSystemStatus();

    // Simulate passage of days.
    cout << "\nSimulating 3 days passing...\n";
    for (int i = 0; i < 3; i++) {
        system.incrementDay();
    }

    // Use case 5: Clear any expired slots (packages stored for 3 or more days).
    system.clearExpiredSlots();
    system.printSystemStatus();

    // Clean up dynamically allocated receipts.
    delete receipt1;
    delete receipt2;
    delete receipt3;

    return 0;
}
