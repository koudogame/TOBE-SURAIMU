#pragma once


class Error
{
public:
    static void showDialog(const char* Str)
    {
        MessageBox(
            NULL,
            Str,
            "Error",
            MB_OK | MB_ICONERROR
        );
    }
};
