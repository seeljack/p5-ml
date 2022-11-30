// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"

using namespace std;

TEST(test_basic) {
    BinarySearchTree<int> tree;
    
    ASSERT_TRUE(tree.empty());
    tree.insert(7);
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);
    tree.insert(2);
    ASSERT_TRUE(tree.size() == 2);
    ASSERT_TRUE(tree.height() == 2);
    tree.insert(1);
    tree.insert(10);
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.height() == 3);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.max_element() == 10);
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.min_greater_than(7) == 10);
   // ASSERT_TRUE(*tree.min_greater_than(1) == 2);
}

TEST(test_copy) {
    BinarySearchTree<int> tree;
    tree.insert(7);
    tree.insert(2);
    tree.insert(1);
    tree.insert(10);
    BinarySearchTree<int> tree2(tree);
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree2.size() == 4);
    ASSERT_TRUE(tree2.height() == 3);
    ASSERT_TRUE(*tree2.min_element() == 1);
    ASSERT_TRUE(*tree2.max_element() == 10);
    ASSERT_TRUE(tree2.check_sorting_invariant());
    ASSERT_TRUE(*tree2.min_greater_than(7) == 10);
 //   ASSERT_TRUE(*tree2.min_greater_than(1) == 2);
}
TEST(test_assignment) {
    BinarySearchTree<int> tree;
    tree.insert(7);
    tree.insert(2);
    tree.insert(1);
    tree.insert(10);
    BinarySearchTree<int> tree2;
    tree2.insert(1);
    tree2.insert(5);
    tree2.insert(9);
    tree2.insert(0);
    tree2 = tree;
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree2.size() == 4);
    ASSERT_TRUE(tree2.height() == 3);
    ASSERT_TRUE(*tree2.min_element() == 1);
    ASSERT_TRUE(*tree2.max_element() == 10);
    ASSERT_TRUE(tree2.check_sorting_invariant());
    ASSERT_TRUE(*tree2.min_greater_than(7) == 10);
    //   ASSERT_TRUE(*tree2.min_greater_than(1) == 2);
}

//TEST(test_stream) {
//ostringstream oss_preorder;
//tree.traverse_preorder(oss_preorder);
//cout << "preorder" << endl;
//cout << oss_preorder.str() << endl << endl;
//ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");
//
//ostringstream oss_inorder;
//tree.traverse_inorder(oss_inorder);
//cout << "inorder" << endl;
//cout << oss_inorder.str() << endl << endl;
//ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
//}

TEST_MAIN()
