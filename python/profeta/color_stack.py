# import libraries
from profeta.lib  import *
from profeta.main import *


class find_top(Goal):
    pass

class upon(Belief):
    pass

# instantiate the engine
PROFETA.start()

find_top("Color") / upon("X", "Color") >> [ find_top("X") ]
find_top("Color")  >> [ show_line("la cima della torre e' un blocco di colore", "Color") ]

PROFETA.assert_belief(upon("verde","rosso"))
PROFETA.assert_belief(upon("rosso","bianco"))
PROFETA.assert_belief(upon("blu","giallo"))

# run the engine shell
PROFETA.run_shell(globals())


