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
