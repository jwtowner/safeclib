/*------------------------------------------------------------------
 * test_strispassword_s
 *
 *
 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "safe_str_lib.h"


#define LEN 64


int main()
{
    boolean_t rc;
    uint32_t len;
    char   str[LEN];

/*--------------------------------------------------*/

    rc = strispassword_s(NULL, LEN);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    rc = strispassword_s("", LEN);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    rc = strispassword_s("Test4You&", 0);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    rc = strispassword_s("Test4You&", 999);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    rc = strispassword_s("", 9);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "Test4You*123");
    len = 8;

    rc = strispassword_s(str, len);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "Test4You*");
    len = strlen(str);

    rc = strispassword_s(str, len);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "Test4You*Test4You*Test4You*");
    len = strlen(str);

    rc = strispassword_s(str, len);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "Eest!22/");
    len = strlen(str);

    rc = strispassword_s(str, len);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "pa$$W0rD");
    len = strlen(str);

    rc = strispassword_s(str, len);
    if (rc != TRUE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    strcpy (str, "Test");
    len = strlen(str);

    rc = strispassword_s(str, len);
    if (rc != FALSE) {
        printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
    }

/*--------------------------------------------------*/

    return (0);
}