import bisect


# PriorityQueue class that allows for a sort function (key function) to be chosen. Can't guarantee efficiency as it relies on a List, but it does the trick.
class PriorityQueue:
    def __init__(self, contents: list = []):  # constructor can take a list of items as default contents
        self.contents = contents

    def push(self, item):  # this is not optimal but it is practical compared to alternatives
        bisect.insort(self.contents,
                      item)  # the items we store supposedly have __lt__ defined in such a way they are compatible with bisect.insort

    def pop(self, item):
        return self.contents.pop(0)

    def qsize(self):
        return len(self.contents)
