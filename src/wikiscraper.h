#ifndef WIKISCRAPER_H
#define WIKISCRAPER_H

#include <QtNetwork>
#include <string>
#include <unordered_map>
#include <unordered_set>

class WikiScraper {

public:
    WikiScraper();

    /*
     * TODO: comment
     */
    //std::unordered_set<std::string> findWikiLinks(const std::string& page_content);

    /*
     * Method: getLinkSet
     * Usage: std::unordered_set<std::string> links = getLinkSet(htmlPage);
     * --------------------------------------------------------------------
     * This method returns an unordered set containing all valid links
     * on the given HTML page, according to the specified criterion.
     */
    std::unordered_set<std::string> getLinkSet(const std::string& page_name);

private:
    std::string getPageSource(const std::string& page_name);

    QNetworkAccessManager manager;
    std::unordered_map<std::string, std::string> page_cache;
    std::unordered_map<std::string, std::unordered_set<std::string>> linkset_cache;
};

/* This fix so that we can initalize a QCoreApplication
 * object to use the async networking facilities for
 * the WikiScraper.
 *
 * In particular, we use a QEventLoop, which needs the existence
 * of a QCoreApplication instance in the program.
 */
#undef main
#define main main(int argc, char** argv) { \
                QCoreApplication a(argc, argv); \
                extern int StudentMain(); \
                return StudentMain(); \
            } \
            int StudentMain


#endif // WIKISCRAPER_H