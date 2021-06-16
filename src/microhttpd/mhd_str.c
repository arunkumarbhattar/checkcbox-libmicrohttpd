/*
  This file is part of libmicrohttpd
  Copyright (C) 2015, 2016 Karlson2k (Evgeny Grin)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
 * @file microhttpd/mhd_str.c
 * @brief  Functions implementations for string manipulating
 * @author Karlson2k (Evgeny Grin)
 */

#include "mhd_str.h"

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif /* HAVE_STDBOOL_H */
#include <string.h>

#include "mhd_assert.h"
#include "mhd_limits.h"

#ifdef MHD_FAVOR_SMALL_CODE
#ifdef _MHD_static_inline
#undef _MHD_static_inline
#endif /* _MHD_static_inline */
/* Do not force inlining and do not use macro functions, use normal static
   functions instead.
   This may give more flexibility for size optimizations. */
#define _MHD_static_inline static
#ifndef INLINE_FUNC
#define INLINE_FUNC 1
#endif /* !INLINE_FUNC */
#endif /* MHD_FAVOR_SMALL_CODE */

/*
 * Block of functions/macros that use US-ASCII charset as required by HTTP
 * standards. Not affected by current locale settings.
 */

#ifdef INLINE_FUNC

#if 0 /* Disable unused functions. */
/**
 * Check whether character is lower case letter in US-ASCII
 *
 * @param c character to check
 * @return non-zero if character is lower case letter, zero otherwise
 */
_MHD_static_inline bool
isasciilower (char c)
{
  return (c >= 'a') && (c <= 'z');
}


#endif /* Disable unused functions. */


/**
 * Check whether character is upper case letter in US-ASCII
 *
 * @param c character to check
 * @return non-zero if character is upper case letter, zero otherwise
 */
_MHD_static_inline bool
isasciiupper (char c)
{
  return (c >= 'A') && (c <= 'Z');
}


#if 0 /* Disable unused functions. */
/**
 * Check whether character is letter in US-ASCII
 *
 * @param c character to check
 * @return non-zero if character is letter in US-ASCII, zero otherwise
 */
_MHD_static_inline bool
isasciialpha (char c)
{
  return isasciilower (c) || isasciiupper (c);
}


#endif /* Disable unused functions. */


/**
 * Check whether character is decimal digit in US-ASCII
 *
 * @param c character to check
 * @return non-zero if character is decimal digit, zero otherwise
 */
_MHD_static_inline bool
isasciidigit (char c)
{
  return (c >= '0') && (c <= '9');
}


#if 0 /* Disable unused functions. */
/**
 * Check whether character is hexadecimal digit in US-ASCII
 *
 * @param c character to check
 * @return non-zero if character is decimal digit, zero otherwise
 */
_MHD_static_inline bool
isasciixdigit (char c)
{
  return isasciidigit (c) ||
         ( (c >= 'A') && (c <= 'F') ) ||
         ( (c >= 'a') && (c <= 'f') );
}


/**
 * Check whether character is decimal digit or letter in US-ASCII
 *
 * @param c character to check
 * @return non-zero if character is decimal digit or letter, zero otherwise
 */
_MHD_static_inline bool
isasciialnum (char c)
{
  return isasciialpha (c) || isasciidigit (c);
}


#endif /* Disable unused functions. */


#if 0 /* Disable unused functions. */
/**
 * Convert US-ASCII character to lower case.
 * If character is upper case letter in US-ASCII than it's converted to lower
 * case analog. If character is NOT upper case letter than it's returned
 * unmodified.
 *
 * @param c character to convert
 * @return converted to lower case character
 */
_MHD_static_inline char
toasciilower (char c)
{
  return isasciiupper (c) ? (c - 'A' + 'a') : c;
}


/**
 * Convert US-ASCII character to upper case.
 * If character is lower case letter in US-ASCII than it's converted to upper
 * case analog. If character is NOT lower case letter than it's returned
 * unmodified.
 *
 * @param c character to convert
 * @return converted to upper case character
 */
_MHD_static_inline char
toasciiupper (char c)
{
  return isasciilower (c) ? (c - 'a' + 'A') : c;
}


