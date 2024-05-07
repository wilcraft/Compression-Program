#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    const char* data;
    int num;
    struct node* pNext;
}Node;
void Add(Node** h, const char* new_data) {
    Node* Temp = (Node*)malloc(sizeof(Node));
    Node* last = *h;
    Node* Next;
    Temp->data = new_data;
    Temp->pNext = NULL;
    Temp->num = NULL;
    if ((*h) == NULL) {
        (*h) = Temp;
        Temp->num++;
        last = Temp;
        return;
    }
    while (last->pNext != NULL) {
        last = last->pNext;
    }
    Temp->num = last->num;
    Temp->num++;
    if (NULL != Temp->pNext) {
        Next = Temp->pNext;
        Temp->num = Next->num;
        Temp->num++;
    }
    last->pNext = Temp;
    last = Temp;
}
void compress(Node** h, FILE* fOutput) {
    Node* curr = *h;
    Node* temp;
    while (NULL != curr) {
        temp = curr->pNext;
        while (NULL != temp) {
            if (curr->data == temp->data && curr != temp) {
                curr->num = temp->num;
            }
            temp = temp->pNext;
        }
        fprintf(fOutput, "%d ", curr->num);
        curr = curr->pNext;
    }
}
void uncompress(Node** h, FILE* fOutput, FILE* FInput) {
    char* ch;
    int string_size, read_size;
    const char* buff = NULL;
    Node* curr = *h;
    fseek(FInput, 0, SEEK_END);
    string_size = ftell(FInput);
    rewind(FInput);
    ch = (char*)malloc(sizeof(char) * (string_size + 1));
    read_size = fread(ch, sizeof(char), string_size, FInput);
    ch[string_size] = '\0';
    char* token = strtok(ch, "  \n");
    while (NULL != token) {
        if (curr->num == atoi(token)) {
            buff = curr->data;
            fprintf(fOutput, "%s ", buff);
            curr = curr->pNext;
            token = strtok(NULL, "  \n");
        }
        else {
            break;
        }
    }
}
/*
--fRead_file = CHANGE FIRST PARAMETER TO THE NAME OF THE FILE THAT YOU WOULD WISH TO COMPRESS
               OR SIMPLY REPLACE TEXT IN THE ALREADY EXISTING ONE
--!!PLACE FILE IN PROJECT DIRECTORY BEFOREHAND!!

--Project made by S.K. - TU Sofia
*/
int main() {
    Node* Head = NULL;
    FILE* fRead_file = fopen("Example.txt", "r");
    FILE* fCompress_file = fopen("compressed.txt", "w");
    FILE* fUncompress_file = fopen("uncompressed.txt", "w");
    char* buffer;
    int string_size, read_size;
    if (fRead_file) {
        fseek(fRead_file, 0, SEEK_END);
        string_size = ftell(fRead_file);
        rewind(fRead_file);
        buffer = (char*)malloc(sizeof(char) * (string_size + 1)); //Byte buffer the size of the file in bytes + 1 for NO-BREAK SPACE
        read_size = fread(buffer, sizeof(char), string_size, fRead_file);
        buffer[string_size] = '\0';
        char* token = strtok(buffer, " \n");
        while (NULL != token) {
            Add(&Head, token);
            token = strtok(NULL, "  \n");
        }
    }

    compress(&Head, fCompress_file);
    fflush(fCompress_file);
    FILE* fTest = fopen("compressed.txt", "r");
    uncompress(&Head, fUncompress_file, fTest);
    fclose(fCompress_file);
    fclose(fUncompress_file);
    fclose(fRead_file);
}