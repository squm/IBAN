#include	<stdio.h>
#include	<ctype.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>

#define	buf_size 28		/* max buf length */
#define	input_len 128		/* input length	*/

char
check_iban(const char *const input, int len) {
  char c, buf[buf_size + 8], iso[buf_size + 8];
  int i, j;

//  strncat(buf, iso, 8);
//  iban.c|15| warning: will never be executed

  for (i = j = 0; i < len; i++, j++) {
    if (j > buf_size - 1) {
      return 1;
    }
    c = input[i];

    // skip spaces
    if (isblank(c)) {
      j--;
    }

    else {

      // alpha & numeric
      if (isalnum(c)) {

        // deny letters after position 2
        if (isalpha(c) && j > 1) {
          return 1;
        }

        // deny numbers at position 1 and 2
        if (isdigit(c) && j < 2) {
          return 1;
        }

        buf[j] = c;

      }

      // if not alpha or numeric
      else {
        return 1;
      }
    } // not spaces
  } // for-loop

  len = j;
  buf[len] = '\0';

  // len must be less than buf_size for strncat
  if (len < 8 || len > 20) {
    return 1;
  }

  // take 'DE87' from "DE87 1234 5678 1234 5678"
  // convert 'DE' to '1314' append '131487'
  i = snprintf(iso, 8, "%d%d%c%c", 
      buf[0] - 55, buf[1] - 55,	// 'DE' country code
      buf[2], buf[3]			// 87
      );

  if (i < 0) {
    fprintf(stderr, "Error while converting\n");
    return 1;
  }

  strncat(buf, iso, 8);

  // shift buf
  char *result = buf + 4;

  // input: 'DE87 1111 2222 3333 8888 '
  //output: '1111222233338888131487'

  printf("output: '%s'\n", result);

#if 1
  char *p;

  errno = 0;

  strtoull(result, &p, 10);

  if (p != '\0' || errno != 0) {
    fprintf(stderr, "Error: Number is too big ?\n");
    perror("Error");
  }

  //  if (n % 97 == 1) return 0;
#endif

  return 1;
}

int
main(int argc, char *argv[]) {
  char buf[input_len + 1];
  int argi, append, len;

  buf[0] = '\0';

  len = 0;
  for (argi = 1; argi < argc && len < input_len - 1; argi++) {
    append = strlen(argv[argi]);
    len += append + 1;
    snprintf(buf, input_len - 1, "%s%s ", buf, argv[argi]);
  }

  printf(" input: '%s'\n", buf);

  if (check_iban(buf, len)) {
    printf("check err '%s'\n", buf);
  }
  else {
    printf("check ok  '%s'\n", buf);
  }

  return 0;
}
