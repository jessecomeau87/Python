import unittest
from test import support
from test.support import import_helper
import sys
import warnings

# Skip this test if the _tkinter module wasn't built.
_tkinter = import_helper.import_module('_tkinter')

# Skip test if tk cannot be initialized.
support.requires('gui')

tix = support.import_module('tkinter.tix', deprecated=True)
from tkinter import TclError


class TestTix(unittest.TestCase):

    def setUp(self):
        try:
            self.root = tix.Tk()
        except TclError:
            if sys.platform.startswith('win'):
                self.fail('Tix should always be available on Windows')
            self.skipTest('Tix not available')
        else:
            self.addCleanup(self.root.destroy)

    def test_tix_warning(self):
        support.import_fresh_module('tkinter.tix', fresh=('tkinter.tix',))
        # Refresh module to issue warning
        with self.assertWarns(DeprecationWarning):
            from tkinter import tix


if __name__ == '__main__':
    unittest.main()
