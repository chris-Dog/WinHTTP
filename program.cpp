#include "stdafx.h"

#define USER_AGENT L"WinHTTP"
#define HOST L"raw.githubusercontent.com"
#define URL_PATH L"/pmolchanov/WinHTTP/master/README.md"
#define HTTPS_ENABLED

int wmain(int argc, WCHAR* argv[])
{

  HINTERNET handle_session = NULL;
  HINTERNET handle_connect = NULL;
  HINTERNET handle_request = NULL;

  do {

    // Initialize WinHTTP and get session handle
    handle_session = WinHttpOpen(USER_AGENT, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if(!handle_session) {
      wprintf(L"WinHttpOpen error: %u", GetLastError());
      break;
    }

    // Specify the server for the HTTP request and get a connection handle
    handle_connect = WinHttpConnect(handle_session, HOST, INTERNET_DEFAULT_PORT, 0);
    if(!handle_connect) {
      wprintf(L"WinHttpConnect error: %u", GetLastError());
      break;
    }

    // Create an HTTP request handle
    DWORD dwFlags = 0;
#ifdef HTTPS_ENABLED
    dwFlags |= WINHTTP_FLAG_SECURE;
#endif
    handle_request = WinHttpOpenRequest(handle_connect, L"GET", URL_PATH, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, dwFlags);
    if(!handle_request) {
      wprintf(L"WinHttpOpenRequest error: %u", GetLastError());
      break;
    }

    // Send request to HTTP server
    if(!WinHttpSendRequest(handle_request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
      wprintf(L"WinHttpSendRequest error: %u", GetLastError());
      break;
    }

    // Wait to receive the status code and response headers from the HTTP server
    if(!WinHttpReceiveResponse(handle_request, NULL)) {
      wprintf(L"WinHttpReceiveResponse error: %u", GetLastError());
      break;
    }

    // Keep checking for data until there is nothing left.
    DWORD bytes_available = 0;
    DWORD bytes_read = 0;
    LPSTR buffer = NULL;
    do 
    {
      // Get amount of data in bytes available to be read
      bytes_available = 0;
      if(!WinHttpQueryDataAvailable(handle_request, &bytes_available)) {
        wprintf(L"WinHttpQueryDataAvailable error: %u", GetLastError());
        break;
      }

      // Allocate and zero out buffer
      int buffer_size = bytes_available + 1;
      buffer = new char[buffer_size];
      ZeroMemory(buffer,buffer_size);

      // Read data to buffer
      if(!WinHttpReadData(handle_request,(LPVOID)buffer, bytes_available, &bytes_read)) {
        wprintf(L"WinHttpReadData error: %u", GetLastError());
        break;
      }

      // Dump buffer to stdout
      printf("%s", buffer);
          
      // Free the memory allocated to the buffer.
      delete [] buffer;
      buffer = NULL;

    } while(bytes_available > 0);

    // Free the memory allocated to the buffer.
    if(buffer) {
      delete [] buffer;
      buffer = NULL;
    }

  } while(0);

  // Close any open handles
  if(handle_request) {
    WinHttpCloseHandle(handle_request);
  }
  if(handle_connect) {
    WinHttpCloseHandle(handle_connect);
  }
  if(handle_session) {
    WinHttpCloseHandle(handle_session);
  }

	return 0;
}

