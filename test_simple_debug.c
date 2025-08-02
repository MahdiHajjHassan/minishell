#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <file> <line> <text>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    
    // Read file into memory
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    
    // Find the line to insert after
    int target_line = atoi(argv[2]);
    int current_line = 1;
    char *pos = content;
    
    while (current_line < target_line && *pos) {
        if (*pos == '\n') current_line++;
        pos++;
    }
    
    if (current_line != target_line) {
        printf("Line %d not found\n", target_line);
        free(content);
        return 1;
    }
    
    // Skip to end of line
    while (*pos && *pos != '\n') pos++;
    if (*pos == '\n') pos++;
    
    // Write the new file
    file = fopen(argv[1], "w");
    fwrite(content, 1, pos - content, file);
    fprintf(file, "%s\n", argv[3]);
    fwrite(pos, 1, content + size - pos, file);
    fclose(file);
    
    free(content);
    return 0;
}