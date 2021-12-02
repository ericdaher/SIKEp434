/********************************************************************************************
* Supersingular Isogeny Key Encapsulation Library
*
* Abstract: portable modular arithmetic for P434
*********************************************************************************************/

#include "../P434_internal.h"
#include "../../internal.h"


// Global constants
extern const uint64_t p434[NWORDS64_FIELD];
extern const uint64_t p434p1[NWORDS64_FIELD]; 
extern const uint64_t p434x2[NWORDS64_FIELD];  
extern const uint64_t p434x4[NWORDS64_FIELD];

__inline void mp_sub434_p2(const digit_t* a, const digit_t* b, digit_t* c)
{ // Multiprecision subtraction with correction with 2*p, c = a-b+2p. 
    unsigned int i, borrow = 0;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, a[i], b[i], borrow, c[i]); 
    }

    borrow = 0;
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(borrow, c[i], ((digit_t*)p434x2)[i], borrow, c[i]);
    }
} 


__inline void mp_sub434_p4(const digit_t* a, const digit_t* b, digit_t* c)
{ // Multiprecision subtraction with correction with 4*p, c = a-b+4p. 
    unsigned int i, borrow = 0;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, a[i], b[i], borrow, c[i]); 
    }

    borrow = 0;
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(borrow, c[i], ((digit_t*)p434x4)[i], borrow, c[i]);
    }
} 


