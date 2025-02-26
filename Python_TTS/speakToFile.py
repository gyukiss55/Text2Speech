from gtts import gTTS
import os
import argparse
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
    
def mp3Write(workFolderName, index, lang, text):
    fileName = f"{workFolderName}\\w_{index}_{lang}.mp3"
    speakToFile (text,fileName,lang)
    
def generateMP3Files (textFileName, workFolderName) : 
    print (f"textFileName:{textFileName}")
    print (f"workFolderName:{workFolderName}")
    # mp3Write(0,'hu',"veszteség")
    with open(textFileName, 'r', encoding='utf-8') as file:
        content = file.read()
        lines = content.splitlines()
        cnt = 0
        
        for line in lines:
            cnt = cnt + 1
            lang = line[0:2]
            txt = line[4:100]
            fn = f"{workFolderName}\\w_{cnt}_{lang}.mp3"
            
            print(f"fn:{fn}")
            print(f"text:\"{txt}\"")
            print(f"lang:\"{lang}\"")
            #speakToFile(txt,fn,lang)
            
            # decode_string =  txt.decode('utf-8');
            
            mp3Write(workFolderName, cnt, lang, txt)
            
            
def main():
    
    # Create the parser
    parser = argparse.ArgumentParser(
        description="A script that demonstrates command-line argument handling using argparse."
    )
    
    # Add positional arguments
    parser.add_argument(
        "input", 
        help="The input file or string"
    )
    
    # Add optional arguments (flags)
    parser.add_argument(
        "-o", "--output", 
        help="The output file", 
        default="default_output.txt"
    )
    
    # Add a flag (boolean option)
    parser.add_argument(
        "-v", "--verbose", 
        help="Increase output verbosity", 
        action="store_true"
    )
    
    # Parse the arguments
    args = parser.parse_args()

    # Access the arguments
    if args.verbose:
        print("Verbose mode is enabled.")
    print(f"Input: {args.input}")
    print(f"Output: {args.output}")
    generateMP3Files (args.input, args.output)
  

if __name__ == '__main__':
    main()