#endif /* Disable unused functions. */


#if defined(MHD_FAVOR_SMALL_CODE) /* Used only in MHD_str_to_uvalue_n_() */
/**
 * Convert US-ASCII decimal digit to its value.
 *
 * @param c character to convert
 * @return value of decimal digit or -1 if @ c is not decimal digit
 */
_MHD_static_inline int
todigitvalue (char c)
{
  if (isasciidigit (c))
    return (unsigned char) (c - '0');

  return -1;
}


#endif /* MHD_FAVOR_SMALL_CODE */


/**
 * Convert US-ASCII hexadecimal digit to its value.
 *
 * @param c character to convert
 * @return value of hexadecimal digit or -1 if @ c is not hexadecimal digit
 */
_MHD_static_inline int
toxdigitvalue (char c)
{
  if (isasciidigit (c))
    return (unsigned char) (c - '0');
  if ( (c >= 'A') && (c <= 'F') )
    return (unsigned char) (c - 'A' + 10);
  if ( (c >= 'a') && (c <= 'f') )
    return (unsigned char) (c - 'a' + 10);

  return -1;
}


/**
 * Caseless compare two characters.
 *
 * @param c1 the first char to compare
 * @param c1 the second char to compare
 * @return boolean 'true' if chars are caseless equal, false otherwise
 */
_MHD_static_inline bool
charsequalcaseless (const char c1, const char c2)
{
  return ( (c1 == c2) ||
           (isasciiupper (c1) ?
            ((c1 - 'A' + 'a') == c2) :
            ((c1 == (c2 - 'A' + 'a')) && isasciiupper (c2))) );
}


#else  /* !INLINE_FUNC */


/**
 * Checks whether character is lower case letter in US-ASCII
 *
 * @param c character to check
 * @return boolean true if character is lower case letter,
 *         boolean false otherwise
 */
#define isasciilower(c) (((char) (c)) >= 'a' && ((char) (c)) <= 'z')


/**
 * Checks whether character is upper case letter in US-ASCII
 *
 * @param c character to check
 * @return boolean true if character is upper case letter,
 *         boolean false otherwise
 */
#define isasciiupper(c) (((char) (c)) >= 'A' && ((char) (c)) <= 'Z')


/**
 * Checks whether character is letter in US-ASCII
 *
 * @param c character to check
 * @return boolean true if character is letter, boolean false
 *         otherwise
 */
#define isasciialpha(c) (isasciilower (c) || isasciiupper (c))


/**
 * Check whether character is decimal digit in US-ASCII
 *
 * @param c character to check
 * @return boolean true if character is decimal digit, boolean false
 *         otherwise
 */
#define isasciidigit(c) (((char) (c)) >= '0' && ((char) (c)) <= '9')


/**
 * Check whether character is hexadecimal digit in US-ASCII
 *
 * @param c character to check
 * @return boolean true if character is hexadecimal digit,
 *         boolean false otherwise
 */
#define isasciixdigit(c) (isasciidigit ((c)) || \
                          (((char) (c)) >= 'A' && ((char) (c)) <= 'F') || \
                          (((char) (c)) >= 'a' && ((char) (c)) <= 'f') )


/**
 * Check whether character is decimal digit or letter in US-ASCII
 *
 * @param c character to check
 * @return boolean true if character is decimal digit or letter,
 *         boolean false otherwise
 */
#define isasciialnum(c) (isasciialpha (c) || isasciidigit (c))


/**
 * Convert US-ASCII character to lower case.
 * If character is upper case letter in US-ASCII than it's converted to lower
 * case analog. If character is NOT upper case letter than it's returned
 * unmodified.
 *
 * @param c character to convert
 * @return converted to lower case character
 */
#define toasciilower(c) ((isasciiupper (c)) ? (((char) (c)) - 'A' + 'a') : \
                         ((char) (c)))


/**
 * Convert US-ASCII character to upper case.
 * If character is lower case letter in US-ASCII than it's converted to upper
 * case analog. If character is NOT lower case letter than it's returned
 * unmodified.
 *
 * @param c character to convert
 * @return converted to upper case character
 */
