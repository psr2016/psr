# import libraries
from profeta.lib  import *
from profeta.main import *

class number(Belief):
    pass

# instantiate the engine
PROFETA.start()

# define a belief plan
+number("X") >> [ show_line("yeah! I know the number", "X") ]

# run the engine shell
PROFETA.run_shell(globals())
