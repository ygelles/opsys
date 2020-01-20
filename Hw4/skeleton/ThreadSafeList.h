#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include <pthread.h>
#include <iostream>
#include <iomanip> // std::setw

using namespace std;

template<typename T>
class List {

public:
    /**
     * Constructor
     */
    List() :listSize(0) {
      dummyData=T();
      head=new Node(dummyData);
      pthread_mutex_init(&listSizeMutex, nullptr);
    }

    /**
     * Destructor
     */
    ~List() {
      pthread_mutex_destroy(&listSizeMutex);
      Node *toRemove = head;
      Node *temp;
      if (head != nullptr) {
        while (toRemove->next != nullptr) {
          temp = toRemove;
          toRemove = toRemove->next;
          delete temp;
        }
      }
    }

    class Node {
    public:
        T data;
        Node *next;
        pthread_mutex_t nodeMutex;

        Node(const T &data) : data(data), next(nullptr) {
          pthread_mutex_init(&nodeMutex, nullptr);
        }

        ~Node(){
          pthread_mutex_destroy(&nodeMutex);
        }

        void nodeLock(){
          pthread_mutex_lock(&nodeMutex);
        }
        void nodeUnlock(){
          pthread_mutex_unlock(&nodeMutex);
        }


        bool operator<(const Node &rhs) const {
          return data < rhs.data;
        }

        bool operator>(const Node &rhs) const {
          return rhs < *this;
        }

        bool operator<=(const Node &rhs) const {
          return !(rhs < *this);
        }

        bool operator>=(const Node &rhs) const {
          return !(*this < rhs);
        }

        bool operator==(const Node &rhs) const {
          return data == rhs.data;
        }

        bool operator!=(const Node &rhs) const {
          return !(rhs == *this);
        }
    };

    /**
     * Insert new node to list while keeping the list ordered in an ascending order
     * If there is already a node has the same data as @param data then return false (without adding it again)
     * @param data the new data to be added to the list
     * @return true if a new node was added and false otherwise
     */
    bool insert(const T &data) {
      Node *temp = head;
      Node* temp2;
      temp->nodeLock();
      while (temp->next != nullptr) {
        temp->next->nodeLock();
        if(temp->next->data >= data){
          break;
        }
        temp2=temp;
        temp = temp->next;
        temp2->nodeUnlock();
      }
      if (temp->next == nullptr) {
        temp->next = new Node(data);
        incListSize();
        __add_hook();
        temp->nodeUnlock();
        return true;
      }
      if (temp->next->data == data) {
        temp->next->nodeUnlock();
        temp->nodeUnlock();
        return false;
      }
      Node *newNode = new Node(data);
      newNode->next = temp->next;
      temp->next = newNode;
      incListSize();
      __add_hook();
      newNode->next->nodeUnlock();
      temp->nodeUnlock();
      return true;
    }

    /**
     * Remove the node that its data equals to @param value
     * @param value the data to lookup a node that has the same data to be removed
     * @return true if a matched node was found and removed and false otherwise
     */
    bool remove(const T &value) {
      Node *temp = head;
      Node* temp2;
      temp->nodeLock();
      while (temp->next != nullptr) {
        temp->next->nodeLock();
        if(temp->next->data == value){
          break;
        }
        temp2=temp;
        temp = temp->next;
        temp2->nodeUnlock();
      }
      if (temp->next == nullptr) {
        temp->nodeUnlock();
        return false;
      }
      Node *toDelete = temp->next;
      temp->next = temp->next->next;
      toDelete->nodeUnlock();
      delete toDelete;
      decListSize();
      __remove_hook();
      temp->nodeUnlock();
      return true;
    }

    /**
     * Returns the current size of the list
     * @return the list size
     */
    unsigned int getSize() {
      return listSize;
    }

    // Don't remove
    void print() {
      //      pthread_mutex_lock(&list_mutex);
      Node *temp = head->next;
      if (temp == NULL) {
        cout << "";
      } else if (temp->next == NULL) {
        cout << temp->data;
      } else {
        while (temp != NULL) {
          cout << right << setw(3) << temp->data;
          temp = temp->next;
          cout << " ";
        }
      }
      cout << endl;
      //     pthread_mutex_unlock(&list_mutex);
    }

    // Don't remove
    virtual void __add_hook() {}

    // Don't remove
    virtual void __remove_hook() {}

    void decListSize() {
      pthread_mutex_lock(&listSizeMutex);
      listSize--;
      pthread_mutex_unlock(&listSizeMutex);
    }

    void incListSize() {
      pthread_mutex_lock(&listSizeMutex);
      listSize++;
      pthread_mutex_unlock(&listSizeMutex);
    }
/*
    bool isSorted(){ //TODO REMOVE
      Node* prev = head->next;
      if(!prev)
        return true;
      pthread_mutex_lock(&prev->nodeMutex);
      Node* curr = head->next->next;
      while(curr) {
        pthread_mutex_lock(&curr->nodeMutex);
        if(prev->data >= curr->data) {
          pthread_mutex_unlock(&curr->nodeMutex);
          pthread_mutex_unlock(&prev->nodeMutex);
          return false;
        }
        pthread_mutex_unlock(&prev->nodeMutex);
        prev = curr;
        curr = curr->next;
      }
      pthread_mutex_unlock(&prev->nodeMutex);
      return true;
    }
*/
    bool isSorted(){ return true;}






private:
    Node *head;
    T dummyData;
    unsigned int listSize;
    pthread_mutex_t listSizeMutex;
    // TODO: Add your own methods and data members
};

#endif //THREAD_SAFE_LIST_H_