#include <Windows.h>
#include <string>

// �������������� ��������� ����������
#define IDC_CHAT_OUTPUT 1001
#define IDC_CHAT_INPUT 1002
#define IDC_CHAT_SEND 1003

HWND hOutput, hInput, hSend;

// ���������� ������� �� ������ "���������"
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        // �������� ��������� ����������
        hOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY,
            10, 10, 380, 200, hwnd, (HMENU)IDC_CHAT_OUTPUT, GetModuleHandle(NULL), NULL);

        hInput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE,
            10, 220, 300, 50, hwnd, (HMENU)IDC_CHAT_INPUT, GetModuleHandle(NULL), NULL);

        hSend = CreateWindowW(L"BUTTON", L"Send", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 220, 70, 50, hwnd, (HMENU)IDC_CHAT_SEND, GetModuleHandle(NULL), NULL);
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED && (HWND)lParam == hSend)
        {
            // ��������� ������� �� ������ "���������"
            // �������� ����� �� ���� �����
            wchar_t text[256];
            GetWindowTextW(hInput, text, 256);

            // ������� ���� �����
            SetWindowTextW(hInput, L"");

            // ��������� ����� � ���� ������
            std::wstring currentText;
            int length = GetWindowTextLengthW(hOutput);
            if (length > 0)
            {
                // ���� ���� ������ �� ������, ��������� ����� ������
                wchar_t* buffer = new wchar_t[length + 2];
                GetWindowTextW(hOutput, buffer, length + 1);
                currentText = std::wstring(buffer) + L"\r\n";
                delete[] buffer;
            }
            currentText += text;
            SetWindowTextW(hOutput, currentText.c_str());
        }
        break;

    case WM_DESTROY:
        // ���������� ������
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// ����� ����� � ����������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ������������ ����� ����
    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"ChatWindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExW(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // ������� ����
    HWND hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"ChatWindowClass", L"Chat Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // ���������� ����
    ShowWindow(hwnd, nCmdShow);