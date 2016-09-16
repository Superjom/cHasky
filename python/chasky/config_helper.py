import sys

import functions
from graph_def_builder import GraphDefBuilder
from argument_def_builder import ArgumentDefBuilder


def CreateGraph(name):
    GraphDefBuilder.Instance(name)


def Field(name, type, shape, doc=''):
    return ArgumentDefBuilder().Name(name).Type(type).Shape(
        *shape).Doc(doc).Finalize()


def DataProvider(name, *fields):
    '''
    provider data for model
    '''
    defi = GraphDefBuilder.Instance().Finalize().data_provider
    defi.name = name
    for field in fields:
        fi = defi.outputs.add()
        fi.name = field.name
        fi.type = field.type
        fi.shape.width = field.shape.width
        fi.shape.height = field.shape.height
        fi.doc = field.doc


def Node(name, func):
    '''
    func: <functions>
    '''
    func_name, attrs = func
    node_def = GraphDefBuilder.Instance().Node()\
        .Name(name) \
        .Func(func_name)

    for key, attr_value in attrs.items():
        node_def.Attr(key, attr_value)


def Edge(src_node, src_arg, trg_node, trg_arg):
    '''
    src_node: str
        edge's source node
    src_arg: str
        edge's source argument
    trg_node:
        target's source node
    trg_arg:
        target's target argument
    '''
    GraphDefBuilder.Instance().Edge() \
        .Source(src_node, src_arg) \
        .Target(trg_node, trg_arg) \
        .Finalize()


def GraphDebugString(graph=None):
    if not graph:
        graph = GraphDefBuilder.Instance()
    return graph.Finalize()


def GraphStringBuffer(graph=None):
    if not graph:
        graph = GraphDefBuilder.Instance()
    return graph.Finalize().SerializeToString()


if __name__ == '__main__':
    CreateGraph("graph1")
    Node("node1", "null_func")
    Node("node2", "null_func")
    Node("node3", "null_func")
    Edge("node1", "output", "node2", "input")

    print GraphDebugString()
