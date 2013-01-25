#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h> // Dynamic arrays

#define L_LOOKAHEAD 3
#define L_SEARCH 5

const char* const original = "in ulm und um ulm wachsen ulmen."; // Input string
char *search = NULL, *lookahead = NULL; // Window pointers
char *m_search = NULL, *m_lookahead = NULL; // Match pointers
short s_search = 1; // Search window size

typedef struct {
    short start;
    short length;
    char first_mismatch;
} reference;

int main() {

    // Pointer to input
    char *input = original;

    // Initialize dynamic symbol array
    GArray *symbols = g_array_new(false, false, sizeof(reference));

    printf("A,L,Z\n");
    printf("-----\n");

    while (*(input + s_search) != '\0') {

        // Set window pointers
        search = input;
        lookahead = input + s_search;

        // Print status info
        printf("Search: [");
        for (int i = 0; i < s_search; i++) {
            printf("%c", *(search + i));
        }
        printf("], lookahead: [");
        for (int i = 0; i < L_LOOKAHEAD; i++) {
            if (*(lookahead + i) == '\0') break;
            printf("%c", *(lookahead + i));
        }
        printf("]\n");

        // Initialize variables for substring matching
        char *match = NULL;
        reference matchref = {0, 0, *lookahead};

        // Find longest match in search window
        for (int i = 0; i < s_search; i++) {

            // If the first char matches, see how long the match is.
            if (*(search + i) == *lookahead) {

                // Store reference to current item in search part
                match = search + i;

                // Move pointers ahead in both windows until no more matches occur
                do {
                    m_search = match;
                    m_lookahead = lookahead;

                    if (*m_search != *m_lookahead) {
                        break;
                    }

                    while (*(++m_search) == *(++m_lookahead) && *m_lookahead != '\0') {
                        // Advance pointers until no more matches occur
                    }

                    if (m_search - match >= matchref.length) {
                        matchref.start = lookahead - match;
                        matchref.length = m_search - match;
                        matchref.first_mismatch = *m_lookahead;
                    }

                } while (++match != lookahead);

            }
        }

        printf("%i,%i,%c\n", matchref.start, matchref.length, matchref.first_mismatch);
        g_array_append_val(symbols, matchref);

        if (matchref.length > 0) {
            // output
            if (s_search + matchref.length + 1 <= L_SEARCH) {
                s_search += matchref.length + 1;
            } else {
                input += matchref.length + 1;
            }
        } else {
            // output
            if (s_search < L_SEARCH) {
                s_search++;
            } else {
                input++;
            }
        }
    }

    // Print normal and encoded version
    printf("\nOriginal version:  \"%s\"\n", original);
    printf("Original length: %zu\n", strlen(original));
    printf("Compressed version: \"");
    reference *symbol = NULL;
    for (int i = 0; i < symbols->len; i++) {
        symbol = &g_array_index(symbols, reference, i);
        printf("%i%i%c", symbol->start, symbol->length, symbol->first_mismatch);
    }
    printf("\"\n");
    printf("Compressed length: %i", symbols->len * 3);
    printf(" (%f%%)\n", (float)(symbols->len * 3) / strlen(original) * 100);
}
