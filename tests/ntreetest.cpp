#include "ntree.h"
#include <iostream>
#include <string>

int main() {
    ntreeNode<int32_t> * root = new ntreeNode<int32_t>(0);

    add_child(root, 1);
    add_child(root, 2);
    add_child(root, 3);

    ntreeNode<int32_t> * p = root->firstChild;
    add_child(p, 4);
    add_child(p, 5);
    add_child(p, 6);

    p = p->sibling;
    add_child(p, 7);
    add_child(p, 8);

    p = p->sibling;
    add_child(p, 9);
    add_child(p, 10);
    add_child(p, 11);
    add_child(p, 12);

    p = p->firstChild->sibling;
    add_child(p, 13);

    p = root->firstChild->firstChild->sibling;

    add_sibling(p, 14);
    add_child(p, 15);
    add_child(p, 16);

    print_tree(root);

    std::cout << "\n\n\n";

    num_nodes_per_level(root);

    std::cout << "\n\n\n";

    p = root->firstChild->sibling->sibling;

    root = choose_child(root, p);

    print_tree(root);

    std::cout << "\n\n\n";

    num_nodes_per_level(root);

    std::cout << "\n\n\n";

    p = root->firstChild->sibling;

    root = choose_child(root, p);

    print_tree(root);

    std::cout << "\n\n\n";

    num_nodes_per_level(root);

    return 0;
}
