#include "WAVUtil.h"
namespace SimpleWAV{

//
// Triming WAV
//
int trim_wav(float startTime_sec, float endTime_sec, WAV& wav, WAV& outwav){
  int datasize=0;
  void* data;
  int res=0;
  unsigned int startSamplePos= startTime_sec*wav.getSampleRate();
  unsigned int endSamplePos  = endTime_sec*wav.getSampleRate();
  if(endSamplePos>wav.getSampleCount()){
    endSamplePos=wav.getSampleCount();
    endTime_sec=(float)endSamplePos/wav.getSampleRate();
  }
  printf("trim %.5f - %.5f [sec] / %.5f [sec]\n", startTime_sec, endTime_sec, (float)wav.getSampleCount()/wav.getSampleRate());
  
  unsigned int startBytePos  = startSamplePos*wav.getChannels()*wav.getBit()/8;
  unsigned int endBytePos  =  endSamplePos*wav.getChannels()*wav.getBit()/8;

  outwav.createHeader(wav.getChannels(), wav.getSampleRate(), wav.getBit());
  datasize=endBytePos-startBytePos;
  data=(void*)malloc(endBytePos-startBytePos);
  memcpy(data, (char*)wav.getData()+startBytePos, datasize);
  res=outwav.setData(data, datasize);
  free(data);
  return 0;
}

//
// triming WAV by TimeSpan[sec]
//
std::vector<WAV> split_wav(float timespan_sec, WAV& wav){
  std::vector<WAV> retval;
  float dataLen_sec = (float)wav.getSampleCount()/wav.getSampleRate();
  if(timespan_sec>=dataLen_sec){
    printf("set time span[%.3f] is longer than data[%.3f]", timespan_sec,dataLen_sec);
    exit(-1);
  }
  int wav_num=(int)dataLen_sec/timespan_sec+1;
  retval.reserve(wav_num);
  
  // last splitted wav is adjusted by left time.
  float startTime=0.0;
  int n=0;
  //char outfname[100];
  while(startTime<dataLen_sec){
    float endTime=startTime+timespan_sec;
    WAV trimwav;
    trim_wav(startTime, endTime, wav, trimwav);
    retval.push_back(trimwav);
    startTime+=timespan_sec;
    n++;
  }
  return retval;
}

//
// Combine WAV data
//
int combine_wav(WAV& wav1, WAV& wav2, WAV& outwav){
  
  if(wav1.getChannels() != wav2.getChannels()){
    printLOG("wav1 and wav2 are not same channel");
    return -1;
  }

  if(wav1.getSampleRate() != wav2.getSampleRate()){
    printLOG("wav1 and wav2 are not same samplerate");
    return -1;
  }

  if(wav1.getBit() != wav2.getBit()){
    printLOG("wav1 and wav2 are not same bit");
    return -1;
  }
  
  outwav.createHeader(wav1.getChannels(), wav1.getSampleRate(), wav1.getBit());
  
  unsigned int datasize=0;
  void* combinedData;
  datasize+=wav1.getDataSize();
  datasize+=wav2.getDataSize();
  combinedData=(void*)malloc(datasize);
  
  memcpy(combinedData, wav1.getData(), wav1.getDataSize());
  memcpy((char*)combinedData+wav1.getDataSize(), wav2.getData(), wav2.getDataSize());    
    
  if(outwav.setData(combinedData, datasize)!=0){
    free(combinedData);
    printLOG("error combine wav1 and wav2");
    return -1;
  }
  
  free(combinedData);
  return 0;
}

}
