#ifndef SOUND_H
#define SOUND_H

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
		unsigned long playMIDIFile(HWND hWndNotify, wchar_t* MIDIFileName);
		int loadWAVFile(wchar_t* fileName);
		int startWAVFile(void);
		int playWAVFile(void);
	protected:
		IXAudio2* xaudio;
		IXAudio2SourceVoice* pSourceVoice;
		unsigned char* wavdata;
		BOOL isRunning;
};

#endif