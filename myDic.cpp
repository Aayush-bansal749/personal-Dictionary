#include <bits/stdc++.h>
using namespace std;

struct Node 
{
    string word;
    string meaning;
    Node* next;
};

Node* head = NULL;
string getFile(char ch) 
{
    ch = tolower(ch);
    string file = "";
    file += ch;     
    file += ".txt"; 
    return file;
}

void addWord() 
{
    string w, m;
    cout << "Enter word: ";
    cin >> w;
    cout << "Enter meaning: ";
    cin.ignore();
    getline(cin, m);
    Node* newNode = new Node();
    newNode->word = w;
    newNode->meaning = m;
    newNode->next = head;
    head = newNode;
    string file = getFile(w[0]);
    ofstream out(file, ios::app);
    out << w << " - " << m << endl;
    out.close();
    cout << "Word added successfully!"<<endl;
}

void loadFiles() 
{
    for(char c = 'a'; c <= 'z'; c++) 
    {
        string file = getFile(c);
        ifstream in(file);
        if (!in) continue;
        string line;
        while (getline(in, line)) 
        {
            int pos = line.find(" - ");
            if (pos == -1) continue;
            string w = line.substr(0, pos);
            string m = line.substr(pos + 1);
            Node* newNode = new Node();
            newNode->word = w;
            newNode->meaning = m;
            newNode->next = head;
            head = newNode;
        }
        in.close();
    }
}

void searchWord() 
{
    string w;
    cout << "Enter word to search: ";
    cin >> w;
    Node* temp = head;
    while (temp != NULL) 
    {
        if (temp->word == w) 
        {
            cout << "Meaning: " << temp->meaning << endl;
            return;
        }
        temp = temp->next;
    }

    cout << "Word not found!"<<endl;
}

void deleteWord() 
{
    string w;
    cout << "Enter word to delete: ";
    cin >> w;
    Node* temp = head;
    Node* prev = NULL;
    bool found = false;
    while (temp != NULL) 
    {
        if (temp->word == w) 
        {
            found = true;
            if (prev == NULL) head = temp->next;
            else prev->next = temp->next;
            delete temp;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if (!found) 
    {
        cout << "Word not found!"<<endl;
        return;
    }
    string file = getFile(w[0]);
    ifstream in(file);
    ofstream tempFile("temp.txt");
    string line;
    while (getline(in, line)) 
    {
        int pos = line.find(" - ");
        string wordInFile = line.substr(0, pos);

        if (wordInFile != w) 
        {
            tempFile << line << endl;
        }
    }
    in.close();
    tempFile.close();
    remove(file.c_str());
    rename("temp.txt", file.c_str());
    cout << "Word deleted!"<<endl;
}

void showWordsByLetter()
{
    char c;
    cout << "Enter starting letter: ";
    cin >> c;
    Node* temp = head;
    bool any = false;
    while (temp != NULL) 
    {
        if (tolower(temp->word[0]) == tolower(c)) 
        {
            cout << temp->word << " - " << temp->meaning << endl;
            any = true;
        }
        temp = temp->next;
    }
    if (!any) cout << "No words found."<<endl;
}

void showAllWords() 
{
    Node* temp = head;
    if (temp == NULL) 
    {
        cout << "No words found."<<endl;
        return;
    }

    while (temp != NULL) 
    {
        cout << temp->word << " - " << temp->meaning << endl;
        temp = temp->next;
    }
}

int main() 
{
    loadFiles();
    int choice;
    while (true) {
        cout << "+-+-+-+-+DICTIONARY MENU+-+-+-+-+"<<endl;
        cout << "1. Add a word"<<endl;
        cout << "2. Search a word"<<endl;
        cout << "3. Delete a word"<<endl;
        cout << "4. Show words starting from a letter"<<endl;
        cout << "5. Show all words"<<endl;
        cout << "6. Exit"<<endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                addWord();
                break;
            case 2:
                searchWord();
                break;
            case 3:
                deleteWord();
                break;
            case 4:
                showWordsByLetter();
                break;
            case 5:
                showAllWords();
                break;
            case 6:
                cout << "Exiting program..."<<endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again."<<endl;
        }
    }
    return 0;
}
