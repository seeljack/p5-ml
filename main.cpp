//main.cpp
// Project UID db1f506d06d84ab787baf250c265e24e

#include <iostream>
#include "csvstream.h"
#include <utility>
#include <set>
using namespace std;

class Classifier {
public:
    Classifier();
    set<string> make_bag_of_words(const string &str);
    void train(string filename);
    void test(string filename);
    double log_prob(string label);
    double log_prob(string label, string word);
private:
    //map of all words with number of occurances in the whole set
    map<string, int> words;
    
//  map of each label with a map of all the words in
//  that set and their number of occurnaces in the label;
    map<string, map<string, int>> labels;
    int num_posts;
    int num_unique_words;
    
};
int main (int argc, char * argv[]) {
    cout << argv[0] << argv[1] << argv[2] << argv[3] << argv[4];
    if(argc != 3 && argc != 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }
    if (argc == 4 && strcmp(argv[3],"[--debug]")) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }
    else {
        string train_file = argv[1];
        string test_file = argv[2];
        cout << train_file << " " << test_file << endl;
    }
}

