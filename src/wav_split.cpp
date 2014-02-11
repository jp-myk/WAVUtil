#include "SimpleWAV.h"

int main(int argc, char *argv[]){
  if(argc<4){
    std::cout << argv[0] << " WAVFILE PREFIX SPLIT_TIME_SPAN[sec]" << std::endl;
    exit(0);
  }
  const char* wavFName=argv[1];
  std::string prefix(argv[2]);
  float splitTime=5.0;
  splitTime=atof(argv[3]);
  WAV wav;
  std::vector<WAV> wavList;
  std::vector<WAV>::iterator it;
  int res=0;
  res=wav.Read(wavFName);
  std::cout << "split!" << std::endl;
  wavList=SimpleWAV::split_wav(splitTime, wav); // extra 0.0 - 1.00[sec]
  int i=0;
  char outfname[100];
  //exit(0);
  for(it=wavList.begin();it!=wavList.end();++it){
    sprintf(outfname, "%s.%d.wav", prefix.c_str(), i);
    it->Write(outfname);
    i++;
  }

  return 0;
}
