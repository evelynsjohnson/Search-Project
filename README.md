# Search-Project
1. Main code functions, the two main functions are the buildIndex and the findQueryMatches Function.Handling movement and object places
   Defines a set of classes representing different types of cells on a game board for a maze-like game, each handling its specific behavior, such as movement, display, and categorization on the game board
   
   BuildIndex Function:
      - Takes a filename as input, reads content from the file (assuming it contains web link and text pairs), and populates a map with an inverted index.
      - The inverted index associates each cleaned token with a set of web links.
      - Returns the number of web pages processed.
    
    FindQueryMatches Function:
      - Takes an inverted index and a search query sentence as input.
      - Processes the search query using set operations (union, intersection, difference) on the index.
      - Returns a set of web links that match the search query.
        
2. Creating test cases (search_test.cpp), testing for null inputs, non-alpha characters, incorrect inputs or filenames, and other edge case.
