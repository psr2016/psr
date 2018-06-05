# import libraries
from profeta.lib  import *
from profeta.main import *

class student(Belief):
    pass

class graduated(Belief):
    pass

# instantiate the engine
PROFETA.start()

+graduated("X") / student("X") >> [ -student("X"), show_line("yeah! Student ", "X", " is now graduated!") ]
+graduated("X")  >> [ show_line("X", " is not a student"), -graduated("X") ]

# run the engine shell
PROFETA.run_shell(globals())
