# import libraries
from profeta.lib  import *
from profeta.main import *

class say_hello(Goal):
    pass

# instantiate the engine
PROFETA.start()

# define a goal plan
say_hello() >> [ show_line("hello world from PROFETA") ]

# run the engine shell
PROFETA.run_shell(globals())
