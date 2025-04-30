import os
import random
import nltk

print("Start downloading dictionary.")
nltk.download('words', quiet=True)
print("Download completed.")

from nltk.corpus import words

def get_filtered_words(min_length=4, max_length=8, custom_list=None):
    if custom_list:
        word_pool = custom_list
    else:
        word_pool = words.words()
    return [w for w in word_pool if min_length <= len(w) <= max_length and w.isalpha()]

def create_text_file(filename, word_pool, total_words, words_per_line=20):
    with open(filename, 'w') as f:
        for i in range(0, total_words, words_per_line):
            line_words = random.choices(word_pool, k=min(words_per_line, total_words - i))
            f.write(' '.join(_.lower() for _ in line_words) + '\n')
            
        print(f'file {filename} created.')

def generate_files(num_files, words_per_file, min_word_len=4, max_word_len=8, custom_list=None, output_dir='generated_files'):
    os.makedirs(output_dir, exist_ok=True)
    word_pool = get_filtered_words(min_word_len, max_word_len, custom_list)
    
    for i in range(num_files):
        filename = os.path.join(output_dir, f'file_{i+1}.txt')
        create_text_file(filename, word_pool, words_per_file)
    print(f'{num_files} files created in "{output_dir}/"')

# Example usage
generate_files(
    num_files=200,
    words_per_file=1_000_000,
    min_word_len=4,
    max_word_len=8,
    # custom_list=["sun", "moon", "star", "light", "dark"]  # Optional: use your own words
)
