#include <bits/stdc++.h>
using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    bool wordEnd;
    char data;
    string meaning;   
    TrieNode(char ch) {
        data = ch;
        wordEnd = false;
        for (int i = 0; i < 26; i++) children[i] = nullptr;
    }
};

class Trie {
public: 
    TrieNode* root;
    Trie() { root = new TrieNode('\0'); }

    void insertHandler(TrieNode* node, string word, int i, string meaning) {      
        if (i == word.size()) {
            node->wordEnd = true;
            node->meaning = meaning; 
            return;
        }       
        int index = word[i] - 'a';
        TrieNode* child;
        if (node->children[index] != nullptr) child = node->children[index];
        else {
            child = new TrieNode(word[i]);
            node->children[index] = child;
        }
        insertHandler(child, word, i + 1, meaning);
    }

    void insert(string word, string meaning) { insertHandler(root, word, 0, meaning); }

    bool searchHandler(TrieNode* node, string word, int i) {
        if (i == word.size()) {
            cout << "Meaning: " << node->meaning << endl;
            return node->wordEnd;
        }
        int index = word[i] - 'a';
        if (!node->children[index]) return false;
        return searchHandler(node->children[index], word, i + 1);
    }

    bool search(string word) { return searchHandler(root, word, 0); }

    bool isEmpty(TrieNode* node) {
        for (int i = 0; i < 26; i++)
            if (node->children[i] != nullptr) return false;
        return true;
    }

    bool deleteHandler(TrieNode* node, string &word, int i) {
        if (i == word.size()) {
            if (!node->wordEnd) return false;
            node->wordEnd = false;           
            return true;
        }
        int index = word[i] - 'a';
        TrieNode* child = node->children[index];
        if (!child) return false; 
        bool deleted = deleteHandler(child, word, i + 1);
        if (deleted && !child->wordEnd && isEmpty(child)) {
            delete child;
            node->children[index] = nullptr;
        }
        return deleted;
    }

    void deleteWord(string word) {
        if (deleteHandler(root, word, 0)) cout << "Deleted the entire word!" << endl;
        else cout << "Word not found!" << endl;
    }

    void printAllWords() { printAllWordsHandler(root, ""); }

    void printAllWordsHandler(TrieNode* node, string current) {
        if (node->wordEnd) cout << current << " : " << node->meaning << endl;
        for (int i = 0; i < 26; i++)
            if (node->children[i]) printAllWordsHandler(node->children[i], current + char('a' + i));
    }

    bool update(string word, string newMeaning) { return updateHandler(root, word, 0, newMeaning); }

    bool updateHandler(TrieNode* node, string word, int i, string newMeaning) {
        if (i == word.size()) {
            node->meaning = newMeaning;
            return node->wordEnd;
        }
        int index = word[i] - 'a';
        if (!node->children[index]) return false;
        return updateHandler(node->children[index], word, i + 1, newMeaning);
    }

    void printWordsStartingWith(char ch) {
        int idx = ch - 'a';
        if (!root->children[idx]) {
            cout << "No words starting with " << ch << endl;
            return;
        }
        printAllWordsHandler(root->children[idx], string(1, ch));
    }
};

struct FileNode {
    char ch;
    string filename;
    FileNode* next;
    FileNode(char c) : ch(c), filename(string(1,c) + ".txt"), next(nullptr) {}
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
    FileNode* temp = head;
    while(temp){
        if(temp->ch == c) return temp->filename;
        temp = temp->next;
    }
    return "";
}

void saveToFile(string word, string meaning, FileNode* head) {
    string fname = getFile(word[0], head);
    ofstream fout(fname, ios::app);
    fout << word << " - " << meaning << endl;
    fout.close();
}

void loadFilesToTrie(Trie* t, FileNode* head) {
    FileNode* temp = head;
    while(temp){
        ifstream fin(temp->filename);
        if(!fin){ temp = temp->next; continue; }

        string line;
        while(getline(fin, line)){
            int bar = line.find(' - ');
            if(bar == -1) continue;
            string word = line.substr(0, bar);
            string meaning = line.substr(bar+1);

            if(word.size() > 0 && isupper(word[0])) word[0] = tolower(word[0]);
            t->insert(word, meaning);
        }
        fin.close();
        temp = temp->next;
    }
}

int main(){
    Trie* t = new Trie();
    FileNode* fHead = createFileList();
    loadFilesToTrie(t, fHead);

    int running = 1, choice;
    string word, meaning;
    char ch;

    cout <<"Welcome to the Dictionary Application"<<endl;
    cout<< "All words are to be inserted lowercase letters"<<endl;

    while(running){
        cout <<"Enter your choice"<<endl;
        cout <<"1. Add a word"<<endl;
        cout <<"2. Search a word"<<endl;
        cout <<"3. Update a word meaning"<<endl;
        cout <<"4. Show words starting from a letter"<<endl;
        cout <<"5. Show all words"<<endl;
        cout <<"6. Delete a word"<<endl;
        cout <<"7. Exit"<<endl;
        cin >> choice;

        switch(choice){
            case 1:
                cout << "Enter word to insert: "; cin >> word;
                cout << "Enter meaning: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, meaning);
                if(!word.empty() && isupper(word[0])) word[0] = tolower(word[0]);
                t->insert(word, meaning);
                saveToFile(word, meaning, fHead);
                break;
            case 2:
                cout << "Enter word to search: "; cin >> word;
                if(!word.empty() && isupper(word[0])) word[0] = tolower(word[0]);
                if(!t->search(word)) cout << "Word not found" << endl;
                break;
            case 3:
                cout << "Enter word to update: "; cin >> word;
                cout << "Enter updated meaning: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, meaning);
                if(!word.empty() && isupper(word[0])) word[0] = tolower(word[0]);
                t->update(word, meaning);
                cout << "Updated!" << endl;
                break;
            case 4:
                cout << "Enter starting letter: "; cin >> ch;
                if(isupper(ch)) ch = tolower(ch);
                t->printWordsStartingWith(ch);
                break;
            case 5:
                t->printAllWords();
                break;
            case 6:
                cout << "Enter word to delete: "; cin >> word;
                if(!word.empty() && isupper(word[0])) word[0] = tolower(word[0]);
                t->deleteWord(word);
                break;
            case 7:
                running = 0;
                cout << "Exited" << endl;
                break;
            default:
                cout << "Invalid choice" << endl;
        }
    }
}
