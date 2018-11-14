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
    using cmp_t = std::function<unsigned long(std::vector<std::string>&, std::vector<std::string>&)>;

    WikiScraper scraper;
    std::unordered_set<std::string> target_set = scraper.getLinkSet(end_page);
    cmp_t comparison = [&scraper, &target_set](std::vector<std::string>& s1,
                                              std::vector<std::string>& s2) {
        std::string page_name1 = s1.at(s1.size() - 1);
        std::string page_name2 = s2.at(s2.size() - 1);
        std::unordered_set<std::string> set1 = scraper.getLinkSet(page_name1);
        std::unordered_set<std::string> set2 = scraper.getLinkSet(page_name2);

        std::unordered_set<std::string> set_union1;
        std::unordered_set<std::string> set_union2;
        std::set_intersection(set1.begin(), set1.end(), target_set.begin(), target_set.end(), std::inserter(set_union1,set_union1.begin()));
        std::set_intersection(set2.begin(), set2.end(), target_set.begin(), target_set.end(), std::inserter(set_union1,set_union2.begin()));
        return set_union1.size() < set_union2.size();
    };

    std::priority_queue<std::vector<std::string>, std::vector<std::vector<std::string> >, cmp_t> ladderQueue(comparison);
    ladderQueue.push({start_page});

    std::unordered_set<std::string> visitedLinks;
    while (!ladderQueue.empty()) {
        std::vector<std::string> curr = ladderQueue.top();
        ladderQueue.pop();
        std::unordered_set<std::string> currLinkSet = scraper.getLinkSet(*(curr.end() - 1));

        //end case
        if (std::find(currLinkSet.begin(), currLinkSet.end(), "/wiki/" + end_page) != currLinkSet.end()) {
            curr.push_back(end_page);
            return curr;
        }

        //continue search
        for (std::string str : currLinkSet) {
            if (std::find(visitedLinks.begin(), visitedLinks.end(), str) == visitedLinks.end()) {
                std::vector<std::string> newLadder = curr;
                newLadder.push_back(str);
                ladderQueue.push(newLadder);
                visitedLinks.insert(str);
            }
        }
    }
    return {};
}

const static std::string kLadderNotFound = "No ladder was found!";
const static std::string kLadderFound = "Ladder was from from: ";
const static int kDefaultStatus = 0;
int main() {
    std::vector<std::string> ladder = findWikiLadder("Fruit", "Strawberry");

    if(ladder.empty()) {
       std::cout << kLadderNotFound << std::endl;
    } else {
        std::cout << kLadderFound << std::endl;
        for (auto it = ladder.begin(); it != ladder.end(); ++it) {
            std::cout << *it;
            if ((it + 1) != ladder.end()) std::cout <<" -> ";
        }
        std::cout << std::endl;
    }
    return kDefaultStatus;
}
