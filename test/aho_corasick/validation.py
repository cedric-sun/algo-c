import ahocorasick

def main():
    A=ahocorasick.Automaton()
    for line in open('patterns.txt').readlines():
        stripped_line = line.strip()
        A.add_word(stripped_line,stripped_line)
    A.make_automaton()
    for end,p in A.iter(open('text.txt').read().strip()):
        print(end-len(p)+1)

if __name__=='__main__':
    main()
