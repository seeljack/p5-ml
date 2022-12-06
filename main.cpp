//main.cpp
// Project UID db1f506d06d84ab787baf250c265e24e

#include <iostream>
#include "csvstream.h"
#include <utility>
#include <set>
#include <cmath>
using namespace std;

// EFFECTS: Return a set of unique whitespace delimited words.x
set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word) {
    words.insert(word);
  }
  return words;
}




class Classifier {
public:
//default constructor
    Classifier(){
        num_posts = 0;
        num_unique_words = 0;
    }

//returns a set of strings with all unique words
    set<string> make_bag_of_words(const string &str){
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
          words.insert(word);
        }
        return words;
      }
    //trains the classifier with the file from filename
    void train(string filename){
        csvstream csvin(filename);
        map<string, string> map;
        while (csvin >> map) {
            for (auto &p : map) {
                const string &label = p.first;
                const string &post = p.second;
                cout << "label = " << label << ", content = " << post << endl;
            }
        }
    }
    //runs the tests on the test file
    void test(string filename){
        csvstream csvin(filename);
        map<string, string> map;
        while (csvin >> map) {
            const string &post = map["content"];
            const string &label = map["tag"];
            set<string> unique_word = unique_words(post);
            //call log_prob and those functions here
        }
      }

    
    //finds the log probability of finding a post with the given label
    //see formula on the spec
    // word is read in map in training data
    // word wasnt seen with label, but was seen in training data
    // No word was seen at all
    double log_prob(string label){
    //get log prob of that label and content
    //go through all the labels in the training and see which ones have the highest probability score
    }
    
    //finds the log probability of finding a post with the given word based on the given label
    //see formula on the spec
    double log_prob(string label, string word);
    
private:
    //map of all words with number of occurances in the whole set
    map<string, int> words;
    
//  map of each label with a map of all the words in
//  that set and their number of occurnaces in the label;
    map<string, map<string, int>> labels;
    
    int num_posts;          //number of posts
    int num_unique_words;   //number of unique words across all posts
};


int main (int argc, char * argv[]) {
    cout.precision(3);
    if(!(argc == 3 || argc == 4)) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }

    if (argc == 4 && !strcmp(argv[3],"[--debug]")) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }

    else {
        string train_file = argv[1];
        string test_file = argv[2];
        Classifier classifier;
        classifier.train(train_file);
    }
}



