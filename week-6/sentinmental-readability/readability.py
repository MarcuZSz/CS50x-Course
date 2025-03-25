import re

def count_letters(text):
    return len(re.findall(r'[a-zA-Z]', text))

def count_words(text):
    return len(text.split())

def count_sentences(text):
    return len(re.findall(r'[.!?]', text))

def readability(text):
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    
    L = (letters / words) * 100
    S = (sentences / words) * 100
    
    index = 0.0588 * L - 0.296 * S - 15.8
    return round(index)

def main():
    text = input("Text: ")
    index = readability(text)
    
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")

if __name__ == "__main__":
    main()