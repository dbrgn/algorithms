#include <stdio.h>
#include <string.h>

#define L_LOOKAHEAD 5
#define L_SEARCH 16

char *input = "in ulm und um ulm wachsen ulmen.\0";
char *search, *lookahead;
char *m_search, *m_lookahead;

struct reference {
    short start;
    short length;
    char first_mismatch;
};

int main() {

    while (*input != '\0') {

        // Set window pointers
        search = input;
        lookahead = input + L_SEARCH;

        // Print status info
        printf("Search: [");
        for (int i = 0; i < L_SEARCH; i++) {
            printf("%c", *(search + i));
        }
        printf("], lookahead: [");
        for (int i = 0; i < L_LOOKAHEAD; i++) {
            printf("%c", *(lookahead + i));
        }
        printf("]\n");

        // Find longest matching substring

        char *match = NULL;
        struct reference matchref = {0, 0, *lookahead};

        // Find longest match in search window
        for (int i = 0; i < L_SEARCH; i++) {

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

                    while (*(++m_search) == *(++m_lookahead)) {
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

        printf("Longest match: (%u, %u, %c)\n", matchref.start, matchref.length, matchref.first_mismatch);

        if (matchref.length > 0) {
            // output
            input += matchref.length + 1;
        } else {
            // output
            input++;
        }
    }

}
