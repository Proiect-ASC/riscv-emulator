import functools


# very basic binary tree support class
@functools.total_ordering
class Node:
    def __init__(self, left=None, right=None, value=None):
        self.left = left
        self.right = right
        self.value = value

    def __lt__(self, other):
        return self.value < other.value

    def __str__(self):
        return f"{self.value}"

    def isleaf(self):
        return self.left is None and self.right is None
