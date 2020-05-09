#include <iostream> 
#include <map> 

class Category; 
class Source; 


/* very rough bash of the 2019 q1, worth noting that youd 100%
 * be wanting to use a vector or anything with an iterator with the globals, 
 * raw arrays not pleasant */


class Category {
private :
    Category *parent; 
    std::string name;
    ~Category(){} //no idea is this is good for 'never be deleted'

public: 
    Category(std::string name_,  Category *parent_ = nullptr): parent(parent_), name(name_) {}

    std::string getName()const  {
            return name; 
    }

    void setParent(Category *parent_) {
        parent = parent_; 
    }
    
        
}; 

class NewsItem {
private: 
    std::string title; 
    std::string content; 
    std::string authorName; 
    const Category &category; 
    int viewcount; 

public:
    NewsItem(std::string title_, std::string content_, std::string authorname_, const Category &category_) :
                        title(title_), content(content_), authorName(authorname_), category(category_) {
                                viewcount = 0; 
                        }


    std::string getTitle() {
        return title; 
    }

    std::string getContent() {
        return content; 
    }

    std::string getName() {
        return authorName; 
    }

    int getViewCount() {
            return viewcount; 
    }

    std::string getCategoryName() const {
        return category.getName(); 
    }

    void increment() {
            viewcount++; 
    }

}; 




class Source {
private: 
    static const int MAX = 1000; 
    NewsItem **items; 
    std::string url; 

public: 
    Source(std::string url_): url(url_) {
        items = new NewsItem*[MAX]; 
    }

    ~Source() {
        for (int i = 0; i < MAX; i++) {
                if (items[i]) {
                    delete items[i]; 
                }
        }
        delete items; 
    }


    void addItem(NewsItem *item) {
            for (int i = 0; i < 1000; i++) {
                    if (items[i] == nullptr) {
                            items[i] = item; 
                    }
            }
                
    }

    NewsItem *findItem(const std::string &title) {
        for (int i = 0; i < 1000; i++) {
                if (items[i] && items[i]->getTitle() == title) {
                        return items[i]; 
                }
        }

        return nullptr; 
    }

    NewsItem **getItems() {
            return items; 
    }

    std::string getUrl() {
            return url; 
    }


}; 

//tbh probably cleanest to 
//make a struct and have an instance of it
//as the sole global variable
//and then could refactor the aux funcs as members

const int MAXSource = 100; 
const int MAXCat = 500; 
Source **sources = new Source*[MAXSource]; 
Category **categories = new Category*[MAXCat]; 


/* START aux funcs */

void addCategory(Category *category) {
    for (int i = 0; i < MAXCat; i++) {
            if (!categories[i]) {
                    categories[i] = category; 
            }
    }
}

Category *findCategory(std::string name) {
    for (int i = 0; i < MAXSource; i++) {
            if (categories[i] && categories[i]->getName() == name) {
                    return categories[i]; 
            }
    }
    return nullptr; 
}

Source *findSource(const std::string &url) {
    for (int i = 0; i < MAXSource; i++) {
            if (sources[i] && sources[i]->getUrl() == url){
                return sources[i]; 
            }
    }
    return nullptr; 
}

/* END auc funcs */ 


//b 
NewsItem *createNewsItem(const std::string &title, const std::string &content, 
                         const std::string &author, const std::string &categoryName,
                         const std::string &sourcePage) {

    Category *found = findCategory(categoryName); 
    Source *source = findSource(sourcePage); 

    if (!found) {
        found = new Category(categoryName); 
        addCategory(found); 
    }

    //not paying attention idk if theyd want us to make one if it doesnt exist
    if (!source) {
            return nullptr; 
    }

    NewsItem *item = new NewsItem(title, content, author, *found); 

    source->addItem(item); 

    return item; 
}

//c
std::string getNewsContent(const std::string &url, const std::string &title) {
    Source* source = findSource(url); 

    if (!source) {
            return NULL; 
    }

    NewsItem *item =  source->findItem(title); 

    if (item) {
            item->increment(); 
            return item->getContent(); 
    }

    return NULL; 
}


//d
std::string getHottestCategory() {

    std::map<std::string, int> scores; 

    //for every source, iterate through entire
    //newsitem array to count clicks. 
    //it is at this point that its quite obvious an stl
    //data structure which has an iterator would be much more pleasant
    //than plain arrays but watever
    for (int i = 0; i < MAXSource; i++) {

        if (sources[i]) {
            NewsItem **items = sources[i]->getItems(); 

            if (items) {
                for (int i = 0; i < 1000; i++) {
                        if (items[i]) {

                            auto name = items[i]->getCategoryName(); 
                            auto count = items[i]->getViewCount(); 

                            if (scores.find(name) != scores.end()) {
                                        scores[name] = scores[name] + count; 
                            } else {
                                scores.insert(std::make_pair(name, count)); 
                            }
                        }
                }
            }
        }

    }

    int max = 0; 
    std::string current; 
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        if (it->second > max) {
                max = it->second; 
                current = it->first; 
        }
    }
    return current; 
}


int main() {

    Category *cat1 = new Category("Horror"); 
    Category *cat2 = new Category("Clickbait trash"); 
    Category *cat3 = new Category("Educational"); 

    categories[0] = cat1; 
    categories[1] = cat2; 
    categories[2] = cat3; 

    Source *source1 = new Source("bbc.com"); 
    Source *source2 = new Source("www.trashclickbait.com"); 
    sources[0] = source1; 
    sources[1] = source2; 

    std::string mindlessWaffle1 = "Hello this is just some dumb bs that"
                                    "im using to fill in some dumb"
                                    "imaginary website"; 
    std::string mindlessWaffle2 = "peter piper picked a peck of picked peppers "
                                    "where is the peck of pickled peppers that peter piper picked?"; 

    createNewsItem("not dumb", mindlessWaffle1, "me", "Educational", "bbc.com"); 
    createNewsItem("Peters Journey", mindlessWaffle2, "Also me", "Horror", "www.trashclickbait.com"); 

    std::cout << getNewsContent("bbc.com", "not dumb") << "\n"; 
    std::cout << getNewsContent("bbc.com", "not dumb") << "\n"; 
    std::cout << getNewsContent("www.trashclickbait.com", "Peters Journey") << "\n"; 
    std::cout << getNewsContent("www.trashclickbait.com", "Peters Journey") << "\n"; 
    std::cout << getNewsContent("www.trashclickbait.com", "Peters Journey") << "\n"; 
    std::cout << getNewsContent("www.trashclickbait.com", "Peters Journey") << "\n"; 

    auto hottest = getHottestCategory();
    std::cout << "Hottest is: " << hottest << "\n"; 

    
    return 0; 

}