__inline void fpadd434(const digit_t* a, const digit_t* b, digit_t* c)
{ // Modular addition, c = a+b mod p434.
  // Inputs: a, b in [0, 2*p434-1]
  // Output: c in [0, 2*p434-1] ldr r1,=0x12345678

	asm(".syntax unified");
	asm(
			"push  {r4-r7, lr}			\n\t"
			"mov r3, r2         \n\t"

			"ldr r4, r0					\n\t"
			"ldr r5, r1 				\n\t"
			"adds r4, r5					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r0, #4]					\n\t"
			"ldr r5, [r1, #4] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r0, #8]					\n\t"
			"ldr r5, [r1, #8] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"ldr r4, [r0, #12]					\n\t"
			"ldr r5, [r1, #12] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #12] 			\n\t"



			"adcs r2, r6        			\n\t"
			"subs r7, r6, #1     			\n\t"

			"ldr r4, r3					\n\t"
			"subs r4, r7					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r3, #4]					\n\t"
			"sbcs r4, r6					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r3, #8]					\n\t"
			"sbcs r4, r6					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"ldr r4, [r3, #12]					\n\t"
			"sbcs r4, r6					\n\t"
			"str r4, [r3, #12] 			\n\t"

			"mov r4, r12						\n\t"
			"sbcs r4, r4						\n\t"
			"mov r12, r4						\n\t"
			"adds  r0, r0, #16			\n\t"
			"adds  r1, r1, #16			\n\t"
			"adds  r3, r3, #16			\n\t"

			"lsls r7, r2, #31          \n\t"
			"adds r2, r6, r7      \n\t"

			"ldr r4, r0					\n\t"
			"ldr r5, r1 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r0, #4]					\n\t"
			"ldr r5, [r1, #4] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r0, #8]					\n\t"
			"ldr r5, [r1, #8] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"adcs r2, r6        			\n\t"

			"mov r4, r12        			\n\t"
			"mov r5, r0               \n\t"
			"adds r5, #12               \n\t"
			"subs r4, r5, r4               \n\t"
			"mov r12, r4               \n\t"




			"ldr r4, r3					\n\t"
			"subs r4, r6					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r3, #4]					\n\t"
			"sbcs r4, r6					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r3, #8]					\n\t"
			"sbcs r4, r7					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"mov r4, r12						\n\t"
			"sbcs r4, r4						\n\t"
			"mov r12, r4						\n\t"

			"adds  r0, r0, #12			\n\t"
			"adds  r1, r1, #12			\n\t"
			"adds  r3, r3, #12			\n\t"

			"lsls r7, r2, #31          \n\t"
			"adds r2, r6, r7      \n\t"

			"ldr r4, r0					\n\t"
			"ldr r5, r1 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r0, #4]					\n\t"
			"ldr r5, [r1, #4] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r0, #8]					\n\t"
			"ldr r5, [r1, #8] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"ldr r4, [r0, #12]					\n\t"
			"ldr r5, [r1, #12] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #12] 			\n\t"

			"adcs r2, r6        			\n\t"

			"mov r4, r12        			\n\t"
			"mov r5, r0               \n\t"
			"adds r5, #16               \n\t"
			"subs r4, r5, r4               \n\t"
			"mov r12, r4               \n\t"

			"ldr r4, r3					\n\t"


			"sbcs r4, r7					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r3, #4]					\n\t"


			"sbcs r4, r7					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r3, #8]					\n\t"


			"sbcs r4, r7					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"ldr r4, [r3, #12]					\n\t"


			"sbcs r4, r7					\n\t"
			"str r4, [r3, #12] 			\n\t"

			"mov r4, r12						\n\t"
			"sbcs r4, r4						\n\t"
			"mov r12, r4						\n\t"
			"adds  r0, r0, #16			\n\t"
			"adds  r1, r1, #16			\n\t"
			"adds  r3, r3, #16			\n\t"

			"lsls r7, r2, #31          \n\t"
			"adds r2, r6, r7      \n\t"

			"ldr r4, r0					\n\t"
			"ldr r5, r1 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r0, #4]					\n\t"
			"ldr r5, [r1, #4] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r0, #8]					\n\t"
			"ldr r5, [r1, #8] 				\n\t"
			"adcs r4, r5					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"mov r4, r12        			\n\t"
			"mov r5, r0               \n\t"
			"adds r5, #12               \n\t"
			"subs r4, r5, r4               \n\t"
			"mov r12, r4               \n\t"

			"ldr r4, r3					\n\t"


			"sbcs r4, r7					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r3, #4]					\n\t"


			"sbcs r4, r7					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r3, #8]					\n\t"


			"sbcs r4, r7					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"mov r4, r12						\n\t"
			"sbcs r4, r4						\n\t"
			"mov r12, r4						\n\t"

			"adds  r0, r0, #12			\n\t"
			"adds  r1, r1, #12			\n\t"
			"adds  r3, r3, #12			\n\t"

			"subs r3, r3, #4*14				\n\t"



			"mov r5, r12              \n\t"
			"ands r7, r7, r5          \n\t"

			"ldr r4, r3					\n\t"
			"adds r4, r4, r7					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ands r6, r6, r5          \n\t"

			"ldr r4, [r0, #4]					\n\t"
			"adcs r4, r6					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r0, #8]					\n\t"
			"adcs r4, r6					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"ldr r4, [r0, #12]					\n\t"
			"adcs r4, r6					\n\t"
			"str r4, [r3, #12] 			\n\t"



			"ands r7, r7, r5          \n\t"

			"adds  r3, r3, #16			\n\t"

			"ldr r4, r3					\n\t"
			"adcs r4, r6					\n\t"
			"str r4, [r3, #0] 			\n\t"

			"ldr r4, [r3, #4]					\n\t"
			"adcs r4, r6					\n\t"
			"str r4, [r3, #4] 			\n\t"

			"ldr r4, [r3, #8]					\n\t"
			"adcs r4, r7					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"adds  r3, r3, #12			\n\t"



			"ands r7, r7, r5          \n\t"

			"ldr r4, r3					\n\t"
			"adcs r4, r4, r7					\n\t"
			"str r4, [r3, #0] 			\n\t"



			"ands r7, r7, r5          \n\t"

			"ldr r4, [r3, #4]					\n\t"
			"adcs r4, r4, r7					\n\t"
			"str r4, [r3, #4] 			\n\t"



			"ands r7, r7, r5          \n\t"

			"ldr r4, [r3, #8]					\n\t"
			"adcs r4, r4, r7					\n\t"
			"str r4, [r3, #8] 			\n\t"



			"ands r7, r7, r5          \n\t"

			"ldr r4, [r3, #12]					\n\t"
			"adcs r4, r4, r7					\n\t"
			"str r4, [r3, #12] 			\n\t"

			"adds  r3, r3, #16			\n\t"



			"ands r7, r7, r5          \n\t"

			"ldr r4, r3					\n\t"
			"adcs r4, r4, r7					\n\t"
			"str r4, [r3, #0] 			\n\t"



			"ands r7, r7, r5          \n\t"

			"ldr r4, [r3, #4]					\n\t"
			"adcs r4, r4, r7					\n\t"
			"str r4, [r3, #4] 			\n\t"



			"ands r7, r7, r5          \n\t"

			"ldr r4, [r3, #8]					\n\t"
			"adcs r4, r4, r7					\n\t"
			"str r4, [r3, #8] 			\n\t"

			"adds  r3, r3, #12			\n\t"
			"pop  {r4-r7,pc}				\n\t"
:
:
:
);
	asm (".syntax divided");
}

__inline void fpsub434(const digit_t* a, const digit_t* b, digit_t* c)
{ // Modular subtraction, c = a-b mod p434.
  // Inputs: a, b in [0, 2*p434-1] 
  // Output: c in [0, 2*p434-1] 
    unsigned int i, borrow = 0;
    digit_t mask;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, a[i], b[i], borrow, c[i]); 
    }
    mask = 0 - (digit_t)borrow;

    borrow = 0;
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(borrow, c[i], ((digit_t*)p434x2)[i] & mask, borrow, c[i]);
    }
}


__inline void fpneg434(digit_t* a)
{ // Modular negation, a = -a mod p434.
  // Input/output: a in [0, 2*p434-1] 
    unsigned int i, borrow = 0;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, ((digit_t*)p434x2)[i], a[i], borrow, a[i]); 
    }
}


