#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

#define RET_OK  1
#define RET_ERR 0

#define MULTIAP_MAC_ADDRESS_STR_LEN 0x0D

#define PRINT_D(x, ...) fprintf( stdout, "%s:%s:%d: " x "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

/** @ brief Allocate a chunk of 'n' bytes and return a pointer to it.
 *
 * If no memory can be allocated, this function exits immediately.
 */
static inline void *memalloc(size_t size)
{
    void *p;

    p = malloc(size);

    if (NULL == p)
    {
        fprintf(stderr, "ERROR: Out of memory!\n");
        exit(1);
    }

    return p;
}

/** @brief Redimension a memory area previously obtained with memalloc().
 *
 * If no memory can be allocated, this function exits immediately.
 */
static inline void *memrealloc(void *ptr, size_t size)
{
    void *p;

    p = realloc(ptr, size);

    if (NULL == p)
    {
        fprintf(stderr, "ERROR: Out of memory!\n");
        exit(1);
    }

    return p;
}

/*func: this function use to verify mac address
* Input:
*      mac_string: is the input mac address
* Output:
*       return: RET_OK: if it is successfull
*               RET_ERR: If it is failed.
*/
uint8_t _validateMacString(const char *mac_string)
{
    uint8_t i;
    for (i = 0; i < (MULTIAP_MAC_ADDRESS_STR_LEN-1); i++)
    {
        if(!isxdigit(mac_string[i]))
            return RET_ERR;
    }
    return RET_OK;
}

/*Function: to create api that can use to convert string to mac address
* Input:
*   input_str: macaddress in string format
* output:
*   out_mac: the out put is the macaddress in type of uint8_t
*/

int charToNumber(char character)
{
    if ((character >= '0') && (character <= '9'))
        return (character - '0');

    if ((character >= 'a') && (character <= 'f'))
        return (character - 'a' + 10);

    if ((character >= 'A') && (character <= 'F'))
        return (character - 'A' + 10);

    return -1;
}

uint8_t macStringArrayToNumber(const char *input_str, uint8_t *mac_addr)
{
    uint8_t i;
    for (i = 0; i < 6; i++)
    {
        int tens, units;
        tens = charToNumber(*input_str++);
        if (tens < 0)
            return -1;

        units = charToNumber(*input_str++);
        if (units < 0)
            return -1;

        *mac_addr++ = (tens << 4) | units;
    }
    return 0;
}

void convertStringToMac(const char *input_str, uint8_t *out_mac)
{
    int ret;

    if((!input_str) || (strlen(input_str) != (MULTIAP_MAC_ADDRESS_STR_LEN-1)))
    {
        printf("[%s-%d] Input string should not null \n", __func__, __LINE__);
    }
    else
    {
        ret = macStringArrayToNumber(input_str, out_mac);
        if(ret < 0)
        {
            PRINT_I("[%s-%d] Failed to convert mac \n", __func__, __LINE__);
            memset(out_mac, 0, MULTIAP_MAC_ADDRESS_LEN);
        }

        PRINT_D("[%s-%d] The entry MAC:  %02X%02X%02X%02X%02X%02X \n", __func__, __LINE__,
                                                                   out_mac[0], out_mac[1], out_mac[2],
                                                                   out_mac[3], out_mac[4], out_mac[5]);
    }
}

/*Function: to create new thread and check thread is already existed?
* Input:
*   thread_id: input parameter for tracking thread
* output:
*   RET_OK: Create new thread successfully
*   RET_ERR: Failed to create thread.
*/

uint8_t is_existed_thread = 0;

void *_helloFunction(void *p)
{
    PRINT_D("enter hello world \n");
    pthread_exit(NULL);
}

uint8_t _createNewThread(pthread_t *thread_id)
{
    uint8_t ret;
    uint8_t status;

    ret = RET_ERR;
    PRINT_D(" Start \n");

    if (is_existed_thread)
    {
        PRINT_D("Already have existed thread \n");
        return ret;
    }

    status = pthread_create(thread_id, NULL, &_helloFunction, NULL);
    if (0 != status)
    {
        PRINT_D("Failed to create thread \n");
        return RET_ERR;
    }

    is_existed_thread = 1;
    status = pthread_detach(*thread_id);
    if (0 != status)
    {
        PRINT_D("Failed to detach thread \n");
        return RET_ERR;
    }

    return RET_OK;
}

void main()
{
    uint8_t status;
    pthread_t thread1;

    status = _createNewThread(&thread1);
    if (RET_OK != status)
    {
        PRINT_D("Failed to create new thread \n");
        return;
    }
}
