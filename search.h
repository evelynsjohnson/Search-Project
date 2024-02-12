#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>

// New Libraries
#include <cctype>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

/**
 * Removes leading and trailing punctuation (+ whitespace), converts to lowercase
 */
string cleanToken(string s) {
    int i = 0;
    bool noAlpha = true;

    // Erase all beginning punct
    while (i < s.size()) {
        if (ispunct(s[i]) || isspace(s[i])) {
            ++i;
        }
        else {
            break;
        }
    }
    s = s.substr(i);

    // Find index of last punctuation starting from end
    i = s.size() - 1;
    while (i >= 0) {
        if (ispunct(s[i]) || isspace(s[i])) {
            --i;
        }
        else {
            break;
        }
    }

    // Erase from that index to end
    s = s.substr(0, i + 1);
    for (char& c : s) {
        if (isalpha(c)) {
            c = tolower(c);
            noAlpha = false;
        }
    }

    // Check if no alpha char / empty
    if (s.empty() || noAlpha) {
        return "";
    }
    return s;
}

/**
 * Cleans an entire string of text by looping through cleanToken
 */
set<string> gatherTokens(string text) {
    set<string> tokenizedText;
    stringstream currentWord(text);
    string token;

    while (currentWord >> token) {
        string currentWord = cleanToken(token);
        if (!currentWord.empty()) {
            tokenizedText.insert(currentWord);
        }
    }

    return tokenizedText;
}

/**
 * Takes in filename, reads content, and populates map with an inverted index
 */
int buildIndex(string filename, map<string, set<string>>& index) {
    ifstream inFile(filename);
    set<string> tempGatherTokens;
    string webLink, webText;
    int webProcessed = 0;

    if(!inFile.is_open()){
        cout << "Invalid filename." << endl;
        return 0;
    }

    while (!inFile.eof()) {
        // If successfully read web link and text
        if (getline(inFile, webLink) && getline(inFile, webText)) {
            tempGatherTokens = gatherTokens(webText);

            // Insert web link into the corresponding set for each 'token'
            for (auto token : tempGatherTokens) {
                index[token].insert(webLink);
            }
            webProcessed++;
        }
    }

    inFile.close();
    return webProcessed;
}

/**
 * Runs a search query on the provided index, and returns a set of results.
 * Search terms are processed using set operations (union, intersection, difference).
 */
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    stringstream ss(sentence);
    set<string> finalMatches;
    string currentWord;

    while (ss >> currentWord) {
        // Determine the modifier (+/-/none) and remove it if +/-
        char mod = currentWord[0];
        if (mod == '+' || mod == '-') {
            currentWord = currentWord.substr(1);
        }

        // Clean word and add the set strings of URLS associated with that word
        currentWord = cleanToken(currentWord);
        set<string> termMatches = index[currentWord];

        if (mod == '+') {           // If +, intersect current and new URLS
            set<string> interSet;
            set_intersection(finalMatches.begin(), finalMatches.end(),
                             termMatches.begin(), termMatches.end(),
                             inserter(interSet, interSet.begin()));
            finalMatches = interSet;
        }
        else if (mod == '-') {      // If -, subtract new URLS from current
            set<string> diffSet;
            set_difference(finalMatches.begin(), finalMatches.end(),
                           termMatches.begin(), termMatches.end(),
                           inserter(diffSet, diffSet.begin()));
            finalMatches = diffSet;
        }
        else {                      // If no mods, add new URLS to current
            set<string> unionSet;
            if (finalMatches.empty()) {
                finalMatches = {};
            }

            set_union(finalMatches.begin(), finalMatches.end(),
                      termMatches.begin(), termMatches.end(),
                      inserter(unionSet, unionSet.begin()));
            finalMatches = unionSet;
        }
    }

    return finalMatches;
}

/**
 * Runs the main command loop for the search program
 */
void searchEngine(string filename) {
    map<string, set<string>> index;
    set<string> matchedQueries;
    string queryInput;

    // Print how many web pages were processed to build the index and how many distinct words were found across all pages
    int webPagesProcessed = buildIndex(filename, index);
    int numDistinctWords = index.size();
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << webPagesProcessed << " pages containing " << numDistinctWords << " unique terms" << endl << endl;
    
    while (true) {
        // Prompt the user to enter a query
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, queryInput);
        
        // If the query is the empty string, exit the command loop
        if (queryInput.empty()) {
            break;
        }

        // Find the matching pages, and print the URLs (one on each line)
        matchedQueries = findQueryMatches(index, queryInput);
        cout << "Found " << matchedQueries.size() << " matching pages" << endl;

        for (auto url : matchedQueries) {
            cout << url << endl;
        }
        cout << endl;

    }

    cout << "Thank you for searching!" << endl;
}
