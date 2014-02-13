#include "WAV.h"

WAV::WAV(const WAV& other){
  init();
  this->createHeader(other.getChannels(), other.getSampleRate(), other.getBit());
    if(this->setData(other.getData(), other.getDataSize())!=0){
      printLOG("copy constructor error.");
      exit(-1);
    }
}
WAV& WAV::operator=( const WAV& other ){
  if (this != &other) {
    this->createHeader(other.getChannels(), other.getSampleRate(), other.getBit());
    if(this->setData(other.getData(), other.getDataSize())!=0){
      printLOG("copy operator error.");
      exit(-1);
    }
  }
  return *this;
}
int WAV::freeWAVData(){
  try{
    if(_monoral8!=(Monoral8*)NULL){ 
      std::cout << "free monoral8" << std::endl;
      free(_monoral8);
    }
    if(_monoral16!=(Monoral16*)NULL){
      free(_monoral16);
    }
    if(_stereo8!=(Stereo8*)NULL){
      free(_stereo8);
    }
    if(_stereo16!=(Stereo16*)NULL){
      free(_stereo16);
    }
  }catch(const char* e){
    printLOG("free error");
    printf("%s\n", e);
    exit(-1);
  }
  //_dataSize=0;
  //_sampleCount=0;
  _monoral8=(Monoral8*)NULL;
  _monoral16=(Monoral16*)NULL;
  _stereo8=(Stereo8*)NULL;
  _stereo16=(Stereo16*)NULL;
  return 0;
}

void WAV::createHeader(unsigned short channels, unsigned int samplerate, unsigned short bit){
  memcpy(_riffID, "RIFF", 4);
  memcpy(_wavID, "WAVE", 4);
  memcpy(_fmtID, "fmt ", 4);
  _fmtSize=16;
  _format=1;
  _channels=channels;
  _sampleRate=samplerate;
  _bit=bit;
  _bytePerSec=samplerate*channels*(bit/8);
  _blockSize=(_bit/8)*channels;
  memcpy(_dataID, "data", 4);
  //size
  //dataSize
  //WAVData
}

int WAV::Write(const char* WAVFName){
  std::ofstream ofs;
  ofs.open(WAVFName);
  ofs.write((const char*)_riffID, 4);
  ofs.write((const char*)&_size, 4);
  ofs.write((const char*)_wavID, 4);
  ofs.write((const char*)_fmtID, 4);
  ofs.write((const char*)&_fmtSize, 4);
  ofs.write((const char*)&_format, 2);
  ofs.write((const char*)&_channels, 2);
  ofs.write((const char*)&_sampleRate, 4);
  ofs.write((const char*)&_bytePerSec, 4);
  ofs.write((const char*)&_blockSize, 2);
  ofs.write((const char*)&_bit, 2);
  ofs.write((const char*)_dataID, 4);
  ofs.write((const char*)&_dataSize, 4);
  if(_channels==1){
    if(_bit==8){
      ofs.write((const char*)_monoral8, _dataSize);
    }else if(_bit==16){
      ofs.write((const char*)_monoral16, _dataSize);
    }else{
      std::cout << "error: bytePerSec 8 16 => " << _bit << std::endl;
      return -1;
    }
  }else if(_channels==2){
    if(_bit==8){
      ofs.write((const char*)_stereo8, _dataSize);
    }else if(_bit==16){
      ofs.write((const char*)_stereo16, _dataSize);
    }else{
      std::cout << "error: bytePerSec 8 16 => " << _bit << std::endl;
      return -1;
    }
  }else{
    std::cout << "error: channel 1 or 2 => " << _channels  << std::endl;
    return -1;
  }
  
  ofs.close();
  return 0;
}


