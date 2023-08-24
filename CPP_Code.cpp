#include <iostream>
#include <queue>
#include <unordered_map>
using namespace std;

// Huffman tree node
class Node
{
public:
    char data;
    int freq;
    Node *left, *right;

    Node(char data, int freq)
    {
        left = right = nullptr;
        this->data = data;
        this->freq = freq;
    }
};

// Comparison function for priority queue
class Compare
{
public:
    bool operator()(Node *l, Node *r)
    {
        if (l->freq == r->freq)
        {
            return l->data > r->data; // Consider ASCII value as tiebreaker
        }
        return (l->freq > r->freq);
    }
};

// Function to print the huffman codes
void printHuffmanCodes(Node *root, string str, unordered_map<char, string> &huffmanCodes)
{
    if (!root)
        return;

    if (root->left == nullptr && root->right == nullptr)
    {
        huffmanCodes[root->data] = str;
    }

    printHuffmanCodes(root->left, str + "0", huffmanCodes);
    printHuffmanCodes(root->right, str + "1", huffmanCodes);
}

// Function to build the Huffman tree and get the Huffman codes
unordered_map<char, string> buildHuffmanTree(unordered_map<char, int> &frequencies)
{
    priority_queue<Node *, vector<Node *>, Compare> pq;

    // Create a node for each character and its frequency
    for (auto pair : frequencies)
    {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() != 1)
    {
        Node *left = pq.top();
        pq.pop();

        Node *right = pq.top();
        pq.pop();

        Node *newNode = new Node('$', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    Node *root = pq.top();
    unordered_map<char, string> huffmanCodes;
    printHuffmanCodes(root, "", huffmanCodes);

    return huffmanCodes;
}

// Function to compress the input string using Huffman codes
string compressString(string input, unordered_map<char, string> &huffmanCodes)
{
    string compressedString = "";

    for (char ch : input)
    {
        compressedString += huffmanCodes[ch];
    }

    return compressedString;
}

// Function to decompress the input string using Huffman codes
string decompressString(string input, unordered_map<string, char> &huffmanCodes)
{
    string decompressedString = "";
    string currentCode = "";

    for (char ch : input)
    {
        currentCode += ch;
        if (huffmanCodes.find(currentCode) != huffmanCodes.end())
        {
            decompressedString += huffmanCodes[currentCode];
            currentCode = "";
        }
    }

    return decompressedString;
}

int main()
{
    unordered_map<char, int> frequencies;
    frequencies['a'] = 40;
    frequencies['b'] = 3;
    frequencies['e'] = 10;
    frequencies['f'] = 2;
    frequencies['x'] = 1;
    frequencies['n'] = 12;
    frequencies['q'] = 13;

    unordered_map<char, string> huffmanCodes = buildHuffmanTree(frequencies);

    string input = "aafqbbaea";
    string compressedString = compressString(input, huffmanCodes);

    cout << "Compressed string: " << compressedString << endl;

    // Print compression code of each alphabet
    cout << "Huffman Codes:" << endl;
    cout << "a: " << huffmanCodes['a'] << endl;
    cout << "b: " << huffmanCodes['b'] << endl;
    cout << "e: " << huffmanCodes['e'] << endl;
    cout << "f: " << huffmanCodes['f'] << endl;
    cout << "x: " << huffmanCodes['x'] << endl;
    cout << "n: " << huffmanCodes['n'] << endl;
    cout << "q: " << huffmanCodes['q'] << endl;

    // Reverse the Huffman codes for decoding
    unordered_map<string, char> reverseHuffmanCodes;
    for (auto pair : huffmanCodes)
    {
        reverseHuffmanCodes[pair.second] = pair.first;
    }

    string decompressedString = decompressString(compressedString, reverseHuffmanCodes);

    cout << "Decompressed string: " << decompressedString << endl;

    return 0;
}
