/*
Author:         Tom Hatskevich
Category:       pwn, heap overflow
Vulnerabilily:  heap overflow in write_note function
Compilation:    gcc notes.c -o notes -s
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define         MAX_NOTES        (50)

typedef struct note_s
{
    char* title;
    unsigned int title_length;
    char* content;
    unsigned int content_length;
} note_t;

typedef enum options_e
{
    CREATE_NOTE = 1,
    WRITE_NOTE,
    EDIT_NOTE,
    READ_NOTE,
    DELETE_NOTE
} options_t;

// ---------- GLOBALS ----------
note_t * notes[MAX_NOTES] = {NULL};


// ---------- SIGNATURES ---------
unsigned int menu();
void create_note();
void fatal_error();
int find_empty_note_index();
void write_note();
void read_note();
void delete_note();
void edit_note();

// ---------- FUNCTIONS ----------


int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);   // ignore buffering
    
    unsigned int option = 0;

    while (true)
    {
        option = menu();

        switch (option)
        {
        case CREATE_NOTE:
        {
            create_note();
            break;
        }
        case WRITE_NOTE:
        {
            write_note();
            break;
        }
        case EDIT_NOTE:
        {
            edit_note();
            break;
        }
        case READ_NOTE:
        {
            read_note();
            break;
        }
        case DELETE_NOTE:
        {
            delete_note();
            break;
        }
        
        default:
            break;
        }
    }

    return 0;
}


unsigned int menu()
{
    unsigned int user_option = 0;

    printf("1.  Create Note\n");
    printf("2.  Write Note\n");
    printf("3.  Edit Note\n");
    printf("4.  Read Note\n");
    printf("5.  Delete Note\n");

    printf(">>>     ");
    scanf("%d", &user_option);

    return user_option;
}


void create_note()
{
    note_t* new_note = NULL;
    int index = 0;

    printf("*******************************\n");
    printf("****** New Note Creation ******\n");

    index = find_empty_note_index();
    if (index == -1)
    {
        printf("[!]     You have too much notes :(\n");
        return;
    }

    new_note = calloc(sizeof(note_t), 1);
    if (new_note == NULL)
    {
        fatal_error();
    }
    notes[index] = new_note;

    printf("[*]     New Note Created [%d]\n", index);
    printf("*******************************\n");
}


void fatal_error()
{
    puts("FATAL ERROR !!!");
    exit(0xDEADBEAF);
}


int find_empty_note_index()
{
    for(unsigned int i = 0; i < MAX_NOTES; ++i)
    {
        if (notes[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}


void write_note()
{
    char* title = NULL;
    char* content = NULL;
    unsigned int title_length = 0;
    unsigned int content_length = 0;
    int index = 0;

    printf("*******************************\n");
    printf("********** Write Note *********\n");

    printf("Note Index: ");
    scanf("%d", &index);
    if (notes[index] == NULL)
    {
        puts("[!]       Invalid index");
        printf("*******************************\n");
        return;
    }

    if (notes[index]->title || notes[index]->content)
    {
        puts("ohhh, Please create new note ...");
        printf("*******************************\n");
        return;
    }

    printf("Title Length:   ");
    scanf("%d", &title_length);
    printf("Content Length:   ");
    scanf("%d", &content_length);

    content = calloc(content_length, 1);
    if (content == NULL)
    {
        fatal_error();
    }
    title = calloc(title_length, 1);
    if (title == NULL)
    {
        fatal_error();
    }

    getchar();
    printf("Title:  ");
    fgets(title, 32, stdin);                // OVERFLOW !!!
    // fgets(title, title_length, stdin);   // need to be with title_length as allocated size
    title[strcspn(title, "\n")] = '\x00';
    printf("Content:    ");
    fgets(content, content_length, stdin);
    content[strcspn(content, "\n")] = '\x00';

    notes[index]->title_length = title_length;
    notes[index]->title = title;
    notes[index]->content_length = content_length;
    notes[index]->content = content;

    printf("*******************************\n");
}


void read_note()
{
    int index = 0;

    printf("*******************************\n");
    printf("********** Read Note *********\n");

    printf("Note Index:     ");
    scanf("%d", &index);
    if (notes[index] == NULL)
    {
        puts("[!]       Invalid index");
        printf("*******************************\n");
        return;
    }

    if (notes[index]->title)
    {
        fwrite(notes[index]->title, 1, notes[index]->title_length, stdout);
        printf("\n");
    }
    else
    {
        puts("Uninitialized Title");
    }
    

    if (notes[index]->content)
    {
        fwrite(notes[index]->content, 1, notes[index]->content_length, stdout);
        printf("\n");
    }
    else
    {
        puts("Uninitialized Content");
    }

    printf("*******************************\n");
}


void delete_note()
{
    int index = 0;

    printf("*******************************\n");
    printf("********* DELETE Note *********\n");

    printf("Note Index:     ");
    scanf("%d", &index);

    if (notes[index])
    {
        if (notes[index]->title)
        {
            free(notes[index]->title);
        }

        if (notes[index]->content)
        {
            free(notes[index]->content);
        }

        free(notes[index]);
        notes[index] = NULL;
    }

    printf("*******************************\n");
}


void edit_note()
{
    int index = 0;

    printf("*******************************\n");
    printf("********** EDIT Note **********\n");

    printf("Note Index:     ");
    scanf("%d", &index);
    getchar();
    if (notes[index] == NULL)
    {
        puts("[!]       Invalid index");
        printf("*******************************\n");
        return;
    }

    if (notes[index]->title)
    {
        printf("Title:  ");
        fgets(notes[index]->title, notes[index]->title_length, stdin);
        notes[index]->title[strcspn(notes[index]->title, "\n")] = '\x00';
    }
    else
    {
        puts("[!]   Uninitialized title, Please write_note");
    }
    
    if (notes[index]->content)
    {
        printf("Content:  ");
        fgets(notes[index]->content, notes[index]->content_length, stdin);
        notes[index]->content[strcspn(notes[index]->content, "\n")] = '\x00';
    }
    else
    {
        puts("[!]   Uninitialized Content, Please write_note");
    }
    
    printf("*******************************\n");
}