#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

struct edge {
  int to, id;
  
  edge (int _to, int _id):
    to(_to), id(_id) {
  }
};

struct graph {
  bool debug;

  int nV, nE;
  vector <vector <edge> > e;

  int curTime;
  vector <int> mark;
  vector <int> tIn, tOut;
  vector <int> l;
  vector <int> stV, stE;

  static const int markWhite = 0;
  static const int markGray = 1;
  static const int markBlack = 2;

  vector <int> bridges;
  vector <int> cutPoints;
  vector <vector <int> > biCompE;
  vector <vector <int> > biCompV;

  graph () {
    nV = -1;
    nE = -1;
    debug = false;
  }

  void read () {
    assert (scanf ("%d%d", &nV, &nE) == 2);
    assert (nV >= 1);
    assert (nE >= 0);
    e.resize (nV);
    for (int i = 0; i < nE; ++i) {
      int a, b;
      assert (scanf ("%d%d", &a, &b) == 2);
      --a; --b;
      e[a].push_back (edge (b, i));
      e[b].push_back (edge (a, i));
    }
    mark = vector <int> (nV, markWhite);
    tIn = vector <int> (nV, -1);
    tOut = vector <int> (nV, -1);
    l = vector <int> (nV, -1);
  }

  void popV (int v = -1) {
    vector <int> comp;
    int x;
    do {
      x = stV[(int)stV.size () - 1];
      stV.pop_back ();
      comp.push_back (x);
    } while ((int)stV.size () > 0 && x != v);
    biCompE.push_back (comp);
  }

  void popE (int e = -1) {
    if ((int)stE.size () == 0) {
      return;
    }
    vector <int> comp;
    int x;
    do {
      x = stE[(int)stE.size () - 1];
      stE.pop_back ();
      comp.push_back (x);
    } while ((int)stE.size () > 0 && x != e);
    biCompV.push_back (comp);
  }

  void DFS (int v, int p = -1, int pe = -1) {
    tIn[v] = curTime++;
    mark[v] = markGray;
    l[v] = tIn[v];
    bool isCut = false;
    int treeEdges = 0;
    stV.push_back (v);
    if (pe != -1) {
      stE.push_back (pe);
    }
    if (debug) {
      printf ("DFS: processing vertex %d "
        "(parent vertex %d, parent edge %d, "
        "in time %d), marked as gray\n", v, p, pe, tIn[v]);
    }

    for (int i = 0; i < (int)e[v].size (); ++i) {
      edge &ce = e[v][i];
      if (ce.id == pe) {
        if (debug) {
          printf ("Found reverse of parent edge, ignore it\n");
        }
        continue;
      }
      if (mark[ce.to] == markWhite) {
        if (debug) {
          printf ("Found white vertex %d, tree edge\n", ce.to);
        }
        DFS (ce.to, v, ce.id);
        l[v] = min (l[v], l[ce.to]);
        if (debug) {
          printf ("Relax l[%d]: new value %d\n", v, l[v]);
        }
        ++treeEdges;
        if (p != -1 && l[ce.to] > tIn[p]) {
          if (debug) {
            printf ("Subtree starting at %d will fall off, "
              "%d will be a cut point\n", ce.to, v);
          }
          isCut = true;
          popE (ce.id);
        } else if (p == -1 && treeEdges > 1) {
          if (debug) {
            printf ("Root %d has more than one tree children, "
              "will be a cut point\n", v);
          }
          popE (ce.id);
        }
      } else if (mark[ce.to] == markGray) {
        if (debug) {
          printf ("Found gray vertex %d, back edge\n", ce.to);
        }
        l[v] = min (l[v], tIn[ce.to]);
        if (debug) {
          printf ("Relax l[%d]: new value %d\n", v, l[v]);
        }
        stE.push_back (ce.id);
      } else if (mark[ce.to] == markBlack) {
        if (debug) {
          printf ("Found black vertex %d, forward edge\n", ce.to);
        }
      }
    }

    if (p != -1 && l[v] > tIn[p]) {
      if (debug) {
        printf ("Found bridge %d: %d -- %d\n", pe, p, v);
      }
      bridges.push_back (pe);
      popV (v);
    }
    if ((p != -1 && isCut) || (p == -1 && treeEdges > 1)) {
      if (debug) {
        printf ("Found cut point %d\n", v);
      }
      cutPoints.push_back (v);
    }

    mark[v] = markBlack;
    tOut[v] = curTime++;
    if (debug) {
      printf ("DFS: processing vertex %d finished (out time %d), "
        "marked as black\n", v, tOut[v]);
    }
  }

  void runDFS () {
    if (debug) {
      printf ("runDFS started\n");
    }
    assert (nV != -1);
    curTime = 0;
    for (int i = 0; i < nV; ++i) {
      if (mark[i] == markWhite) {
        if (debug) {
          printf ("run search from vertex %d\n", i);
        }
        DFS (i);
        popV ();
        popE ();
      }
    }
    if (debug) {
      printf ("runDFS finished\n");
    }
  }

  void dumpBridges () {
    sort (bridges.begin (), bridges.end ());
    int count = (int)bridges.size ();
    printf ("%d\n", count);
    for (int i = 0; i < count; ++i) {
      printf ("%d\n", bridges[i] + 1);
    }
  }

  void dumpCutPoints () {
    sort (cutPoints.begin (), cutPoints.end ());
    int count = (int)cutPoints.size ();
    printf ("%d\n", count);
    for (int i = 0; i < count; ++i) {
      printf ("%d\n", cutPoints[i] + 1);
    }
  }

  void dumpBiCompE () {
    vector <int> color (nV, -1);
    for (int i = 0; i < (int)biCompE.size (); ++i) {
      for (int j : biCompE[i]) {
        color[j] = i;
      }
    }
    printf ("%d\n", (int)biCompE.size ());
    for (int i = 0; i < nV; ++i) {
      printf ("%d%c", color[i] + 1, " \n"[i + 1 == nV]);
    }
  }

  void dumpBiCompV () {
    vector <int> color (nE, -1);
    for (int i = 0; i < (int)biCompV.size (); ++i) {
      for (int j : biCompV[i]) {
        color[j] = i;
      }
    }
    printf ("%d\n", (int)biCompV.size ());
    for (int i = 0; i < nE; ++i) {
      printf ("%d%c", color[i] + 1, " \n"[i + 1 == nE]);
    }
  }
};

int main (void) {
  graph g;
  g.debug = true;
  g.read ();
  g.runDFS ();
  g.dumpBridges ();
  g.dumpBiCompE ();
  g.dumpCutPoints ();
  g.dumpBiCompV ();
  return 0;
}
