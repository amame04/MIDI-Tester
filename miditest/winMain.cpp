#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>

#pragma comment(lib,"winmm.lib")

HMIDIOUT hMidiOut;

void CALLBACK my_callback(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    printf("MidiInProc: wMsg=%08X, p1=%08X, p2=%08X\n", wMsg, dwParam1, dwParam2);
    midiOutShortMsg(hMidiOut, dwParam1);
}

//int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdLine, _In_ int nCmdShow)
int main(int argc, char** argv)
{
    UINT numIn = midiInGetNumDevs();
    UINT devidIn;
    MMRESULT resIn;
    MIDIINCAPS inCaps;

    printf("--------------------\n");
    printf("MIDI-IN DEVICE\n");
    printf("--------------------\n");
    printf("Number of input devides: %d\n", numIn);

    for (devidIn = 0; devidIn < numIn; devidIn++)
    {
        resIn = midiInGetDevCaps(devidIn, &inCaps, sizeof(inCaps));
        if (resIn != MMSYSERR_NOERROR) continue;
        printf("ID=%d: %ws\n", devidIn, inCaps.szPname);
    }


    UINT numOut = midiOutGetNumDevs();
    UINT devidOut;
    MMRESULT resOut;
    MIDIOUTCAPS outCaps;

    printf("--------------------\n");
    printf("MIDI-OUT DEVICE\n");
    printf("--------------------\n");
    printf("Number of output devides: %d\n", numOut);

    for (devidOut = 0; devidOut < numOut; devidOut++)
    {
        resOut = midiOutGetDevCaps(devidOut, &outCaps, sizeof(outCaps));
        if (resOut != MMSYSERR_NOERROR) continue;
        printf("ID=%d: %ws\n", devidOut, outCaps.szPname);
    }


    HMIDIIN hMidiIn;
    MMRESULT res;
    UINT devid = 1u;
    TCHAR errmsg[MAXERRORLENGTH];

    printf("--------------------\n");
    printf("MIDI-IN SIGNAL\n");
    printf("--------------------\n");

    printf("MIDI-IN Device ID: ");
    scanf_s("%u", &devid);

    res = midiInOpen(&hMidiIn, devid, (DWORD_PTR)my_callback, 0, CALLBACK_FUNCTION);
    midiOutOpen(&hMidiOut, MIDI_MAPPER, (DWORD_PTR)NULL, 0, CALLBACK_NULL);
    
    if (res != MMSYSERR_NOERROR) {
        printf("Cannot open MIDI input device (ID=%u): ", devid);
        midiInGetErrorText(res, errmsg, sizeof(errmsg-1));
        printf("%ls\n", errmsg);
        return 1;
    }

    printf("Succeeded to open a MIDI input device (ID=%u).\n", devid);

    midiInStart(hMidiIn);

    while (1) {
        Sleep(10);
    }
    midiOutReset(hMidiOut);
    midiOutClose(hMidiOut);

    midiInStop(hMidiIn);
    midiInReset(hMidiIn);

    midiInClose(hMidiIn);
    printf("Closed a MIDI input device.\n");

    return 0;
}