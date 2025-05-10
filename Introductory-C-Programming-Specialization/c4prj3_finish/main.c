#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deck.h"
#include "future.h"
#include "eval.h"
#include "input.h"

#define DEFAULT_TRIALS 10000

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <input_file> [num_trials]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse command-line arguments
    const char *input_file = argv[1];
    size_t num_trials = (argc == 3) ? strtoul(argv[2], NULL, 10) : DEFAULT_TRIALS;

    // Open the input file
    FILE *f = fopen(input_file, "r");
    if (f == NULL) {
        perror("Could not open input file");
        return EXIT_FAILURE;
    }

    // Read hands and initialize future cards
    size_t n_hands = 0;
    future_cards_t fc = {NULL, 0};
    deck_t **hands = read_input(f, &n_hands, &fc);
    fclose(f);

    // Create the remaining deck
    deck_t *remaining_deck = build_remaining_deck(hands, n_hands);

    // Initialize win counts
    unsigned *win_counts = calloc(n_hands + 1, sizeof(*win_counts));
    if (win_counts == NULL) {
        perror("Failed to allocate memory for win counts");
        return EXIT_FAILURE;
    }

    // Perform Monte Carlo trials
    for (size_t i = 0; i < num_trials; i++) {
        shuffle(remaining_deck);
        future_cards_from_deck(remaining_deck, &fc);

        size_t winning_hand = 0;
        int tie = 0;

        for (size_t j = 1; j < n_hands; j++) {
            int result = compare_hands(hands[winning_hand], hands[j]);
            if (result < 0) {
                winning_hand = j;
                tie = 0;
            } else if (result == 0) {
                tie = 1;
            }
        }

        if (tie) {
            win_counts[n_hands]++;
        } else {
            win_counts[winning_hand]++;
        }
    }

    // Print results
    for (size_t i = 0; i < n_hands; i++) {
        printf("Hand %zu won %u / %zu times (%.2f%%)\n",
               i, win_counts[i], num_trials,
               (100.0 * win_counts[i]) / num_trials);
    }
    printf("And there were %u ties\n", win_counts[n_hands]);

    // Free allocated memory
    free(win_counts);
    free_deck(remaining_deck);
    for (size_t i = 0; i < n_hands; i++) {
        free_deck(hands[i]);
    }
    free(hands);
    for (size_t i = 0; i < fc.n_decks; i++) {
        free(fc.decks[i].cards);
    }
    free(fc.decks);

    return EXIT_SUCCESS;
}