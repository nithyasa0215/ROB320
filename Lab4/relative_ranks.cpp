#include <queue>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm> 


// Given an array of integers score, return an array of strings answer where answer[i] is one of the following:
//   * "Gold" if score[i] is the highest in the list.
//   * "Silver" if score[i] is the second highest in the list.
//   * "Bronze" if score[i] is the third highest in the list.

// Example:
// 1. Input: score = [5, 4, 3, 2, 1]
//    Output: ["Gold", "Silver", "Bronze", "4", "5"]
// 2. Input: score = [10, 3, 8, 9, 4]
//    Output: ["Gold", "5", "Bronze", "Silver", "4"]

class RelativeRanks {
public:
    std::vector<std::string> findRelativeRanks(std::vector<int>& score) {
        // TODO: Implement the function
        // tjis is what we will be returning
        std::vector<std::string> result(score.size());
        // this is the score??
        std::vector<int> sorted_score = score;
        // this is the scores that will be sorted and placed into 
        // I imported the alogoruthm library to help me sort 
        std::sort(sorted_score.rbegin(), sorted_score.rend());
        // this is where the ranked will be sorted into a map 
        std::unordered_map<int, std::string> rank_map;
        
        for (size_t i = 0; i < sorted_score.size(); ++i) {
            // logic to determine the rank
            if (i == 0) rank_map[sorted_score[i]] = "Gold";
            else if (i == 1) rank_map[sorted_score[i]] = "Silver";
            else if (i == 2) rank_map[sorted_score[i]] = "Bronze";
            else rank_map[sorted_score[i]] = std::to_string(i + 1);
        }
        /// this will be where the calculated ranks will now be sorted into the result
        for (size_t i = 0; i < score.size(); ++i) {
            result[i] = rank_map[score[i]];
        }
        
        return result;
    }
};

int main() {
    RelativeRanks rr;
    std::vector<int> score = {5, 4, 3, 2, 1};
    std::vector<std::string> result = rr.findRelativeRanks(score);
    for (std::string s : result) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    score = {10, 3, 8, 9, 4};
    result = rr.findRelativeRanks(score);
    for (std::string s : result) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
    return 0;
}