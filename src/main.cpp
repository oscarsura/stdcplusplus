#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"

using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;

/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    WikiScraper scraper;
    std::unordered_set<std::string> target_set = scraper.getLinkSet(end_page);
    auto comparison = [&scraper, &target_set](std::vector<std::string>& s1, std::vector<std::string>& s2) {
        std::string page_name1 = s1.at(s1.size() - 1);
        std::string page_name2 = s2.at(s2.size() - 1);
        std::unordered_set<std::string> set1 = scraper.getLinkSet(page_name1);
        std::unordered_set<std::string> set2 = scraper.getLinkSet(page_name2);

        std::unordered_set<std::string> set_union;
        std::set_intersection(set1.begin(), set1.end(), target_set.begin(), target_set.end(), std::back_inserter(set_union));
    };
    return {};
}

int main() {
    auto ladder = findWikiLadder("Fruit", "Strawberry");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";

        // Print the ladder here!
    }

    return 0;
}
