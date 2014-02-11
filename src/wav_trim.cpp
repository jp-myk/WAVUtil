#include "SimpleWAV.h"

int main(int argc, char *argv[]){
  if(argc<5){
    std::cout << argv[0] << " WAVFILE OUTWAVFLE START_TIME[sec] END_TIME[sec]" << std::endl;
    exit(0);
  }
  const char* wavFName=argv[1];
  const char* output_wavFName=argv[2];
  float start= 0.0;
  float end  = 1.0;
  start= atof(argv[3]);
  end  = atof(argv[4]);
  WAV wav,outwav;
  int res=0;
  res=wav.Read(wavFName);
  std::cout << "trim!" << std::endl;
  res=SimpleWAV::trim_wav(start, end, wav, outwav); // extra 0.0 - 1.00[sec]
  res=outwav.Write(output_wavFName);
  if(res==0) std::cout << "sucess" << std::endl;

  return 0;
}
