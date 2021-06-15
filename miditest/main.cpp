#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>

#pragma comment(lib,"winmm.lib")

HMIDIOUT hMidiOut;

void CALLBACK my_callback(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    // ���͂��ꂽMIDI���b�Z�[�W��\��
    printf("MidiInProc: wMsg=%08X, p1=%08X, p2=%08X\n", wMsg, dwParam1, dwParam2);
    // MIDI-OUT�ɓ]��
    midiOutShortMsg(hMidiOut, dwParam1);
}

void printMidiInDevice() {
    // MIDI-IN�f�o�C�X�̐����擾
    UINT numIn = midiInGetNumDevs();
    UINT devidIn;
    MMRESULT resIn;
    MIDIINCAPS inCaps;

    // MIDI-IN�f�o�C�X�̈ꗗ��\��
    printf("Number of input devides: %d\n", numIn);

    for (devidIn = 0; devidIn < numIn; devidIn++)
    {
        resIn = midiInGetDevCaps(devidIn, &inCaps, sizeof(inCaps));
        // �G���[���A���Ă������΂�
        if (resIn != MMSYSERR_NOERROR) continue;
        printf("ID=%d: %ws\n", devidIn, inCaps.szPname);
    }
}

void printMidiOutDevice() {
    // MIDI-OUT�f�o�C�X�̐����擾
    UINT numOut = midiOutGetNumDevs();
    UINT devidOut;
    MMRESULT resOut;
    MIDIOUTCAPS outCaps;

    //MIDI-OUT�f�o�C�X�̈ꗗ��\��
    printf("Number of output devides: %d\n", numOut);

    for (devidOut = 0; devidOut < numOut; devidOut++)
    {
        resOut = midiOutGetDevCaps(devidOut, &outCaps, sizeof(outCaps));
        // �G���[���A���Ă������΂�
        if (resOut != MMSYSERR_NOERROR) continue;
        printf("ID=%d: %ws\n", devidOut, outCaps.szPname);
    }
}

int printMidiInSignal(UINT devidIn) {
    HMIDIIN hMidiIn;
    MMRESULT res;
    TCHAR errmsg[MAXERRORLENGTH];

    // �����Ŏw�肳�ꂽMIDI-IN�ƃf�t�H���g��MIDI-OUT(ID:0)���J��
    res = midiInOpen(&hMidiIn, devidIn, (DWORD_PTR)my_callback, 0, CALLBACK_FUNCTION);
    midiOutOpen(&hMidiOut, MIDI_MAPPER, (DWORD_PTR)NULL, 0, CALLBACK_NULL);

    // �G���[���o����I������
    if (res != MMSYSERR_NOERROR) {
        printf("Cannot open MIDI input device (ID=%u): ", devidIn);
        midiInGetErrorText(res, errmsg, sizeof(errmsg - 1));
        printf("%ls\n", errmsg);
        return 1;
    }

    // ����������M����҂�������
    printf("Succeeded to open a MIDI input device (ID=%u).\n", devidIn);

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

int main(int argc, char** argv)
{
    //MIDI-I/O�f�o�C�X�̈ꗗ��\��
    printf("--------------------\n");
    printf("MIDI-IN DEVICE\n");
    printf("--------------------\n");
    printMidiInDevice();

    printf("--------------------\n");
    printf("MIDI-OUT DEVICE\n");
    printf("--------------------\n");
    printMidiOutDevice();

    //MIDI-IN�f�o�C�X���J���M����\��
    printf("--------------------\n");
    printf("MIDI-IN SIGNAL\n");
    printf("--------------------\n");

    UINT devidIn;
    printf("MIDI-IN Device ID: ");
    scanf_s("%u", &devidIn);

    printMidiInSignal(devidIn);

    return 0;
}