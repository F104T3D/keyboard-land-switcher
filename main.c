#include <windows.h>
#include <stdio.h>
#include <string.h>

HHOOK keyboardHook;
int currentLayout = 0;
char layout1[9], layout2[9];

void ToggleLayout() {
    currentLayout = !currentLayout;

    const char* layoutToLoad = currentLayout ? layout2 : layout1;

    HKL newLayout = LoadKeyboardLayout(layoutToLoad, KLF_ACTIVATE);
    if (!newLayout) {
        char errorMsg[128];
        sprintf(errorMsg, "fuck %s", layoutToLoad);
        MessageBox(NULL, errorMsg, "Error", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;

        if (p->vkCode == VK_OEM_6) {
            ToggleLayout();
            
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    MSG msg;

    printf("First layout ID (e.g. 00000409 for English US): ");
    scanf("%8s", layout1);
    printf("Second layout ID (e.g. 00000401 for Arabic): ");
    scanf("%8s", layout2);

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (!keyboardHook) {
        MessageBox(NULL, "Keyboard hook failed", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    printf("switch layouts. ctrl+c exit.\n");

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}
