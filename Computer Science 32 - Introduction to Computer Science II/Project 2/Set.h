#ifndef SET
#define SET
#include <string>



using ItemType = std::string;

class Set
{
public:
    Set();
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value); 
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
    ~Set(); 
    Set(const Set& other); 
    Set& operator=(const Set& rhs);



private:

    struct Node
    {
        ItemType value;
        Node* next;
        Node* prev;
    };

    Node *head;
    int mSize;



};

void unite(const Set& s1, const Set& s2, Set& result); 

void excludeInclude(const Set& s1, const Set& s2, Set& result); 

#endif