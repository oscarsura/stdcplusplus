#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include "error.h"
#include "wikiscraper.h"

static const std::string sentinel = "/wiki/";
std::unordered_set<std::string> WikiScraper::findWikiLinks(const std::string& page_content) {
    std::string html = page_content;
    std::istringstream iss(html);

    std::string line;
    while (iss >> line) {
        std::cout << line << std::endl;
    }
    std::cout << "should have printed the whole file" << std::endl;
    return {};
}

std::unordered_set<std::string> WikiScraper::getLinkSet(const std::string& page_name) {
    if(linkset_cache.find(page_name) == linkset_cache.end()) {
        auto links = findWikiLinks(getPageSource(page_name));
        linkset_cache[page_name] = links;
    }
    return linkset_cache[page_name];
}

WikiScraper::WikiScraper() {
    (void)getPageSource("Main_Page");
#ifdef _WIN32
    // define something for Windows
    system("cls");
#else
    // define it for a Unix machine
    system("clear");
#endif
}

std::string WikiScraper::createPageUrl(const std::string& page_name) {
    return "https://en.wikipedia.org/wiki/" + page_name;
}

void WikiScraper::notFoundError(const std::string& msg, const std::string& page_name, const std::string& url) {
    const std::string title = "    AN ERROR OCCURED DURING EXECUTION.    ";
    const std::string border(title.size() + 4, '*');
    std::cerr << std::endl;
    errorPrint(border);
    errorPrint("* " + title + " *");
    errorPrint(border);
    errorPrint();
    errorPrint("Reason: " + msg);
    errorPrint();
    errorPrint("Debug Information:");
    errorPrint();
    errorPrint("\t- Input parameter: " + page_name);
    errorPrint("\t- Attempted url: " + url);
    errorPrint();
}

std::string WikiScraper::getPageSource(const std::string& page_name) {
    const static std::string not_found = "Wikipedia does not have an article with this exact name.";
    if(page_cache.find(page_name) == page_cache.end()) {
        QUrl url(createPageUrl(page_name).c_str());

        QNetworkRequest request(url);
        QNetworkReply *reply(manager.get(request));
        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()) , &loop, SLOT(quit()));
        loop.exec();

        std::string ret = reply->readAll().toStdString();
        if(std::search(ret.begin(), ret.end(), not_found.begin(), not_found.end()) != ret.end()){
            notFoundError("Page does not exist!", page_name, url.toString().toStdString());
            return "";
        }
        size_t indx = ret.find("plainlinks hlist navbar mini");
        if(indx != std::string::npos) {
            return ret.substr(0, indx);
        }
        page_cache[page_name] = ret;
        std::cout << ret << std::endl;
    }
    return page_cache[page_name];
}