void fpdiv2_434(const digit_t* a, digit_t* c)
{ // Modular division by two, c = a/2 mod p434.
  // Input : a in [0, 2*p434-1] 
  // Output: c in [0, 2*p434-1] 
    unsigned int i, carry = 0;
    digit_t mask;
        
    mask = 0 - (digit_t)(a[0] & 1);    // If a is odd compute a+p434
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(carry, a[i], ((digit_t*)p434)[i] & mask, carry, c[i]);
    }

    mp_shiftr1(c, NWORDS_FIELD);
} 


void fpcorrection434(digit_t* a)
{ // Modular correction to reduce field element a in [0, 2*p434-1] to [0, p434-1].
    unsigned int i, borrow = 0;
    digit_t mask;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(borrow, a[i], ((digit_t*)p434)[i], borrow, a[i]); 
    }
    mask = 0 - (digit_t)borrow;

    borrow = 0;
    for (i = 0; i < NWORDS_FIELD; i++) {
        ADDC(borrow, a[i], ((digit_t*)p434)[i] & mask, borrow, a[i]);
    }
}

void digit_x_digit(const digit_t a, const digit_t b, digit_t* c)
{ // Digit multiplication, digit * digit -> 2-digit result
	asm(".syntax unified");
	asm(
			"push {r5, r6, r7, lr}			\n\t"
			"uxth r6, r0				\n\t"
			"uxth r7, r1				\n\t"
			"lsrs r0, r0, #16				\n\t"
			"lsrs r1, r1, #16 			\n\t"
			"movs r5, r6 			\n\t"
			"muls r5, r5, r7 			\n\t"
			"muls r6, r6, r1				\n\t"
			"muls r1, r1, r0				\n\t"
			"muls r0, r0, r7				\n\t"
			"lsls r7, r6, #16				\n\t"
			"lsrs r6, r6, #16 			\n\t"
			"adds r5, r7 			\n\t"
			"adcs r1, r6 			\n\t"
			"lsls r7, r0, #16				\n\t"
			"lsrs r6, r0, #16				\n\t"
			"adds r5, r7				\n\t"
			"adcs r1, r6				\n\t"
			"str  r5, [r2, #0] 			\n\t"
			"str  r1, [r2, #4] 			\n\t"
			"pop {r5, r6, r7, pc}			\n\t"
		:
		:
		:
	);
	asm (".syntax divided");
}

// CÓDIGO GERADO PELO COMPILADOR
//0000af68:   push    {r4, r5, r6, r7, lr}
//0000af6a:   movs    r4, r1
//0000af6c:   uxth    r1, r1
//0000af6e:   movs    r7, r1
//0000af70:   uxth    r6, r0
//0000af72:   lsrs    r0, r0, #16
//0000af74:   muls    r1, r0
//0000af76:   muls    r7, r6
//0000af78:   lsrs    r4, r4, #16
//0000af7a:   muls    r6, r4
//0000af7c:   muls    r0, r4
//0000af7e:   uxth    r3, r1
//0000af80:   lsrs    r4, r7, #16
//0000af82:   adds    r4, r4, r3
//0000af84:   uxth    r3, r6
//0000af86:   adds    r4, r4, r3
//0000af88:   lsrs    r6, r6, #16
//0000af8a:   lsrs    r3, r1, #16
//0000af8c:   adds    r3, r3, r6
//0000af8e:   uxth    r1, r0
//0000af90:   lsrs    r5, r4, #16
//0000af92:   adds    r3, r3, r1
//0000af94:   adds    r3, r3, r5
//0000af96:   lsrs    r1, r0, #16
//0000af98:   lsrs    r0, r3, #16
//0000af9a:   adds    r0, r0, r1
//0000af9c:   uxth    r7, r7
//0000af9e:   lsls    r0, r0, #16
//0000afa0:   lsls    r4, r4, #16
//0000afa2:   uxth    r3, r3
//0000afa4:   eors    r4, r7
//0000afa6:   eors    r3, r0
//0000afa8:   str     r4, [r2, #0]
//0000afaa:   str     r3, [r2, #4]
//0000afac:   pop     {r4, r5, r6, r7, pc}
//

