#!/usr/bin/python
import logging
import unittest
from chasky import functions
import chasky.chasky_engine as engine
from chasky.config_helper import Node, Edge, \
    CreateGraph, DestroyGraph, GraphStringBuffer, GraphDebugString, DataProvider, \
    Field


class VersionTest(unittest.TestCase):
    def test_version(self):
        version = engine.GetVersion()
        logging.info("version: %s" % engine.GetVersion())
        self.assertTrue(version)


class SessionTest(unittest.TestCase):
    def setUp(self):
        CreateGraph("graph1")
        self.data_provider = DataProvider("data_provider",
                     './1.txt',
                     4,
                     Field("fea1", "float_mat",
                           dim=5, doc="first feature"),
                     Field("fea2", "float_mat",
                           dim=4, doc="another feature"))
        Node("node1", functions.IdenticalFunc(5, 4))
        Node("node2", functions.IdenticalFunc(5, 4))
        Node("node3", functions.IdenticalFunc(5, 4), is_end=True)
        Edge("data_provider", "fea1", "node1", "input")
        Edge("node1", "output", "node2", "input")
        Edge("node2", "output", "node3", "input")

        debug_str = GraphDebugString()
        print 'debug_str', debug_str

        str_buf = GraphStringBuffer()

        self.session = engine.Session()
        self.assertTrue(self.session.CreateGraph(str_buf))
        self.session.StartExec()

    def tearDown(self):
        DestroyGraph()
        self.session = None

    def test_init(self):
        session = engine.Session()
        self.assertTrue(session)

    def test_data_provider(self):
        '''
        test all the data flow
        '''
        for batch in self.data_provider.batchs():
            print 'batch:', batch
            self.session.Compute(batch)


class FunctionLibTest(unittest.TestCase):
    def test_debug_string(self):
        print engine.FunctionLibDebugString()


if __name__ == '__main__':
    unittest.main()
