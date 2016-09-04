import logging
import unittest
import chasky.chasky as chasky


class VersionTest(unittest.TestCase):
    def test_version(self):
        version = chasky.GetVersion()
        logging.info("version: %s" % chasky.GetVersion())
        self.assertTrue(version)


if __name__ == '__main__':
    unittest.main()
