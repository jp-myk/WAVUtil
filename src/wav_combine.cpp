
#include "SimpleWAV.h"

int main(int argc, char *argv[]){
  if(argc<4){
    std::cout << argv[0] << " WAVFILE1 WAVFILE2 OUTPUTWAV" << std::endl;
    exit(0);
  }
  const char* wavFName1=argv[1];
  const char* wavFName2=argv[2];
  const char* output_wavFName=argv[3];
  WAV wav1,wav2,wav3;
  int res=0;
  res=wav1.Read(wavFName1);
  res=wav2.Read(wavFName2);
  std::cout << "combine!" << std::endl;
  res=SimpleWAV::combine_wav(wav1, wav2, wav3);
  res=wav3.Write(output_wavFName);
  if(res==0) std::cout << "success" << std::endl;
  
  return 0;
}

