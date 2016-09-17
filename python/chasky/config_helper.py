import sys
from ctypes import c_float

import functions
from graph_def_builder import GraphDefBuilder
from argument_def_builder import ArgumentDefBuilder
from chasky_engine import StringVector

g_batch_size = None


def CreateGraph(name):
    GraphDefBuilder.Instance(name)


def DestroyGraph():
    GraphDefBuilder.Destroy()


def Field(name, type, dim, doc=''):
    return dict(name=name, type=type, dim=dim, doc=doc)


class DataProvider(object):
    def __init__(self, name, path, batch_size, *fields):
        '''
        provider data for model

        @name: name of the data provider
        @path: the file which contains data
        @batch_size: size of a batch

        fields in the data file is split by ';', for example, the following
        record has two fields:

            0 1 0;1 2 3

        the first field is '0 1 0', and the second field is '1 2 3'
        '''
        global g_batch_size
        self.path = path
        self.batch_size = batch_size
        g_batch_size = batch_size
        # register definition to graph's definition
        defi = GraphDefBuilder.Instance().Finalize().data_provider
        defi.name = name
        self.fields = fields
        for field in fields:
            fi = defi.outputs.add()
            fi.name = field['name']
            fi.type = field['type']
            fi.shape.width = field['dim']
            fi.shape.height = self.batch_size
            fi.doc = field['doc']

        self.batch_records = []

    def batchs(self):
        with open(self.path) as f:
            for line in f:
                fs = line.strip().split(';')
                assert len(fs) == len(self.fields)
                self.batch_records.append(fs)
                print self.batch_size, len(self.batch_records)
                if len(self.batch_records) == self.batch_size:
                    yield self._release_batch()
            last_rcd = self._release_batch()
            if last_rcd:
                yield last_rcd

    def _release_batch(self):
        if not self.batch_records:
            return
        c_records = StringVector(len(self.fields))
        field_id = 0
        for i, field_def in enumerate(self.fields):
            c_batch = (c_float *
                       (field_def['dim'] * len(self.batch_records)))()
            dim = field_def['dim']
            for no, fs in enumerate(self.batch_records):
                field = fs[i]
                digits = map(float, field.split())
                for j in range(dim):
                    c_batch[j + no * dim] = digits[j]
            # c_records.append(str(buffer(c_batch)))
            c_records[field_id] = str(buffer(c_batch))
            field_id += 1
        self.batch_records = []
        return c_records


def Node(name, func, is_end=False):
    '''
    func: <functions>
    is_end: the end point of the graph
    '''
    func_name, attrs = func
    node_def = GraphDefBuilder.Instance().Node()\
        .Name(name) \
        .Func(func_name) \
        .SetIsEnd(is_end)

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

    data_provider = DataProvider("data provider",
                                 "1.txt",
                                 4,
                                 Field(name="fea1",
                                       type="float_mat",
                                       dim=5),
                                 Field(name="fea1",
                                       type="float_mat",
                                       dim=4), )

    for batch in data_provider.batchs():
        print len(batch), batch, buffer(batch[0]), len(buffer(batch[0]))
