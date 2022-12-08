//main.cpp
// Project UID db1f506d06d84ab787baf250c265e24e

#include <iostream>
#include "csvstream.h"
#include <utility>
#include <set>
#include <cmath>
#include <cassert>  //assert
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
            const string label = map["tag"];
            const string post = map["content"];
            cout << "label = " << label << ", content = " << post << endl;
        }
    }
    //runs the tests on the test file
    void test(string filename){
        csvstream csvin(filename);
        map<string, string> map;
        while (csvin >> map) {
            const string &post = map["content"];
            // const string &label = map["tag"];
            set<string> unique_word = unique_words(post);
            //call log_prob and those functions here
            cout << most_likely_label(post) << "\n";
        }
      }
    //inserts a new post into the classifier
    void insert(const string label, const string post) {
        set<string> bag = make_bag_of_words(post);
        for (auto &p : bag) {
           allwords[p] += 1;
        }
        labels[label].insert(bag);
        num_posts++;
        num_unique_words = static_cast<int>(allwords.size());
    }

    string most_likely_label(string post) {
        set<string> bag = make_bag_of_words(post);
        string best_label = labels.begin()->first;
        double max = log_prob(bag, labels.begin()->first);
        for (auto &p : labels) {
            if (log_prob(bag, p.first) > max) {
                max = log_prob(post, p.first);
                best_label = p.first;
            }
        }
        return best_label;
    }
    
    //returns the sum of the log probabilities of the post having the label
    double log_prob(set<string> post, string label){
        double sum = log_prob(label);
        for (auto &p : post){
            sum += log_prob(label, p);
        }
        return sum;
    }
    
    //finds the log probability of finding a post with the given label
    //see formula on the spec
    // word is read in map in training data
    // word wasnt seen with label, but was seen in training data
    // No word was seen at all
    double log_prob(string label){
    //get log prob of that label and content
    //go through all the labels in the training and see which ones have the highest probability score
        double np = static_cast<double>(num_posts);
        return log(static_cast<double>(num_posts_label(label))/np);
    }
    
    //finds the log probability of finding a post with the given word based on the given label
    //see formula on the spec
    double log_prob(string label, string word) {
        double np = static_cast<double>(num_posts);
        if (allwords.find(word) == allwords.end()){
            return log(1/np);
        }
        else if (labels[label].words.find(word) == labels[label].words.end()) {
            return log(static_cast<double>(num_posts_word(word))/np);
        }
        else{
            return log(static_cast<double>(num_posts_label_and_word(label, word)) \
                       /static_cast<double>(num_posts_label(label)));
        }
    }
    
    //number of posts in the whole set with word
    int num_posts_word(string word) {
        int total = 0;
        for (auto &p : labels) {
            total += p.second.posts_with_word(word);
        }
        return total;
    }
    
    //number of posts in whole set with label
    int num_posts_label(string label) {
        return labels[label].get_num_posts();
    }
    
    //number of posts with label that contain word
    int num_posts_label_and_word(string label, string word) {
        return labels[label].posts_with_word(word);
    }
    
private:
//    Need to be able to store:
//      -The total number of posts in the entire training set.
//      -The number of unique words in the entire training set. (The vocabulary size.)
//      -For each word, the number of posts in the entire training set that contain .
//      -For each label, the number of posts with that label.
//      -For each label and word, the number of posts with label that contain that word.
    
    //Label which stores a map of all the words in that label
    //and the number of posts they appear in, as well as the
    //total number of posts in the label
    class Label {
        friend Classifier;
    private:
        //map of all words under that label, with the number of posts that contain that word
        map<string,int> words;
        int num_posts;
        
    public:
        Label() {
            num_posts = 0;
        }
//        Label &operator=(const Label &rhs) {
//          if (this == &rhs) {
//            return *this;
//          }
//            num_posts = rhs.num_posts;
//            words = rhs.words;
//          return *this;
//        }
        
        void insert(set<string> post) {
            num_posts++;
            for (auto &p : post) {
                words[p] +=1;
            }
        }

        int posts_with_word (string word) {
            return words[word];
        }
        int get_num_posts() {
            return num_posts;
        }
    };
    //map of all words with number of occurances in the whole set
    map<string, int> allwords;
    
    //map of all labels with a corresponding class Label, which stores a
    //map of all the words in that label and the number of posts they appear
    //in, as well as the total number of posts in the label
    map<string, Label> labels;
//  map of each label with a map of all the words in
//  that set and their number of occurnaces in the label;
//  map<string, map<string, int>> labels;
    
    int num_posts;          //number of posts
    int num_unique_words;   //number of unique words across all posts
};

// void test();
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
        ifstream infile;
        ifstream infile2;
        infile.open(argv[1]);
        infile2.open(argv[2]);
        cout << "enter: 0 for test suite, 1 for standard main funciton" << endl;
        int input;
        cin >> input;
        if (input != 0 && input != 1) {
            cout << "bruh" << endl;
            return -1;
        }
        else if (input == 0) {
            // test();                               
            return 0;
        }
        else if(!infile.is_open()){
            cout << "Error opening file: " << argv[1] << endl;
        }

        else if(!infile2.is_open()){
            cout << "Error opening file: " << argv[2] << endl;
        }

        else{
            string train_file = argv[1];
            string test_file = argv[2];
            Classifier classifier;
            
            cout << "training data:" << "\n";
            classifier.train(train_file);
            csvstream csvin(train_file);
            map<string, string> map;
            int line_counter = 0;
            int vocab_counter = 0;
            while(csvin >> map){
                line_counter += 1;
                cout << map.size() <<  "\n";
                vocab_counter += map.size();
                string label = map["tag"];
                cout << label.size() << "\n";
                vocab_counter += label.size();
            }

            cout << "Trained on " << line_counter << " examples" << "\n";
            cout << "Vocabulary Size = " << vocab_counter << "\n";

            cout << "test data: " << "\n";
            classifier.test(test_file);
            return 0;
         }  

    }
}

// GO TO LINE 229 AND 207 AND REMOVE COMMENT WHEN TEST
// void test() {
//     cout << "testing..." << endl;
//     Classifier classifier;
//     classifier.insert("euchre", "the left bower took the trick");
//     classifier.insert("euchre", "the right bower took the trick");
//     classifier.insert("calculator", "how do you add the first one");
//     assert(classifier.num_posts_word("the") == 3);
//     assert(classifier.num_posts_label("euchre") == 2);
//     assert(classifier.num_posts_label_and_word("euchre", "trick") == 2);
//     assert(classifier.num_posts_label_and_word("euchre", "left") == 1);
//     assert(classifier.most_likely_label("left bower") == "euchre");
//     assert(classifier.most_likely_label("add") == "calculator");
//     assert(classifier.most_likely_label("the") == "euchre");
//     cout << "test passed!!!!!!" << endl;
// }


