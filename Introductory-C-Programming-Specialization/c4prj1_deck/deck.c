#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"

void print_hand(deck_t * hand){
  card_t ** card =hand -> cards ;
  card_t card1;
  for (size_t i = 0; i < hand->n_cards; i++) {
    card1 = *card[i];
    print_card(card1);
  }
}

int deck_contains(deck_t * d, card_t c) {
  card_t ** card = d->cards;
  for (size_t i = 0; i < d->n_cards; i++) {
    if (card[i]->value == c.value && card[i]->suit == c.suit) {
      return 1;
    }
  }
  return 0;
}

void shuffle(deck_t * d){
  card_t ** card = d->cards;
  size_t n = d->n_cards;
  int j;
  card_t * temp;
  for (size_t i = 0; i < n; i++) {
    j = rand() % n;
    temp = card[i];
    card[i] = card[j];
    card[j] = temp;
  }
}

void assert_full_deck(deck_t * d) {
  card_t ** card = d->cards;
  int count;

  for (size_t i = 0; i < d->n_cards; i++) {
    count = 0;
    for (size_t j = i + 1; j < d->n_cards; j++) {
      if (card[i]->value == card[j]->value && card[i]->suit == card[j]->suit) {
        count++;
      }
    }
    assert(count==0);
  }
}

void add_card_to(deck_t * deck, card_t c) {
    // Allocate memory for the new card
    card_t * c1 = malloc(sizeof(*c1));
    if (c1 == NULL) {
      perror("Failed to allocate memory for new card");
      exit(EXIT_FAILURE);
    }
  
    // Assign the card's value and suit
    c1->value = c.value;
    c1->suit = c.suit;
  
    // Reallocate memory for the cards array
    deck->cards = realloc(deck->cards, (deck->n_cards + 1) * sizeof(*deck->cards));
    if (deck->cards == NULL) {
      perror("Failed to reallocate memory for cards");
      exit(EXIT_FAILURE);
    }
  
    // Add the new card to the deck
    deck->cards[deck->n_cards] = c1;
  
    // Increment the number of cards in the deck
    deck->n_cards++;
}

card_t * add_empty_card(deck_t * deck) {
    // Allocate memory for the new card
    card_t * c1 = malloc(sizeof(*c1));
    if (c1 == NULL) {
      perror("Failed to allocate memory for new card");
      exit(EXIT_FAILURE);
    }
  
    // Set the card's value and suit to 0 (invalid card)
    c1->value = 0;
    c1->suit = 0;
  
    // Reallocate memory for the cards array
    deck->cards = realloc(deck->cards, (deck->n_cards + 1) * sizeof(*deck->cards));
    if (deck->cards == NULL) {
      perror("Failed to reallocate memory for cards");
      exit(EXIT_FAILURE);
    }
  
    // Add the new card to the deck
    deck->cards[deck->n_cards] = c1;
  
    // Increment the number of cards in the deck
    deck->n_cards++;
  
    // Return a pointer to the newly added card
    return c1;
}

deck_t * make_deck_exclude(deck_t * excluded_cards) {
    // Allocate memory for the new deck
    deck_t * new_deck = malloc(sizeof(*new_deck));
    if (new_deck == NULL) {
      perror("Failed to allocate memory for new deck");
      exit(EXIT_FAILURE);
    }
  
    // Initialize the new deck
    new_deck->cards = NULL;
    new_deck->n_cards = 0;
  
    // Loop through all possible cards (52 cards in a standard deck)
    for (unsigned i = 0; i < 52; i++) {
      card_t card = card_from_num(i); // Generate a card from its number
  
      // Check if the card is in the excluded_cards deck
      if (!deck_contains(excluded_cards, card)) {
        // Add the card to the new deck if it's not excluded
        add_card_to(new_deck, card);
      }
    }
  
    return new_deck; // Return the newly created deck
}

void free_deck(deck_t * deck) {
    if (deck == NULL) {
      return;
    }
  
    // Free each card in the deck
    for (size_t i = 0; i < deck->n_cards; i++) {
      free(deck->cards[i]);
    }
  
    // Free the cards array
    free(deck->cards);
  
    // Free the deck structure itself
    free(deck);
}


deck_t * build_remaining_deck(deck_t ** hands, size_t n_hands) {
    // Create a deck to hold all the cards from the hands
    deck_t * all_cards = malloc(sizeof(*all_cards));
    if (all_cards == NULL) {
      perror("Failed to allocate memory for all_cards deck");
      exit(EXIT_FAILURE);
    }
  
    // Initialize the deck
    all_cards->cards = NULL;
    all_cards->n_cards = 0;
  
    // Add all cards from all hands to the all_cards deck
    for (size_t i = 0; i < n_hands; i++) {
      for (size_t j = 0; j < hands[i]->n_cards; j++) {
        add_card_to(all_cards, *(hands[i]->cards[j]));
      }
    }
  
    // Use make_deck_exclude to create the remaining deck
    deck_t * remaining_deck = make_deck_exclude(all_cards);
  
    // Free the temporary all_cards deck
    free_deck(all_cards);
  
    return remaining_deck;
}