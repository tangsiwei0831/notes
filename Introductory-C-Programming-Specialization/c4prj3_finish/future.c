#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
#include "cards.h"
#include "future.h"


void add_future_card(future_cards_t * fc, size_t index, card_t * ptr) {
    // Ensure there are enough decks to accommodate the index
    while (fc->n_decks <= index) {
        fc->decks = realloc(fc->decks, (fc->n_decks + 1) * sizeof(*fc->decks));
        fc->decks[fc->n_decks].cards = NULL;
        fc->decks[fc->n_decks].n_cards = 0;
        fc->n_decks++;
    }

    // Add the card pointer to the specified deck
    fc->decks[index].cards = realloc(fc->decks[index].cards, 
                                     (fc->decks[index].n_cards + 1) * sizeof(*(fc->decks[index].cards)));
    fc->decks[index].cards[fc->decks[index].n_cards] = ptr;
    fc->decks[index].n_cards++;
}

void future_cards_from_deck(deck_t * deck, future_cards_t * fc) {
    // Ensure there are enough cards in the deck to fill all placeholders
    if (deck->n_cards < fc->n_decks) {
      fprintf(stderr, "Error: Not enough cards in the deck to fill all placeholders.\n");
      exit(EXIT_FAILURE);
    }
  
    // Iterate through each future card deck
    for (size_t i = 0; i < fc->n_decks; i++) {
      if(fc->decks[i].n_cards == 0) continue;
      // Assign the drawn card's value and suit to all placeholders in this future card deck
      for (size_t j = 0; j < fc->decks[i].n_cards; j++) {
        fc->decks[i].cards[j]->value = deck->cards[i]->value;
        fc->decks[i].cards[j]->suit = deck->cards[i]->suit;
      }
    }
}