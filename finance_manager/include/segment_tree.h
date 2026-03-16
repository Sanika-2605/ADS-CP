#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#define MAX_DAYS 366

typedef struct SegmentTree {
    double tree[4 * MAX_DAYS];
    double values[MAX_DAYS + 1];
    int n;
} SegmentTree;

void initSegmentTree(SegmentTree *segmentTree, int days);
void updateExpense(SegmentTree *segmentTree, int day, double amount);
double queryRange(SegmentTree *segmentTree, int left, int right);

#endif
