#include <vector>
#include <iostream>

void load_scores(std::vector<int>& destination); // CHANGE: Use reference instead of pointer


int main(){
    // Load Scores
    std::vector<int> scores;
    load_scores(scores); // CHANGE: No need to pass address when using reference

    // Handle Empty Vector
    if (scores.empty()) { // IMPROVEMENT: Prevent nullptr dereferencing and division by zero
        std::cerr << "No scores available.\n";
        return 1;
    }

    // Calculate Metrics and Print
    // Declare and Initialize
    int total = 0;
    int count = 0; // IMPROVEMENT: Could use scores.size() instead
    const int* highest_ptr = nullptr;
    const int* lowest_ptr = nullptr;

    for (const int& s : scores) {
        // Update totals
        total += s;
        count++;

        // Compare and Set Highest/Lowest
        if (highest_ptr && lowest_ptr) { // CHANGE: Check both pointers before dereferencing
            if (s > *highest_ptr) {
                highest_ptr = &s;
            }
            if (s < *lowest_ptr) {
                lowest_ptr = &s;
            }
        }
        else { // First value; set both High/Low
            lowest_ptr = highest_ptr = &s;
        }

        // Print Value
        std::cout << "\nScore " << count << ": " << s;
    }

    // Print Metrics (Calc Average)
    // IMPROVEMENT: double provides better precision than float
    auto average = static_cast<double>(total) / count;

    std::cout << "\nTotal: " << total;
    std::cout << "\nAverage: " << average;
    std::cout << "\nHighest: " << *highest_ptr;
    std::cout << "\nLowest: " << *lowest_ptr << '\n';

    return 0; // IMPROVEMENT: Explicit return (optional in main)
}

// IMPROVEMENT: Return Bool rather than void for ack of success
void load_scores(std::vector<int>& destination) { // CHANGE: Reference parameter
    int tot_scores{}; // IMPROVEMENT: Initialize variable

    std::cout << "How many scores do you wish to enter: ";

    // IMPROVEMENT: Validate input and reject zero/negative counts
    if (!(std::cin >> tot_scores) || tot_scores <= 0) {
        std::cerr << "Invalid number of scores.\n";
        return;
    }

    int new_val{};

    for (int i = 0; i < tot_scores; i++) {
        std::cout << "\nEnter Score " << i + 1 << ": ";

        // IMPROVEMENT: Validate individual score input
        if (!(std::cin >> new_val)) {
            std::cerr << "Invalid score.\n";
            return;
        }

        destination.push_back(new_val);
    }
}