#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
#include "cards.h"
#include "future.h"

deck_t * hand_from_string(const char * str, future_cards_t * fc) {
    // Allocate memory for the new hand
    deck_t * hand = malloc(sizeof(*hand));
    if (hand == NULL) {
      perror("Failed to allocate memory for hand");
      exit(EXIT_FAILURE);
    }
    hand->cards = NULL;
    hand->n_cards = 0;
  
    // Parse the input string
    const char * ptr = str;
    while (*ptr != '\0') {
      // Skip whitespace
      while (*ptr == ' ' || *ptr == '\t') {
        ptr++;
      }
  
      // Check for end of string
      if (*ptr == '\0' || *ptr == '\n') {
        break;
      }
  
      // Handle future cards (e.g., ?0, ?1)
      if (*ptr == '?') {
        ptr++;
        char * endptr;
        size_t index = strtoul(ptr, &endptr, 10); // Parse the number after '?'
        if (ptr == endptr) {
          fprintf(stderr, "Invalid future card format: %s\n", str);
          exit(EXIT_FAILURE);
        }
        ptr = endptr;
  
        // Add an empty card to the hand and track it in future cards
        card_t * empty_card = add_empty_card(hand);
        add_future_card(fc, index, empty_card);
      }
      // Handle regular cards (e.g., Kh, Qs)
      else {
        if (*(ptr + 1) == '\0') {
          fprintf(stderr, "Invalid card format: %s\n", str);
          exit(EXIT_FAILURE);
        }
        card_t card = card_from_letters(*ptr, *(ptr + 1));
        add_card_to(hand, card);
        ptr += 2;
      }
    }
  
    // Ensure the hand has at least 5 cards
    if (hand->n_cards < 5) {
      fprintf(stderr, "Error: Hand must have at least 5 cards: %s\n", str);
      exit(EXIT_FAILURE);
    }
  
    return hand;
}


deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc) {
    // Initialize the array of hands and the number of hands
    deck_t ** hands = NULL;
    *n_hands = 0;
  
    // Buffer to store each line of input
    char * line = NULL;
    size_t line_size = 0;
  
    // Read each line from the file
    while (getline(&line, &line_size, f) >= 0) {
      // Parse the line into a hand
      deck_t * hand = hand_from_string(line, fc);
  
      // Reallocate the hands array to add the new hand
      hands = realloc(hands, (*n_hands + 1) * sizeof(*hands));
      if (hands == NULL) {
        perror("Failed to reallocate memory for hands");
        exit(EXIT_FAILURE);
      }
  
      // Add the new hand to the array
      hands[*n_hands] = hand;
      (*n_hands)++;
    }
  
    // Free the line buffer
    free(line);
  
    return hands;
}