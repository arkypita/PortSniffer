//
// PortSniffer - Monitor the traffic of arbitrary serial or parallel ports
// Copyright 2020 Colin Finck, ENLYZE GmbH <c.finck@enlyze.com>
//
// SPDX-License-Identifier: MIT
//

#include <initguid.h>
#include "PortSniffer-Tool.h"


static int
_PrintUsage()
{
    printf("Usage: PortSniffer-Tool [OPTIONS]\n");
    printf("\n");
    printf("Installation Options:\n");
    printf("    /install                Install the driver.\n");
    printf("    /uninstall              Uninstall the driver.\n");
    printf("\n");
    printf("Setup Options:\n");
    printf("    /ports                  Get all ports that can be monitored.\n");
    printf("    /attached               Get all ports the driver is currently attached to.\n");
    printf("    /attach PORT            Attach the driver to the given port.\n");
    printf("    /detach PORT            Detach the driver from the given port.\n");
    printf("    /version                Get the version of the running driver.\n");
    printf("\n");
    printf("Monitoring:\n");
    printf("    /monitor PORT TYPES     Monitor the given port.\n");
    printf("                            TYPES may be one or more of:\n");
    printf("                               R - Read requests\n");
    printf("                               W - Write requests\n");
    printf("\n");

    return 1;
}

HANDLE
OpenPortSniffer(void)
{
    HANDLE hPortSniffer;

    hPortSniffer = CreateFileW(L"\\\\.\\EnlyzePortSniffer", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPortSniffer == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Could not open \"\\\\.\\EnlyzePortSniffer\", last error is %lu.\n", GetLastError());
        fprintf(stderr, "Either the PortSniffer Driver is not installed or not attached to any port.\n");
    }

    return hPortSniffer;
}

int __cdecl
wmain(
    __in int argc,
    __in wchar_t* argv[]
    )
{
    printf("**********************************************************************\n");
    printf("ENLYZE PortSniffer Tool " PORTSNIFFER_VERSION_COMBINED "\n");
    printf("Copyright 2020 Colin Finck, ENLYZE GmbH <c.finck@enlyze.com>\n");
    printf("**********************************************************************\n\n");

    if (argc == 2 && wcscmp(argv[1], L"/install") == 0)
    {
        return HandleInstallParameter();
    }
    else if (argc == 2 && wcscmp(argv[1], L"/uninstall") == 0)
    {
        return HandleUninstallParameter();
    }
    else if (argc == 2 && wcscmp(argv[1], L"/ports") == 0)
    {
        return HandlePortsParameter();
    }
    else if (argc == 2 && wcscmp(argv[1], L"/attached") == 0)
    {
        return HandleAttachedParameter();
    }
    else if (argc == 3 && wcscmp(argv[1], L"/attach") == 0)
    {
        return HandleAttachParameter(argv[2]);
    }
    else if (argc == 3 && wcscmp(argv[1], L"/detach") == 0)
    {
        return HandleDetachParameter(argv[2]);
    }
    else if (argc == 2 && wcscmp(argv[1], L"/version") == 0)
    {
        return HandleVersionParameter();
    }
    else if (argc == 4 && wcscmp(argv[1], L"/monitor") == 0)
    {
        return HandleMonitorParameter(argv[2], argv[3]);
    }
    else
    {
        return _PrintUsage();
    }
}
