WAVUtil
=======

Simple WAV Utility Tool

- only linear PCM
- channels(1 or 2), bit(8 or 16)
- combine, trim, split

## build tool
```
$ make
```

## WAV header print and read,write test
```
$ ./bin/wav_test data/test.wav
riffID	RIFF
size	1051116
wavID	WAVE
fmtID	fmt 
fmtSize	16
format	1
channels	1
sampleRate	16000
bytePerSec	32000
blockSize	2
bitID	16
dataID	data
dataSize	1051080
```


## WAV combine
```
$ ./bin/wav_combine data/test.wav data/test.wav data/test.combined.wav
```

## WAV trimming
```
$ ./bin/wav_trim data/test.wav data/test.trim.wav 1.000 5.014
```

## WAV split
```
$ ./bin/wav_split data/test.wav data/split 10.000
split!
trim 0.00000 - 10.00000 [sec] / 32.84625 [sec]
trim 10.00000 - 20.00000 [sec] / 32.84625 [sec]
trim 20.00000 - 30.00000 [sec] / 32.84625 [sec]
trim 30.00000 - 32.84625 [sec] / 32.84625 [sec]
```
