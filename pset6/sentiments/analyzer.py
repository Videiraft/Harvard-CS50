import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = []
        with open(positives) as lines:
            for line in lines:
                if not line.startswith(";"):
                    self.positives.append(line.strip("\n"))
        
        self.negatives = []
        with open(negatives) as lines:
            for line in lines:
                if not line.startswith(";"):
                    self.negatives.append(line.strip("\n"))
                    

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        self.tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = self.tokenizer.tokenize(text)
        
        score = 0
        for word in tokens:
            for line in self.positives:
                if word.lower() == line:
                    score +=1
            for line in self.negatives:
                if word.lower() == line:
                    score -= 1
        return score
