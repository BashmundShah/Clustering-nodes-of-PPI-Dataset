#include <iostream>
#include <fstream>
using namespace std;

int i=0;
class List{
   private:
      struct node{
           string word;
           int val;
           node *next;
      			 };
           node *Head;
           node *Tail;
   public:
       List();
       void addWord(string);
       void display();
       int search(string);
};

List::List()
{
    this->Head = NULL;
    this->Tail = NULL;
}


void List::addWord(string word)
{
    node *temp = new node();
    temp->word = word;
    temp->val = ++i;
    temp->next = NULL;
    if(!this->Head)
    {
        this->Head = this->Tail = temp;
        return;
    }
    this->Tail->next = temp;
    this->Tail = temp;
    return;
}

void List::display()
{
    node *temp = this->Head;
    while(temp)
    {
        cout << temp->word << "   " << temp->val << endl;
        temp = temp->next;
    }
    return;
}

int List::search(string word)
{
    node *temp = this->Head;
    while(temp)
    {
        if(temp->word == word)
            return temp->val;
        temp = temp->next;
    }
    return 0;
}

List usr;

void firstColumn()
{
    ifstream myFile;
    string word = "\0";
    myFile.open("PPI-I Dataset.txt");
    while(myFile >> word)
    {
        if(!usr.search(word))
        {
            usr.addWord(word);
        }
        myFile >> word;
        myFile >> word;
    }
}

void secondColumn()
{
    ifstream myFile;
    string word = "\0";
    myFile.open("PPI-I Dataset.txt");
    myFile >> word;
    while(myFile >> word)
    {
        if(!usr.search(word))
        {
            usr.addWord(word);
        }
        myFile >> word;
        myFile >> word;
    }
}

void makeOutputFile()
{
    ifstream myFile;
    ofstream outputFile;
    outputFile.open("Numbered PPI-I Dataset.txt");
    string word = "\0";
    myFile.open("PPI-I Dataset.txt");
    while(myFile >> word)
    {
        if(word[0] == '0')
        {
          outputFile << word << endl;
          continue;
        }
        outputFile << usr.search(word) << "   ";
    }

}
int main()
{
    firstColumn();
    secondColumn();
    makeOutputFile();
    usr.display();
    return 0;
}
