rm work/*.mp3
py .\Python_TTS\speakToFile.py inputMP3.txt -o .\work
.\bin\MergeFiles.exe .\work