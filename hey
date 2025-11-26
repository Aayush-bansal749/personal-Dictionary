#include <bits/stdc++.h>
using namespace std;

// -----------------------------
// HUFFMAN CODING IMPLEMENTATION
// -----------------------------

struct HuffmanNode {
    char ch;                 // character (only for leaf nodes)
    int freq;                // frequency
    HuffmanNode* left;       // left child
    HuffmanNode* right;      // right child

    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    HuffmanNode(int f, HuffmanNode* l, HuffmanNode* r) : ch('\0'), freq(f), left(l), right(r) {}
};

// Comparator for min-heap (priority_queue)
struct CompareNode {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;  // smaller freq = higher priority
    }
};

// Recursively free the Huffman tree
void freeTree(HuffmanNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

// Build frequency map from text
unordered_map<char,int> buildFrequency(const string& text) {
    unordered_map<char,int> freq;
    for (char c : text) {
        freq[c]++;
    }
    return freq;
}

// Build Huffman tree using min-heap
HuffmanNode* buildHuffmanTree(const unordered_map<char,int>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNode> pq;

    // Create leaf node for each character
    for (auto& p : freqMap) {
        HuffmanNode* node = new HuffmanNode(p.first, p.second);
        pq.push(node);
    }

    // Edge case: only one unique character
    if (pq.size() == 1) {
        HuffmanNode* only = pq.top(); pq.pop();
        HuffmanNode* root = new HuffmanNode(only->freq, only, nullptr);
        return root;
    }

    // Combine two smallest nodes until one tree remains
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* merged = new HuffmanNode(left->freq + right->freq, left, right);
        pq.push(merged);
    }

    // Root of the Huffman tree
    return pq.top();
}

// DFS to build char -> code mapping
void buildCodes(HuffmanNode* node, const string& path, unordered_map<char,string>& codes) {
    if (!node) return;

    // Leaf node
    if (!node->left && !node->right) {
        // If only one unique char, assign "0"
        codes[node->ch] = path.empty() ? "0" : path;
        return;
    }

    // Go left with '0', right with '1'
    buildCodes(node->left, path + "0", codes);
    buildCodes(node->right, path + "1", codes);
}

// Encode text using Huffman codes
string encodeText(const string& text, const unordered_map<char,string>& codes) {
    string result;
    for (char c : text) {
        result += codes.at(c); // assumes every char is in codes
    }
    return result;
}

// Decode bitstring using Huffman tree
string decodeText(const string& bits, HuffmanNode* root) {
    string result;
    HuffmanNode* node = root;

    for (char bit : bits) {
        if (bit == '0') node = node->left;
        else node = node->right;

        // If leaf node reached
        if (node && !node->left && !node->right) {
            result += node->ch;
            node = root; // restart for next symbol
        }
    }

    return result;
}

// ------------------------------------
// COMPRESSED DICTIONARY IMPLEMENTATION
// ------------------------------------

class CompressedDictionary {
private:
    unordered_map<string,string> data;   // word -> compressed meaning (bitstring)
    HuffmanNode* root;                   // root of Huffman tree
    unordered_map<char,string> codes;    // char -> Huffman code

public:
    // Constructor: build dictionary from initial list of (word, meaning)
    CompressedDictionary(const vector<pair<string,string>>& entries) {
        root = nullptr;
        rebuild(entries);
    }

    // Destructor: free Huffman tree
    ~CompressedDictionary() {
        freeTree(root);
    }

    // Internal: rebuild Huffman tree and codes from full word-meaning list
    void rebuild(const vector<pair<string,string>>& wordMeaningPairs) {
        // Combine all meanings to build single Huffman model
        string allText;
        for (auto& p : wordMeaningPairs) {
            allText += p.second;
        }

        if (allText.empty()) {
            root = nullptr;
            codes.clear();
            data.clear();
            return;
        }

        auto freqMap = buildFrequency(allText);
        if (root) freeTree(root);
        root = buildHuffmanTree(freqMap);
        codes.clear();
        buildCodes(root, "", codes);

        // Re-encode all meanings and fill data
        data.clear();
        for (auto& p : wordMeaningPairs) {
            const string& word = p.first;
            const string& meaning = p.second;
            string compressed = encodeText(meaning, codes);
            data[word] = compressed;
        }
    }

    // Lookup: return decoded meaning or empty string if not found
    string lookup(const string& word) const {
        auto it = data.find(word);
        if (it == data.end() || !root) return "";
        const string& bits = it->second;
        return decodeText(bits, root);
    }

    // Insert new word: meaning pair (simple version: rebuilds model)
    void insert(const string& word, const string& meaning) {
        // Decode all current meanings
        vector<pair<string,string>> all;
        all.reserve(data.size() + 1);

        for (auto& p : data) {
            string decoded = decodeText(p.second, root);
            all.push_back({p.first, decoded});
        }
        // Add new one
        all.push_back({word, meaning});

        // Rebuild Huffman from updated list
        rebuild(all);
    }

    // Debug: print codes and compressed data
    void debugPrint() const {
        cout << "Huffman Codes:\n";
        for (auto& p : codes) {
            cout << "'" << p.first << "' -> " << p.second << "\n";
        }
        cout << "\nDictionary (word -> compressed bits):\n";
        for (auto& p : data) {
            cout << p.first << " -> " << p.second << "\n";
        }
        cout << "\n";
    }
};

// --------------
// SIMPLE DEMO
// --------------

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Initial dictionary entries
    vector<pair<string,string>> entries = {
        {"apple",  "A sweet red or green fruit."},
        {"banana", "A long yellow fruit that monkeys like."},
        {"cat",    "A small domestic animal that says meow."}
    };

    CompressedDictionary cd(entries);

    // Lookup example
    string query = "banana";
    string meaning = cd.lookup(query);
    if (!meaning.empty()) {
        cout << "Meaning of '" << query << "': " << meaning << "\n\n";
    } else {
        cout << "Word not found.\n\n";
    }

    // Insert example
    cd.insert("dog", "A loyal domestic animal that barks.");
    cout << "Meaning of 'dog': " << cd.lookup("dog") << "\n\n";

    // Debug info (for understanding)
    cd.debugPrint();

    return 0;
}
