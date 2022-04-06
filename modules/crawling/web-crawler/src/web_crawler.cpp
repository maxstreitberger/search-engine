#ifndef WEB_CRAWLER_CPP
#define WEB_CRAWLER_CPP

#include "web_crawler.hpp"
#include "doc_store.hpp"

void WebCrawler::start() {
    LOG(INFO) << "Start web crawler";
    bool isValidURL = checkIfURL(&origin_path);

    if (isValidURL) {
        std::queue<std::string> urls;
        urls.push(origin_path);
        int counter_visited_pages = 0;
        while (!urls.empty() && (counter_visited_pages != depth_limit)) {
            LOG(INFO) << "Crawl url: " << urls.front();

            if (!checkIfURL(&urls.front())) {
                urls.pop();
                continue;
            }

            std::string htmlDoc = getHTML(urls.front());
            already_visited_pages.push_back(urls.front());
            getURLs(htmlDoc, &urls);
            registerPage(urls.front(), htmlDoc);
            counter_visited_pages += 1;
            urls.pop();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
        store.processDocuments();
    }
}

bool WebCrawler::checkIfURL(const std::string* url) {
    std::regex url_regex("https?://(www.)?[-a-zA-Z0-9@:%._+~#=]{2,256}.[a-z]{2,4}([-a-zA-Z0-9@:%_+.~#?&//=]*)");
    if (std::regex_match(*url, url_regex)) {
        LOG(INFO) << "Valid URL entered: " << *url;
        return true;
    } else {
        LOG(ERROR) << "Not a valid URL entered: '" << *url << "'";
        return false;
    }   
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string WebCrawler::getHTML(std::string url) {
    LOG(INFO) << "Get HTML page";
    CURL * curl;
    CURLcode res;
    std::string buffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return buffer;
}

void WebCrawler::search_for_links(GumboNode* node, std::queue<std::string>* urls) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    GumboAttribute* href;
    if (node->v.element.tag == GUMBO_TAG_A && (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
        std::string value = href->value;
        LOG(INFO) << "Found url: " << value;
        if (value[0] == '/' && value[1] == '/') {
            std::string url = protocol + value.erase(0, 2);
            LOG(INFO) << "Add url: " << url;
            if (checkIfURLWasAlreadyVisited(&url) == false) urls->push(url);
        } else if (value[0] == '/') {
            std::string url = base_url + value;
            LOG(INFO) << "Add url: " << url;
            if (checkIfURLWasAlreadyVisited(&url) == false) urls->push(url);
        }
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_links(static_cast<GumboNode*>(children->data[i]), urls);
    }
}

bool WebCrawler::checkIfURLWasAlreadyVisited(std::string* url) {
    int url_size = url->size();
    
    if ((*url)[url_size - 1] == '/') {
        (*url).pop_back();
    }

    if (std::find(already_visited_pages.begin(), already_visited_pages.end(), *url) != already_visited_pages.end()) {
        LOG(INFO) << *url << " already visited";
        std::cout << *url << " already visited" << std::endl;
        return true;
    } else {
        LOG(INFO) << *url << " not visited yet";
        std::cout << *url << " not visited yet" << std::endl;
        return false;
    }
}

void WebCrawler::extractBaseURL(std::string* url) {
    std::size_t found = url->find('/');
    if (found != std::string::npos) {
        protocol = url->substr(0, found + 2);
        base_url = url->substr(found + 2);
    }

    found = base_url.find('/');
    if (found != std::string::npos) {
        base_url = base_url.substr(0, found);
    }
    base_url = protocol + base_url;
}

void WebCrawler::getURLs(std::string htmlDoc, std::queue<std::string>* urls) {
    LOG(INFO) << "Get URLs";
    GumboOutput* output = gumbo_parse(htmlDoc.c_str());
    
    search_for_links(output->root, urls);
    
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

std::string WebCrawler::removeTags(std::string htmlDoc) {
    LOG(INFO) << "Remove HTML tags";
    GumboOutput* output = gumbo_parse(htmlDoc.c_str());
    std::string text = cleanText(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return text;
}

std::string WebCrawler::cleanText(GumboNode* node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_SCRIPT && node->v.element.tag != GUMBO_TAG_STYLE) {
        std::string contents = "";
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = cleanText((GumboNode*) children->data[i]);
            if (i != 0 && !text.empty()) {
                contents.append(" ");
            }
            contents.append(text);
        }
        return contents;
    } else {
        return "";
    }
}

void WebCrawler::registerPage(std::string url, std::string htmlDoc) {
    int new_id = pages->size() + 1;
    
    std::string content = removeTags(htmlDoc);
    docmeta::DocumentMeta page = docmeta::DocumentMeta(new_id, content, url);
    LOG(INFO) << "Page to register: " << page;

    std::set<docmeta::DocumentMeta>::iterator it = std::find_if(pages->begin(), pages->end(), [&page](const docmeta::DocumentMeta pg) {
        LOG(INFO) << "Page with path = " << page.path << " was already crawled"; 
        return pg.path == page.path;
    });

    if (it != pages->end()) {
        if ((it->content) != (page.content)) {
            page.id = it->id;
            pages->erase(it);
        }
    }

    pages->insert(page);
    LOG(INFO) << "Successfully registered page: " << page;
}

#endif