#include <Windows.h>
#include <winhttp.h>
#include <iostream>
#include <string>

#pragma comment(lib, "winhttp.lib")

int main()
{
    std::string api_key = "key";
    std::string url = "https://www.virustotal.com/vtapi/v2/file/scan";
    std::string file_path = filepath;

    HANDLE hFile = CreateFileA(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    DWORD file_size = GetFileSize(hFile, NULL);

    if (file_size == INVALID_FILE_SIZE)
    {
        std::cout << "Error getting file size" << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    LPBYTE file_buffer = new BYTE[file_size];

    if (file_buffer == NULL)
    {
        std::cout << "Error allocating memory" << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    DWORD bytes_read;

    if (!ReadFile(hFile, file_buffer, file_size, &bytes_read, NULL))
    {
        std::cout << "Error reading file" << std::endl;
        CloseHandle(hFile);
        delete[] file_buffer;
        return 1;
    }

    CloseHandle(hFile);

    HINTERNET hSession = WinHttpOpen(L"VirusTotal API", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession == NULL)
    {
        std::cout << "Error creating WinHTTP session" << std::endl;
        delete[] file_buffer;
        return 1;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, L"www.virustotal.com", INTERNET_DEFAULT_HTTPS_PORT, 0);

    if (hConnect == NULL)
    {
        std::cout << "Error creating WinHTTP connection" << std::endl;
        WinHttpCloseHandle(hSession);
        delete[] file_buffer;
        return 1;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/vtapi/v2/file/scan", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (hRequest == NULL)
    {
        std::cout << "Error creating WinHTTP request" << std::endl;
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        delete[] file_buffer;
        return 1;
    }

    std::string headers = "Content-Type: multipart/form-data; boundary=------------------------7d4a6d158c9";
    WinHttpAddRequestHeaders(hRequest, headers.c_str(), headers.length(), WINHTTP_ADDREQ_FLAG_ADD);

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)body.c_str(), body.length(), body.length(), 0))
    {
        std::cout << "Error sending WinHTTP request" << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        delete[] file_buffer;
        return 1;
    }

    if (!WinHttpReceiveResponse(hRequest, NULL))
    {
        std::cout << "Error receiving WinHTTP response" << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        delete[] file_buffer;
        return 1;
    }

    DWORD dwStatusCode = 0;
    DWORD dwBufferLength = sizeof(dwStatusCode);

    if (!WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwStatusCode, &dwBufferLength, NULL))
    {
        std::cout << "Error querying WinHTTP headers" << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        delete[] file_buffer; return 1;
    }

    std::cout << "HTTP status code: " << dwStatusCode << std::endl;

    delete[] file_buffer;

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return 0;
}
