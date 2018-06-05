# import libraries
from profeta.lib  import *
from profeta.main import *

class factorial(Goal):
    pass

# instantiate the engine
PROFETA.start()

factorial(0, "Acc") >> [ show_line("factorial is ", "Acc") ]
factorial("N", "Acc") >> [ "Acc = Acc * N", "N = N - 1", factorial("N", "Acc") ]

factorial("N") >> [ factorial("N", 1) ]

# run the engine shell
PROFETA.run_shell(globals())
