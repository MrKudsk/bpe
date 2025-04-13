#include <stdio.h>
#include <string.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct {
  char pair[2];
} Pair;

typedef struct {
  Pair key;
  size_t value;
} Freq;

int compare_freqs(const void *a, const void *b) {
  const Freq *af = a;
  const Freq *bf = b;
  return (int)af->value - (int)bf->value;
}

typedef struct {
  Freq *items;
  size_t count;
  size_t capacity;
} Freqs;

int main() {
  const char *text =
      "The original BPE algorithm operates by iteratively replacing the most "
      "common contiguous sequences of characters in a target text with unused "
      "'placeholder' bytes. The iteration ends when no sequences can be found, "
      "leaving the target text effectively compressed. Decompression can be "
      "performed by reversing this process, querying known placeholder terms "
      "against their corresponding denoted sequence, using a lookup table. In "
      "the original paper, this lookup table is encoded and stored alongside "
      "the compressed text.";

  int text_size = strlen(text);

  Freq *freq = NULL;

  for (int i = 0; i < text_size - 1; ++i) {
    Pair pair = {.pair = {text[i], text[i + 1]}};
    ptrdiff_t i = hmgeti(freq, pair);
    if (i < 0)
      hmput(freq, pair, 1);
    else
      freq[i].value += 1;
  }

  // Freqs sorted_freqs = {0};

  // for (ptrdiff_t i = 0; i < hmlen(freq); ++i) {
  //     da_append(&sorted_freqs, freq[i]);
  // }

  // qsort(sorted_freqs.items, sorted_freqs.count, sizeof(*sorted_freqs.items),
  // compare_freqs);

  // for (size_t i = 0; i < 10; ++i) {
  //     Freq *freq = &sorted_freqs.items[i];
  //     printf("(%u, %u) => %zu\n", freq->key.l, freq->key.r, freq->value);
  // }
  Freqs sorted_freqs = {0};

  for (ptrdiff_t i = 0; i < hmlen(freq) - 1; ++i) {
    da_append(&sorted_freqs, freq[i]);
  }

  qsort(sorted_freqs.items, sorted_freqs.count, sizeof(*sorted_freqs.items),
        compare_freqs);

  for (size_t i = 0; i < sorted_freqs.count; ++i) {
    Freq *freq = &sorted_freqs.items[i];
    printf("%c%c => %zu\n", freq->key.pair[0], freq->key.pair[1], freq->value);
  }

  return 0;
}
