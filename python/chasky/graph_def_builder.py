import sys
sys.path.append('../../build/protos/')
from chasky.core.framework.graph_pb2 import GraphDef
from node_def_builder import NodeDefBuilder
from attr_value_builder import AttrValueBuilder

class GraphDefBuilder(object):
    # record current graph
    cur_graph = None

    @staticmethod
    def Instance(name=None):
        assert name or GraphDefBuilder.cur_graph
        GraphDefBuilder.cur_graph = GraphDefBuilder().Name(name) \
                                    if name else GraphDefBuilder.cur_graph
        return GraphDefBuilder.cur_graph

    def __init__(self):
        self.def_ = GraphDef()

    def Name(self, x):
        self.def_.name = x
        return self

    def Node(self):
        '''
        Add a node to graph
        @x: NodeDef
        '''
        x = self.def_.nodes.add()
        return NodeDefBuilder(x) 

    def Finalize(self):
        return self.def_


if __name__ == '__main__':
    graph = GraphDefBuilder() \
            .Name('graph1')

    graph.Node() \
               .Name("encoder") \
               .SetIsSource(True) \
               .Func("RNN") \
               .Input("input", "ids") \
               .Input("src_word", "embedding") \
               .Finalize()

    graph.Node() \
               .Name("attention") \
               .SetIsSource(False) \
               .Func("AddN") \
               .Input("encoder", "activation") \
               .Attr("size", 54) \
               .Attr("type", 0, is_dtype=True) \
               .Input("encoder", "activation2") \
               .Finalize()

    print graph.Finalize()
