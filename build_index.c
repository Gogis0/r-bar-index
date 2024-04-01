/*

Repair -- an implementation of Larsson and Moffat's compression and
decompression algorithms.
Copyright (C) 2010-current_year Gonzalo Navarro

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Author's contact: Gonzalo Navarro, Dept. of Computer Science, University of
Chile. Blanco Encalada 2120, Santiago, Chile. gnavarro@dcc.uchile.cl

Modified by Gogis0 to output the SLP in a plain format

*/

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "hashing.h"

const uint64_t mersenne_prime_exponent = 61;
const uint64_t prime = ((uint64_t)1 << 61) - 1; // 2^61 - 1 // Must use mersenne_prime_exponent here
const uint64_t base = 411910476928516559; // randomly generated

typedef struct {
    unsigned int left, right;
} Tpair;

typedef struct {
    uint64_t left, right;
} Context;


long u; // |text| and later current |C| with gaps

unsigned int alph; // size of terminal alphabet, or smallest non terminal symbol

Tpair *R; // rules

size_t n; // |R|
// int n; // |R|

char *ff;
FILE *f;
size_t *sizes;
uint64_t *hashes;

size_t get_nonterminal_size(int i) {
  return (i < alph) ? 1 : sizes[i - alph];
}

uint64_t get_nonterminal_hash(int i) {
  return (i < alph) ? (uint64_t)i : hashes[i - alph];
}

void print_rule(unsigned int i, FILE *f) {
  fprintf(f, "%u %u %zd %zd\n",
    R[i - alph].left,
    R[i - alph].right,
    get_nonterminal_size(R[i - alph].left),
    get_nonterminal_hash(i - alph));
}

void compute_sizes() {
  for (int i = 0; i < n; i++) {
    sizes[i] = get_nonterminal_size(R[i].left) + get_nonterminal_size(R[i].right);
  }
}

void binarize_rules(int c, unsigned int *C) {
  unsigned int rule_count = n + alph;
  while (c > 1) {
    unsigned int new_c = 0;
    for (int i = 0; i < c-1; i += 2) {
        //fprintf(Tf, "%u %u\n", C[i], C[i+1]);
        R[n++] = (Tpair){C[i], C[i+1]};
        C[new_c++] = rule_count++;
    }
    if (c % 2 == 1) C[new_c++] = C[c-1];
    c = new_c;
  }
}

void compute_hashes() {
  for (int i = 0; i < n; i++) {
    hashes[i] = concat(get_nonterminal_hash(R[i].left),
                       get_nonterminal_hash(R[i].right),
                       get_nonterminal_size(R[i].right),
                       base,
                       mersenne_prime_exponent);
    //printf("%d: %zd\n", i, hashes[i]);
  }
  fputs("==== GRAMMAR HASHES COMPUTED!\n", stderr);
}


char get_char_at(int i, size_t x) {
  if (x < alph) return (char)x;
  uint64_t left_size = get_nonterminal_size(R[x - alph].left);
  if (i < left_size) {
    return get_char_at(i, R[x - alph].left);
  } else {
    return get_char_at(i - left_size, R[x - alph].right);
  }
}

char char_at(int i) { return get_char_at(i, n+alph); } 

void load_rules(FILE *f) {
  uint64_t n;
  fread(&n, sizeof(uint64_t), 1, f);
  R = (void *)malloc(n * sizeof(Tpair));
  fread(R, sizeof(Tpair), n, f);
}

void load_suffixient(char *filename, uint64_t* size, uint64_t **suffixient_set) {
  char full_filename[1024];
  strcpy(full_filename, filename);
  strcat(full_filename, ".suffixient");
  FILE *file = fopen(full_filename, "rb");
  fread(size, sizeof(uint64_t), 1, file);
  *suffixient_set = (uint64_t *)malloc((*size) * sizeof(uint64_t));
  fread(*suffixient_set, sizeof(uint64_t), (*size), file); 
  fclose(file);
}

// get contexts at the suffixient positions
void get_suffixient_contexts(
  const uint64_t *suffixient,
  const uint64_t n,
  const uint64_t N,
  Context *C,
  uint context_size) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < context_size; j++) {
      int pos = suffixient[i] - j - 1;
      if (pos < 0) break;
      C[i].left |= (char_at(pos) - 'A') << j * 2;
    }
    for (int j = 0; j < context_size; j++) {
      int pos = suffixient[i] + j + 1;
      if (pos >= N) break;
      C[i].right |= (char_at(pos) - 'A') << j * 2;
    }
  }
  fputs("==== CONTEXTS COMPUTED!\n", stderr);
}

