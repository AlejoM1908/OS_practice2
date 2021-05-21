#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
* The Struct Node allows to create new nodes for the Linked List
*/
typedef struct Node{
    int key;
    long int data;
    struct Node* next;
} Node;

/**
* The Struct Node allows to create new Linked Lists
*/
typedef struct LinkedList{
    struct Node* head;
    struct Node* tail;
    int size;
} LinkedList;

/**
* The function init creates a clean Linked List
* @return a pointer to the new Linked List
*/
LinkedList* init(){
    // Assigning the dynamic space for the list
    LinkedList* list = malloc(sizeof(LinkedList));

    // Initializing the values of the list
    list -> head = NULL;
    list -> tail = NULL;
    list -> size = 0;

    //Returning the pointer
    return list;
}

/**
* The function isEmptyList help to determine if the Linked List is empty or not
* @param list is a pointer to the list we want to apply the function to
* @return true if the list is empty, false on the contrary
*/
bool isEmptyList(struct LinkedList* list){
    return list -> size == 0;
}

/**
* The function length is used for counting the amount of nodes in the list
* @param list is a pointer to the list we want to apply the function to
* @return the length of the list
*/
int length(struct LinkedList* list){
    return list -> size;
}

/**
* The function freeNode is used to free the dynamic memory assigned to
* the paramether node
* @param node is a pointer to the node we want to apply the function to
*/
void freeNode(struct Node* node){
    free(node);
}

/**
* The function addFront is used to insert a new node with a key and data 
* in the HEAD of the list
* @param list is a pointer to the list we want to apply the function to
* @param key is the key stored in the new node
* @param value is the data stored in the new node
*/
void addFront(struct LinkedList* list, int key, long int value){
    // Assigning the dynamic space for the new node and storing the key and data
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode -> key = key;
    newNode -> data = value;
    
    // Checking if the list is empty
    if (list -> size == 0){
        list -> head = newNode;
        list -> tail = newNode;
    }
    else{
        newNode -> next = list -> head;
        list -> head = newNode;
    }

    list -> size ++;
}

/**
* The function addBack is used to insert a new node with a key and data in the
* TAIL of the list
* @param list is a pointer to the list we want to apply the function to
* @param key is the key stored in the new node
* @param value is the data stored in the new node
*/
void addBack(struct LinkedList* list, int key, long int value){
    // Assigning the dynamic space for the new node and storing the key and data
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode -> key = key;
    newNode -> data = value;
    newNode -> next = NULL;
    
    // Checking if the list is empty
    if (list -> size == 0){
        list -> head = newNode;
        list -> tail = newNode;
    }
    else{
        list -> tail -> next = newNode;
        list -> tail = newNode; 
    }

    list -> size ++;
}

/**
* The function showFront is used to see the key stored in the HEAD node
* @param list is a pointer to the list we want to apply the function to
* @return the key stored in the HEAD of the list
*/
int showFront(struct LinkedList* list){
    return list -> head -> key;
}

/**
* The function showBack is used to see the key stored in the Tail node
* @param list is a pointer to the list we want to apply the function to
* @return the data stored in the TAIL of the list
*/
int showBack(struct LinkedList* list){
    return list -> tail -> key;
}

/**
* The function showNode is used with the function find to return the data
* stored in the node in a specific key
* @param node is the pointer to the node we want the info from
* @return the array of data store in the node
*/
long int showNode(struct Node* node){
    if (node != NULL)
        return node -> data;
}

/**
* The function find is used to search a value in the whole list and the
* node where it's stored
* @param list is a pointer to the list we want to apply the function to
* @param key to be searched in the list
* @return a pointer to the node where the value is stored
*/
Node* find(struct LinkedList* list, int value){
    // Creating a pointer to traverse the list nodes
    Node* temp = list -> head;

    // Traversing all the nodes until the value is found
    while (temp != NULL){
        if (temp -> key == value)
            return temp;
        else
            temp = temp -> next;
    }

    // Value wasn't found
    return NULL;
}

