/*------------------------------------------------------------------
 * test_strncat_s
 * File 'str/strncat_s.c'
 * Lines executed:93.90% of 82
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_STRNCAT_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define LEN   ( 128 )

static char   str1[LEN];
static char   str2[LEN];

int test_strncat_s (void)
{
    errno_t rc;
    int32_t ind;
    int len;
    int errs = 0;

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");

    print_msvcrt(use_msvcrt);
    /* probe for msvcrt or safec.dll being active */
    rc = strncat_s(NULL, LEN, str1, LEN);
    init_msvcrt(rc == ESNULLP, &use_msvcrt);
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    strcpy(str1, "a");
    strcpy(str2, "bcde");

    rc = strncat_s(NULL, 1, str2, 0);
    ERR_MSVC(ESNULLP, EINVAL);

    rc = strncat_s(NULL, 0, str2, 0);
    ERR(EOK);

    rc = strncat_s(str1, 0, str2, 0);
    ERR_MSVC(ESZEROL, EINVAL);
    EXPSTR(str1, "a");

    rc = strncat_s(str1, 1, str2, 0);
    ERR_MSVC(ESZEROL, EINVAL); 
    EXPSTR(str1, "");

    rc = strncat_s(str1, 2, str2, 0);
    ERR(EOK);
    EXPSTR(str1, "");

    rc = strncat_s(str1, 3, str2, 0);
    ERR(EOK);
    EXPSTR(str1, "");

/*--------------------------------------------------*/

    strcpy(str1, "a");
    rc = strncat_s(str1, 0, str2, LEN);
    ERR_MSVC(ESZEROL, EINVAL);

/*--------------------------------------------------*/

    strcpy(str1, "a");
    /* valid with the windows sec_api */
    rc = strncat_s(str1, (RSIZE_MAX_STR+1), str2, LEN);
    ERR_MSVC(ESLEMAX, 0);
    if (!use_msvcrt) {
        EXPSTR(str1, "a"); /* untouched */
    } else {
        EXPSTR(str1, "abcde") /* valid */
    }

    strcpy(str1, "a");
    rc = strncat_s(str1, (RSIZE_MAX_STR), str2, (RSIZE_MAX_STR+1));
    ERR_MSVC(ESLEMAX, 0);
    if (!use_msvcrt) {
        EXPSTR(str1, ""); /* cleared */
    } else {
        EXPSTR(str1, "abcde") /* valid */
    }

/*--------------------------------------------------*/

    strcpy(str1, "a");
    rc = strncat_s(str1, LEN, NULL, LEN);
    ERR_MSVC(ESNULLP, EINVAL);
    /*EXPSTR(str1, "a");*/
    if (!use_msvcrt)
        CHECK_SLACK(str1, LEN);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = strncat_s(str1, 1, str2, LEN);
    ERR_MSVC(ESUNTERM, EINVAL);
    if (!use_msvcrt)
        CHECK_SLACK(str1, 1);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = strncat_s(str1, 2, str2, LEN);
    ERR_MSVC(ESUNTERM, EINVAL);
    if (!use_msvcrt)
        CHECK_SLACK(str1, 2);

/*--------------------------------------------------*/

    strcpy(str1, "a");
    strcpy(str2, "b");

    rc = strncat_s(str1, 2, str2, 1);
    ERR_MSVC(ESNOSPC, ERANGE);
    if (!use_msvcrt)
        CHECK_SLACK(str1, 2);

/*--------------------------------------------------*/

    strcpy(str1, "abcd");

    rc = strncat_s(&str1[0], 8, &str1[3], 4);
    ERR_MSVC(ESOVRLP, ERANGE);
    EXPNULL(str1)
    if (!use_msvcrt)
        CHECK_SLACK(str1, 8);

    strcpy(str1, "abcd");

    rc = strncat_s(&str1[0], 4, &str1[3], 4);
    ERR_MSVC(ESOVRLP, EINVAL);
    EXPNULL(str1)
    if (!use_msvcrt)
        CHECK_SLACK(str1, 4);

/*--------------------------------------------------*/

    strcpy(str1, "abcd");

    rc = strncat_s(&str1[0], 3, &str1[3], 4);
    ERR_MSVC(ESUNTERM, EINVAL);
    EXPNULL(str1)
    if (!use_msvcrt)
        CHECK_SLACK(str1, 3);

/*--------------------------------------------------*/

    strcpy(str1, "abcdefgh");

    rc = strncat_s(&str1[3], 5, &str1[0], 4);
    ERR_MSVC(ESUNTERM, EINVAL);
    EXPNULL(&str1[3])
    if (!use_msvcrt)
        CHECK_SLACK(&str1[3], 5);

/*--------------------------------------------------*/

    strcpy(str1, "abcdefgh");

    rc = strncat_s(&str1[3], 12, &str1[0], 4);
    ERR_MSVC(ESOVRLP, EOK);
    if (!use_msvcrt) {
        EXPNULL(&str1[3]);
        CHECK_SLACK(&str1[3], 12);
    } else {
        EXPSTR(str1, "abcdefghabcd"); /* Windows appends! */
    }

/*--------------------------------------------------*/

    strcpy(str1, "a");
    strcpy(str2, "b");

    rc = strncat_s(str1, 3, str2, 1);
    ERR(EOK)
    EXPSTR(str1, "ab");
    if (!use_msvcrt)
        CHECK_SLACK(&str1[2], 3-2);

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaa");
    strcpy(str2, "keep it simple");

    rc = strncat_s(str1, 50, str2, LEN);
    ERR(EOK)
    EXPSTR(str1, "aaaaaaaaaakeep it simple");
    len = strlen(str1);
    if (!use_msvcrt)
        CHECK_SLACK(&str1[len], 50-len);

/*--------------------------------------------------*/
/* TR example */

    strcpy(str1, "good");
    strcpy(str2, "bye");

    rc = strncat_s(str1, 100, str2, 100);
    ERR(EOK)
    EXPSTR(str1, "goodbye");
    len = strlen(str1);
    if (!use_msvcrt)
        CHECK_SLACK(&str1[len], 100-len);

/*--------------------------------------------------*/
/* TR example */

    strcpy(str1, "hello");

    rc = strncat_s(str1, 6, "", 1);
    ERR(EOK)
    EXPSTR(str1, "hello");
    len = strlen(str1);
    if (!use_msvcrt)
        CHECK_SLACK(&str1[len], 6-len);

/*--------------------------------------------------*/
/* TR example */

    strcpy(str1, "hello");

    rc = strncat_s(str1, 6, "X", 2);
    ERR_MSVC(ESNOSPC, ERANGE);
    CHECK_SLACK(str1, 6);

/*--------------------------------------------------*/
/* TR example */

    strcpy(str1, "abc");

    rc = strncat_s(str1, 7, "defghijklmn", 3);
    ERR(EOK)
    EXPSTR(str1, "abcdef");
    len = strlen(str1);
    if (!use_msvcrt)
        CHECK_SLACK(&str1[len], 6-len);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_strncat_s());
}
#endif
