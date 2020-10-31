#include "structures.h"
#include <iostream>

void ll_test() {

    linkedlist<char> ll;

    ll.print();

    ll.insertRight(0, 'a');

    ll.print();

    ll.append('k');

    ll.print();

    ll.append('d');

    ll.print();

    /*std::cout << "index of 0:     " << ll.search(0) << std::endl;
    std::cout << "index of 1:     " << ll.search(1) << std::endl;
    std::cout << "index of 2:     " << ll.search(2) << std::endl;
    std::cout << "length of list: " << ll.len() << std::endl;
     */
    ll.insertRight(2, 'z');

    ll.print();

    ll.insertLeft(1, 'f');

    ll.print();

    for (int i = 1; i < 4; i++) {
        std::cout << "value at index " << i << ": " << ll.get_item(i) << std::endl;
    }

    std::cout << "\nswapping indices 1 and 2...\n";
    ll.swap(1, 2);
    ll.print();

    std::cout << "\nswapping indices 2 and 1...\n";
    ll.swap(2, 1);
    ll.print();

    std::cout << "\nmaximum value in list: " << ll.max();
    std::cout << "\nminimum value in list: " << ll.min();

    std::cout << "\ndeleting head...\n";
    ll.deleteIndex(0);
    ll.print();

    std::cout << "\ndeleting index 2...\n";
    ll.deleteIndex(2);
    ll.print();

    std::cout << "\ndeleting index 2...\n";
    ll.deleteIndex(2);
    ll.print();

    ll.append('k');
    ll.append('y');
    ll.append('l');
    ll.append('i');
    ll.insertLeft(2, 'a');
    ll.insertLeft(6, 'h');
    ll.insertLeft(4, 'q');
    ll.insertLeft(2, 'a');

    ll.print();

    std::cout << "\nlength of list: " << ll.len();

    std::cout << "\nperforming an inertion sort...\n";
    ll.insertionSort();
    ll.print();
}

void dll_test() {

    doublylinkedlist <int> dll;

    dll.print();

    dll.insertRight(0, 0);

    dll.print();

    dll.append(10);

    dll.print();

    dll.append(3);

    dll.print();

    /*std::cout << "index of 0:     " << dll.search(0) << std::endl;
    std::cout << "index of 1:     " << dll.search(1) << std::endl;
    std::cout << "index of 2:     " << dll.search(2) << std::endl;
    std::cout << "length of list: " << dll.len() << std::endl;
     */
    dll.insertRight(2, -2);

    dll.print();

    dll.insertLeft(1, 5);

    dll.print();

    for (int i = 1; i < 4; i++) {
        std::cout << "value at index " << i << ": " << dll.get_item(i) << std::endl;
    }

    std::cout << "\nswapping indices 1 and 2...\n";
    dll.swap(1, 2);
    dll.print();

    std::cout << "\nswapping indices 2 and 1...\n";
    dll.swap(2, 1);
    dll.print();

    std::cout << "\nmaximum value in list: " << dll.max();
    std::cout << "\nminimum value in list: " << dll.min();

    std::cout << "\ndeleting head...\n";
    dll.deleteIndex(0);
    dll.print();

    std::cout << "\ndeleting index 2...\n";
    dll.deleteIndex(2);
    dll.print();

    std::cout << "\ndeleting index 2...\n";
    dll.deleteIndex(2);
    dll.print();

    dll.append(10);
    dll.append(-3);
    dll.append(11);
    dll.append(9);
    dll.insertLeft(2, 0);
    dll.insertLeft(6, 7);
    dll.insertLeft(4, -10);
    dll.insertLeft(2, 0);

    dll.print();

    std::cout << "\nlength of list: " << dll.len();

    std::cout << "\nperforming an insertion sort...\n";
    dll.insertionSort();
    dll.print();

    dll.set_item(0, -1000);
    dll.set_item(3, 1000);
    dll.set_item(9, 5000);

    dll.print();
}

void q_test() {

    queue <int> q;
    int i;

    for (i = 0; i < 5; i++) {
        q.enqueue(i);
    }

    q.print();

    for (i = 0; i < 4; i++) {
        std::cout << "popping " << q.dequeue() << "...\nstate of queue: ";
        q.print();
        std::cout << "next to be popped: " << q.peek() << "\n\n";
    }
}

void s_test() {

    stack<int> s;
    int i;

    for (i = 0; i < 5; i++) {
        s.enstack(i);
    }

    s.print();

    for (i = 0; i < 4; i++) {
        std::cout << "popping " << s.destack() << "...\nstate of stack: ";
        s.print();
        std::cout << "next to be popped: " << s.peek() << "\n\n";
    }
}

void t_test() {

    tree<int> t;

    for (int i = 0; i < 7; i++) {
        t.insert(i);
    }

    std::cout << "level 0: " << t.root->data << "\n";
    std::cout << "level 1: " << t.root->left->data << ", " << t.root->right->data << "\n";
    std::cout << "level 2: " << t.root->left->left->data << ", " << t.root->left->right->data;
    std::cout << ", " << t.root->right->left->data << ", " << t.root->right->right->data << "\n";
}

int main() {

    ll_test();
    dll_test();
    q_test();
    s_test();
    t_test();

    return 0;

}