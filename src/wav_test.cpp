#include "SimpleWAV.h"

int main(int argc, char *argv[]){
  if(argc<2){
    std::cout << argv[0] << " FILENAME" << std::endl;
    exit(0);
  }
  const char* wavFName=argv[1];
  std::string outFNameStr=wavFName;
  outFNameStr+="_cp.wav";
  WAV wav;
  int res=0;
  res=wav.Read(wavFName);
  wav.Write(outFNameStr.c_str());
  wav.dumpHeader();
  return 0;
}
