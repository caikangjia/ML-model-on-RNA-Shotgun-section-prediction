#include "BinarySearchTree.h"
#include "unit_test_framework.h"


TEST(test_stub) {
    // Add your tests here
    ASSERT_TRUE(true);
}

TEST(sample){
    BinarySearchTree<int> bst;


}

TEST(empty){
    BinarySearchTree<int> bst;
    ASSERT_TRUE(bst.empty());
}

TEST(insert){
    BinarySearchTree<int> bst;
    int v1 = 5, v2 = 3, v3 = 7;
    bst.insert(v1);
    auto i = bst.insert(v3);
    ASSERT_EQUAL(*i, v3);
    i = bst.insert(v2);
    ASSERT_EQUAL(*i, v2);
    ASSERT_EQUAL((int)bst.height(), 2);
    ASSERT_EQUAL((int)bst.size(), 3);
}

TEST(height){
    BinarySearchTree<int> bst;
    int v1 = 5, v2 = 3, v3 = 7;
    ASSERT_EQUAL((int)bst.height(), 0);
    bst.insert(v2);
    bst.insert(v1);
    bst.insert(v3);
    ASSERT_EQUAL((int)bst.height(), 3);
    bst.insert(-1);
    ASSERT_EQUAL((int)bst.height(), 3);
    bst.insert(0);
    ASSERT_EQUAL((int)bst.height(), 3);
    bst.insert(-2);
    bst.insert(-3);
    ASSERT_EQUAL((int)bst.height(), 4);
    bst.insert(1);
    bst.insert(2);
    ASSERT_EQUAL((int)bst.height(), 5);
}

TEST(size){
    BinarySearchTree<int> bst;
    int v1 = 5, v2 = 3, v3 = 7;
    ASSERT_EQUAL((int)bst.size(), 0);
    bst.insert(v2);
    bst.insert(v1);
    bst.insert(v3);
    ASSERT_EQUAL((int)bst.size(), 3);
    bst.insert(-1);
    ASSERT_EQUAL((int)bst.size(), 4);
    bst.insert(0);
    ASSERT_EQUAL((int)bst.size(), 5);
    bst.insert(-2);
    bst.insert(-3);
    ASSERT_EQUAL((int)bst.size(), 7);
    bst.insert(1);
    bst.insert(2);
    ASSERT_EQUAL((int)bst.size(), 9);
}

TEST(traverse_inorder){
    BinarySearchTree<int> bst;

    std::ostringstream oss_inorder;

    bst.traverse_inorder(oss_inorder);

    ASSERT_EQUAL(oss_inorder.str(), "");

    bst.insert(3);
    bst.insert(2);
    bst.insert(7);
    bst.insert(0);
    bst.insert(9);
    bst.insert(5);
    bst.insert(1);
    bst.insert(-2);

    bst.traverse_inorder(oss_inorder);

    ASSERT_EQUAL(oss_inorder.str(), "-2 0 1 2 3 5 7 9 ");
}

TEST(traverse_preorder){
    BinarySearchTree<int> bst;

    std::ostringstream oss_preorder;

    bst.traverse_preorder(oss_preorder);

    ASSERT_EQUAL(oss_preorder.str(), "");

    bst.insert(3);
    bst.insert(2);
    bst.insert(7);
    bst.insert(0);
    bst.insert(9);
    bst.insert(5);
    bst.insert(1);
    bst.insert(-2);

    bst.traverse_preorder(oss_preorder);

    ASSERT_EQUAL(oss_preorder.str(), "3 2 0 -2 1 7 5 9 ");
}

TEST(check_invariant){
    BinarySearchTree<int> bst;
    auto root_i = bst.insert(3);
    ASSERT_EQUAL(root_i, bst.begin());
    ASSERT_TRUE(*root_i == 3);
    bst.insert(1);
    bst.insert(2);
    bst.insert(12);
    bst.insert(10);
    auto i = bst.insert(8);
    bst.insert(11);

    ASSERT_TRUE(bst.check_sorting_invariant());

    *i = 11;
    ASSERT_FALSE(bst.check_sorting_invariant());
    *i = 13;
    ASSERT_FALSE(bst.check_sorting_invariant());
    *i = 2;
    ASSERT_FALSE(bst.check_sorting_invariant());
    *i = 3;
    ASSERT_FALSE(bst.check_sorting_invariant());
    *i = 4;
    ASSERT_TRUE(bst.check_sorting_invariant());
}

TEST(iterator_basis){
    BinarySearchTree<int> bst;
    auto root_i = bst.insert(3);
    ASSERT_EQUAL(root_i, bst.begin());
    ASSERT_TRUE(*root_i == 3);
    bst.insert(1);
    bst.insert(2);
    bst.insert(12);
    bst.insert(10);
    auto i = bst.insert(8);
    bst.insert(11);

    ASSERT_TRUE(i != root_i);
    ++root_i;
    ASSERT_TRUE(i == root_i);
    ASSERT_EQUAL(*root_i, 8);
    ++++root_i;
    ASSERT_EQUAL(*root_i, 11);
    ++root_i;
    ASSERT_EQUAL(*root_i, 12);
    
}

TEST(iterator_others){
    BinarySearchTree<int> bst;
    auto root_i = bst.insert(3);
    ASSERT_EQUAL(root_i, bst.begin());
    ASSERT_TRUE(*root_i == 3);
    bst.insert(1);
    bst.insert(2);
    bst.insert(12);
    bst.insert(10);
    auto i = bst.insert(8);
    bst.insert(11);

    ASSERT_EQUAL(*bst.min_element(), 1);
    ASSERT_EQUAL(*bst.max_element(), 12);

    BinarySearchTree<int>::Iterator ii;
    ASSERT_TRUE(ii == bst.end());

    ASSERT_EQUAL(i, bst.min_greater_than(7));
    ASSERT_EQUAL(++i, bst.min_greater_than(9));
    ASSERT_EQUAL(i, bst.find(10));


}

TEST(others){
    BinarySearchTree<int> bst;
    auto root_i = bst.insert(3);
    ASSERT_EQUAL(root_i, bst.begin());
    ASSERT_TRUE(*root_i == 3);
    bst.insert(1);
    bst.insert(2);
    bst.insert(12);
    bst.insert(10);
    bst.insert(8);
    bst.insert(11);

    BinarySearchTree<int> bst2;
    bst2 = bst;
    auto r_2 = bst2.begin();
    ++++r_2;
    ASSERT_EQUAL(*r_2, 3);
    while(*r_2 != 12){
        ASSERT_TRUE(*r_2 == *root_i);
        ++r_2;
        ++root_i;
    }        
    ASSERT_TRUE(*r_2 == *root_i);
    *r_2 = 13;
    ASSERT_TRUE(*root_i == 12);

}

TEST_MAIN()
