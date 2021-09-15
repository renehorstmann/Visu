import os
import ctypes

from pbase import *

if os.name == 'posix':
    vulib = ctypes.cdll.LoadLibrary('/usr/local/lib/libvisu.so')
elif os.name == 'nt':
    vulib = ctypes.cdll.LoadLibrary('C:/Libraries/lib/libvisu.dll')
else:
    print('unknown os?:', os.name)
    exit(1)

# after loading lib!
from . import control
from . import visubase
from . import object
from . import gtk

from .svisu import *

# pure python