// write suffixient contexts to disk
void write_suffixient_contexts(const char *name, const uint64_t n, const Context *C) {
  char fname[1024];
  strcpy(fname, name);
  strcat(fname, ".suff_contexts");
  FILE *f;
  f = fopen(fname, "wb");
  fwrite(C, sizeof(Context), n, f);
  fputs("==== CONTEXTS WRITTEN!\n", stderr);
}

// read C and R
void read_grammar(char *name) {
  // read .R file, store data in alpha and R[]
  char fname[1024];
  uint64_t len;
  FILE *f;
  struct stat s;

  strcpy(fname, name);
  strcat(fname, ".R");
  if (stat(fname, &s) != 0) {
    fprintf(stderr, "Error: cannot stat file %s\n", fname);
    exit(1);
  }
  len = s.st_size;
  f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "Error: cannot open file %s for reading\n", fname);
    exit(1);
  }
  if (fread(&alph, sizeof(int), 1, f) != 1) {
    fprintf(stderr, "Error: cannot read file %s\n", fname);
    exit(1);
  }
  // note that in the original char-based repair the R file contains also
  // a map between the 0...alph-1 and the actual symbols in the input file
  // here alph is 256 and there is no such map (this is why the two .R .C
  // formats are not compatible).

  // n is the number of rules, sizeof(int) accounts for alpha
  n = (len - sizeof(int)) / sizeof(Tpair);
  // allocate and read array of rules stored as pairs
  R = (void *)malloc(n * 2 * sizeof(Tpair));
  sizes = (void *)malloc(n * 2 * sizeof(size_t));
  hashes = (void *)malloc(n * 2 * sizeof(size_t));

  if (fread(R, sizeof(Tpair), n, f) != n) {
    fprintf(stderr, "Error: cannot read file %s\n", fname);
    exit(1);
  }

  // open C file and get the nuber of symbols in it
  strcpy(fname, name);
  strcat(fname, ".C");
  if (stat(fname, &s) != 0) {
    fprintf(stderr, "Error: cannot stat file %s\n", fname);
    exit(1);
  }
  f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "Error: cannot open file %s for reading\n", fname);
    exit(1);
  }
  unsigned int c = s.st_size/sizeof(unsigned int);
  // open C file and load the symbols into the C array
  unsigned int *C = (void *)malloc(c * sizeof(unsigned int));
  if (fread(C, sizeof(unsigned int), c, f) != c) {
    fprintf(stderr, "Error: cannot read file %s\n", fname);
    exit(1);
  }
  binarize_rules(c, C);
  fclose(f);
  fputs("==== GRAMMAR LOADED!\n", stderr);
}

void write_grammar(char *name, int n) {
  char outname[1024];
  strcpy(outname, name);
  strcat(outname, ".plainslp");
  FILE *f;
  f = fopen(outname, "w");
  if (f == NULL) {
    fprintf(stderr, "Error: cannot open file %s for writing\n", outname);
    exit(1);
  }

  fprintf(f, "%zd\n", get_nonterminal_size(n+alph));
  for (int i = 0; i < n; i++) {
    print_rule(i+alph, f);
  }
  fputs("==== GRAMMAR written!\n", stderr);
}

// print the initial command line message
void print_cmd_line(int argc, char **argv) {
  fputs("==== Command line:\n", stderr);
  for (int i = 0; i < argc; i++)
    fprintf(stderr, " %s", argv[i]);
  fputs("\n", stderr);
  if (argc != 2) {
    fprintf(stderr,
            "Usage: %s <filename>\n"
            "Generate <filename>.plainslp from its .C and .R "
            "extensions, and <filename>.suff_conetexts from <filename>.suffixient.\n",
            argv[0]);
    exit(1);
  }
}

int main(int argc, char **argv) {
  print_cmd_line(argc, argv);

  // read the suffixient set
  uint64_t suff_size, *suffixient;
  load_suffixient(argv[1], &suff_size, &suffixient);


  // open output file
  read_grammar(argv[1]);
  compute_sizes();
  compute_hashes();
  uint64_t N = get_nonterminal_size(n+alph);

  Context *contexts = (void *)malloc(suff_size * sizeof(Context));
  get_suffixient_contexts(suffixient, suff_size, N, contexts, 32);

  write_suffixient_contexts(argv[1], suff_size, contexts);
  free(suffixient);

  write_grammar(argv[1], n);
 
  return 0;
}
