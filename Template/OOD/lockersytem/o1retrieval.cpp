std::unordered_map<Size, std::vector<std::pair<int, int>>> freeSlots;

// For a package of size MEDIUM, acceptable capacities are MEDIUM and LARGE.
std::vector<Size> validCapacities = { Size::MEDIUM, Size::LARGE };
for (auto cap : validCapacities) {
    if (!freeSlots[cap].empty()) {
        auto [lockerIndex, slotIndex] = freeSlots[cap].back();
        freeSlots[cap].pop_back();
        // Allocate the package in lockers[lockerIndex].slots[slotIndex]
        // And update OTP mapping, etc.
        break;
    }
}
