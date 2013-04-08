#include	<stdio.h>
#include	<ctype.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>

#define	buf_size 64		/* max buf length */
#define	input_len 128		/* input length	*/

#define	NOT_REGISTERED -1

char IBAN['Z' + 1]['Z' + 1];

char
check_iban(const char *const input, int len) {
  char c, buf[buf_size], *result;
  int i, j;

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
        buf[j] = c;
      }
      else {
        return 1;
      }

    } // not spaces
  } // for-loop

  len = j;
  buf[len] = '\0';

  // format: letter, letter, digit, digit ...
  //  index:    0       1      2      3

  // deny letters at position 2 and 3
  if (isalpha(buf[2]) || isalpha(buf[3])) {
    return 1;
  }

  // deny numbers at position 0 and 1
  if (isdigit(buf[0]) || isdigit(buf[1])) {
    return 1;
  }

  // first number shouldn't be any of: 00, 01, 99
  if (
      (buf[2] == '0' && buf[3] == '0') ||
      (buf[2] == '0' && buf[3] == '1') ||
      (buf[2] == '9' && buf[3] == '9')
     ) {
    return 1;
  }

  // Norway has shortest IBAN, which is 15, longest is 32
  if (len < 15 || len > 32) {
    return 1;
  }

  // get uppercase 2-alpha country code
  i = buf[0] = toupper(buf[0]);
  j = buf[1] = toupper(buf[1]);

  // check if country is registered
  if (IBAN[i][j] == NOT_REGISTERED) {
    fprintf(stderr, "Country '%c%c' unknown\n", i, j);
    return 1;
  }

  // check iban length
  if (IBAN[i][j] != len) {
    fprintf(stderr, "IBAN '%s' length is %d, but should be %d\n",
        buf, len, IBAN[i][j]);
    return 1;
  }

  // take 'DE87' from "DE87 1234 5678 1234 5678"
  // convert 'DE' to '1314' append '131487'
  snprintf(buf, buf_size - 1, "%s%d%d%c%c", 
      buf,
      buf[0] - 55, buf[1] - 55,
      buf[2], buf[3]
      );

  if (i < 0) {
    fprintf(stderr, "Error while converting\n");
    return 1;
  }

  // shift buf
  result = buf + 4;

  // input: 'DE87 1111 2222 3333 8888 '
  //output: '1111222233338888131487'
  printf("output: '%s'\n", result);

#if 0
  char *p;

  errno = 0;

  long long unsigned int n =
  strtoull(result, &p, 10);
//  strtoull(result, &p, 10);

  if (p != '\0' || errno != 0) {
    fprintf(stderr, "Error: Number is too big ?\n");
    perror("Error");
  }

  if (n % 97 == 1) return 0;
#endif

  return 0;
}

int
main(int argc, char *argv[]) {
  int i, j;
  for (i = 0; i <= 'Z'; i++) {
    for (j = 0; j <= 'Z'; j++) {
      IBAN[i][j] = NOT_REGISTERED;
    }
  }

  IBAN['A']['L'] = 28; IBAN['A']['D'] = 24; IBAN['A']['T'] = 20;
  IBAN['A']['Z'] = 28; IBAN['B']['H'] = 22; IBAN['B']['E'] = 16;
  IBAN['B']['A'] = 20; IBAN['B']['R'] = 29; IBAN['B']['G'] = 22;
  IBAN['C']['R'] = 21; IBAN['H']['R'] = 21; IBAN['C']['Y'] = 28;
  IBAN['C']['Z'] = 24; IBAN['D']['K'] = 18; IBAN['D']['O'] = 28;
  IBAN['E']['E'] = 20; IBAN['F']['O'] = 18; IBAN['F']['I'] = 18;
  IBAN['F']['R'] = 27; IBAN['G']['E'] = 22; IBAN['D']['E'] = 22;
  IBAN['G']['I'] = 23; IBAN['G']['R'] = 27; IBAN['G']['L'] = 18;
  IBAN['G']['T'] = 28; IBAN['H']['U'] = 28; IBAN['I']['S'] = 26;
  IBAN['I']['E'] = 22; IBAN['I']['L'] = 23; IBAN['I']['T'] = 27;
  IBAN['K']['Z'] = 20; IBAN['K']['W'] = 30; IBAN['L']['V'] = 21;
  IBAN['L']['B'] = 28; IBAN['L']['I'] = 21; IBAN['L']['T'] = 20;
  IBAN['L']['U'] = 20; IBAN['M']['K'] = 19; IBAN['M']['T'] = 31;
  IBAN['M']['R'] = 27; IBAN['M']['U'] = 30; IBAN['M']['D'] = 24;
  IBAN['M']['C'] = 27; IBAN['M']['E'] = 22; IBAN['N']['L'] = 18;
  IBAN['N']['O'] = 15; IBAN['P']['K'] = 24; IBAN['P']['S'] = 29;
  IBAN['P']['L'] = 28; IBAN['P']['T'] = 25; IBAN['R']['O'] = 24;
  IBAN['S']['M'] = 27; IBAN['S']['A'] = 24; IBAN['R']['S'] = 22;
  IBAN['S']['K'] = 24; IBAN['S']['I'] = 19; IBAN['E']['S'] = 24;
  IBAN['S']['E'] = 24; IBAN['C']['H'] = 21; IBAN['T']['N'] = 24;
  IBAN['T']['R'] = 26; IBAN['A']['E'] = 23; IBAN['G']['B'] = 22;
  IBAN['V']['G'] = 24;

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
