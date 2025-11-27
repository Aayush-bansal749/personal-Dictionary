#include <bits/stdc++.h>
using namespace std;

/* ------------------ HUFFMAN CODING ------------------ */

struct HuffNode {
    char ch;
    int freq;
    HuffNode *left, *right;
    HuffNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct HuffCmp {
    bool operator()(HuffNode* a, HuffNode* b) {
        return a->freq > b->freq;
    }
};

HuffNode* huffRoot = nullptr;
unordered_map<char, string> huffCode;   // char -> bitstring

void buildCodeTable(HuffNode* node, const string &path) {
    if (!node) return;
    if (!node->left && !node->right) {
        // leaf
        huffCode[node->ch] = path.empty() ? "0" : path; // handle single-char case
        return;
    }
    buildCodeTable(node->left,  path + "0");
    buildCodeTable(node->right, path + "1");
}

void buildHuffman(const string &corpus) {
    if (corpus.empty()) {
        huffRoot = nullptr;
        huffCode.clear();
        return;
    }

    unordered_map<char,int> freq;
    for (char c : corpus) freq[c]++;

    priority_queue<HuffNode*, vector<HuffNode*>, HuffCmp> pq;
    for (auto &p : freq) {
        pq.push(new HuffNode(p.first, p.second));
    }

    while (pq.size() > 1) {
        HuffNode* a = pq.top(); pq.pop();
        HuffNode* b = pq.top(); pq.pop();
        HuffNode* merged = new HuffNode('\0', a->freq + b->freq);
        merged->left = a;
        merged->right = b;
        pq.push(merged);
    }

    huffRoot = pq.top();
    huffCode.clear();
    buildCodeTable(huffRoot, "");
}

string huffmanEncode(const string &text) {
    if (!huffRoot || huffCode.empty()) return text; // fallback: no compression
    string bits;
    for (char c : text) {
        auto it = huffCode.find(c);
        if (it != huffCode.end()) bits += it->second;
        else bits += c; // fallback for unseen chars
    }
    return bits;
}

string huffmanDecode(const string &bits) {
    if (!huffRoot || huffCode.empty()) return bits; // fallback
    string out;
    HuffNode* curr = huffRoot;
    for (char b : bits) {
        if (b == '0') curr = curr->left;
        else if (b == '1') curr = curr->right;
        else {
            // invalid bit -> treat as raw char
            out += b;
            curr = huffRoot;
            continue;
        }

        if (curr && !curr->left && !curr->right) {
            out += curr->ch;
            curr = huffRoot;
        }
    }
    return out;
}

/* ------------------ TRIE FOR DICTIONARY ------------------ */

class TrieNode {
public:
    TrieNode* children[26];
    bool wordEnd;
    string encodedMeaning; // Huffman encoded meaning

    TrieNode() {
        wordEnd = false;
        encodedMeaning = "";
        for (int i = 0; i < 26; i++) children[i] = nullptr;
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() { root = new TrieNode(); }

    void insertHandler(TrieNode* node, const string &word, int i, const string &meaning) {
        if (i == (int)word.size()) {
            node->wordEnd = true;
            node->encodedMeaning = huffmanEncode(meaning);
            return;
        }
        int idx = word[i] - 'a';
        if (idx < 0 || idx >= 26) return;
        if (!node->children[idx]) node->children[idx] = new TrieNode();
        insertHandler(node->children[idx], word, i + 1, meaning);
    }

    void insert(const string &word, const string &meaning) {
        if (word.empty()) return;
        insertHandler(root, word, 0, meaning);
    }

    bool searchHandler(TrieNode* node, const string &word, int i) {
        if (!node) return false;
        if (i == (int)word.size()) {
            if (node->wordEnd) {
                string meaning = huffmanDecode(node->encodedMeaning);
                cout << "Meaning: " << meaning << endl;
                return true;
            }
            return false;
        }
        int idx = word[i] - 'a';
        if (idx < 0 || idx >= 26) return false;
        return searchHandler(node->children[idx], word, i + 1);
    }

    bool search(const string &word) {
        return searchHandler(root, word, 0);
    }

    bool isEmpty(TrieNode* node) {
        for (int i = 0; i < 26; i++)
            if (node->children[i]) return false;
        return true;
    }

    bool deleteHandler(TrieNode* node, const string &word, int i) {
        if (!node) return false;
        if (i == (int)word.size()) {
            if (!node->wordEnd) return false;
            node->wordEnd = false;
            node->encodedMeaning.clear();
            return isEmpty(node);
        }
        int idx = word[i] - 'a';
        if (idx < 0 || idx >= 26 || !node->children[idx]) return false;

        bool shouldDeleteChild = deleteHandler(node->children[idx], word, i + 1);
        if (shouldDeleteChild) {
            delete node->children[idx];
            node->children[idx] = nullptr;
            return !node->wordEnd && isEmpty(node);
        }
        return false;
    }

    void deleteWord(const string &word) {
        if (deleteHandler(root, word, 0))
            cout << "Deleted the entire word!" << endl;
        else
            cout << "Word not found!" << endl;
    }

    bool updateHandler(TrieNode* node, const string &word, int i, const string &newMeaning) {
        if (!node) return false;
        if (i == (int)word.size()) {
            if (!node->wordEnd) return false;
            node->encodedMeaning = huffmanEncode(newMeaning);
            return true;
        }
        int idx = word[i] - 'a';
        if (idx < 0 || idx >= 26 || !node->children[idx]) return false;
        return updateHandler(node->children[idx], word, i + 1, newMeaning);
    }

    bool update(const string &word, const string &newMeaning) {
        return updateHandler(root, word, 0, newMeaning);
    }

    void printAllWordsHandler(TrieNode* node, string current) {
        if (!node) return;
        if (node->wordEnd) {
            cout << current << " : " << huffmanDecode(node->encodedMeaning) << endl;
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                printAllWordsHandler(node->children[i], current + char('a' + i));
            }
        }
    }

