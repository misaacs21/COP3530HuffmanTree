#include "huffman_tree.h"

using namespace std;

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/
huffman_tree::huffman_tree(const std::string &file_name){
    int frequencies[128] = {0}; //full ascii table
    priority_queue<Node*, vector<Node*>, priorityQueueHelper> priQueue; //priority queue with comparison override
    ifstream in(file_name); //read document...referenced from tests.cpp
    string document((istreambuf_iterator<char>(in)), istreambuf_iterator<char>()); //convert document to string...referenced from tests.cpp

    for (int i=0; i < (int)document.length(); i++) { //goes through string and increments frequencies of corresponding ascii character
        int asciiChar = (int)document[i];
        frequencies[asciiChar] += 1;
    }


    for (int i=0; i< 128; i++) { //if the character exists in the string, make a node for it and add to priority queue + vector
        if(frequencies[i] != 0) {
            Node* currNode = new Node(i,frequencies[i],false);
            priQueue.push(currNode);
            planter.push_back(currNode);
        }
    }

    if(priQueue.size() == 1) { //if there's only one letter, make that the head and exit huffman_tree
        head = priQueue.top();
    }

    else { //otherwise...
        for (int i =0; priQueue.size() > 1; i++) { //until only the head of the tree is in the priority queue
            Node* parentNode = new Node(129,0,false); //create parentNode with junk ascii character
            int freqOne = priQueue.top()->frequency; //take smallest frequency character
            priQueue.top()->parent = parentNode; //link it to parentNode and note that it's a left child
            priQueue.top()->left = true;
            parentNode->leftChild = priQueue.top();

            priQueue.pop();

            int freqTwo = priQueue.top()->frequency; //take second smallest frequency character
            priQueue.top()->parent = parentNode; //link it to parentNode
            parentNode->rightChild = priQueue.top();

            priQueue.pop();

            parentNode->frequency = freqOne+freqTwo; //make parentNode's frequency the sum of both child frequencies
            priQueue.push(parentNode); //add to priority queue to be compared with other priority queue nodes
        }
        head = priQueue.top(); //make the head of the tree the only remaining node in priorityQueue
    }
}

huffman_tree::~huffman_tree(){ //deconstructor

}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const {
    for (int i = 0; i < (int)planter.size(); i++) { //for the vector of all individual nodes/characters in document
        if (character == (char)(planter[i]->character)) { //if the current character in planter node matches the wanted character
            Node* currNode = planter[i];
            string characterCode = "";
            while(currNode->parent != nullptr) { //go from bottom to top of tree
                if(currNode->left == true) {
                    characterCode = "0" + characterCode; //add binary to front of string
                }
                else {
                    characterCode = "1" + characterCode;
                }
                currNode = currNode->parent;
            }
            return characterCode;
        }
    }
	return ""; //if the character does not exist
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/

std::string huffman_tree::encode(const std::string &file_name) const {
    ifstream in(file_name);
    string document((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    string encodedDocument = "";
    if (planter.size() == 1) { //if there's only a head node, return 0
        return "0";
    }
    else {
        for (int i=0; i < (int)document.length(); i++) { //for the length of the given document string
            char asciiChar = document[i];
            string characterCode = get_character_code(asciiChar); //get character code
            if (characterCode == "") { //if at any point the character is not valid, return an empty string
                return "";
            }
            else { //otherwise add character code to string
                encodedDocument += get_character_code(asciiChar);
            }
        }
    }

	return encodedDocument;
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const {
    Node* currNode;
    currNode = head;
    string decodedDocument = "";

    if (head->leftChild == nullptr && head->rightChild == nullptr) { //if the head is the only node in the tree
        decodedDocument += (char)head->character; //return only that character
        return decodedDocument;
    }
    for(int i = 0; i < (int)string_to_decode.length(); i++) { //for the entirety of the given string
        char currentChar = string_to_decode[i];
        if (currentChar == '0') { //if it's a 0 move left on the tree
            currNode = currNode->leftChild;
        }
        else if (currentChar == '1'){ //if it's a 1 move right on the tree
            currNode = currNode->rightChild;
        }
        if (currNode->leftChild == nullptr && currNode->rightChild == nullptr) { //if it's a leaf
            decodedDocument += (char)(currNode->character); //add that letter
            currNode = head; //go back to top of tree
            continue;
        }
    }
    return decodedDocument;
}
