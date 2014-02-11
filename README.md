WAVUtil
=======

Simple WAV Utility Tool
mono, stereo, Bit(8,16), combine, trim, split

## build tool
```
$ make
```

## WAV header print and read,write test
```
$ ./bin/wav_test data/test.wav
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
$ ./bin/wav_split data/test.wav data/split 3.000
```
