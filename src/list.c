#include <list.h>
#include <dbg.h>

List* List_create()
{
	return calloc(1, sizeof(List)); // Used calloc because it zero-initialized the data structure
}

void List_destroy(List* list)
{
	LIST_FOREACH(list, first, next, cur){ // we go through the entire list
		if(cur->prev){ // and if the previous exists we simply deallocate memory from it
			free(cur->prev); // Visually looks cool *****->####
		}
	}
	
	free(list->last); // at last we free the end node
	free(list); // and then free the whole list
}

void List_clear(List* list)
{
	// Traversing the whole list
	LIST_FOREACH(list, first, next, cur){
		free(cur->value); // and freeing the value element of then ode
	}
}

void List_clear_destroy(List* list)
{
	// Combines both of the operations into a handy function
	List_clear(list);
	List_destroy(list);
}

void List_push(List* list, void* value)
{
	ListNode* node = calloc(1, sizeof(ListNode)); // first we create a new node
	check_mem(node); // safety measure
	
	node->value = value; // giving it the provided value
	
	if(list->last == NULL){ // condition when the list is just created
		list->first = node; // we simply assign the node as both the first and the last
		list->last = node;
	}else{ // however when the last exists we simply make it as the last
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}
	
	list->count++; // increment the no. of nodes
	
	error:
		return;
}

void* List_pop(List* list)
{
	ListNode* node = list->last; // creating a pointer to the last node
	return node != NULL ? List_remove(list, node) : NULL; // simply removing the last node
}

void List_unshift(List* list, void* value)
{
	// In this function we will push node to the beginning of the list
	ListNode* node = calloc(1, sizeof(ListNode)); // create a new node as usual
	check_mem(node); // safety first
	
	node->value = value; // giving it the provided value
	
	if(list->first == NULL){ // if the list is just created then
		list->first = node; // make the list as both the first and last element
		list->last = node;
	}else{ // however when first exists we simply make the node as first
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}
	
	list->count++;
	
	error:
		return;
}

void* List_shift(List* list)
{
	ListNode* node = list->first; // create a pointer to the first node
	return node != NULL ? List_remove(list, node) : NULL; // simply remove the first node if it exists
}

void* List_remove(List* list, ListNode* node)
{
	void* result = NULL;
	
	check(list->first && list->last, "List is empty.");
	check(node, "node can't be NULL");
	
	if(node == list->first && node == list->last){ // checking if the list only has one element
		list->first = NULL; // simply setting the first and last to be null
		list->last = NULL;
	}else if(node == list->first){ // if it is the first node
		list->first = node->next; // then we make the next of first as the new first
		check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
		list->first->prev = NULL; // and set the previous to be null
	}else if(node == list->last){ // if it is the last node
		list->last = node->prev; // then make its previous the last
		check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
		list->last->next = NULL; // and sets its next to be null
	}else{ // if it is something in the middle
		ListNode* after = node->next; // then we simly connect its next and previous with one another
		ListNode* before = node->prev;
		after->prev = before;
		before->next = after;
	}
	
	list->count--; // decrement the no. of nodoes
	result = node->value; // the value to be returned
	free(node); // simply deallocate the memory from the node
	
	error:
		return result;
}

List* List_split(List* list, unsigned int position)
{
	// Say I have a list of 4 elements and then I want to split it at position 2
	// Before : |1|->|2|->|3|->|4|->X, After : |1|->|2|->X and |3|->|4|->X 
	// If we choose position 2 that means I would need to split it at the end of 2
	// i.e. the next of 2 and we would need to make some changes
	// -> Update the first of the new list
	// -> Update the last of the new list
	// -> Update the last of the original list
	// -> Update both lists count 
	check(list != NULL, "Invalid List!");
	check(list->first && list->last, "List is empty!");
	check(List_count(list) > 2, "Can't split a single element list!")
	check((position != 0) && (position < List_count(list)), "Invalid position!");
	
	ListNode* node = NULL;
	int pos = 1;
	
	// Linear Traveral upto the specified position
	LIST_FOREACH(list, first, next, cur){
		if(pos == position){
			node = cur;
			break;
		}
		pos++;
	}
	
	// Creating a new List
	List* new_list = List_create(); 
	check(new_list != NULL, "Failed to create new list!");
	
	// Updating the first and last of the new list
	new_list->first = node->next;
	new_list->first->prev = NULL;
	new_list->last = list->last;
	
	// Updating the last of the original list
	list->last = node;
	list->last->next = NULL;
	
	// Updating the count of both the list
	new_list->count = List_count(list) - position;
	list->count = position;
	
	return new_list;
error:
	return NULL;
}

void List_merge(List* src, List* dest)
{
	check(src != NULL, "Invalid List!");
	check(dest != NULL, "Invalid List!");
	
	// okay so inorder to merge these two linked lists
	// we must take into account some factors such as:
	// -> Updating the end of the src list
	// -> Updating the length of the src list
	// -> Updating the first and last of the dest list
	// -> Updating the length of the dest list
	
	// 1. Attach the last of the of src list to the first of the dest list
	src->last->next = dest->first;
	dest->first->prev = src->last;
	
	// 2. Update the end of the src list
	src->last = dest->last;
	
	// 3. Update the length of the list
	src->count = src->count + List_count(dest);
	
	// 4. Update the first and last of the dest list with length
	dest->first = dest->last = NULL;
	dest->count = 0;
	
error: // fallthrough
	return;
}