#define toasciiupper(c) ((isasciilower (c)) ? (((char) (c)) - 'a' + 'A') : \
                         ((char) (c)))


/**
 * Convert US-ASCII decimal digit to its value.
 *
 * @param c character to convert
 * @return value of hexadecimal digit or -1 if @ c is not hexadecimal digit
 */
#define todigitvalue(c) (isasciidigit (c) ? (int) (((char) (c)) - '0') : \
                         (int) (-1))


/**
 * Convert US-ASCII hexadecimal digit to its value.
 * @param c character to convert
 * @return value of hexadecimal digit or -1 if @ c is not hexadecimal digit
 */
#define toxdigitvalue(c) (isasciidigit (c) ? (int) (((char) (c)) - '0') : \
                          ( (((char) (c)) >= 'A' && ((char) (c)) <= 'F') ? \
                            (int) (((unsigned char) (c)) - 'A' + 10) : \
                            ( (((char) (c)) >= 'a' && ((char) (c)) <= 'f') ? \
                              (int) (((unsigned char) (c)) - 'a' + 10) : \
                              (int) (-1) )))

/**
 * Caseless compare two characters.
 *
 * @param c1 the first char to compare
 * @param c1 the second char to compare
 * @return boolean 'true' if chars are caseless equal, false otherwise
 */
#define charsequalcaseless(c1, c2) \
  ( ((c1) == (c2)) || \
           (isasciiupper (c1) ? \
             (((c1) - 'A' + 'a') == (c2)) : \
             (((c1) == ((c2) - 'A' + 'a')) && isasciiupper (c2))) )

#endif /* !INLINE_FUNC */


#ifndef MHD_FAVOR_SMALL_CODE
/**
 * Check two strings for equality, ignoring case of US-ASCII letters.
 *
 * @param str1 first string to compare
 * @param str2 second string to compare
 * @return non-zero if two strings are equal, zero otherwise.
 */
int
MHD_str_equal_caseless_ (const char *str1,
                         const char *str2)
{
  while (0 != (*str1))
  {
    const char c1 = *str1;
    const char c2 = *str2;
    if ( (c1 == c2) ||
         (isasciiupper (c1) ?
          ((c1 - 'A' + 'a') == c2) :
          (isasciiupper (c2) && (c1 == (c2 - 'A' + 'a')))) )
    {
      str1++;
      str2++;
    }
    else
      return 0;
  }
  return 0 == (*str2);
}


#endif /* ! MHD_FAVOR_SMALL_CODE */


/**
 * Check two string for equality, ignoring case of US-ASCII letters and
 * checking not more than @a maxlen characters.
 * Compares up to first terminating null character, but not more than
 * first @a maxlen characters.
 *
 * @param str1 first string to compare
 * @param str2 second string to compare
 * @param maxlen maximum number of characters to compare
 * @return non-zero if two strings are equal, zero otherwise.
 */
int
MHD_str_equal_caseless_n_ (const char *const str1,
                           const char *const str2,
                           size_t maxlen)
{
  size_t i;

  for (i = 0; i < maxlen; ++i)
  {
    const char c1 = str1[i];
    const char c2 = str2[i];
    if (0 == c2)
      return 0 == c1;
    if (charsequalcaseless (c1, c2))
      continue;
    else
      return 0;
  }
  return ! 0;
}


/**
 * Check two string for equality, ignoring case of US-ASCII letters and
 * checking exactly @a len characters.
 * Compares exactly @a len characters, including binary zero characters.
 * @param str1 first string to compare
 * @param str2 second string to compare
 * @param len number of characters to compare
 * @return non-zero if two strings are equal, zero otherwise.
 */
bool
MHD_str_equal_caseless_bin_n_ (const char *const str1,
                               const char *const str2,
                               size_t len)
{
  size_t i;

  for (i = 0; i < len; ++i)
  {
    const char c1 = str1[i];
    const char c2 = str2[i];
    if (charsequalcaseless (c1, c2))
      continue;
    else
      return 0;
  }
  return ! 0;
}


