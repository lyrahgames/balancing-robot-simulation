project = pid-balance-simulation

cxx.std = latest
using cxx

hxx{*}: extension = hpp
cxx{*}: extension = cpp

import libs = sfml-graphics%lib{sfml-graphics}

exe{pid}: {hxx cxx}{**} $libs