int WAV::Read(const char* WAVFName){
  std::ifstream ifs(WAVFName, std::ios::binary);
  //WAVHeader wav;
  //unsigned char header_buf[44];
  if(!ifs){
    std::cerr << "[ERROR] can't open " << WAVFName << std::endl;
    return -1;
  }
  
  //ifs.read((char*)header_buf, 44);

  //ファイルがRIFF形式であるか;  
  ifs.read(_riffID,4);
  if(ifs.bad() || strncmp(_riffID,"RIFF",4)!=0) return -1;
  ifs.read((char*)&_size,4); // fileSize
  
  
  //ファイルがWAVEファイルであるか
  ifs.read((char*)_wavID,4);
  if(ifs.bad() || strncmp(_wavID,"WAVE",4)!=0) return -1;
  
  //fmt のチェック
  ifs.read((char*)_fmtID,4);
  if(strncmp(_fmtID, "fmt ", 4)){
    std::cerr << "fmt not found" << std::endl;
    return -1;
  }
  // fmt チャンクのバイト数
  ifs.read((char*)&_fmtSize,4);
  if(_fmtSize != 16){
    std::cerr << "not LinearPCM" << std::endl;
    return -1;
  }
  //フォーマットIDから拡張部分までのヘッダ部分を取り込む
  // LinearPCMなので16byte分のデータ読み込む
  ifs.read((char*)&_format,2); //LinearPCMファイルならば1が入る
  if(_format!=1){
    std::cerr << "not LinearPCM" << std::endl;
    return -1;
  }
  ifs.read((char*)&_channels,2); 
  ifs.read((char*)&_sampleRate,4); 
  ifs.read((char*)&_bytePerSec,4); 
  ifs.read((char*)&_blockSize,2); 
  ifs.read((char*)&_bit,2);
  //--
  
   // "data"
  ifs.read((char*)_dataID,4);
  if(ifs.bad() || strncmp(_dataID,"data",4)!=0) return -1;
   // 波形データのバイト数
  ifs.read((char*)&_dataSize,4);
  
  //モノラルならサンプル数を、ステレオなら左右１サンプルずつの組の数
  _sampleCount=_dataSize / (_channels*(_bit/8));
  //_monoral8=NULL;
  //_monoral16=NULL;
  //_stereo8=NULL;
  //_stereo16=NULL;
  try{
    if(_channels==1){
      if(_bit==8){
	if((_monoral8=(Monoral8*)malloc(_dataSize)) == NULL){
	  return -1;
	}
	ifs.read((char*)_monoral8, _dataSize);
      }else if(_bit==16){
	if((_monoral16=(Monoral16*)malloc(_dataSize)) == NULL){
	  return -1;
	}
	//std::cout << "1block=" << sizeof(Monoral16) << std::endl;
	//std::cout << "samplecount=" << _sampleCount << std::endl;
	//std::cout << "dataSize=1block*sampleCount=" << sizeof(Monoral16)* _sampleCount << "=" <<_dataSize << std::endl;
	ifs.read((char*)_monoral16, _dataSize);
	
      }else{
	return -1;
      }
    }else   if(_channels==2){
      // ToDO LR insert data
      if(_bit==8){
	if((_stereo8=(Stereo8*)malloc(_dataSize)) == NULL){
	  return -1;
	}
	ifs.read((char*)_stereo8, _dataSize);
      }else if(_bit==16){
	if((_stereo16=(Stereo16*)malloc(_dataSize)) == NULL){
	    return -1;
	}
	ifs.read((char*)_stereo16, _dataSize);
      }else{
	return -1;
      }
    }else{
      return -1;
    }
  }catch(const char* e){
    printLOG("malloc error");
    printf("%s\n", e);
    exit(-1);
  }

  
  ifs.close();
  return -1;
}


int WAV::setData(const void *data, unsigned int dataSize){
  //freeWAVData(); // 初期化
  _dataSize=dataSize;
  _size=_dataSize+36;
  //モノラルならサンプル数を、ステレオなら左右１サンプルずつの組の数
  _sampleCount=_dataSize / (_channels*(_bit/8));
  _monoral8=(Monoral8*)NULL;
  _monoral16=(Monoral16*)NULL;
  _stereo8=(Stereo8*)NULL;
  _stereo16=(Stereo16*)NULL;
  try{
    if(_channels==1){
      if(_bit==8){
	_monoral8=(Monoral8*)malloc(dataSize);
	memcpy((void*)_monoral8, data, dataSize);
	//_monoral8=(Monoral8*)data;
      }else if(_bit==16){
	_monoral16=(Monoral16*)malloc(dataSize);
	memcpy((void*)_monoral16, data, dataSize);
	//_monoral16=(Monoral16*)data;
      }else{
	return -1;
      }
    }else   if(_channels==2){
      // ToDO LR insert data
      if(_bit==8){
	_stereo8=(Stereo8*)malloc(dataSize);
	memcpy((void*)_stereo8, data, dataSize);
	//_stereo8=(Stereo8*)data;
	
      }else if(_bit==16){
	_stereo16=(Stereo16*)malloc(dataSize);
	memcpy((void*)_stereo16, data, dataSize);
	//_stereo16=(Stereo16*)data;
      }else{
	return -1;
      }
    }else{
      return -1;
    }
  }catch(const char* e){
    std::cout << "malloc error" << std::endl;
    exit(-1);
  }

  return 0;

}

const void* WAV::getData()const{
  if(_channels==1){
    if(_bit==8){
      return (void*)_monoral8;
    }else if(_bit==16){
      return (void*)_monoral16;
      
    }else{
      return (void*)NULL;
    }
  }else   if(_channels==2){
    if(_bit==8){
      return (void*)_stereo8;
      
    }else if(_bit==16){
      return (void*)_stereo16;
    }else{
      return (void*)NULL;
    }
  }else{
    return (void*)NULL;
  }
  
} 

void WAV::dumpHeader(){
  printf("riffID\t%c%c%c%c\n",_riffID[0],_riffID[1],_riffID[2],_riffID[3]);
  std::cout << "size\t" << _size   << std::endl;
  printf("wavID\t%c%c%c%c\n",_wavID[0],_wavID[1],_wavID[2],_wavID[3]);
  printf("fmtID\t%c%c%c%c\n",_fmtID[0],_fmtID[1],_fmtID[2],_fmtID[3]);
  std::cout << "fmtSize\t" << _fmtSize << std::endl;
  std::cout << "format\t" << _format << std::endl;
  std::cout << "channels\t" << _channels << std::endl;
  std::cout << "sampleRate\t" << _sampleRate << std::endl;
  std::cout << "bytePerSec\t" << _bytePerSec << std::endl;
  std::cout << "blockSize\t" << _blockSize << std::endl;
  std::cout << "bitID\t" << _bit << std::endl;
  printf("dataID\t%c%c%c%c\n",_dataID[0],_dataID[1],_dataID[2],_dataID[3]);
  std::cout << "dataSize\t" << _dataSize << std::endl;

}
