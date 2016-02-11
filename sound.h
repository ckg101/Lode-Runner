#ifndef SOUND_H
#define SOUND_H

#include <xaudio2.h>

typedef struct tWAVFILEHEADER
{
	char chunkID[4];
	unsigned long chunkSize;
	char waveID[4];
	char ckid[4];
	unsigned long cksize;
	unsigned short wFormatTag;
	unsigned short nChannels;
	unsigned long nSamplesPerSec;
	unsigned long nAvgBytesPerSec;
	unsigned short nBlockAlign;
	unsigned short wBitsPerSample;
	unsigned long subchunkID;
	unsigned long subchunkSize;
} WAVFILEHEADER;

class SOUND
{
	public:
		SOUND();
		SOUND(IXAudio2* xa);
		~SOUND();
		unsigned long loadMIDIFile(HWND hWndNotify, wchar_t* MIDIFileName);
		unsigned long playMIDIFile(void);
		unsigned long stopMIDIFile(void);
		int loadWAVFile(wchar_t* fileName);
		int startWAVFile(void);
		int playWAVFile(void);
		void stopWAVFile(void);
		bool getMIDIStatus(void);
	private:
		IXAudio2* xaudio;
		IXAudio2SourceVoice* pSourceVoice;
		unsigned char* wavdata;
		WAVEFORMATEX wfx;
		WAVFILEHEADER wfh;
		wchar_t errormsg[256];
		BOOL isRunning;
		UINT wDeviceID;
		MCI_OPEN_PARMS mciOpenParms;
		MCI_PLAY_PARMS mciPlayParms;
		MCI_STATUS_PARMS mciStatusParms;
		MCI_SEEK_PARMS mciSeekParms;
		MCI_GENERIC_PARMS mciGenericParms;
		HWND hWndNotify;
		bool isMIDIPlaying;
};

#endif