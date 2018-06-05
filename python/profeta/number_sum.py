# import libraries
from profeta.lib  import *
from profeta.main import *

class sum_all(Goal):
    pass

class number(Belief):
    pass

# instantiate the engine
PROFETA.start()

sum_all("N") / number("X") >> [ "N = N + X", -number("X"), sum_all("N") ]
sum_all("N") >> [ show_line("the sum is", "N") ]
sum_all() >> [ sum_all(0) ]

# run the engine shell
PROFETA.run_shell(globals())