/**
 * Check whether @a str has case-insensitive @a token.
 * Token could be surrounded by spaces and tabs and delimited by comma.
 * Match succeed if substring between start, end (of string) or comma
 * contains only case-insensitive token and optional spaces and tabs.
 * @warning token must not contain null-charters except optional
 *          terminating null-character.
 * @param str the string to check
 * @param token the token to find
 * @param token_len length of token, not including optional terminating
 *                  null-character.
 * @return non-zero if two strings are equal, zero otherwise.
 */
bool
MHD_str_has_token_caseless_ (const char *str,
                             const char *const token,
                             size_t token_len)
{
  if (0 == token_len)
    return false;

  while (0 != *str)
  {
    size_t i;
    /* Skip all whitespaces and empty tokens. */
    while (' ' == *str || '\t' == *str || ',' == *str)
      str++;

    /* Check for token match. */
    i = 0;
    while (1)
    {
      const char sc = *(str++);
      const char tc = token[i++];

      if (0 == sc)
        return false;
      if (! charsequalcaseless (sc, tc))
        break;
      if (i >= token_len)
      {
        /* Check whether substring match token fully or
         * has additional unmatched chars at tail. */
        while (' ' == *str || '\t' == *str)
          str++;
        /* End of (sub)string? */
        if ((0 == *str) || (',' == *str) )
          return true;
        /* Unmatched chars at end of substring. */
        break;
      }
    }
    /* Find next substring. */
    while (0 != *str && ',' != *str)
      str++;
  }
  return false;
}


/**
 * Remove case-insensitive @a token from the @a str and put result
 * to the output @a buf.
 *
 * Token could be surrounded by spaces and tabs and delimited by comma.
 * The token match succeed if substring between start, end (of string) or
 * comma contains only case-insensitive token and optional spaces and tabs.
 * The quoted strings and comments are not supported by this function.
 *
 * The output string is normalised: empty tokens and repeated whitespaces
 * are removed, no whitespaces before commas, exactly one space is used after
 * each comma.
 *
 * @param str the string to process
 * @param str_len the length of the @a str, not including optional
 *                terminating null-character.
 * @param token the token to find
 * @param token_len the length of @a token, not including optional
 *                  terminating null-character.
 * @param[out] buf the output buffer, not null-terminated.
 * @param[in,out] buf_size pointer to the size variable, at input it
 *                         is the size of allocated buffer, at output
 *                         it is the size of the resulting string (can
 *                         be up to 50% larger than input) or negative value
 *                         if there is not enough space for the result
 * @return 'true' if token has been removed,
 *         'false' otherwise.
 */
