#include "SimpleWAV.h"

int main(int argc, char *argv[]){
  if(argc<2){
    std::cout << argv[0] << " FILENAME" << std::endl;
    exit(0);
  }
  const char* wavFName=argv[1];
  WAV wav;
  int res=0;
  res=wav.Read(wavFName);
  if(res!=0){
    std::cout << "error open WAVFILE:" << wavFName << std::endl;
    exit(-1);
  }
  SimpleWAV::cat_wav(wav);
  return 0;
}
