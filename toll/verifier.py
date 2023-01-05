import re
import string
import sys


class VerifierReader(object):
  def __init__(self, text):
    self.text = text
    self.position = 0

  def HasNext(self):
    return self.position < len(self.text)

  def Read(self, target):
    actual = self.text[self.position : self.position + len(target)]
    assert actual == target, "Could not read '{0}'".format(target)
    self.position += len(target)

  def ReadSpace(self):
    self.Read(' ')

  def ReadEndl(self):
    self.Read('\n')

  def ReadEof(self):
    assert self.position == len(self.text), 'Found junk at the end of the file'

  def ReadChar(self):
    assert self.position < len(self.text), 'Unexpected EOF'
    char = self.text[self.position]
    self.position += 1
    return char

  def ReadLine(self, valid_characters):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      self.position += 1
      if c == '\n':
        break
      assert c in valid_characters
    assert self.position - old_position > 1, 'Nothing before next newline'
    return self.text[old_position:self.position - 1]

  def ReadRegex(self, target_regex):
    match = re.compile(target_regex).match(self.text, self.position)
    assert match is not None, 'Could not match /%s/' % target_regex
    self.position = match.end()
    return match.group(0)

  def ReadList(self, n, read_fn, *args):
    ls = []
    for i in range(n):
      if i > 0:
        self.Read(' ')
      ls.append(read_fn())
    self.ReadEndl()
    return ls

  def ReadInt(self, inclusive_min, inclusive_max):
    value = int(self.ReadRegex(r'0|(-?[1-9][0-9]*)'))
    assert inclusive_min <= value <= inclusive_max, (
        'Failed on %d <= %d <= %d' % (inclusive_min, value, inclusive_max))
    return value

  def ReadIntList(self, n, inclusive_min, inclusive_max):
    return self.ReadList(n, lambda: self.ReadInt(inclusive_min, inclusive_max))

  def ReadString(self, valid_characters=string.ascii_letters + string.digits,
                 inc_min_len=1, inc_max_len=10**6):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      if c not in valid_characters:
        break
      self.position += 1
    st = self.text[old_position:self.position]
    assert inc_min_len <= len(st) <= inc_max_len, (
        'Invalid string length: %d' % len(st))
    return st

  def ReadStringList(self, n,
                     valid_characters=string.ascii_letters + string.digits,
                     inc_min_len=1, inc_max_len=10**6):
    return self.ReadList(
        n,
        lambda: self.ReadString(valid_characters, inc_min_len, inc_max_len))


class UnionFind:
  def __init__(self, n):
    self.parent = list(range(n))
  
  def find(self, u):
    ucopy = u
    while u != self.parent[u]:
      u = self.parent[u]
    while ucopy != u:
      self.parent[ucopy], ucopy = u, self.parent[ucopy]
    return u
  
  def union(self, u, v):
    self.parent[self.find(u)] = self.find(v)


def minimum_spanning_tree(n, edges):
  dsu, mst = UnionFind(n), []
  for _, u, v, i in sorted([(w, u, v, i) for i, (u, v, w) in enumerate(edges)]):
    if dsu.find(u) != dsu.find(v):
      dsu.union(u, v)
      mst.append(i)
  return mst


def main(argv):
  vr = VerifierReader(sys.stdin.read())

  N = vr.ReadInt(2, 100000)
  vr.ReadSpace()
  M = vr.ReadInt(N - 1, 200000)
  vr.ReadEndl()

  edges = []
  for _ in range(M):
    U = vr.ReadInt(1, N)
    vr.ReadSpace()
    V = vr.ReadInt(U + 1, N)
    vr.ReadSpace()
    W = vr.ReadInt(1, 10**9)
    vr.ReadEndl()
    edges.append((U - 1, V - 1, W))

  vr.ReadEof()

  mst_edges = [(u, v, (w, 0)) for u, v, w in edges]
  for i in minimum_spanning_tree(N, edges):
    u, v, w = edges[i]
    mst_edges[i] = (u, v, (w, 1))
  assert sum(mst_edges[i][2][1] for i in minimum_spanning_tree(N, mst_edges)) == N - 1, (
      'Minimum spanning tree must be unique')


if __name__ == "__main__":
  sys.exit(main(sys.argv))