bool
MHD_str_remove_token_caseless_ (const char *str,
                                size_t str_len,
                                const char *const token,
                                const size_t token_len,
                                char *buf,
                                ssize_t *buf_size)
{
  const char *s1; /**< the "input" string / character */
  char *s2;       /**< the "output" string / character */
  size_t t_pos;   /**< position of matched character in the token */
  bool token_removed;

  mhd_assert (NULL == memchr (token, 0, token_len));
  mhd_assert (NULL == memchr (token, ' ', token_len));
  mhd_assert (NULL == memchr (token, '\t', token_len));
  mhd_assert (NULL == memchr (token, ',', token_len));
  mhd_assert (0 <= *buf_size);

  s1 = str;
  s2 = buf;
  token_removed = false;

  while ((size_t) (s1 - str) < str_len)
  {
    const char *cur_token; /**< the first char of current token */
    size_t copy_size;

    /* Skip any initial whitespaces and empty tokens */
    while ( ((size_t) (s1 - str) < str_len) &&
            ((' ' == *s1) || ('\t' == *s1) || (',' == *s1)) )
      s1++;

    /* 's1' points to the first char of token in the input string or
     * points just beyond the end of the input string */

    if ((size_t) (s1 - str) >= str_len)
      break; /* Nothing to copy, end of the input string */

    cur_token = s1; /* the first char of input token or
                       the char after the input buffer */

    /* Check the token with case-insensetive match */
    t_pos = 0;
    while ( ((size_t) (s1 - str) < str_len) && (token_len > t_pos) &&
            (charsequalcaseless (*s1, token[t_pos])) )
    {
      s1++;
      t_pos++;
    }
    if ( (token_len == t_pos) && (0 != token_len) )
    {
      /* 'token' matched, check that current input token does not have
       * any suffixes */
      while ( ((size_t) (s1 - str) < str_len) &&
              ((' ' == *s1) || ('\t' == *s1)) )
        s1++;
      /* 's1' points to the first non-whitespace char after the token matched
       * requested token or points just beyond the end of the input string after
       * the requested token */
      if (((size_t) (s1 - str) == str_len) || (',' == *s1))
      {/* full token match, do not copy current token to the output */
        token_removed = true;
        continue;
      }
    }

    /* 's1' points to some non-whitespace char in the token in the input
     * string, to the ',', or just beyond the end of the input string */
    /* The current token in the input string does not match excluded token,
     * it must be copied to the output string */
    /* the current token size excluding leading whitespaces and current char */
    copy_size = (size_t) (s1 - cur_token);
    if (buf == s2)
    { /* The first token to copy to the output */
      if (buf + *buf_size < s2 + copy_size)
      { /* Not enough space in the output buffer */
        *buf_size = (ssize_t) -1;
        return false;
      }
    }
    else
    { /* Some token was already copied to the output buffer */
      if (buf + *buf_size < s2 + copy_size + 2)
      { /* Not enough space in the output buffer */
        *buf_size = (ssize_t) -1;
        return false;
      }
      *(s2++) = ',';
      *(s2++) = ' ';
    }
    /* Copy non-matched token to the output */
    if (0 != copy_size)
    {
      memcpy (s2, cur_token, copy_size);
      s2 += copy_size;
    }

    while ( ((size_t) (s1 - str) < str_len) && (',' != *s1))
    {
      /* 's1' points to some non-whitespace char in the token */
      while ( ((size_t) (s1 - str) < str_len) &&
              (',' != *s1) && (' ' != *s1) && ('\t' != *s1) )
      {
        if (buf + *buf_size <= s2) /* '<= s2' equals '< s2 + 1' */
        { /* Not enough space in the output buffer */
          *buf_size = (ssize_t) -1;
          return false;
        }
        *(s2++) = *(s1++);
      }
      /* 's1' points to some whitespace char in the token in the input
       * string, to the ',', or just beyond the end of the input string */
      /* Skip all whitespaces */
      while ( ((size_t) (s1 - str) < str_len) &&
              ((' ' == *s1) || ('\t' == *s1)) )
        s1++;

      /* 's1' points to the first non-whitespace char in the input string
       * after whitespace chars or just beyond the end of the input string */
      if (((size_t) (s1 - str) < str_len) && (',' != *s1))
      { /* Not the end of the current token */
        if (buf + *buf_size <= s2) /* '<= s2' equals '< s2 + 1' */
        { /* Not enough space in the output buffer */
          *buf_size = (ssize_t) -1;
          return false;
        }
        *(s2++) = ' ';
      }
    }
  }
  mhd_assert (((ssize_t) (s2 - buf)) <= *buf_size);
  *buf_size = (ssize_t) (s2 - buf);
  return token_removed;
}


#ifndef MHD_FAVOR_SMALL_CODE
/* Use individual function for each case */

/**
 * Convert decimal US-ASCII digits in string to number in uint64_t.
 * Conversion stopped at first non-digit character.
 *
 * @param str string to convert
 * @param[out] out_val pointer to uint64_t to store result of conversion
 * @return non-zero number of characters processed on succeed,
 *         zero if no digit is found, resulting value is larger
 *         then possible to store in uint64_t or @a out_val is NULL
 */
size_t
MHD_str_to_uint64_ (const char *str,
                    uint64_t *out_val)
{
  const char *const start = str;
  uint64_t res;

  if (! str || ! out_val || ! isasciidigit (str[0]))
    return 0;

  res = 0;
  do
  {
    const int digit = (unsigned char) (*str) - '0';
    if ( (res > (UINT64_MAX / 10)) ||
         ( (res == (UINT64_MAX / 10)) &&
           ((uint64_t) digit > (UINT64_MAX % 10)) ) )
      return 0;

    res *= 10;
    res += digit;
    str++;
  } while (isasciidigit (*str));

  *out_val = res;
  return str - start;
}


