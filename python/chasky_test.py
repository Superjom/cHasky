import logging
import unittest
import chasky.chasky as chasky


class VersionTest(unittest.TestCase):
    def test_version(self):
        version = chasky.GetVersion()
        logging.info("version: %s" % chasky.GetVersion())
        self.assertTrue(version)


class SessionTest(unittest.TestCase):
    def test_init(self):
        session = chasky.Session()
        self.assertTrue(session)

class FunctionLibTest(unittest.TestCase):
    def test_debug_string(self):
        print chasky.FunctionLibDebugString()


if __name__ == '__main__':
    unittest.main()
