class UnImplementedError(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return '<UnImplementedError [%s]>' % repr(self.value)
