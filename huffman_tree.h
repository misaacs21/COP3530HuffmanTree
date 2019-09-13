#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
using namespace std;

class huffman_tree {
	public:
		huffman_tree(const std::string &file_name);
		~huffman_tree();
		
		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;

		struct Node { //node struct for huffman tree
			int character;
			int frequency;

			Node *leftChild = NULL;
			Node *rightChild = NULL;
			Node *parent = NULL;
			bool left; //determines if node is a left child or not

			Node(int chara,int freq, bool leftPos) {
				character = chara;
				frequency = freq;
				left = leftPos;
			}
		};
		struct priorityQueueHelper { //overrides comparison in priorityQueue to compare frequencies and arrange least to greatest
			bool operator()(Node const *node1, Node const *node2) {
				bool lesser = node1->frequency > node2->frequency;
				return lesser;
			}
		};

		std::vector<Node*> planter; //stores all nodes in an easy to access vector for get_character_code
		Node *head; //establishes the head of the tree

};

#endif