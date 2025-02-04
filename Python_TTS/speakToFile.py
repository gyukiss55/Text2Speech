from gtts import gTTS
import os
import playsound

def speakToFile(text, fn="en.mp3", lang='en'):
    # Convert the text to speech
    tts = gTTS(text=text, lang=lang)
    
    # Save the audio file
    filename = fn
    tts.save(filename)
    
    # Play the audio file
    # playsound.playsound(filename, block=True)
    
    # Clean up the temporary file
    # os.remove(filename)
    
def test1(index):
    sample_text = "Casuality"
    fileNameEng = f"Casuality_eng{index}.mp3"
    lang_eng = 'en'
    speakToFile(sample_text,fileNameEng,lang_eng)
    sample_text = "veszteség"
    fileNameHun = f"Casuality_hun{index}.mp3"
    lang_hun = 'hu'
    speakToFile(sample_text,fileNameHun,lang_hun)
    
def mp3Write(index, lang, text):
    fileName = f"inputMP3\\w_{index}_{lang}.mp3"
    speakToFile (text,fileName,lang)

if __name__ == "__main__":
    # mp3Write(0,'hu',"veszteség")
    with open('inputMP3.txt', 'r', encoding='utf-8') as file:
        content = file.read()
        lines = content.splitlines()
        cnt = 1
        
        for line in lines:
            lang = line[0:2]
            txt = line[4:100]
            fn = f"w_{cnt}_{lang}.mp3"
            
            print(f"fn:{fn}")
            print(f"text:\"{txt}\"")
            print(f"lang:\"{lang}\"")
            #speakToFile(txt,fn,lang)
            
            # decode_string =  txt.decode('utf-8');
            
            mp3Write(cnt, lang, txt)
            cnt = cnt + 1

