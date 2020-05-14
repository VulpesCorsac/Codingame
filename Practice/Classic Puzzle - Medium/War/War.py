class Game():
    cards = {'2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9, '10': 10, 'J': 11, 'Q': 12, 'K': 13, 'A': 14}
    deck  = [[] for _ in range(2)]
    table = [[] for _ in range(2)]
    winner = "PAT"
    turn = 0
    war = False

    def __init__(self):
        for player in range(2):
            for _ in range(int(input())):
                self.deck[player].append(input())

    def printResult(self):
        if self.winner == "PAT":
            print("PAT")
        else:
            print("{0} {1}".format(self.winner, self.turn))

    def putCardsOnTable(self, n):
        for player in range(2):
            for _ in range(n):
                self.table[player].append(self.deck[player].pop(0))

    def appendCards(self, win_player):
        for player in range(2):
            for card in self.table[player]:
                self.deck[win_player].append(card)
            self.table[player] = []

    def checkCards(self):
        player1_card_value = self.cards[self.table[0][-1][:-1]]
        player2_card_value = self.cards[self.table[1][-1][:-1]]
        if player1_card_value > player2_card_value:
            self.appendCards(0)
            self.war = False
        elif player1_card_value < player2_card_value:
            self.appendCards(1)
            self.war = False
        else:
            self.war = True

    def haveEnoughCards(self, n):
        len1, len2 = len(self.deck[0]), len(self.deck[1])
        if len1 >= n and len2 >= n:
            return True
        else:
            if self.war:
                self.winner = "PAT"
            elif len1 - n < 0 and len2 - n < 0:
                self.winner = "PAT"
            elif (len1 > len2):
                self.winner = "1"
            else:
                self.winner = "2"

            return False

    def nextTurn(self):
        if self.war:
            if self.haveEnoughCards(3):
                self.putCardsOnTable(3)
            else:
                return False
        if self.haveEnoughCards(1):
            if not (self.war):
                self.turn += 1
            self.putCardsOnTable(1)
            self.checkCards()
        else:
            return False
        return True

    def run(self):
        while self.nextTurn():
            pass


game = Game()
game.run()
game.printResult()
