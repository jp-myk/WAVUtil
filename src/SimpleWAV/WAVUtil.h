#include <vector>
#include "WAV.h"

namespace SimpleWAV{
  
  // @note wav trimming function
  // @param1 start time second to trim WAV(ex. 1.045)
  // @param2 end time second to trim WAV(ex. 5.001)
  // @param3 input original WAV
  // @param4 output trimmed WAV 
  // @return 0:success -1:fail
  int trim_wav(float startTime_sec, float endTime_sec, WAV& wav, WAV& outwav);
  
  // @note wav split function every seconds
  // @param1 time span
  // @param2 original WAV
  // @return output trimmed WAV vector 
  std::vector<WAV> split_wav(float timespan_sec, WAV& wav);
  
  // @note wav combine function
  // @param1 input1 wav
  // @param2 input2 wav
  // @param3 output combined wav
  // @return 0:success -1:error
  int combine_wav(WAV& wav1, WAV& wav2, WAV& outwav);

}
