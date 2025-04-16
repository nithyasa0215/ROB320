#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Given a list of strings, group anagrams together.
// An Anagram is a word or phrase formed by rearranging the letters of a
// different word or phrase, typically using all the original letters exactly
// once.

// Example:
// 1. Input: ["eat", "tea", "tan", "ate", "nat", "bat"]
//    Output: [["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]

std::vector<std::vector<std::string>> group_anagrams(std::vector<std::string>& strs) {
    // TODO: Implement the function
    std::vector<std::vector<std::string>> result;

    std::map<std::string, std::vector<std::string>> anagram_groups;
    for (const std::string& str : strs) {
        // this will sort the string
        std::string sorted_str = str;
        std::sort(sorted_str.begin(), sorted_str.end());
        // this will add the sorting string back into the map
        anagram_groups[sorted_str].push_back(str);
    }
    
    for (const auto& pair : anagram_groups) {
        // this will add the new one??
        result.push_back(pair.second);
    }

    return result;
}

int main() {
    std::vector<std::string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
    std::vector<std::vector<std::string>> result = group_anagrams(strs);
    for (std::vector<std::string> group : result) {
        for (std::string s : group) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}