/**
 * Convert not more then @a maxlen decimal US-ASCII digits in string to
 * number in uint64_t.
 * Conversion stopped at first non-digit character or after @a maxlen
 * digits.
 *
 * @param str string to convert
 * @param maxlen maximum number of characters to process
 * @param[out] out_val pointer to uint64_t to store result of conversion
 * @return non-zero number of characters processed on succeed,
 *         zero if no digit is found, resulting value is larger
 *         then possible to store in uint64_t or @a out_val is NULL
 */
size_t
MHD_str_to_uint64_n_ (const char *str,
                      size_t maxlen,
                      uint64_t *out_val)
{
  uint64_t res;
  size_t i;

  if (! str || ! maxlen || ! out_val || ! isasciidigit (str[0]))
    return 0;

  res = 0;
  i = 0;
  do
  {
    const int digit = (unsigned char) str[i] - '0';

    if ( (res > (UINT64_MAX / 10)) ||
         ( (res == (UINT64_MAX / 10)) &&
           ((uint64_t) digit > (UINT64_MAX % 10)) ) )
      return 0;

    res *= 10;
    res += digit;
    i++;
  } while ( (i < maxlen) &&
            isasciidigit (str[i]) );

  *out_val = res;
  return i;
}


/**
 * Convert hexadecimal US-ASCII digits in string to number in uint32_t.
 * Conversion stopped at first non-digit character.
 *
 * @param str string to convert
 * @param[out] out_val pointer to uint32_t to store result of conversion
 * @return non-zero number of characters processed on succeed,
 *         zero if no digit is found, resulting value is larger
 *         then possible to store in uint32_t or @a out_val is NULL
 */
size_t
MHD_strx_to_uint32_ (const char *str,
                     uint32_t *out_val)
{
  const char *const start = str;
  uint32_t res;
  int digit;

  if (! str || ! out_val)
    return 0;

  res = 0;
  digit = toxdigitvalue (*str);
  while (digit >= 0)
  {
    if ( (res < (UINT32_MAX / 16)) ||
         ((res == (UINT32_MAX / 16)) && ( (uint32_t) digit <= (UINT32_MAX
                                                               % 16)) ) )
    {
      res *= 16;
      res += digit;
    }
    else
      return 0;
    str++;
    digit = toxdigitvalue (*str);
  }

  if (str - start > 0)
    *out_val = res;
  return str - start;
}


/**
 * Convert not more then @a maxlen hexadecimal US-ASCII digits in string
 * to number in uint32_t.
 * Conversion stopped at first non-digit character or after @a maxlen
 * digits.
 *
 * @param str string to convert
 * @param maxlen maximum number of characters to process
 * @param[out] out_val pointer to uint32_t to store result of conversion
 * @return non-zero number of characters processed on succeed,
 *         zero if no digit is found, resulting value is larger
 *         then possible to store in uint32_t or @a out_val is NULL
 */
size_t
MHD_strx_to_uint32_n_ (const char *str,
                       size_t maxlen,
                       uint32_t *out_val)
{
  size_t i;
  uint32_t res;
  int digit;
  if (! str || ! out_val)
    return 0;

  res = 0;
  i = 0;
  while (i < maxlen && (digit = toxdigitvalue (str[i])) >= 0)
  {
    if ( (res > (UINT32_MAX / 16)) ||
         ((res == (UINT32_MAX / 16)) && ( (uint32_t) digit > (UINT32_MAX
                                                              % 16)) ) )
      return 0;

    res *= 16;
    res += digit;
    i++;
  }

  if (i)
    *out_val = res;
  return i;
}


/**
 * Convert hexadecimal US-ASCII digits in string to number in uint64_t.
 * Conversion stopped at first non-digit character.
 *
 * @param str string to convert
 * @param[out] out_val pointer to uint64_t to store result of conversion
 * @return non-zero number of characters processed on succeed,
 *         zero if no digit is found, resulting value is larger
 *         then possible to store in uint64_t or @a out_val is NULL
 */
