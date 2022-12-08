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
    void train(string filename, bool debug){
        csvstream csvin(filename);
        map<string, string> map;
        if (debug) {
            cout << "training data:" << "\n";
        }
        while (csvin >> map) {
            const string label = map["tag"];
            const string post = map["content"];
            insert(label, post);
            if (debug) {
                cout << "  label = " << label << ", content = " << post << endl;
            }
        }
        
//        std::map<string, string> map2;
//        int line_counter = 0;
//        int vocab_counter = 0;
//        while(csvin >> map){
//            line_counter += 1;
//            cout << map2.size() <<  "\n";
//            vocab_counter += map2.size();
//            string label = map2["tag"];
//            cout << label.size() << "\n";
//            vocab_counter += label.size();
//        }

        cout << "trained on " << num_posts << " examples" << "\n";
        if (debug) {
            cout << "vocabulary size = " << num_unique_words << "\n";
        }
        cout << "\n";
    }

    //runs the tests on the test file
//    test data:
//      correct = euchre, predicted = euchre, log-probability score = -13.7
//      content = my code segfaults when bob is the dealer
//
//      correct = euchre, predicted = calculator, log-probability score = -12.5
//      content = no rational explanation for this bug
//
//      correct = calculator, predicted = calculator, log-probability score = -13.6
//      content = countif function in stack class not working
//
//    performance: 2 / 3 posts predicted correctly
    void test(string filename){
        cout << "test data:" << "\n";
        int posts = 0;
        int correct = 0;
        csvstream csvin(filename);
        map<string, string> map;
        while (csvin >> map) {
            const string &post = map["content"];
            set<string> bag = make_bag_of_words(post);
            const string &label = map["tag"];
            cout << "  correct = " << label << ", predicted = ";
            cout << most_likely_label(post) << ", log-probability score = ";
            cout << log_prob(bag, most_likely_label(post)) << endl;
            cout << "  content = " << post << endl << endl;
            posts++;
            if (label == most_likely_label(post)) {
                correct++;
            }
        }
        cout << "performance: " << correct << " / " << posts;
        cout << " posts predicted correctly" << endl;
      }
    
//classes:
//  calculator, 3 examples, log-prior = -0.981
//  euchre, 5 examples, log-prior = -0.47
//   classifier parameters:
//   calculator:assert, count = 1, log-likelihood = -1.1
    void debug(bool d) {
        if (d) {
            cout << "classes:" << endl;
            for (auto &p : labels) {
                cout << "  " << p.first << ", " << p.second.num_posts;
                cout << " examples, log-prior = " << log_prob(p.first) << endl;
            }
            cout << "classifier parameters:" << endl;
            for (auto &p : labels) {
                for (auto &q : p.second.words) {
                    cout << "  " << p.first << ":" << q.first << ", count = ";
                    cout << q.second << ", log-likelihood = ";
                    cout << log_prob(p.first, q.first) << endl;
                }
            }
            cout << endl;
        }
        else {
            return;
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
    
    //finds the log probability of finding a post
    //with the given label
    //see formula on the spec
    // word is read in map in training data
    // word wasnt seen with label, but was seen in training data
    // No word was seen at all
    double log_prob(string label){
    //get log prob of that label and content
    //go through all the labels in the training and
    //see which ones have the highest probability score
        double np = static_cast<double>(num_posts);
        double npl = static_cast<double>(num_posts_label(label));
        double aw = allwords[label];
        if(npl != 0){
            return log(npl/np);
        }
        else if(aw != 0){
        //    (Use when  does not occur in posts labeled  but does occur in the training data overall.)
                return log(log(aw/np));
        }
        else{
            return log(1/np);
        }

    }
    
    //finds the log probability of finding a post with the given word
    //based on the given label
    //see formula on the spec
    double log_prob(string label, string word) {
        double np = static_cast<double>(num_posts);
        if (allwords.find(word) == allwords.end()){
            return log(1.0/np);
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
        //map of all words under that label, with the
        //number of posts that contain that word
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

void tests();
int main (int argc, char * argv[]) {
    cout.precision(3);
    
//    cout << "enter: 0 for test suite, 1 for standard main funciton" << endl;
//    int input;
//    cin >> input;
//    if (input != 0 && input != 1) {
//        cout << "bruh" << endl << endl;
//        return -1;
//    }
//    else if (input == 0) {
//        tests();
//        return 0;
//    }
    
    
    bool debug = false;
    if(!(argc == 3 || argc == 4)) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }

    if (argc == 4) {
        if(!strcmp(argv[3],"[--debug]")) {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return -1;
        }
        else {
            debug = true;
        }
    }
    ifstream infile;
    ifstream infile2;
    infile.open(argv[1]);
    infile2.open(argv[2]);
    
    if(!infile.is_open()){
        cout << "Error opening file: " << argv[1] << endl;
    }
    
    else if(!infile2.is_open()){
        cout << "Error opening file: " << argv[2] << endl;
    }
    
    string train_file = argv[1];
    string test_file = argv[2];
    Classifier classifier;
    classifier.train(train_file,debug);
    classifier.debug(debug);
    classifier.test(test_file);
    return 0;
}

 //GO TO LINE 229 AND 207 AND REMOVE COMMENT WHEN TEST
 void tests() {
     cout << "testing. . ." << endl;
     Classifier classifier;
     classifier.insert("euchre", "the left bower took the trick");
     classifier.insert("euchre", "the right bower took the trick");
     classifier.insert("calculator", "how do you add the first one");
     assert(classifier.num_posts_word("the") == 3);
     assert(classifier.num_posts_label("euchre") == 2);
     assert(classifier.num_posts_label_and_word("euchre", "trick") == 2);
     assert(classifier.num_posts_label_and_word("euchre", "left") == 1);
     assert(classifier.most_likely_label("left bower") == "euchre");
     assert(classifier.most_likely_label("add") == "calculator");
     assert(classifier.most_likely_label("the") == "euchre");
     
//    classifier.insert("euchre","can the upcard ever be the left bower");
//    classifier.insert("euchre","when would the dealer ever prefer a card \
     to the upcard");
//    classifier.insert("euchre","bob played the same card twice is he cheating");
//    classifier.insert("euchre","how to remove a card from players hand");
//    classifier.insert("euchre","anyone want to play some euchre");
//    classifier.insert("calculator","how to assert rational invariants");
//    classifier.insert("calculator","does stack need its own big three");
//    classifier.insert("calculator","valgrind memory error not sure what it means");
//     set<string> bag = classifier.make_bag_of_words("my code segfaults when\
     bob is the dealer");
//     cout << classifier.log_prob(bag, "euchre") << endl;
//     bag = classifier.make_bag_of_words("no rational explanation for this bug");
//     cout << classifier.log_prob(bag, "calculator") << endl;
//     bag = classifier.make_bag_of_words("countif function in stack class not \
     working");
//     cout << classifier.log_prob(bag, "calculator") << endl;
     
     
     cout << "test passed!!!!!!" << endl << endl;
 }


