#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>  // Include for sleep function

#define len 50
typedef struct 
{
	char name[len];
	char author[len];
	int isbn;
	int year;
	int availability;
	char ava[len];
}book;
typedef struct 
{
	char name[len];
	int id;
	book borrowedbooks[10];
	int i ;
}user;
int choose(void);
int addbook(book*, FILE*,int*);
int adduser(user*, FILE*,int*);
void borrow(book*, int, user*, int, FILE*);
int read(book*, FILE*,int*,user*);
//int readusers(user*, FILE*);
int main(void)
{
	FILE* f;
	int c;
	int count;
	int countusers;
	if ((f = fopen("books.txt", "r")) == NULL)
	{
		printf("File can't be opened\n");
		exit(1);
	}
	
	book* b;
	user* u;
	b = (book*)malloc(100 * sizeof(book));
	u = (user*)malloc(100 * sizeof(user));
	count =read(b,f,&countusers,u);
	c = choose();
	
	if (c == 1)
	{
		addbook(b,f,&count);
	}
	if (c == 2)
	{
		adduser(u,f,&countusers);
	}
	if (c == 3)
	{
		 borrow(b, count, u, countusers, f);
	}
	return 0;
}
int choose(void)
{
	int c;
	do {
		printf("please enter your choice\n1\tadd new book\n2\tadd new user\n3\tborrow a book\n4\treturn a book\n5\tdiplay avalable books\n");
		scanf("%d%*c", &c);
	} while (c != 1 && c != 2 && c != 3 && c != 4 && c != 5);
	return c;
}

int addbook(book*b, FILE*f,int *i)
{
	
	(* i)++;
	if ((f = fopen("books.txt", "a")) == NULL)
	{
		printf("File can't be opened\n");
		exit(1);
	}
	printf("\nEnter book title: ");
	scanf("%[^\n]%*c", b[*i].name);
	printf("\nEnter author name: ");
	scanf("%[^\n]%*c", b[*i].author);
	printf("\nEnter book's isbn: ");
	scanf("%d%*c", &b[*i].isbn);
	printf("\nEnter book publication year: ");
	scanf("%d%*c", &b[*i].year);
	b[*i].availability= 1;
	fprintf(f,"%s\n", b[*i].name);
	fprintf(f,"%s\n", b[*i].author);
	fprintf(f,"%d\n", b[*i].isbn);
	fprintf(f,"%d\n", b[*i].year);
	fprintf(f,"availability: %d\n", b[*i].availability);
	
	return *i;
}

int adduser(user* u, FILE* f,int*i)
{
	
	(*i)++;
	if ((f = fopen("books.txt", "a")) == NULL)
	{
		printf("File can't be opened\n");
		exit(1);
	}
	printf("\nEnter user name: ");
	scanf("%[^\n]%*c", u[*i].name);
	printf("\nEnter user id: ");
	scanf("%d%*c", &u[*i].id);
	u[*i].i = 0;
	fprintf(f, "USER: %s\n", u[*i].name);
	fprintf(f, "USER ID: %d\n", u[*i].id);
	fprintf(f, "USER Borrowed books: %d\n", u[*i].i);

	return *i;
}
void borrow(book* b, int count, user* u, int usercount, FILE* f) {
	int i, j;
	char bookname[len];
	char username[len];
	char s[1000];
	int found = 0;
	long pos = -1;

	printf("Enter the name of the book you want to borrow: ");
	scanf("%[^\n]%*c", bookname);

	f = fopen("books.txt", "r+");
	if (f == NULL) {
		printf("File can't be opened\n");
		exit(1);
	}

	for (i = 0; i < count; i++) {
		if (strcmp(bookname, b[i].name) == 0) {
			if (!b[i].availability) {
				printf("Book is not available\n");
				fclose(f);
				return;
			}

			printf("Enter the name of the user who wants to borrow: ");
			scanf("%[^\n]%*c", username);

			for (j = 0; j < usercount; j++) {
				if (strcmp(username, u[j].name) == 0) {
					if (u[j].i >= 10) {
						printf("User has borrowed 10 books. Cannot borrow more.\n");
						fclose(f);
						return;
					}

					found = 1;
					u[j].borrowedbooks[u[j].i++] = b[i];
					b[i].availability = 0;

					// Find the position to update the line
					fseek(f, 0, SEEK_SET);
					while (fgets(s, sizeof(s), f) != NULL) {
						pos = ftell(f) - strlen(s)-2;
						if (strstr(s, bookname) != NULL) {
							
							fgets(s, sizeof(s), f);
							fgets(s, sizeof(s), f);
							fgets(s, sizeof(s), f);
							fgets(s, sizeof(s), f);
							pos = ftell(f) - strlen(s);
							fseek(f, pos, SEEK_SET);
							// Update the availability status
							fprintf(f, " 0 %s\n", username);
							fflush(f);  // Flush the file buffer

							break;
						}
					}

					fclose(f);

					Sleep(1000);  // Add a 1-second delay

					printf("Done: User %s borrowed %s\n", u[j].name, b[i].name);
					return;
				}
			}

			if (!found) {
				printf("User doesn't exist\n");
				fclose(f);
				return;
			}
		}
	}

	printf("Book not found\n");
	fclose(f);
}


int read(book* b, FILE* f, int* count, user* u)
{
	char s[1000];
	int i = 0;
	*count = 0;
	while (fgets(s, 1000, f) != NULL)
	{
		if (strstr(s, "USER:") != NULL)

		{
			sscanf(s, "USER: %[^\n]%*c", u[*count].name);
			fgets(s, 1000, f);
			sscanf(s, "USER ID: %d", &u[*count].id);
			fgets(s, 1000, f);
			sscanf(s, "USER Borrowed books: %d", &u[*count].i);
			(*count)++;
		}
		else
		{
			sscanf(s, "%[^\n]%*c", b[i].name);
			fgets(s, 1000, f);
			sscanf(s, "%[^\n]%*c", b[i].author);
			fgets(s, 1000, f);
			sscanf(s, "%d", &b[i].isbn);
			fgets(s, 1000, f);
			sscanf(s, "%d", &b[i].year);
			fgets(s, 1000, f);
			sscanf(s, "availability: %d %s", &b[i].availability,b[i].ava);
			fgets(s, 1000, f);
			i++;
		}
	}
	return i;
}