//void digit_x_digit(const digit_t a, const digit_t b, digit_t* c)
//{ // Digit multiplication, digit * digit -> 2-digit result
//    register digit_t al, ah, bl, bh, temp;
//    digit_t albl, albh, ahbl, ahbh, res1, res2, res3, carry;
//    digit_t mask_low = (digit_t)(-1) >> (sizeof(digit_t)*4), mask_high = (digit_t)(-1) << (sizeof(digit_t)*4);
//
//    al = a & mask_low;                        // Low part
//    ah = a >> (sizeof(digit_t) * 4);          // High part
//    bl = b & mask_low;
//    bh = b >> (sizeof(digit_t) * 4);
//
//    albl = al*bl;
//    albh = al*bh;
//    ahbl = ah*bl;
//    ahbh = ah*bh;
//    c[0] = albl & mask_low;                   // C00
//
//    res1 = albl >> (sizeof(digit_t) * 4);
//    res2 = ahbl & mask_low;
//    res3 = albh & mask_low;
//    temp = res1 + res2 + res3;
//    carry = temp >> (sizeof(digit_t) * 4);
//    c[0] ^= temp << (sizeof(digit_t) * 4);    // C01
//
//    res1 = ahbl >> (sizeof(digit_t) * 4);
//    res2 = albh >> (sizeof(digit_t) * 4);
//    res3 = ahbh & mask_low;
//    temp = res1 + res2 + res3 + carry;
//    c[1] = temp & mask_low;                   // C10
//    carry = temp & mask_high;
//    c[1] ^= (ahbh & mask_high) + carry;       // C11
//}


void mp_mul(const digit_t* a, const digit_t* b, digit_t* c, const unsigned int nwords)
{ // Multiprecision comba multiply, c = a*b, where lng(a) = lng(b) = nwords.   
    unsigned int i, j;
    digit_t t = 0, u = 0, v = 0, UV[2];
    unsigned int carry = 0;
    
    for (i = 0; i < nwords; i++) {
        for (j = 0; j <= i; j++) {
            MUL(a[j], b[i-j], UV+1, UV[0]); 
            ADDC(0, UV[0], v, carry, v);
            ADDC(carry, UV[1], u, carry, u);
            t += carry;
        }
        c[i] = v;
        v = u; 
        u = t;
        t = 0;
    }

    for (i = nwords; i < 2*nwords-1; i++) {
        for (j = i-nwords+1; j < nwords; j++) {
            MUL(a[j], b[i-j], UV+1, UV[0]); 
            ADDC(0, UV[0], v, carry, v);
            ADDC(carry, UV[1], u, carry, u);
            t += carry;
        }
        c[i] = v;
        v = u; 
        u = t;
        t = 0;
    }
    c[2*nwords-1] = v; 
}


void rdc_mont(digit_t* ma, digit_t* mc)
{ // Efficient Montgomery reduction using comba and exploiting the special form of the prime p434.
  // mc = ma*R^-1 mod p434x2, where R = 2^448.
  // If ma < 2^448*p434, the output mc is in the range [0, 2*p434-1].
  // ma is assumed to be in Montgomery representation.
    unsigned int i, j, carry, count = p434_ZERO_WORDS;
    digit_t UV[2], t = 0, u = 0, v = 0;

    for (i = 0; i < NWORDS_FIELD; i++) {
        mc[i] = 0;
    }

    for (i = 0; i < NWORDS_FIELD; i++) {
        for (j = 0; j < i; j++) {
            if (j < (i-p434_ZERO_WORDS+1)) { 
                MUL(mc[j], ((digit_t*)p434p1)[i-j], UV+1, UV[0]);
                ADDC(0, UV[0], v, carry, v);
                ADDC(carry, UV[1], u, carry, u);
                t += carry; 
            }
        }
        ADDC(0, v, ma[i], carry, v);
        ADDC(carry, u, 0, carry, u);
        t += carry; 
        mc[i] = v;
        v = u;
        u = t;
        t = 0;
    }    

    for (i = NWORDS_FIELD; i < 2*NWORDS_FIELD-1; i++) {
        if (count > 0) {
            count -= 1;
        }
        for (j = i-NWORDS_FIELD+1; j < NWORDS_FIELD; j++) {
            if (j < (NWORDS_FIELD-count)) { 
                MUL(mc[j], ((digit_t*)p434p1)[i-j], UV+1, UV[0]);
                ADDC(0, UV[0], v, carry, v);
                ADDC(carry, UV[1], u, carry, u);
                t += carry;
            }
        }
        ADDC(0, v, ma[i], carry, v);
        ADDC(carry, u, 0, carry, u);
        t += carry; 
        mc[i-NWORDS_FIELD] = v;
        v = u;
        u = t;
        t = 0;
    }
    ADDC(0, v, ma[2*NWORDS_FIELD-1], carry, v);
    mc[NWORDS_FIELD-1] = v;
}
