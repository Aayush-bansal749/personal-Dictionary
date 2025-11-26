#include <bits/stdc++.h>
using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    bool wordEnd;
    char data;

    TrieNode(char ch) {
        data = ch;
    TrieNode(char ch) {
        data=ch;
        wordEnd = false;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode('\0');
    }

    void insertHandler(TrieNode* node, const string& word, int i) {
        if (i == word.size()) {
            node->wordEnd = true;
            return;
        }
        int index = word[i] - 'a';
        TrieNode* child;
        if (node->children[index] != nullptr) {
            child = node->children[index];
        } else {
            child = new TrieNode(word[i]);
            node->children[index] = child;
        }
        insertHandler(child, word, i + 1);
    }

    void insert(const string& word) {
        insertHandler(root, word, 0);
    }

    bool searchUtil(TrieNode* node, const string& word, int i) {
        if (i == word.size()) {
            return node->wordEnd;
        }
        int index = word[i] - 'a';
        TrieNode* child;
        if (node->children[index] != nullptr) {
            child = node->children[index];
        } else {
            return false;
        }
        return searchUtil(child, word, i + 1);
    }

    bool search(const string& word) {
        return searchUtil(root, word, 0);
    }

    bool isEmpty(TrieNode* node) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) return false;
        }
        return true;
    }

    bool deleteUtil(TrieNode* node, const string& word, int i) {
        if (i == word.size()) {
            if (!node->wordEnd) return false;
            node->wordEnd = false;
            return true;
        }
        int index = word[i] - 'a';
        TrieNode* child = node->children[index];
        if (!child) return false;
        bool deleted = deleteUtil(child, word, i + 1);
        if (deleted && !child->wordEnd && isEmpty(child)) {
            delete child;
            node->children[index] = nullptr;
        }
        return deleted;
    }

    bool deleteWord(const string& word) {
        return deleteUtil(root, word, 0);
    }
};

struct HuffNode {
    char ch;
    int freq;
    HuffNode* left;
    HuffNode* right;

    HuffNode(char c, int f) {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    HuffNode(int f, HuffNode* l, HuffNode* r) {
        ch = '\0';
        freq = f;
        left = l;
        right = r;
    }
};

struct HuffCompare {
    bool operator()(HuffNode* a, HuffNode* b) {
        return a->freq > b->freq;
    }
};

class Huffman {
public:
    HuffNode* root;
    unordered_map<char,string> codes;

    Huffman() {
        root = nullptr;
    }

    void build(const string& text) {
        if (text.empty()) {
            root = nullptr;
            codes.clear();
            return;
        }

        unordered_map<char,int> freq;
        for (char c : text) {
            freq[c]++;
        }

        priority_queue<HuffNode*, vector<HuffNode*>, HuffCompare> pq;
        for (auto &p : freq) {
            pq.push(new HuffNode(p.first, p.second));
        }

        if (pq.size() == 1) {
            HuffNode* only = pq.top();
            pq.pop();
            root = new HuffNode(only->freq, only, nullptr);
        } else {
            while (pq.size() > 1) {
                HuffNode* left = pq.top();
                pq.pop();
                HuffNode* right = pq.top();
                pq.pop();
                HuffNode* merged = new HuffNode(left->freq + right->freq, left, right);
                pq.push(merged);
            }
            root = pq.top();
        }

        codes.clear();
        buildCodes(root, "");
    }

    string encode(const string& text) {
        string ans;
        for (char c : text) {
            if (codes.count(c)) {
                ans += codes[c];
            }
        }
        return ans;
    }

