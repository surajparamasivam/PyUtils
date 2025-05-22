#include <windows.h>
#include <wlanapi.h>
#include <stdio.h>

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

int enableWifi() {
    HANDLE hClient = NULL;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    DWORD dwResult = 0;
    WLAN_INTERFACE_INFO_LIST* pIfList = NULL;

    // Initialize WLAN API
    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS) {
        printf("WlanOpenHandle failed with error: %u\n", dwResult);
        return 1;
    }

    // Get list of WLAN interfaces
    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS) {
        printf("WlanEnumInterfaces failed with error: %u\n", dwResult);
        WlanCloseHandle(hClient, NULL);
        return 1;
    }

    // Enable each wireless interface
    for (int i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
        WLAN_INTERFACE_INFO* pIfInfo = &pIfList->InterfaceInfo[i];
        
        // Enable the interface
        BOOL bEnabled = TRUE;
        dwResult = WlanSetInterface(
            hClient,
            &pIfInfo->InterfaceGuid,
            wlan_intf_opcode_radio_state,
            sizeof(BOOL),
            &bEnabled,
            NULL
        );

        if (dwResult != ERROR_SUCCESS) {
            printf("Failed to enable interface %ws. Error: %u\n", 
                   pIfInfo->strInterfaceDescription, dwResult);
        } else {
            printf("Successfully enabled interface: %ws\n", 
                   pIfInfo->strInterfaceDescription);
        }
    }

    // Cleanup
    if (pIfList != NULL) {
        WlanFreeMemory(pIfList);
    }
    WlanCloseHandle(hClient, NULL);
    return 0;
}

int main() {
    if (enableWifi() == 0) {
        printf("WiFi enabled successfully!\n");
    } else {
        printf("Failed to enable WiFi.\n");
    }
    return 0;
}
