# import libraries
from profeta.lib  import *
from profeta.main import *

class obj(Belief):
    pass

class upon(Belief):
    pass

class owned(Belief):
    pass

class pick(Goal):
    pass

class put(Goal):
    pass

# instantiate the engine
PROFETA.start()

pick("X") / (obj("X") & upon("Y", "X")) >> \
 [ show_line("cannot pick", "X", "since it is under the","Y") ]

pick("X") / (obj("X") & upon("X", "Y")) >> \
  [ show_line("X", "picked"),
    -obj("X"), -upon("X", "Y"), +owned("X") ]

pick("X") / obj("X") >> [ show_line("X", "picked"),
                          -obj("X"), +owned("X") ]

pick("X") / owned("X") >> [ show("you've still got", "X") ]

pick("X") >> [ show_line("cannot pick", "X", "since it is not present") ]

put("X") / owned("X") >> [ show_line("X", "is now on the table"), -owned("X"), +obj("X") ]
put("X") / obj("X") >> [ show_line("X", "is already on the table") ]
put("X") >> [ show_line("X", "does not exist") ]

put("X", "Y") / (owned("X") & obj("Y") & upon("Z", "Y") ) >> [ show_line("Y", "has", "Z", "on its top") ]
put("X", "Y") / (owned("X") & obj("Y")) >> \
    [ -owned("X"), +obj("X"), +upon("X", "Y"),
      show_line("done") ]


PROFETA.assert_belief(obj("cube"))
PROFETA.assert_belief(obj("cylinder"))
PROFETA.assert_belief(obj("prism"))

PROFETA.assert_belief(upon("cube", "cylinder"))
PROFETA.assert_belief(upon("prism", "cube"))

# run the engine shell
PROFETA.run_shell(globals())

