/* compiled with -msse2, etc*/

//#include <mmintrin.h>  /* MMX */
//#include <xmmintrin.h> /* SSE */
#include <emmintrin.h> /* SSE2 */
//#include <pmmintrin.h> /* SSE3 */
//#include <tmmintrin.h> /* SSSE3 */
//#include <smmintrin.h> /* SSE4.2 SSE4.1 */

#include<sys/time.h>
#include<time.h>
#include<stdio.h>
typedef unsigned long long ticks;

static __inline__ ticks getticks(void)
{
     unsigned a, d;
     asm("cpuid");
     asm volatile("rdtsc" : "=a" (a), "=d" (d));

     return (((ticks)a) | (((ticks)d) << 32));
}

/* plain version x = a*x */
void sapxy(int n, float a, float *x)
{
  int i;
  for (i=0; i<n; i++) x[i] = x[i]*a;
}

/* sse version */

void sse_sapxy(int n, float a, float *x)
{
  float aa[4];
  __m128 v_a;
  __m128 *v_x;
  int i;

  aa[0] = aa[1] = aa[2] = aa[3] = a;
  v_a = _mm_load_ps(aa);
  v_x = (__m128 *)x;

  for (i=0; i<n/4*4; i+=4) {
    *v_x = _mm_mul_ps(*v_x, v_a);
    v_x++;
  }
  for (;i<n; i++)
    x[i] = x[i] * a;
}


void sse_sapxy_unaligned(int n, float a, float *x)
{
  float aa[4];
  __m128 v_a;
  int i;

  aa[0] = aa[1] = aa[2] = aa[3] = a;
  v_a = _mm_loadu_ps(aa);

  if (((unsigned long long)x % 16) == 0) { // check if memory is aligned.
    __m128 *p;
    p = (__m128 *)x;
    for (i=0; i<n/4*4; i+=4) {
      *p = _mm_mul_ps(*p, v_a);
      p++;
      /*
      __m128 v_i;
      __m128 v_o;
      v_i = _mm_load_ps(x+i);
      v_o = _mm_mul_ps(v_i, v_a);
      _mm_store_ps(x+i, v_o);
      */
    }
  } else {
    for (i=0; i<n/4*4; i+=4) {
      __m128 v_i;
      __m128 v_o;
      v_i = _mm_loadu_ps(x+i);
      v_o = _mm_mul_ps(v_i, v_a);
      _mm_storeu_ps(x+i, v_o);
    }
  }

  for ( ;i<n; i++)
    x[i] = a*x[i];
}

float a[1000000];
float b[1000000];

main()
{
  unsigned long long int t1, t2, t3;


  int i, j;

  for (i=0; i<1000000; i++) {
    a[i] = i*1.0;
    b[i] = i*1.0;
  }

  t1 = getticks();
  
  sapxy(1000000, 3.14159, a);

  t2 = getticks();
  printf("time = %lld\n", t2-t1);

  t1 = getticks();
  sse_sapxy_unaligned(1000000, 3.14159, b+1);
  t2 = getticks();

  printf("time = %lld\n", t2-t1);
  for (i=0; i<999999; i++) {
    if (a[i] != b[i]) {printf("Wrong.\n"); exit(0);}
    //printf("%d %f %f\n", i, z3[i], z4[i]);
  }
  printf("Correct, results matched.\n");
  return 0;
}