/**
* The function exists is used to know if a value is stored in the list
* @param list is a pointer to the list we want to apply the function to
* @param value to be searched in the list
* @return true if the value exists in the list already, false on the contrary
*/
bool exists(struct LinkedList* list, int value){
    Node* temp = find(list, value);

    // Checking if the value was found
    if (temp != NULL)
        return true;
    else
        return false;
}

/**
* The function delete is used to delete a node from the list and reconect
* all the nodes to assure integrity in the data
* @param list is a pointer to the list we want to apply the function to
* @param node to be deleted from the list
* @return the data stored in the node that was deleted or -1 if error
*/
int delete(struct LinkedList* list, struct Node* node){
    int value = 0;
    //Checking if the list has any node in it
    if (list -> size == 0)
        return -1;
    //Checking if there is a single node in the list
    else if (list -> head == list -> tail){
        list -> head = NULL;
        list -> tail = NULL;
        list -> size --;
        value = node -> key;
        freeNode(node);
        return value;
    }
    //Checking if the node to delete is the one in the head of the list
    else if (list -> head == node){
        list -> head = node -> next;
        value = node -> key;
        freeNode(node);
        list -> size --;
        return value;
    }

    //Searching for the node before the one to delete
    Node* temp = list -> head;

    while (temp -> next != node && temp -> next != NULL){
        temp = temp -> next;
        
    }

    //Checking if finding the parent of the node was successful
    if (temp -> next == NULL)
        return -1;
    //Checking if the node to delete is the one in the tail of the list
    else if (list -> tail == node){
        temp -> next = NULL;
        list -> tail = temp;
        value = node -> key;
        freeNode(node);
    }
    //The node to delete is in the middle of the list
    else{
        temp -> next = node -> next;
        value = node -> key;
        freeNode(node);
    }

    list -> size --;
    return value;
}

/**
* The function deleteFront is used to delete the HEAD node and change
* the HEAD pointer
* @param list is a pointer to the list we want to apply the function to
* @return the data stored in the node that was deleted or -1 if error
*/
int deleteFront(struct LinkedList* list){
    return delete(list, list -> head);
}

/**
* The function deleteBack is used to delete the TAIL node and change
* the TAIL pointer
* @param list is a pointer to the list we want to apply the function to
* @return the data stored in the node that was deleted or -1 if error
*/
int deleteBack(struct LinkedList* list){
    return delete(list, list -> tail);
}

/**
* The function updateData is used to change the value stored in the list
* with the assigned key
* @param list is a pointer to the list we want to apply the function to
* @param key is the key stored in the new node
* @param value is the data stored in the new node
*/
void updateData(struct LinkedList* list, int key, long int value){
    if (!exists(list, key))
        find(list, key) -> data = value;
}

/**
* The function freeList is used to free the dynamic memory assigned to
* the whole list, traversing and freeing all nodes dyanmic memory too
* @param list is a pointer to the list we want to apply the function to
*/
void freeList(struct LinkedList* list){
    while (!isEmptyList(list)){
        deleteFront(list);
    }

    free(list);
}

/**
* The function printList is used to print visually the whole list into
* console
* @param list is a pointer to the list we want to apply the function to
*/
void printList(struct LinkedList* list){
    // Checking if the list is empty
    if (length(list) == 0){
        printf("La lista esta vacia");
        return;
    }

    // Initializing needed variables and adding head tag
    Node* temp = list -> head;
    char result [1000] = "", number[10] = "";
    strcat(result, "[HEAD] -> ");

    // Traversing and concatenating the data stored in the nodes
    while (temp != NULL){
        sprintf(number,"%d -> ", temp -> key);
        strcat(result, number);
        temp = temp -> next;
    }

    // Adding tail tag and printing
    strcat(result, "[TAIL]\n");
    printf(result);
}
