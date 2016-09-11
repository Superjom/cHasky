from core.framework.argument_pb2 import ArgumentDef


class ArgumentDefBuilder(object):
    def __init__(self):
        self.def_ = ArgumentDef()

    def Name(self, x):
        self.def_.name = x
        return self

    def Type(self, x):
        self.def_.name = x
        return self

    def Shape(self, width, height):
        self.def_.shape.width = width
        self.def_.shape.height = height
        return self

    def Doc(self, x):
        self.def_.doc = x
        return self

    def Finalize(self):
        return self.def_
