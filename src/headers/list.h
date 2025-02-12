#ifndef lcthw_list_h
#define lcthw_list_h

#include <stdlib.h>

struct ListNode; // I don't know why this is written so here.

// A list node that is operated on in the linked list
typedef struct ListNode{
	struct ListNode* next; // pointer to the next node
	struct ListNode* prev; // pointer to the previous node
	void* value; // This is the value that the node will hold, it is declared void for used for any type
}ListNode;

// This represents our linked list, with reference to the first and last element
typedef struct List{
	int count; // This variable keeps track of the no. of nodes
	ListNode* first;
	ListNode* last;
}List;

List* List_create(); // function to return a newly allocated list
void List_destroy(List* list); // This function will take a list pointer and deallocates all the memory
void List_clear(List* list); // This is equivalent to truncating a file i.e. we simply clear all the data and not the data structure itself
void List_clear_destroy(List* list); // Basically combing both the functions into one

#define List_count(A) ((A)->count) // macro to obtain the count of nodes from a list
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL) // macro to obtain the value of the first element if it exists
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL) // macro to obtain the value of the last element if it exists

void List_push(List* list, void* value); // function to push a node in the list from the last
void* List_pop(List* list); // function to pop a node from the last and return its value
void List_unshift(List* list, void* value); // function to push a node at the beginning
void* List_shift(List* list); // function to remove a node from the beginning

void* List_remove(List* list, ListNode* value); // function to remove any given node and return its value

// A handy macro to traverse the entire list
// Basically initializes certain values and defines a for loop using those variables
// but the for loop is without a body and you must provide that body yourself
// So here we pass 4 parameters which are List(L), Starting variable(S) which can be either first or last
// Move(M) which could be next or prev depending on how you want to traverse the list
// Finally we have Variable(V) which holds the reference to the current variable
#define LIST_FOREACH(L, S, M, V) ListNode* _node = NULL; ListNode* V = NULL;\
for(V = _node = L->S; _node != NULL; V = _node = _node->M)
	
// Additional functions
List* List_split(List* list, unsigned int position); // Splits the list at the given point and returns the newly created list
void List_merge(List* src, List* dest); // Merges both the list from dest to src

#endif