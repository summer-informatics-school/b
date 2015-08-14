#include <cstdio>
#include <vector>
#include <cassert>

template <typename T, class op>
class sTree {
private:
  int count;
  struct node {
    T sum;
    enum {lazyAdd, lazySet} lazyOp;
    T lazyVal;
  };
  std::vector <node> t;

  void update(int v, int l, int r) {
    t[v].sum = op::mul(t[v].lazyVal, (r - l));
    if (t[v].lazyOp != node::lazySet) {
      if (v >= count) {
        return;
      }
      int nl = v * 2, nr = v * 2 + 1;
      t[v].sum = op::add(t[v].sum, t[nl].sum);
      t[v].sum = op::add(t[v].sum, t[nr].sum);
    }
  }

  void push(int v) {
    if (v >= count) {
      return;
    }
    int nl = v * 2, nr = v * 2 + 1;
    if (t[v].lazyOp == node::lazySet) {
      t[nl].lazyOp = node::lazySet;
      t[nl].lazyVal = t[v].lazyVal;
      t[nr].lazyOp = node::lazySet;
      t[nr].lazyVal = t[v].lazyVal;
      t[v].lazyVal = op::IND;
      t[v].lazyOp = node::lazyAdd;
    } else {
      t[nl].lazyVal = op::add(t[nl].lazyVal, t[v].lazyVal);
      t[nr].lazyVal = op::add(t[nr].lazyVal, t[v].lazyVal);
      t[v].lazyVal = op::IND;
      t[v].lazyOp = node::lazyAdd;
    }
  }

  void setValueR(int v, int vl, int vr, int l, int r, T val) {
    if (r <= vl || l >= vr) {
      return;
    }
    push(v);
    if (l <= vl && vr <= r) {
      t[v].lazyOp = node::lazySet;
      t[v].lazyVal = val;
    } else {
      int vm = (vl + vr) / 2;
      setValueR(v * 2, vl, vm, l, r, val);
      setValueR(v * 2 + 1, vm, vr, l, r, val);
    }
    update(v, vl, vr);
  }

  void incrValueR(int v, int vl, int vr, int l, int r, T val) {
    if (r <= vl || l >= vr) {
      return;
    }
    push(v);
    if (l <= vl && vr <= r) {
      t[v].lazyVal = op::add(t[v].lazyVal, val);
    } else {
      int vm = (vl + vr) / 2;
      setValueR(v * 2, vl, vm, l, r, val);
      setValueR(v * 2 + 1, vm, vr, l, r, val);
    }
    update(v, vl, vr);
  }

  T getOpR(int v, int vl, int vr, int l, int r) {
    if (r <= vl || l >= vr) {
      return op::IND;
    } else if (l <= vl && vr <= r) {
      return t[v].sum;
    } else {
      int vm = (vl + vr) / 2;
      push(v);
      T cl = getOpR(v * 2, vl, vm, l, r);
      T cr = getOpR(v * 2 + 1, vm, vr, l, r);
      update(v, vl, vr);
      return op::add(cl, cr);
    }
  }

public:
  sTree(int _count) {
    count = _count;
    while (count & (count - 1)) {
      ++count;
    }
    t.resize(count * 2);
    for (int i = 0; i < count * 2; ++i) {
      t[i].lazyOp = node::lazySet;
      t[i].lazyVal = op::IND;
      t[i].sum = op::IND;
    }
  }

  void setValue(int l, int r, T val) {
    assert(0 <= l && l <= r && r <= count);
    setValueR(1, 0, count, l, r, val);
  }
  void incrValue(int l, int r, T d) {
    assert(0 <= l && l <= r && r <= count);
    incrValueR(1, 0, count, l, r, d);
  }
  T getOp(int l, int r) {
    assert(0 <= l && l <= r && r <= count);
    return getOpR(1, 0, count, l, r);
  }
};

class opSum {
public:
  static const int IND = 0;
  static int add(int a, int b) {
    return a + b;
  }
  static int mul(int a, int cnt) {
    return a * cnt;
  }
};

const int inf = (int)1e9;
class opMin {
public:
  static const int IND = +inf;
  static int add(int a, int b) {
    if (a < b) {
      return a;
    } else {
      return b;
    }
  }
  static int mul(int a, int cnt) {
    return a;
  }
};

class opMax {
public:
  static const int IND = -inf;
  static int add(int a, int b) {
    if (a > b) {
      return a;
    } else {
      return b;
    }
  }
  static int mul(int a, int cnt) {
    return a;
  }
};
