
/* crc_fsa.c */

#include <stdio.h>
#include <stdlib.h>

#define  POLY   0x011D //285   /* the generator polynomial */
#define  HPOW2  256            /* highest power of 2 in the gp */

int main()
{
  int r,c, t, cs, cs2, pos;
  int i, bb, li,
      bim,        /* bits in the message */
      btbc,       /* bit to be changed */
      bigp,       /* bits in the gp */
      debug = 0;
  int msg[hpo2];  /* the actual message */
  int fsa_table[hpo2][2];
  int checksum[hpo2];
  time_t tt;

  /* initialize the random number generator */
  srand((unsigned) time(&tt));

  /* determine the number of bits (bigp) in the gp */
  li = gp >> 1;
  bigp = 1;
  while (li) { li = li >> 1; bigp++; }

  /* determine the maximum number of bits in the message */
  bim = hpo2 - bigp;

  printf("generator polynomial          %2d\n",gp);
  printf("highest power of 2 in gp      %2d\n",hpo2);
  printf("bits in generator polynomial  %2d\n",bigp);
  printf("bits in message               %2d\n\n",bim);

  /* Both the sender and the receiver need to build the FST */
  for (r = 0; r < hpo2; r++)
    for (c = 0; c < 2; c++)
      {
        t = r*2+c;
        if (t >= hpo2) t ^= gp;
        fsa_table[r][c] = t;
      }

  /* display the FST */
  if (debug)
    {
      printf("FST table for gp = %d\n\n",gp);
      printf("index     0     1\n");
      printf("        -----------\n");
      for (r = 0; r < hpo2; r++)
        { printf("   %2d  | ",r);
          for (c = 0; c < 2; c++) printf("%2d  | ",fsa_table[r][c]);
          printf("\n");
        }
    }

  /* Only the receiver will need to build the Error Correction
     table for the GP */
  for (pos=hpo2-1; pos > 0; pos--) checksum[pos]=0;
  t = 1;
  for (pos=hpo2-1; pos > 0; pos--)
    {
      if (checksum[t]) printf("%d is not a valid gp\n",gp), exit(0);
      else checksum[t] = pos;
      t *= 2;
      if (t >= hpo2) t ^= gp;
    }

  /* display the error correction table for the gp */
  if (debug)
    {
      printf("\n\nError Correction table for gp = %d\n",gp);
      printf("index   position of\n");
      printf("        the bad bit\n");
      printf("            --- \n");
      for (r=1; r < hpo2; r++)
        printf("    %2d     | %2d |\n",r,checksum[r]);
      printf("\n");
    }

  /* generate a random message */
  for (i=0; i <= bim; i++) msg[i] = rand()%2;

  /* display the message */
  printf("The original message         ");
  for (i=1; i <= bim; i++) printf("%d",msg[i]);
  printf("\n");

  /* zero out then compute the checksum */
  for (i=bim+1; i < hpo2; i++) msg[i] = 0;
  cs = 0;
  for (r = 1; r < hpo2; r++)
    cs = fsa_table[cs][msg[r]];

  /* Put the checksum on the end of the message */
  for (i = bim+1; i < hpo2; i++) msg[i] = (cs >> hpo2-i-1) & 1;

  /* display the message and the checksum */
  if (debug)
    {
      printf("with the checksum            ");
      for (i=1; i < hpo2; i++)
        printf("%d",msg[i]);
      printf("\n");
    }

  /* The message|checksum can now be sent.  Simulate a problem by
     changing a random bit */
  btbc = rand()%(hpo2-1)+1;
  msg[btbc] ^= 1;

 /*
  * Part 2
  * The receiver has received a message with 1 bad bit.
  */
  printf("After receiving the message, msg = ");
  for (r = 1; r < hpo2; r++) printf("%d",msg[r]);
  printf("\n");

  /* compute the checksum */
  cs2 = 0;
  for (r = 1; r < hpo2; r++)
    cs2 = fsa_table[cs2][msg[r]];

  if (debug) printf("the checksum is %d\n",cs2);

  if (cs2 > 0)
    {
      /* determine the bad bit */
      pos = checksum[cs2];

      /* correct the bad bit */
      msg[pos] ^= 1;
    }

  /* display the (corrected) message */
  printf("The corrected message        ");
  for (i=1; i <= bim; i++) printf("%d",msg[i]); printf("\n");

  printf("Normal Termination\n");
}
