import bisect


# basic PQ
class PriorityQueue:
    def __init__(self, contents: list = []):  # constructor can take a list of items as default contents
        self.contents = contents

    def push(self, item):  # this is not optimal but it is practical compared to alternatives
        bisect.insort(self.contents,
                      item)  # the items we store supposedly have __lt__ defined in such a way they are compatible with bisect.insort

    def pop(self):
        return self.contents.pop(0)

    def qsize(self):
        return len(self.contents)
