/*------------------------------------------------------------------
 * memcpy_s
 *
 * October 2008, Bo Berry
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#include "safe_mem_lib.h"
#include "safe_mem_constraint.h"


/**
 * NAME
 *    memcpy_s
 *
 * SYNOPSIS
 *    #include "safe_mem_lib.h"
 *    errno_t
 *    memcpy_s(void *dest, rsize_t dmax, const void *src, rsize_t smax)
 *
 * DESCRIPTION
 *    This function copies at most smax bytes from src to dest, up to
 *    dmax.
 *
 * SPECIFIED IN
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * INPUT PARAMETERS
 *    dest      pointer to memory that will be replaced by src.
 *
 *    dmax      maximum length of the resulting dest
 *
 *    src       pointer to the memory that will be copied to dest
 *
 *    smax      maximum number bytes of src to copy
 *
 *  OUTPUT PARAMETERS
 *    dest      is updated
 *
 * RUNTIME CONSTRAINTS
 *    Neither dest nor src shall be a null pointer.
 *    Neither dmax nor smax shall be zero.
 *    dmax shall not be greater than RSIZE_MAX_MEM.
 *    smax shall not be greater than dmax.
 *    Copying shall not take place between regions that overlap.
 *    If there is a runtime-constraint violation, the memcpy_s function
 *        stores zeros in the ﬁrst dmax bytes of the region pointed to
 *        by dest if dest is not a null pointer and smax is valid.
 *
 * RETURN VALUE
 *    EOK        successful operation
 *    ESNULLP    NULL pointer
 *    ESZEROL    zero length
 *    ESLEMAX    length exceeds max limit
 *    ESOVRLP    source memory overlaps destination
 *
 * ALSO SEE
 *    memcpy16_s(), memcpy32_s(), memmove_s(), memmove16_s(),
 *     memmove32_s()
 *
 */
errno_t
memcpy_s (void *dest, rsize_t dmax, const void *src, rsize_t smax)
{
    uint8_t *dp;
    const uint8_t  *sp;

    dp = dest;
    sp = src;

    if (dp == NULL) {
        invoke_safe_mem_constraint_handler("memcpy_s: dest is NULL",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (dmax == 0) {
        invoke_safe_mem_constraint_handler("memcpy_s: dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (dmax > RSIZE_MAX_MEM) {
        invoke_safe_mem_constraint_handler("memcpy_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    if (smax == 0) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy_s: smax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (smax > dmax) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy_s: smax exceeds dmax",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    if (sp == NULL) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy_s: src is NULL",
                   NULL, ESNULLP);
        return (ESNULLP);
    }


    /*
     * overlap is undefined behavior, do not allow
     */
    if( ((dp > sp) && (dp < (sp+smax))) ||
        ((sp > dp) && (sp < (dp+dmax))) ) {
        mem_prim_set(dp, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy_s: overlap undefined",
                   NULL, ESOVRLP);
        return (ESOVRLP);
    }

    /*
     * now perform the copy
     */
    mem_prim_move(dp, sp, smax);

    return (EOK);
}