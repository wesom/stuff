# Definition for singly-linked list.
class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None


class Solution:
    def partition(self, head, x):
        """
        :type head: ListNode
        :type x: int
        :rtype: ListNode
        """
        pl = left = ListNode(None)
        pr = right = ListNode(None)
        p = head
        while p:
            if p.val < x:
                pl.next = p
                pl = pl.next
            else:
                pr.next = p
                pr = pr.next
            p = p.next
        pl.next, pr.next = right.next, None
        return left.next

if __name__ == '__main__':
    def display(n):
        p = n
        while p:
            print(p.val)
            p = p.next
    n1 = ListNode(1)
    n2 = ListNode(4)
    n3 = ListNode(3)
    n4 = ListNode(2)
    n5 = ListNode(5)
    n6 = ListNode(2)
    n1.next = n2
    n2.next = n3
    n3.next = n4
    n4.next = n5
    n5.next = n6
    display(n1)
    s = Solution()
    ret = s.partition(n1, 3)
    print("after:")
    display(ret)