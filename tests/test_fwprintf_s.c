/*------------------------------------------------------------------
 * test_fwprintf_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <unistd.h>

#define LEN   ( 128 )

static FILE* out;
static wchar_t   wstr[LEN];
static char      str[LEN];
 
int test_fwprintf_s (void)
{
    errno_t rc;
    int32_t ind;
    int errs = 0;

    out = fopen("tmp", "w");

/*--------------------------------------------------*/

    rc = fwprintf_s(out, NULL, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    wstr[0] = L'\0'; 
    rc = fwprintf_s(out, L"%s%n\n", wstr, &ind);
    ERR(EINVAL)

    rc = fwprintf_s(out, L"%s%%n\n", wstr);
    ERR(3)

    rc = fwprintf_s(out, L"%%n\n");
    ERR(3);

/*--------------------------------------------------*/

    /* TODO
    rc = fwprintf_s(out, L"%s", NULL);
    ERR(ESNULLP)
    */

/*--------------------------------------------------*/

    wcscpy(wstr, L"12");
    strcpy(str, "34");

    rc = fwprintf_s(out, L"%ls%s", wstr, str);
    ERR(4)

/*--------------------------------------------------*/
    fclose(out);
    unlink("tmp");

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_fwprintf_s());
}
#endif