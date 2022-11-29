// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"

using namespace std;

TEST(test_basic) {
    BinarySearchTree<int> tree;
    
    tree.insert(7);
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);
    tree.insert(2);
    ASSERT_TRUE(tree.size() == 2);
    ASSERT_TRUE(tree.height() == 2);
    tree.insert(1);
    tree.insert(10);
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.height() == 3);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.max_element() == 10);
}

TEST_MAIN()