size_t
MHD_strx_to_uint64_ (const char *str,
                     uint64_t *out_val)
{
  const char *const start = str;
  uint64_t res;
  int digit;
  if (! str || ! out_val)
    return 0;

  res = 0;
  digit = toxdigitvalue (*str);
  while (digit >= 0)
  {
    if ( (res < (UINT64_MAX / 16)) ||
         ((res == (UINT64_MAX / 16)) && ( (uint64_t) digit <= (UINT64_MAX
                                                               % 16)) ) )
    {
      res *= 16;
      res += digit;
    }
    else
      return 0;
    str++;
    digit = toxdigitvalue (*str);
  }

  if (str - start > 0)
    *out_val = res;
  return str - start;
}


/**
 * Convert not more then @a maxlen hexadecimal US-ASCII digits in string
 * to number in uint64_t.
 * Conversion stopped at first non-digit character or after @a maxlen
 * digits.
 *
 * @param str string to convert
 * @param maxlen maximum number of characters to process
 * @param[out] out_val pointer to uint64_t to store result of conversion
 * @return non-zero number of characters processed on succeed,
 *         zero if no digit is found, resulting value is larger
 *         then possible to store in uint64_t or @a out_val is NULL
 */
size_t
MHD_strx_to_uint64_n_ (const char *str,
                       size_t maxlen,
                       uint64_t *out_val)
{
  size_t i;
  uint64_t res;
  int digit;
  if (! str || ! out_val)
    return 0;

  res = 0;
  i = 0;
  while (i < maxlen && (digit = toxdigitvalue (str[i])) >= 0)
  {
    if ( (res > (UINT64_MAX / 16)) ||
         ((res == (UINT64_MAX / 16)) && ( (uint64_t) digit > (UINT64_MAX
                                                              % 16)) ) )
      return 0;

    res *= 16;
    res += digit;
    i++;
  }

  if (i)
    *out_val = res;
  return i;
}


#else  /* MHD_FAVOR_SMALL_CODE */

/**
 * Generic function for converting not more then @a maxlen
 * hexadecimal or decimal US-ASCII digits in string to number.
 * Conversion stopped at first non-digit character or after @a maxlen
 * digits.
 * To be used only within macro.
 *
 * @param str the string to convert
 * @param maxlen the maximum number of characters to process
 * @param out_val the pointer to variable to store result of conversion
 * @param val_size the size of variable pointed by @a out_val, in bytes, 4 or 8
 * @param max_val the maximum decoded number
 * @param base the numeric base, 10 or 16
 * @return non-zero number of characters processed on succeed,
 *         zero if no digit is found, resulting value is larger
 *         then @max_val, @val_size is not 16/32 or @a out_val is NULL
 */
size_t
MHD_str_to_uvalue_n_ (const char *str,
                      size_t maxlen,
                      void *out_val,
                      size_t val_size,
                      uint64_t max_val,
                      int base)
{
  size_t i;
  uint64_t res;
  int digit;
  const uint64_t max_v_div_b = max_val / base;
  const uint64_t max_v_mod_b = max_val % base;
  /* 'digit->value' must be function, not macro */
  int (*const dfunc)(char) = (base == 16) ?
                             toxdigitvalue : todigitvalue;

  if (! str || ! out_val ||
      ((base != 16) && (base != 10)) )
    return 0;

  res = 0;
  i = 0;
  while (maxlen > i && 0 <= (digit = dfunc (str[i])))
  {
    if ( ((max_v_div_b) < res) ||
         (( (max_v_div_b) == res) && ( (max_v_mod_b) < (uint64_t) digit) ) )
      return 0;

    res *= base;
    res += digit;
    i++;
  }

  if (i)
  {
    if (8 == val_size)
      *(uint64_t*) out_val = res;
    else if (4 == val_size)
      *(uint32_t*) out_val = (uint32_t) res;
    else
      return 0;
  }
  return i;
}


#endif /* MHD_FAVOR_SMALL_CODE */