    string decode(const string& bits) {
        if (!root) return "";
        string ans;
        HuffNode* node = root;
        for (char b : bits) {
            if (b == '0' && node->left) {
                node = node->left;
            } else if (b == '1' && node->right) {
                node = node->right;
            }
            if (node && !node->left && !node->right) {
                ans += node->ch;
                node = root;
            }
        }
        return ans;
    }

private:
    void buildCodes(HuffNode* node, string path) {
        if (!node) return;
        if (!node->left && !node->right) {
            if (path.empty()) {
                codes[node->ch] = "0";
            } else {
                codes[node->ch] = path;
            }
            return;
        }
        buildCodes(node->left, path + "0");
        buildCodes(node->right, path + "1");
    }
};

int main() {
    Trie t;
    Huffman h;

    vector<pair<string,string>> entries;
    unordered_map<string,string> compressedMeaning;

    auto rebuildHuffman = [&]() {
        string allText;
        for (auto &p : entries) {
            allText += p.second;
        }
        h.build(allText);
        compressedMeaning.clear();
        for (auto &p : entries) {
            compressedMeaning[p.first] = h.encode(p.second);
        }
    };

    int running = 1;
    while (running) {
        cout << "Enter your choice\n";
        cout << "1. Add a word with meaning\n";
        cout << "2. Search a word and show meaning\n";
        cout << "3. Delete a word\n";
        cout << "4. Show all words\n";
        cout << "5. Exit\n";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string word;
            string meaning;
            cout << "Enter word (lowercase): ";
            cin >> word;
            cout << "Enter meaning (single line, no spaces -> or use _ instead of space): ";
            cin >> meaning;

            entries.push_back({word, meaning});
            t.insert(word);
            rebuildHuffman();
            cout << "Word added\n";
        } else if (choice == 2) {
            string word;
            cout << "Enter word to search: ";
            cin >> word;
            if (!t.search(word)) {
                cout << "Word not found\n";
            } else {
                string bits = compressedMeaning[word];
                string decoded = h.decode(bits);
                cout << "Compressed meaning: " << bits << "\n";
                cout << "Decoded meaning: " << decoded << "\n";
            }
        } else if (choice == 3) {
            string word;
            cout << "Enter word to delete: ";
            cin >> word;
            if (!t.search(word)) {
                cout << "Word not found\n";
            } else {
                bool ok = t.deleteWord(word);
                if (ok) {
                    vector<pair<string,string>> temp;
                    for (auto &p : entries) {
                        if (p.first != word) temp.push_back(p);
                    }
                    entries.swap(temp);
                    rebuildHuffman();
                    cout << "Deleted the entire word\n";
                } else {
                    cout << "Delete failed\n";
                }
            }
        } else if (choice == 4) {
            cout << "Words in dictionary:\n";
            for (auto &p : entries) {
                cout << p.first << "\n";
            }
        } else if (choice == 5) {
            running = 0;
            cout << "Exited\n";
        } else {
            cout << "Invalid choice\n";
        }
    }

    return 0;
public: 
  TrieNode* root;

  Trie() {
      root = new TrieNode('\0');
  }

  void insertHandler(TrieNode* node, string word, int i) {      
    if (i == word.size()) {
      node->wordEnd = true;
      return;
    }       
    int index = word[i] - 'a';
    TrieNode* child;
    if (node->children[index] != NULL) {
      child = node->children[index];
    }
    else {
      child = new TrieNode(word[i]);
      node->children[index] = child;
    }
    insertHandler(child, word, i + 1);
  }

  void insert(string word) {
    insertHandler(root, word, 0);
  }

  bool searchUtil(TrieNode* node, string word, int i) {
    if (i == word.size()) {
        return node->wordEnd;
    }
    int index = word[i] - 'a';
    TrieNode* child;
    if (node->children[index] != NULL) {
        child = node->children[index];
    }
    else {
        return false;
    }
    return searchUtil(child, word, i + 1);
  }

  bool search(string word) {
    return searchUtil(root, word,0);
  }

  bool isEmpty(TrieNode* node) {
    for (int i = 0; i < 26; i++)
        if (node->children[i] != nullptr)
            return false;
    return true;
  }

  bool deleteUtil(TrieNode* node, string &word, int i) {
    if (i == word.size()) {
        if (!node->wordEnd) return false;
        node->wordEnd = false;           
        return true;
    }
    int index = word[i] - 'a';
    TrieNode* child = node->children[index];
    if (!child) return false; 
    bool deleted = deleteUtil(child, word, i + 1);
    if (deleted && !child->wordEnd && isEmpty(child)) {
        delete child;
        node->children[index] = nullptr;
    }
    return deleted;
  }

  void deleteWord(string word) {
    if (deleteUtil(root, word, 0))
        cout << "Deleted the entire word!" << endl;
    else
        cout << "Word not found!" << endl;
  }

};

int main(){
  Trie* t = new Trie();
  int running = 1;
  int choice;
  while (running){
    cout <<"Enter your choice"<<endl;
    cout <<"1. Add a word"<<endl;
    cout <<"2. Search a word"<<endl;
    cout <<"3. delete a word"<<endl;
    cout <<"4. Show words starting from a letter"<<endl;
    cout <<"5. Show all words"<<endl;
    cout <<"6. Exit"<<endl;
    cin>> choice;
    switch(choice){
      case 1:
        t->insert("hello");
        break;
      case 2:
        if(t->search("hello")){
          cout <<"Word found"<<endl;
        } else {
          cout <<"Word not found"<<endl;
        }
        break;
      case 3:
        t->deleteWord("hello");
        break;
      case 4:
        cout <<"Showing words starting from a letter";
        break;
      case 5:
        cout <<"Showing all words";
        break;
      case 6:
        running = 0;
        cout <<"Exited"<<endl;
        break;
      default:
        cout <<"Invalid choice"<<endl;
    }
  }
}
