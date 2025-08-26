#include <stdio.h>
#include <stdlib.h>


// Define node structure
struct node {
    void *data;
    struct node* next;
};

// Function to create a new node
struct node* create_node(void *data) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    if (!new_node) {
        printf("Memory allocation error\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Insert a node at the beginning
void insert(struct node** head, void *data) {
    struct node* new_node = create_node(data);
    new_node->next = *head;
    *head = new_node;
}

// Free the list
void free_list(struct node* head, void (*delete_handler)(void*)) {
    struct node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        if(delete_handler) delete_handler(temp->data);
        free(temp);
    }
}

void *find(struct node *head, int (*compare)(void*, void*), void *key) {
    while(head != NULL) {
        if(0 == compare(head->data, key)) return head->data;
        head = head->next;
    }

    return NULL;
}

struct node *delete(struct node *head, int (*compare)(void*,void*), void *key, void (*delete_handler)(void*)) {
    struct node *origin = head;
    struct node *last = NULL;
    while(head) {
        if(0 == compare(head->data, key)) {
            struct node *tmp = head;
            head = head->next;
            if(last == NULL) { // first node 
                origin = head;
            } else { // middle node
                last->next = head;
            }
            // free node
            if(delete_handler) delete_handler(tmp->data);
            free(tmp);
        } else {
            last = head;
            head = head->next;
        }
    }

    return origin;
}



/*====================================== SPECIFIC =============*/

struct allocation {
    int id;
    int data;
};


void print_list(struct node *head) {
    while(head) {
        struct allocation *allocation = (struct allocation*)(head->data);
        printf("Key: %d, Value: %d\n", allocation->id, allocation->data);
        head = head->next;
    }
}

int find_by_id(void *curr_ptr, void *key_ptr) {
    struct allocation *curr = (struct allocation*)curr_ptr;
    struct allocation *key = (struct allocation*)key_ptr;
    if(curr->id == key->id) return 0;
}

void my_delete_handler(void *data) {
    struct allocation *allocation = (struct allocation*)data;

    printf("Freeing: %d\n", allocation->id);
}

// Example usage
int main() {
    
    struct allocation alloc1;
    alloc1.id = 1;
    alloc1.data = 52;

    struct allocation alloc2;
    alloc2.id = 2;
    alloc2.data = 69;

    struct allocation alloc3;
    alloc3.id = 3;
    alloc3.data = 100;

    struct node *head = create_node(&alloc1);
    insert(&head, &alloc2);
    insert(&head, &alloc3);
    
    print_list(head);

    struct allocation key;
    key.id = 3;
    
    struct allocation *found = find(head, find_by_id, &key);
    printf("%d\n", found->data);
    
    head = delete(head, find_by_id, &key, my_delete_handler);
    print_list(head);
   
    free_list(head, my_delete_handler); 
    return 0;
}
