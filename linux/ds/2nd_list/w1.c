#include <stdio.h>
#include <stdlib.h>

struct node_t {
	int data;
	struct node_t *next;
	struct node_t *pre;
};

int main()
{
	struct node_t *head = NULL;
	struct node_t *tail = NULL; 
	struct node_t *new = NULL;
	int n;
	while (1) {
		printf("Pls input the n:");
		scanf("%d", &n);
		while (getchar() != '\n')
			;
		if (n == -1)
			break;

		new = (struct node_t *) malloc (sizeof(struct node_t));
		new->data = n;
		new->next = NULL;
		new->pre = NULL;
	
		if (head == NULL) {
			head = new;
			tail = new;
			tail->next = head;
			tail->pre = head;
		} else {
			if (head->data > new->data) {
				new->next = head;
				new->pre = head->pre;
				head->pre->next = new;
				head->pre = new;
				head = new;
			} else {
				for (tail = head; 
						tail->next->data < new->data && tail->next != head;
						tail = tail->next) 
					;
								new->next = tail->next;
					new->pre = tail;
					tail->next->pre = new;
					tail->next = new;
			}
		}
	}	

	//for (tail = head; tail->next != head; tail = tail->next){
	//	printf("%d", tail->data);
	//}
	tail = head;
	printf("%d\t", tail->data);
	do {
		tail = tail->next;
		printf("%d\t", tail->data);
	} while (tail->next != head);	
	printf("\n");

	for (tail = head->next; tail->next != head; tail = new) {
		new = tail->next;
		free(tail);
	}
	return 0;
}