    void printAllWords() {
        printAllWordsHandler(root, "");
    }

    void printWordsStartingWith(char ch) {
        if (ch < 'a' || ch > 'z') {
            cout << "Invalid starting letter." << endl;
            return;
        }
        int idx = ch - 'a';
        TrieNode* child = root->children[idx];
        if (!child) {
            cout << "No words starting with " << ch << endl;
            return;
        }
        printAllWordsHandler(child, string(1, ch));
    }
};

/* ------------------ FILE HANDLING (a.txt, b.txt, ...) ------------------ */

struct FileNode {
    char ch;
    string filename;
    FileNode* next;
    FileNode(char c) : ch(c), filename(string(1, c) + ".txt"), next(nullptr) {}
};

FileNode* createFileList() {
    FileNode* head = new FileNode('a');
    FileNode* temp = head;
    for (char c = 'b'; c <= 'z'; c++) {
        temp->next = new FileNode(c);
        temp = temp->next;
    }
    return head;
}

string getFile(char c, FileNode* head) {
    c = tolower(c);
    FileNode* temp = head;
    while (temp) {
        if (temp->ch == c) return temp->filename;
        temp = temp->next;
    }
    return "";
}

void saveToFile(const string &word, const string &meaning, FileNode* head) {
    if (word.empty()) return;
    string fname = getFile(word[0], head);
    if (fname.empty()) return;
    ofstream fout(fname, ios::app);
    fout << word << " - " << meaning << endl;
    fout.close();
}

void loadFilesToTrie(Trie &t, FileNode* head, string &corpus) {
    FileNode* temp = head;
    while (temp) {
        ifstream fin(temp->filename);
        if (!fin) {
            temp = temp->next;
            continue;
        }
        string line;
        while (getline(fin, line)) {
            int bar = line.find(" - ");
            if (bar == -1) continue;
            string word = line.substr(0, bar);
            string meaning = line.substr(bar + 3);

            // normalize to lowercase
            for (char &c : word) c = tolower(c);

            if (!word.empty()) {
                t.insert(word, meaning);  // will be encoded later, after Huffman build
                corpus += meaning;
            }
        }
        fin.close();
        temp = temp->next;
    }
}

/* ------------------ MAIN ------------------ */

int main() {
    Trie t;
    FileNode* fHead = createFileList();
    string corpus;

    // First pass: load meanings to build Huffman corpus
    loadFilesToTrie(t, fHead, corpus);

    // Build Huffman codes from all meanings
    buildHuffman(corpus);

    // Reload files so that Trie actually stores encoded meanings
    // (simple way: clear Trie and insert again with encoding)
    Trie encodedTrie;
    corpus.clear();
    loadFilesToTrie(encodedTrie, fHead, corpus);
    t = encodedTrie; // shallow copy is fine here since we aren't managing custom pointers extensively

    int running = 1, choice;
    string word, meaning;
    char ch;

    cout << "Welcome to the Dictionary Application (Trie + Huffman)" << endl;
    cout << "Note: All words should be in lowercase letters." << endl;

    while (running) {
        cout << "\nEnter your choice" << endl;
        cout << "1. Add a word" << endl;
        cout << "2. Search a word" << endl;
        cout << "3. Update a word meaning" << endl;
        cout << "4. Show words starting from a letter" << endl;
        cout << "5. Show all words" << endl;
        cout << "6. Delete a word" << endl;
        cout << "7. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter word to insert: ";
            cin >> word;
            for (char &c : word) c = tolower(c);
            cout << "Enter meaning: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, meaning);
            t.insert(word, meaning);
            saveToFile(word, meaning, fHead);
            cout << "Word inserted." << endl;
            break;

        case 2:
            cout << "Enter word to search: ";
            cin >> word;
            for (char &c : word) c = tolower(c);
            if (!t.search(word)) cout << "Word not found" << endl;
            break;

        case 3:
            cout << "Enter word to update: ";
            cin >> word;
            for (char &c : word) c = tolower(c);
            cout << "Enter updated meaning: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, meaning);
            if (t.update(word, meaning)) {
                // For simplicity, we only update in Trie, not rewriting file.
                cout << "Updated in Trie (file update can be added separately)." << endl;
            } else {
                cout << "Word not found in Trie." << endl;
            }
            break;

        case 4:
            cout << "Enter starting letter: ";
            cin >> ch;
            ch = tolower(ch);
            t.printWordsStartingWith(ch);
            break;

        case 5:
            t.printAllWords();
            break;

        case 6:
            cout << "Enter word to delete: ";
            cin >> word;
            for (char &c : word) c = tolower(c);
            t.deleteWord(word);
            // File deletion sync is possible but omitted for brevity
            break;

        case 7:
            running = 0;
            cout << "Exited." << endl;
            break;

        default:
            cout << "Invalid choice" << endl;
        }
    }

    return 0;
}
