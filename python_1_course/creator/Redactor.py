import sys
import argparse


# check that char c is punctuation mark
def is_punctuation_mark(c):
    return c in {'.', ',', '?', '!', '-', ':', "'"}


# split text lines to tokens (word with next punctuation mark)
def split_to_tokens(lines):
    tokens = []
    for line in lines:
        if line == "\n":
            # We want to have no more than one "\n"
            if tokens[-1] != ["\n"]:
                tokens.append(["\n"])
            continue
        new_word = True
        for c in line:
            # last "\n" in line we don't need to use
            if c == "\n":
                continue
            if is_punctuation_mark(c):
                new_word = True
                tokens[-1].append(c)
            # we don't use " ", but new word (token) is starting
            elif c == ' ':
                new_word = True
            else:
                if new_word:
                    tokens.append([])
                new_word = False
                tokens[-1].append(c)
    # transform lists with chars to strings
    # and delete last and begin "\n"
    for ind in range(len(tokens)):
        tokens[ind] = ''.join(tokens[ind])
    if tokens[0] == "\n":
        tokens.pop(0)
    if tokens[-1] == "\n":
        tokens.pop()
    return tokens


# create text lines from tokens with line_length and paragraph_spaces
def form_text_from_tokens(tokens, line_length, paragraph_spaces):
    new_paragraph = True
    new_line = True
    current_len = 0
    text_lines = []
    # I want that in end of file we have 1 empty string so "\n"
    tokens.append("\n")
    for token in tokens:
        # "\n" means that begin new paragraph
        if token == "\n":
            text_lines[-1].append("\n")
            new_paragraph = True
            new_line = True
            continue
        d = len(token)
        # create spaces and line for new paragraph
        if new_paragraph:
            text_lines.append([])
            text_lines[-1].append(' ' * paragraph_spaces)
            current_len = paragraph_spaces
            new_paragraph = False
        if not new_line:
            if current_len + d + 1 <= line_length:
                text_lines[-1].append(' ')
                text_lines[-1].append(token)
                current_len += d + 1
            else:
                # go to new line because word doesn't fit
                text_lines[-1].append("\n")
                text_lines.append([])
                new_line = True
                current_len = 0
        if new_line:
            if current_len + d <= line_length:
                text_lines[-1].append(token)
                current_len += d
                new_line = False
            else:
                sys.exit(1)
    for ind in range(len(text_lines)):
        text_lines[ind] = ''.join(text_lines[ind])
    return text_lines


def read_from_stdin():
    lines = sys.stdin.readlines()
    return split_to_tokens(lines)


def read_from_file(filename):
    with open(filename) as f:
        lines = f.readlines()
        return split_to_tokens(lines)


def write_to_stdout(lines):
    for line in lines:
        print(line, end='')


def write_to_file(filename, lines):
    with open(filename, 'w') as f:
        for line in lines:
            f.write(line)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input',
                        default=None, help="input file name")
    parser.add_argument('-o', '--output',
                        default=None, help="output file name")
    parser.add_argument('-l', '--line-length', type=int,
                        help="max length of line")
    parser.add_argument('-p', '--paragraph-spaces', type=int,
                        help="spaces for new paragraph")
    args = parser.parse_args()

    if args.input is None:
        input_lines = read_from_stdin()
    else:
        input_lines = read_from_file(args.input)

    input_tokens = split_to_tokens(input_lines)
    output_lines = form_text_from_tokens(
        input_tokens, args.line_length, args.paragraph_spaces)

    if args.output is None:
        write_to_stdout(output_lines)
    else:
        write_to_file(args.output, output_lines)

if __name__ == "__main__":
    main()
