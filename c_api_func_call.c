#include <stdio.h>

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
