# import libraries
from profeta.lib  import *
from profeta.main import *
import random

class restore_numbers(Goal):
    pass

class compute_max(Goal):
    pass

class number(Belief):
    pass

# instantiate the engine
PROFETA.start()

restore_numbers() / number("processed", "X") >> [ -number("processed","X"),
							+number("X"), restore_numbers() ]


compute_max("Max") / (number("X") & (lambda : X > Max) ) >>[ -number("X"), 
								+number("processed","X"),
								compute_max("X")]
compute_max("Max") / number("X") >> [ -number("X"), +number("processed","X"), 
					compute_max("Max") ]
compute_max("Max") >> [ show_line("il massimo e' = ", "Max"), restore_numbers() ]

compute_max() / number("X") >> [ -number("X"), +number("processed","X"), compute_max("X") ]

for i in range(0,20):
	x = int(random.uniform(1, 100))
	PROFETA.assert_belief(number(x))

# run the engine shell
PROFETA.run_shell(globals())

