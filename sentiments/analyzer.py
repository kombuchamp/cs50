
import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # Initialize lists for positive and negative words
        self.positives = []
        self.negatives = []
        
        # Open files and store lines in corresponding lists
        try:
            with open(positives, 'r') as f:
                for line in f:
                    self.store_line(line, self.positives)
            with open(negatives, 'r') as f:
                for line in f:
                    self.store_line(line, self.negatives)
        except IOError as e:
            print("Couldnt open file %s" % e)


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # Score of text sentiment
        score = 0
        
        target_text = text.lower()

        # Tokenizes the tekst into list of words (tokens)
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(target_text)

        # Checks if the word is in list of
        # positive or negative words
        for token in tokens:
            if token in self.positives:
                score+=1
            elif token in self.negatives:
                score-=1
        

        return score
    
    # Stores line of text in corresponding list
    def store_line(self, line, list):
        string = line.strip(" \n")
        if string.startswith(';') or string == "" or string == "\n":
            return
        list.append(string)
        return
        