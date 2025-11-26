#include<bits/stdc++.h>
using namespace std;

int main(){
  int choice;
  while (1){
    cout <<"Enter your choice"<<endl;
    cout <<"1. Add a word"<<endl;
    cout <<"2. Search a word"<<endl;
    cout <<"3. delete a word"<<endl;
    cout <<"4. Show words starting from a letter"<<endl;
    cout <<"5. Show all words"<<endl;
    cin>> choice;
    switch(choice){
      case 1:
        cout <<"Adding a word";
        break;
      case 2:
        cout <<"Searching a word";
        break;
      case 3:
        cout <<"Deleting a word";
        break;
      case 4:
        cout <<"Showing words starting from a letter";
        break;
      case 5:
        cout <<"Showing all words";
        break;
      default:
        cout <<"Invalid choice";
    }
    
  }
}
