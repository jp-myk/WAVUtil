#ifndef _WAV_H_
#define _WAV_H_
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cmath>

#define printLOG( msg ) printf("%s,%s,%d:%s\n",__FILE__,__FUNCTION__,__LINE__,msg)

#define HEADER_SIZE 44
typedef unsigned char  Monoral8;
typedef signed short   Monoral16;
typedef struct{
	signed short l;
	signed short r;
}Stereo16;

typedef struct{
	unsigned char l;
	unsigned char r;
}Stereo8;


class WAV{
 private:
  int freeWAVData();
 public:
  WAV(){
    _monoral8=(Monoral8*)NULL;
    _monoral16=(Monoral16*)NULL;
    _stereo8=(Stereo8*)NULL;
    _stereo16=(Stereo16*)NULL;
  };
  ~WAV(){
    freeWAVData();
  };
  WAV(const WAV& other); // copy constructor 
  WAV& operator=( const WAV& other ); // = operator
  bool empty(){
    return _dataSize==0 ? true : false;
  }
  // HEADER
  char _riffID[4]; // "riff"
  unsigned int _size;  // ファイルサイズ-8
  char _wavID[4];  // "WAVE"
  char _fmtID[4];  // "fmt "
  unsigned int _fmtSize; // fmtチャンクのバイト数 //LinearPCMならば16
  unsigned short _format; // フォーマット         //LinearPCMならば1
  unsigned short _channels; // チャンネル数 1
  unsigned int _sampleRate; // サンプリングレート 16000
  unsigned int _bytePerSec; // データ速度 32000
  unsigned short _blockSize; // ブロックサイズ 2
  unsigned short _bit;  // 量子化ビット数 16
  char _dataID[4]; // "data"
  unsigned int _dataSize; // 波形データのバイト数
  
  // BODY
  unsigned long _sampleCount; // = _whdr.dataSize / (_whdr.channels*(_whdr.bit/8));
  Monoral8 *_monoral8;
  Monoral16 *_monoral16;
  Stereo8 *_stereo8;
  Stereo16 *_stereo16;
  
  // Method
  int Read(const char* WAVFName);
  int Write(const char* WAVFName);
  unsigned short getChannels()const { return _channels;}
  unsigned int getSampleRate()const { return _sampleRate;}
  unsigned short getBit()const { return _bit;}
  const void* getData()const;
  unsigned int getDataSize()const { return _dataSize;}
  unsigned int getSampleCount()const { return _sampleCount;}
  void createHeader(unsigned short channels, unsigned int samplerate, unsigned short bit);
  void setChannels(unsigned short channels_){ _channels=channels_;}
  void setSampleRate(unsigned int sampleRate_){ _sampleRate=sampleRate_;}
  void setBit(unsigned short bit_){ _bit=bit_;}
  void setDataSize(unsigned int datasize_){ 
    _dataSize=datasize_;
    _size=_dataSize+36;

  }
  int setData(const void *data, unsigned int datasize);

  void setSampleCount(unsigned int samplecount_){ _sampleCount=samplecount_;}
  void dumpHeader();
};



#endif 
