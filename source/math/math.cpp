/*!
    \file math.cpp
    \brief Math implementation
    \author Ivan Shynkarenka
    \date 01.09.2016
    \copyright MIT License
*/

#include "math/math.h"

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace CppCommon {

uint64_t Math::MulDiv64(uint64_t operant, uint64_t multiplier, uint64_t divider)
{
#if defined(__GNUC__) && defined(__SIZEOF_INT128__)
    __uint128_t a = operant;
    __uint128_t b = multiplier;
    __uint128_t c = divider;

    return (uint64_t)(a * b / c);
#elif defined(_MSC_VER)
#if defined(_M_IX86)
    // Declare 128bit storage
    struct
    {
        unsigned long DW[4];
    } var128, quotient;

    // Change semantics for intermediate results for Full Div by renaming the vars
    #define REMAINDER quotient
    #define QUOTIENT edi

    // Save combined sign on stack
    _asm
    {
        mov     eax, dword ptr[operant+4]
        xor     eax, dword ptr[multiplier+4]
        xor     eax, dword ptr[divider+4]
        pushfd
    }

    _asm
    {
        // First check divider for 0
        mov     eax, dword ptr[divider+4]
        or      eax, dword ptr[divider]
        jnz     dividerOK
        div     eax
dividerOK:
        lea     edi,[var128]                    // edi = &var128
        // Check multiplier for 1 or 0
        xor     eax, eax
        cmp     eax, dword ptr[multiplier+4]
        jnz     startMUL
        cmp     eax, dword ptr[multiplier]
        jnz     multiNotNUL
        xor     edx, edx
        popfd                                   // cleanup stack
        jmp     done
multiNotNUL:
        // Set result HI part to 0
        xor     eax,eax
        mov     dword ptr[edi+12], eax
        mov     dword ptr[edi+8], eax
        mov     eax, 1
        cmp     eax, dword ptr[multiplier]
        jnz     smallMUL
        // Multiplier is 1 so just copy operant to result
        mov     eax, dword ptr[operant+4]
        mov     dword ptr[edi+4], eax
        mov     eax, dword ptr[operant]
        mov     dword ptr[edi], eax
        jmp     startDIV
smallMUL:
        // Test for 32/32 bit multiplication
        xor     eax, eax
        mov     ecx, dword ptr[operant+4]
        or      ecx, eax         ;test for both hiwords zero.
        jnz     startMUL
        // Do 32/32 bit multiplication
        mov     ecx, dword ptr[multiplier]
        mov     eax, dword ptr[operant]
        mul     ecx
        mov     dword ptr[edi+4], edx
        mov     dword ptr[edi], eax
        jmp     startDIV
startMUL:
        // Check signs
        // Multiply: var128 = operant * multiplier
        mov     eax, dword ptr[multiplier]      // eax = LO(multiplier)
        mul     dword ptr[operant]              // edx:eax = eax * LO(operant)
        mov     dword ptr[edi], eax             // var128.DW0 = eax
        mov     ecx, edx                        // ecx = edx

        mov     eax, dword ptr[multiplier]      // eax = LO(multiplier)
        mul     dword ptr[operant+4]            // edx:eax = eax * HI(operant)
        add     eax, ecx                        // eax = eax + ecx
        adc     edx, 0                          // edx = edx + 0 + carry
        mov     ebx, eax
        mov     ecx, edx

        mov     eax, dword ptr[multiplier+4]
        mul     dword ptr[operant]
        add     eax, ebx
        mov     dword ptr[edi+4], eax
        adc     ecx, edx
        pushfd

        mov     eax, dword ptr[multiplier+4]
        mul     dword ptr[operant+4]
        popfd
        adc     eax, ecx
        adc     edx, 0
        mov     dword ptr[edi+8], eax
        mov     dword ptr[edi+12], edx
startDIV:
        // Divide: var128 = var128 / divider
        //
        // Test divider = 32bit value
        mov     eax, dword ptr[divider+4]
        cmp     eax, 0
        jnz     fullDIV
        mov     ecx, dword ptr[divider]
        cmp     ecx, 1
        jz      applySign

        // Start 128/32 bit division
        mov     eax, dword ptr[edi+12]
        xor     edx, edx
        div     ecx
        mov     dword ptr[quotient+12], eax

        mov     eax, dword ptr[edi+8]
        div     ecx
        mov     dword ptr[quotient+8], eax

        mov     eax, dword ptr[edi+4]
        div     ecx
        mov     dword ptr[quotient+4], eax

        mov     eax, dword ptr[edi]
        div     ecx
        mov     dword ptr[quotient], eax

        // Copy the quotient to the result storage (var128)
        mov     eax, dword ptr[quotient+12]
        mov     dword ptr[edi+12], eax
        mov     eax, dword ptr[quotient+8]
        mov     dword ptr[edi+8], eax
        mov     eax, dword ptr[quotient+4]
        mov     dword ptr[edi+4], eax
        mov     eax, dword ptr[quotient]
        mov     dword ptr[edi], eax
        // To sign correction and return
        jmp     applySign

fullDIV:
        // Full 128/64 bit division
        xor     eax, eax
        mov     dword ptr[REMAINDER+12], eax
        mov     dword ptr[REMAINDER+8], eax
        mov     dword ptr[REMAINDER+4], eax
        mov     dword ptr[REMAINDER], eax

        mov     ecx, 128
loop1:
        // Compute REMAINDER:QUOTIENT = REMAINDER:QUOTIENT shl 1
        shl     dword ptr[QUOTIENT], 1
        rcl     dword ptr[QUOTIENT+4], 1
        rcl     dword ptr[QUOTIENT+8], 1
        rcl     dword ptr[QUOTIENT+12], 1
        rcl     dword ptr[REMAINDER], 1
        rcl     dword ptr[REMAINDER+4], 1
        rcl     dword ptr[REMAINDER+8], 1
        rcl     dword ptr[REMAINDER+12], 1

        // Test (REMAINDER >= Divider)
        xor     eax, eax
        cmp     dword ptr[REMAINDER+12], eax
        ja      iftrue
        jb      iffalse

        cmp     dword ptr[REMAINDER+8], eax
        ja      iftrue
        jb      iffalse

        mov     eax, dword ptr[REMAINDER+4]
        cmp     eax, dword ptr[divider+4]
        ja      iftrue
        jb      iffalse

        mov     eax, dword ptr[REMAINDER]
        cmp     eax, dword ptr[divider]
        jb      iffalse
iftrue:
        // Remainder = remainder - divider
        mov     eax, dword ptr[divider]
        sub     dword ptr[REMAINDER], eax
        mov     eax, dword ptr[divider+4]
        sbb     dword ptr[REMAINDER+4], eax
        xor     eax, eax
        sbb     dword ptr[REMAINDER+8], eax
        sbb     dword ptr[REMAINDER+12], eax
        // Quotient = quotient +1
        add     dword ptr[QUOTIENT], 1
        adc     dword ptr[QUOTIENT+4], 0
        adc     dword ptr[QUOTIENT+8], 0
        adc     dword ptr[QUOTIENT+12], 0
iffalse:
        // Loop size = 101 bytes, is less than 127 so loop is possible
        loop    loop1

applySign:
        // Correct the sign of the result based on the stored combined sign
        popfd
        jns     storeRes
        not     dword ptr[edi+12]
        not     dword ptr[edi+ 8]
        not     dword ptr[edi+ 4]
        not     dword ptr[edi]
        add     dword ptr[edi], 1
        adc     dword ptr[edi+ 4], 0
        adc     dword ptr[edi+ 8], 0
        adc     dword ptr[edi+12], 0

storeRES:
        // Get low order qword from var128
        mov     edx, dword ptr[edi+4]
        mov     eax, dword ptr[edi]
done:
    }
    // result is returned in edx:eax
#elif defined (_M_X64 )
#pragma warning(push)
#pragma warning(disable: 4244) // C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    uint64_t a = operant;
    uint64_t b = multiplier;
    uint64_t c = divider;

    //  Normalize divisor
    unsigned long shift;
    _BitScanReverse64(&shift, c);
    shift = 63 - shift;

    c <<= shift;

    // Multiply
    a = _umul128(a, b, &b);
    if (((b << shift) >> shift) != b)
    {
        // Overflow
        return 0xFFFFFFFFFFFFFFFF;
    }
    b = __shiftleft128(a, b, shift);
    a <<= shift;

    uint32_t div;
    uint32_t q0, q1;
    uint64_t t0, t1;

    // 1st Reduction
    div = (uint32_t)(c >> 32);
    t0 = b / div;
    if (t0 > 0xFFFFFFFF)
        t0 = 0xFFFFFFFF;
    q1 = (uint32_t)t0;
    while (1)
    {
        t0 = _umul128(c, (uint64_t)q1 << 32, &t1);
        if (t1 < b || (t1 == b && t0 <= a))
            break;
        q1--;
    }
    b -= t1;
    if (t0 > a)
        b--;
    a -= t0;

    if (b > 0xFFFFFFFF)
    {
        // Overflow
        return 0xFFFFFFFFFFFFFFFF;
    }

    // 2nd reduction
    t0 = ((b << 32) | (a >> 32)) / div;
    if (t0 > 0xFFFFFFFF)
        t0 = 0xFFFFFFFF;
    q0 = (uint32_t)t0;

    while (1)
    {
        t0 = _umul128(c, q0, &t1);
        if (t1 < b || (t1 == b && t0 <= a))
            break;
        q0--;
    }

    return ((uint64_t)q1 << 32) | q0;
#pragma warning(pop)
#endif
#else
    #error MulDiv64 is no supported!
#endif
}

} // namespace CppCommon
