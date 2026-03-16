#include <string.h>
#include "segment_tree.h"

static void buildTree(SegmentTree *segmentTree, int node, int start, int end)
{
    if (start == end) {
        segmentTree->tree[node] = segmentTree->values[start];
    } else {
        int mid = (start + end) / 2;
        buildTree(segmentTree, node * 2, start, mid);
        buildTree(segmentTree, node * 2 + 1, mid + 1, end);
        segmentTree->tree[node] = segmentTree->tree[node * 2] + segmentTree->tree[node * 2 + 1];
    }
}

static void updateTree(SegmentTree *segmentTree, int node, int start, int end, int index, double value)
{
    if (start == end) {
        segmentTree->tree[node] += value;
    } else {
        int mid = (start + end) / 2;
        if (index <= mid) {
            updateTree(segmentTree, node * 2, start, mid, index, value);
        } else {
            updateTree(segmentTree, node * 2 + 1, mid + 1, end, index, value);
        }
        segmentTree->tree[node] = segmentTree->tree[node * 2] + segmentTree->tree[node * 2 + 1];
    }
}

static double queryTree(SegmentTree *segmentTree, int node, int start, int end, int left, int right)
{
    if (right < start || end < left) {
        return 0.0;
    }

    if (left <= start && end <= right) {
        return segmentTree->tree[node];
    }

    int mid = (start + end) / 2;
    return queryTree(segmentTree, node * 2, start, mid, left, right) +
           queryTree(segmentTree, node * 2 + 1, mid + 1, end, left, right);
}

void initSegmentTree(SegmentTree *segmentTree, int days)
{
    int i;

    if (days < 1) {
        days = 1;
    }
    if (days > MAX_DAYS) {
        days = MAX_DAYS;
    }

    segmentTree->n = days;
    memset(segmentTree->tree, 0, sizeof(segmentTree->tree));
    memset(segmentTree->values, 0, sizeof(segmentTree->values));

    for (i = 1; i <= segmentTree->n; i++) {
        segmentTree->values[i] = 0.0;
    }

    buildTree(segmentTree, 1, 1, segmentTree->n);
}

void updateExpense(SegmentTree *segmentTree, int day, double amount)
{
    if (day < 1 || day > segmentTree->n) {
        return;
    }

    segmentTree->values[day] += amount;
    updateTree(segmentTree, 1, 1, segmentTree->n, day, amount);
}

double queryRange(SegmentTree *segmentTree, int left, int right)
{
    if (left < 1) {
        left = 1;
    }
    if (right > segmentTree->n) {
        right = segmentTree->n;
    }
    if (left > right) {
        return 0.0;
    }

    return queryTree(segmentTree, 1, 1, segmentTree->n, left, right);
}