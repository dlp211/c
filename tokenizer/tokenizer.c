/*
 * file: tokenizer.c
 * author: David L Patrzeba (c) holder
 * author: Jesse Ziegler (c) holder
 * license: MIT (http://opensource.org/licenses/MIT) (c) 2013
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tokenizer type */
struct TokenizerT_ {

  /* This points to the string of delimiters. Should never be changed. */
  char *sepr;
  /*
   * This is a place holder to the front of the token string. Should never be
   * changed.
   */
  char *head;
  /* This is a pointer to the next item to tokenize. */
  char *tail;
};

/* Use TokenizerT as the type. */
typedef struct TokenizerT_ TokenizerT;

/*
 * assign_char_from_digit takes three arguments, the pointer to the begining of
 * a string that represents some integer, the length of that string, and the
 * base that it is in.  It then converts the string to its character
 * representation and returns it.
 */
char assign_char_from_digit (
    char const *const s,
    size_t const length,
    unsigned short base
) {

    char *const val = malloc( length + 1 );
    int i;

    for ( i = 0; i != length; ++i ) {
      val[i] = s[i];
    }
    val[length] = '\0';

    long val_char = strtol( val, NULL, base );
    free( val );
    return val_char;
}

/*
 * assign_char takes a const pointer to a constant character (immutable) and
 * returns the appropriate escape character.
 * If the character is not one of the encoded escape characters, it prints
 * an error message and exits the program.
 */
char assign_char ( char const *const s ) {
  switch ( *s ) {
    case 'a':   return '\a';
    case 'b':   return '\b';
    case 'n':   return '\n';
    case 'r':   return '\r';
    case 't':   return '\t';
    case '\\':  return '\\';
    case '?':   return '\?';
    case '\'':  return '\'';
    case '\"':  return '\"';
    default:
      printf("ERROR, the character after \'\\\' was not recognized");
      exit(EXIT_FAILURE);
  }
}

/*
 * simplify_string takes a string with escape characters still in the form of
 * '\X' where X is some character and returns a new string and substitutes
 * escape character combinations with the actual escape character.
 */
char *simplify_string ( char const *s ) {
  char *iterator;
  char *return_string;
  if ( !( return_string = iterator = malloc( strlen( s ) + 1 ) ) ) {
      return NULL;
  }

  for ( ; *s; ++s, ++iterator ) {
    if ( *s == '\\' ) {
      if ( isdigit(*++s) ) {
        *iterator = assign_char_from_digit( s, 3, 8 );
        s += 2;
      }
      else if ( *s == 'x' ) {
        *iterator = assign_char_from_digit( ++s, 2, 16 );
        ++s;
      }
      else {
        *iterator = assign_char( s );
      }
    }
    else {
      *iterator = *s;
    }
  }
  *iterator = '\0';
  return return_string;
}

/*
 * TKCreate creates a new TokenizerT object for a given set of serarator
 * characters (given as a string) and a tken stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependant on them
 * staying immutable after returning.
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 */
TokenizerT *TKCreate ( char const *const seperators, char const *const ts ) {
  /*
   * Make copies of the two strings.
   * This uses malloc so it is creating dynamically allocated strings.
   */
  char *const delimiters = simplify_string( seperators );
  char *const token = simplify_string( ts );
  TokenizerT *const tk = malloc( sizeof( TokenizerT ) );

  /* If neither string is NULL create a TokenizerT and return a pointer. */
  if ( delimiters && token && tk ) {
    *tk = (TokenizerT) {delimiters, token, token};
    return tk;
  }

  /* Else return NULL. */
  return NULL;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 */
void TKDestroy ( TokenizerT *const tk ) {
  free(tk->sepr);
  free(tk->head);
  free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a character
 * string.  Space for the returned token should be dynamically allocated.  The
 * caller is responsible for freeing the space once it is no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 */
char *TKGetNextToken ( TokenizerT *const tk ) {
  /*
   * Move pass any delimiters at the front of the remaining tokens or
   * return if the end of the string.
   * might need to implement my own will eliminate \0 check on every iteration.
   */
  while ( strchr( tk->sepr, *tk->tail ) ) {
    if ( !*tk->tail ) {
      return 0;
    }
    tk->tail++;
  }

  /* Create a placeholder that points to the begining of the next token. */
  char const *const head = tk->tail;
  /* While it isn't a delimiter keep going. */
  while ( !strchr( tk->sepr, *tk->tail ) ) {
      ++tk->tail;
  }

  /* Return the token. */
  char *ret;
  if ( ( ret = malloc( tk->tail - head + 1 ) ) ) {
    ret[tk->tail - head] = '\0';
    if ( *tk->tail ) {
      ++tk->tail;
      return strncpy( ret, head, tk->tail - head - 1);
    }
    return strncpy( ret, head, tk->tail - head);
  }
  return 0;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string conatins the seperator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
int main ( int argc, char **argv ) {
  char *token;

  /*
   * Checks to make sure that we have the right amount of args and if
   * the right amount of args have been supplied, try to create a
   * TokenizerT.  Otherwise it fails and exits.
   */
  if ( argc != 3 ) {
    printf("Incorrect number of arguments\n");
    return EXIT_FAILURE;
  }
  TokenizerT *const tk = TKCreate( argv[1], argv[2] );
  if ( !tk ) {
    printf("Could not create tokenizer\n");
    return EXIT_FAILURE;
  }

  /* Iterates over the tokens and prints them out */
  while ( ( token = TKGetNextToken(tk) ) ) {
    printf("%s\n", token);
    free(token);
  }

  /* Cleanup and finish. */
  TKDestroy(tk);
  return EXIT_SUCCESS;
}

