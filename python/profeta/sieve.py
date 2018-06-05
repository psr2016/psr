# import libraries
from profeta.lib  import *
from profeta.main import *

class number(Belief):
    pass

# instantiate the engine
PROFETA.start()

+number("X") / ( number("Y") & (lambda: X != Y) & (lambda: (X % Y) == 0) ) >> [ -number("X") ]

# populate the KB
for i in range(2,100):
    PROFETA.assert_belief(number(i))

# run the engine shell
PROFETA.run_shell(globals())
