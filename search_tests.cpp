#include <gtest/gtest.h>

#include "search.h"
#include "tests/build_index.h"
#include "tests/clean_token.h"
#include "tests/find_query_matches.h"
#include "tests/gather_tokens.h"

/*===================
    cleanToken Tests
  ===================*/
TEST(CleanToken, CleanTokenTests) {
    string expected1 = "this is a test";
    string expected2 = "this is .a test";

    EXPECT_EQ(expected1, cleanToken("...this is a test..."));    // punctuation at both ends
    EXPECT_EQ(expected2, cleanToken("this is .a test"));         // punctuation in middle, and not at either end
    EXPECT_EQ(expected2, cleanToken("...this is .a test"));      // punctuation in middle, and at start (but not end)
    EXPECT_EQ(expected2, cleanToken("this is .a test..."));      // punctuation in middle, and at end (but not start)
}

/*======================
    gatherTokens Tests
  ======================*/
// space(s) at beginning of text
TEST(GatherTokens, LeadingSpaces) {
    string text = "     test";
    set<string> expected = {"test"};
    set<string> studentResult = gatherTokens(text);
    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// space(s) at end of text
TEST(GatherTokens, TrailingSpaces) {
    string text = "test     ";
    set<string> expected = {"test"};
    set<string> studentResult = gatherTokens(text);
    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

// multiple spaces between tokens
TEST(GatherTokens, MultiSpaces) {
    string text = "this     is     a     test";
    set<string> expected = {"this", "is", "a", "test"};
    set<string> studentResult = gatherTokens(text);
    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

/*===================
    buildIndex Tests
  ===================*/
// tested for tiny.txt
TEST(BuildIndex, TinyTxt) {
    string filename = "tiny.txt";

    map<string, set<string>> expected = {
        {"eggs", {"www.shoppinglist.com"}},
        {"milk", {"www.shoppinglist.com"}},
        {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
        {"bread", {"www.shoppinglist.com"}},
        {"cheese", {"www.shoppinglist.com"}},

        {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
        {"green", {"www.rainbow.org"}},
        {"orange", {"www.rainbow.org"}},
        {"yellow", {"www.rainbow.org"}},
        {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
        {"indigo", {"www.rainbow.org"}},
        {"violet", {"www.rainbow.org"}},
        
        {"one", {"www.dr.seuss.net"}},
        {"two", {"www.dr.seuss.net"}},

        {"i'm", {"www.bigbadwolf.com"}},
        {"not", {"www.bigbadwolf.com"}},
        {"trying", {"www.bigbadwolf.com"}},
        {"to", {"www.bigbadwolf.com"}},
        {"eat", {"www.bigbadwolf.com"}},
        {"you", {"www.bigbadwolf.com"}},
    };

    map<string, set<string>> studentIndex;
    int studentNumProcesed = buildIndex(filename, studentIndex);

    string indexTestFeedback = "buildIndex(\"" + filename + "\", ...) index incorrect\n";
    EXPECT_EQ(expected, studentIndex) << indexTestFeedback;
}

// tested for an invalid file
TEST(BuildIndex, InvalidFilenameTest) {
    string filename = "invalidfilename.txt";
    map<string, set<string>> studentIndex;
    
    int studentNumProcesed = buildIndex(filename, studentIndex);
    EXPECT_EQ({}, studentNumProcesed);
}

/*===================
    findQuery Tests
  ===================*/
// first search term not in map
TEST(FindQueryMatches, InvalidFirstTerm) {
    set<string> expected = {"random.org"};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "nonexistent laws"));
}

// later search term modified with '+' not in map
TEST(FindQueryMatches, InvalidLaterPlusTerm) {
    set<string> expected = {};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "hello laws +nonexistent"));
}

// later search term modified with '-' not in map
TEST(FindQueryMatches, InvalidLaterMinusTerm) {
    set<string> expected = {"example.com", "random.org", "uic.edu"};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "hello laws -nonexistent"));
}

// later search term (unmodified) not in map
TEST(FindQueryMatches, InvalidLaterUnmodTerm) {
    set<string> expected = {"example.com", "random.org", "uic.edu"};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "hello laws nonexistent aviation"));
}

