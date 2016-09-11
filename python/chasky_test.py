#!/usr/bin/python
import logging
import unittest
from chasky import functions
import chasky.chasky_engine as engine
from chasky.config_helper import Node, Edge, \
    CreateGraph, GraphStringBuffer, GraphDebugString


class VersionTest(unittest.TestCase):
    def test_version(self):
        version = engine.GetVersion()
        logging.info("version: %s" % engine.GetVersion())
        self.assertTrue(version)


class SessionTest(unittest.TestCase):
    def test_init(self):
        session = engine.Session()
        self.assertTrue(session)

    def test_create_graph(self):
        CreateGraph("graph1")
        Node("node1", functions.NullFunc)
        Node("node2", functions.NullFunc)
        Node("node3", functions.NullFunc)
        Edge("node1", "output", "node2", "input")

        debug_str = GraphDebugString()
        print 'debug_str', debug_str

        str_buf = GraphStringBuffer()

        session = engine.Session()
        self.assertTrue(session.CreateGraph(str_buf))


class FunctionLibTest(unittest.TestCase):
    def test_debug_string(self):
        print engine.FunctionLibDebugString()


if __name__ == '__main__':
    unittest.main()
