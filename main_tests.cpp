//
//  main_tests.cpp
//  p5-ml
//
//  Created by Josh Cussen on 12/4/22.
//

#include "main.cpp"
#include "unit_test_framework.h"

TEST(test_bag_of_words) {
    Classifier classifier;
    string string1 = "the left bower took the trick";
    string string2 = "took took trick the left bower bower";
    set<string> set1 = classifier.make_bag_of_words(string1);
    set<string> set2 = classifier.make_bag_of_words(string2);
    ASSERT_EQUAL(set1, set2);
}

//TEST(test_train) {
//    Classifier classifier;
//    classifier.train("train_small.csv");
//}

TEST_MAIN()

