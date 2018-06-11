# import libraries
from profeta.lib  import *
from profeta.main import *


class find_top(Goal):
    pass

class pick(Goal):
    pass

class disassemble(Goal):
    pass

class upon(Belief):
    pass

class owned(Belief):
    pass

class block(Belief):
    pass

# instantiate the engine
PROFETA.start()

# il blocco da prendere e' al top della torre attuale, ma ha un oggetto di sotto
disassemble("ColorToPick", "ColorToPick") / upon("ColorToPick", "X") >> \
	[ 
		-upon("ColorToPick","X"),
		-block("ColorToPick"),
		+owned("ColorToPick"),
		show_line("ho preso il blocco", "ColorToPick") 
	]
# il blocco da prendere e' al top della torre attuale, ma non ha oggetti di sotto
disassemble("ColorToPick", "ColorToPick") >> \
	[ 
		-block("ColorToPick"),
		+owned("ColorToPick"),
		show_line("ho preso il blocco", "ColorToPick")
	]
# il blocco da prendere non e' al top della torre attuale
disassemble("ColorToPick", "Top") / upon("Top", "X") >> \
	[ 
		-upon("Top","X"),
		show_line("ho rimosso il blocco", "Top"), 
		disassemble("ColorToPick", "X")
	]

find_top("ColorToPick", "Color") / upon("X", "Color") >> \
	[ 
		find_top("ColorToPick", "X")
	]
find_top("ColorToPick", "Color") >> \
	[ 
		show_line("la cima della torre e' un blocco di colore", "Color"),
		disassemble("ColorToPick", "Color") 
	]

pick("Color") / block("Color") >> [ find_top("Color", "Color") ]
pick("Color") >> [ show_line("il blocco", "Color", "non esiste") ]

PROFETA.assert_belief(block("verde"))
PROFETA.assert_belief(block("rosso"))
PROFETA.assert_belief(block("giallo"))
PROFETA.assert_belief(block("nero"))
PROFETA.assert_belief(block("blu"))
PROFETA.assert_belief(block("bianco"))

PROFETA.assert_belief(upon("verde","rosso"))
PROFETA.assert_belief(upon("rosso","bianco"))
PROFETA.assert_belief(upon("blu","giallo"))

# run the engine shell
PROFETA.run_shell(globals())


