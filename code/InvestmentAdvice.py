class InvestmentAdvice:
  def getInvestments(self, advice, recent, money, timeLeft, roundsLeft):
    ret = []
    for i in range(len(advice)):
      ret.append(100 if advice[i] > 0 else 0)
    return ret

# -------8<------- end of solution submitted to the website -------8<-------

import sys
sol = InvestmentAdvice()
roundsLeft = 99
while roundsLeft > 1:
  A = int(raw_input())
  advice = []
  for i in range(A):
    advice.append(int(raw_input()))
  R = int(raw_input())
  recent = []
  for i in range(R):
    recent.append(int(raw_input()))
  money = int(raw_input())
  timeLeft = int(raw_input())
  roundsLeft = int(raw_input())
  ret = sol.getInvestments(advice, recent, money, timeLeft, roundsLeft)
  print len(ret)
  for x in ret:
    print x
  sys.stdout.flush()
