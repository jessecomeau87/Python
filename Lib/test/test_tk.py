from test import support
# Skip test if _tkinter wasn't built.
support.import_module('_tkinter')

# Make sure tkinter._fix runs to set up the environment
support.import_fresh_module('tkinter')

# Skip test if tk cannot be initialized.
support.requires('gui')

from tkinter.test import runtktests

def test_main():
    support.run_unittest(
            *runtktests.get_tests(text=False, packages=['test_tkinter']))

if __name__ == '__main__':
    test_main()
