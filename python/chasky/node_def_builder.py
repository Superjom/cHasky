import sys
#from graph_def_builder import GraphDefBuilder
from core.framework.attr_value_pb2 import AttrValue
from core.framework.graph_pb2 import NodeDef
from attr_value_builder import AttrValueBuilder

class NodeDefBuilder(object):
    def __init__(self, node=None):
        self.node_def_ = node if node else NodeDef() 

    def Name(self, name):
        '''
        @name: str
        '''
        self.node_def_.name = name
        return self

    def SetIsSource(self, is_source):
        '''
        @is_source: bool
        '''
        self.node_def_.is_source = is_source
        return self

    def SetIsEnd(self, is_end):
        '''
        @is_end: bool
        '''
        self.node_def_.is_end = is_end
        return self

    def Func(self, x):
        '''
        '''
        # TODO change signature to function
        self.node_def_.signature = x
        return self

    def Attr(self, key, value, is_dtype=False):
        '''
        Add an attribute
        @key: str
        @attr_value: AttrValue
            value of attribute
        @is_dtype: whether is dtype
            distinguish between real int and DataType
        '''
        attr = self.node_def_.attr[key]
        print 'attr value', value
        attr_builder = AttrValueBuilder(attr).Value(value, is_dtype)
        return self

    def Input(self, src_node_name, out_arg_name):
        '''
        Add an input
        '''
        signature = "{src_node_name}:{arg}".format(
            src_node_name = src_node_name,
            arg = out_arg_name,)
        self.node_def_.input.append(signature)
        return self

    def Finalize(self):
        #TODO add checks
        #GraphDefBuilder.Instance().Node(self.node_def_)
        return self.node_def_

if __name__ == '__main__':
    node_def = NodeDefBuilder() \
               .Name("node1") \
               .SetIsSource(False) \
               .Func("AddN") \
               .Input("src_node1", "activation") \
               .Input("src_node2", "activation") \
               .Finalize()
    print node_def
