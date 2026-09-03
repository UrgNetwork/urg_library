/*
 * Test program to diagnose COM port detection issue
 */

#include <windows.h>
#include <setupapi.h>
#include <stdio.h>

int32_t main(void)
{
    // GUID for COM port class - GUID_DEVCLASS_PORTS
    GUID GUID_DEVCLASS_PORTS = {
        0x4D36E978L, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};

    HDEVINFO hdi;
    SP_DEVINFO_DATA sDevInfo;
    DWORD i = 0;

    printf("Testing DIGCF_PRESENT | DIGCF_DEVICEINTERFACE...\n");
    hdi = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL,
                              DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hdi == INVALID_HANDLE_VALUE)
    {
        printf("  Result: FAILED (INVALID_HANDLE_VALUE)\n");
    }
    else
    {
        printf("  Result: OK (valid handle)\n");
    }

    printf("\nTesting DIGCF_PRESENT (device info set)...\n");
    hdi = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
    if (hdi == INVALID_HANDLE_VALUE)
    {
        printf("  Result: FAILED (INVALID_HANDLE_VALUE)\n");
        return 1;
    }
    else
    {
        printf("  Result: OK (valid handle)\n");
    }

    sDevInfo.cbSize = sizeof(SP_DEVINFO_DATA);
    printf("\nEnumerating devices:\n");
    for (i = 0; SetupDiEnumDeviceInfo(hdi, i, &sDevInfo); ++i)
    {
        char device_desc[256] = {0};
        char friendly_name[256] = {0};
        DWORD size = 0;
        DWORD regType = 0;

        SetupDiGetDeviceRegistryPropertyA(hdi, &sDevInfo, SPDRP_DEVICEDESC,
                                          &regType, (BYTE *)device_desc, sizeof(device_desc), &size);

        SetupDiGetDeviceRegistryPropertyA(hdi, &sDevInfo, SPDRP_FRIENDLYNAME,
                                          &regType, (BYTE *)friendly_name, sizeof(friendly_name), &size);

        printf("  [%d] desc='%s', friendly='%s'\n", i, device_desc, friendly_name);
    }

    printf("Total devices found: %d\n", i);
    SetupDiDestroyDeviceInfoList(hdi);

    return 0;
}
